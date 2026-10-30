#ifndef NET_HPP
#define NET_HPP

struct NetSegmentGraphicPackage {
    Point<u32> source;
    Point<u32> target;
    COLORREF   layer;
    u32        thickness;
};

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class Net : public IgraphicLayout<LayoutT> {
    std::string                          _name;
    std::deque<NetSegmentGraphicPackage> _segments;
    std::deque<Component<LayoutT>*>      _components;

    COLORREF get_layer_color(const std::string& layer) const {
        COLORREF color[5] = {
            RGB(30, 144, 255),  // metal1
            RGB(220, 20, 60),   // metal2
            RGB(34, 139, 34),   // metal3
            RGB(148, 0, 211),   // metal4
            RGB(255, 140, 0)    // metal5
        };

        if (layer.contains("1")) return color[0];
        if (layer.contains("2")) return color[1];
        if (layer.contains("3")) return color[2];
        if (layer.contains("4")) return color[3];
        if (layer.contains("5")) return color[4];

        return RGB(100, 100, 100);
    }

    u32 get_wire_thickness(const LefLayer& layer) const {
        f64 t = layer.thickness;

        u32 thickness = 1;
        if (t >= 1.0) {
            thickness = 3;
        } else if (t >= 0.6) {
            thickness = 2;
        } else {
            thickness = 1;
        }

        return thickness;
    }

public:
    Net(const LayoutT& parent_layout,
        const DieArea& die_area, 
        const DefNet& def_net, 
        const LefDataPackage& lef_data,
        const std::deque<Component<LayoutT>*>& components) : IgraphicLayout<LayoutT>([parent_layout, die_area, def_net]() -> LayoutT {
                                                                 Point<f64> parent_size = Point<f64> {
                                                                     .x = sc<f64>(parent_layout.width),
                                                                     .y = sc<f64>(parent_layout.height)
                                                                 }; 
                                                             
                                                                 f64 scale = std::min(parent_size.x / die_area.width(), 
                                                                                      parent_size.y / die_area.height());
                                                                 
                                                                 Point<f64> offset = Point<f64> {
                                                                     .x = parent_layout.pos_x + (parent_size.x - die_area.width() * scale) * 0.5,
                                                                     .y = parent_layout.pos_y + (parent_size.y - die_area.height() * scale) * 0.5
                                                                 };
      
                                                                 auto get_net = [def_net](const u32&(*func)(const u32&, const u32&)) -> Point<f64> {
                                                                     const auto& first_segment = def_net.segments.front();
                                                                     if (std::holds_alternative<DefNet::Routing>(first_segment)) {
                                                                         const auto& routing = std::get<DefNet::Routing>(first_segment);
                                                                         return Point<f64> {
                                                                             .x = sc<f64>(func(routing.source.x, routing.target.x)),
                                                                             .y = sc<f64>(func(routing.source.y, routing.target.y))
                                                                         };
                                                                     } else if (std::holds_alternative<DefNet::Contact>(first_segment)) {
                                                                         const auto& contact = std::get<DefNet::Contact>(first_segment);
                                                                         return Point<f64> {
                                                                             .x = sc<f64>(contact.source.x),
                                                                             .y = sc<f64>(contact.source.y)
                                                                         };
                                                                     } else {
                                                                         std::cerr << "Net: unknown segment type." << std::endl;
                                                                         return Point<f64> { .x = 0.0, .y = 0.0 };
                                                                     }
                                                                 };
                                                             
                                                                 Point<f64> net_max = get_net(std::max<u32>);
                                                                 Point<f64> net_min = get_net(std::min<u32>);
      
                                                                 for (const auto& segment : def_net.segments) {
                                                                     if (std::holds_alternative<DefNet::Routing>(segment)) {
                                                                          const auto& routing = std::get<DefNet::Routing>(segment);
                                                                          net_max.x = std::max(net_max.x, sc<f64>(routing.source.x));
                                                                          net_max.x = std::max(net_max.x, sc<f64>(routing.target.x));
                                                                          net_max.y = std::max(net_max.y, sc<f64>(routing.source.y));
                                                                          net_max.y = std::max(net_max.y, sc<f64>(routing.target.y));
                                                                          net_min.x = std::min(net_min.x, sc<f64>(routing.source.x));
                                                                          net_min.x = std::min(net_min.x, sc<f64>(routing.target.x));
                                                                          net_min.y = std::min(net_min.y, sc<f64>(routing.source.y));
                                                                          net_min.y = std::min(net_min.y, sc<f64>(routing.target.y));
                                                                     } else if (std::holds_alternative<DefNet::Contact>(segment)) {
                                                                          const auto& contact = std::get<DefNet::Contact>(segment);
                                                                          net_max.x = std::max(net_max.x, sc<f64>(contact.source.x));
                                                                          net_max.y = std::max(net_max.y, sc<f64>(contact.source.y));
                                                                          net_min.x = std::min(net_min.x, sc<f64>(contact.source.x));
                                                                          net_min.y = std::min(net_min.y, sc<f64>(contact.source.y));
                                                                     } else {
                                                                          std::cerr << "Net: unknown segment type." << std::endl;
                                                                     }
                                                                 }
      
                                                                 Point<f64> net_size = Point<f64> {
                                                                     .x = net_max.x - net_min.x,
                                                                     .y = net_max.y - net_min.y
                                                                 };
                                                                 
                                                                 Point<f64> screen_pos = Point<f64> {
                                                                     .x = offset.x + (net_min.x - die_area.top_left.x) * scale,
                                                                     .y = offset.y + (die_area.bottom_right.y - (net_min.y + net_size.y)) * scale
                                                                 };
                                                                 
                                                                 Point<f64> screen_size = Point<f64> {
                                                                     .x = std::max(sc<f64>(2.0), net_size.x * scale),
                                                                     .y = std::max(sc<f64>(2.0), net_size.y * scale)
                                                                 };
                                                                 
                                                                 return GraphicPositionExtentPackage {
                                                                     .pos_x = sc<u32>(std::round(screen_pos.x)),
                                                                     .pos_y = sc<u32>(std::round(screen_pos.y)),
                                                                     .width = sc<u32>(std::round(screen_size.x)),
                                                                     .height = sc<u32>(std::round(screen_size.y))
                                                                 };
                                                             }(), RGB(0, 0, 0)),
                                                             _name(def_net.name),
                                                             _segments(),
                                                             _components() {
        Point<f64> parent_size = Point<f64> {
            .x = sc<f64>(parent_layout.width),
            .y = sc<f64>(parent_layout.height)
        }; 
        
        f64 scale = std::min(parent_size.x / die_area.width(), 
                             parent_size.y / die_area.height());
        
        Point<f64> offset = Point<f64> {
            .x = parent_layout.pos_x + (parent_size.x - die_area.width() * scale) * 0.5,
            .y = parent_layout.pos_y + (parent_size.y - die_area.height() * scale) * 0.5
        };

        auto translate_coords = [die_area, offset, scale](const Point<u32>& point) -> Point<u32> {
            Point<f64> d = Point<f64> {
                .x = sc<f64>(point.x) - sc<f64>(die_area.top_left.x),
                .y = sc<f64>(die_area.bottom_right.y) - sc<f64>(point.y) 
            };

            return Point<u32> {
                .x = sc<u32>(std::round(offset.x + d.x * scale)),
                .y = sc<u32>(std::round(offset.y + d.y * scale))
            };
        };

        for (const auto& segment : def_net.segments) {
            if (std::holds_alternative<DefNet::Routing>(segment)) {
                const auto& routing = std::get<DefNet::Routing>(segment);
                _segments.push_back(NetSegmentGraphicPackage {
                    .source = translate_coords(routing.source),
                    .target = translate_coords(routing.target),
                    .layer = this->get_layer_color(routing.layer),
                    .thickness = this->get_wire_thickness(lef_data.layers.at(routing.layer))
                });
            } else if (std::holds_alternative<DefNet::Contact>(segment)) {
                const auto& contact = std::get<DefNet::Contact>(segment);
                _segments.push_back(NetSegmentGraphicPackage {
                    .source = translate_coords(contact.source),
                    .target = translate_coords(contact.source),
                    .layer = this->get_layer_color(contact.layer),
                    .thickness = this->get_wire_thickness(lef_data.layers.at(contact.layer))
                });
        }   }

        for (auto* macro : components) {
            for (const auto& connection : def_net.connections) {
                if (macro->check_name(connection.instance_name)) {
                    _components.push_back(macro);
    }   }   }   }

    void send_message(Point<u32> mouse_position, Message message, const WindowExtentPackage& actual_paint_window_extent) override;

    void draw(HDC h_dc, const WindowExtentPackage&) override {
        for (const auto& segment : _segments) {
            HPEN old_pen_segment;
            HPEN pen_segment = PaintTools::TakePen(h_dc, segment.layer, old_pen_segment, segment.thickness);

            if (segment.source.x == segment.target.x && segment.source.y == segment.target.y) {
                PaintTools::PaintFilledRectangle(h_dc, segment.source.x - segment.thickness, segment.source.y - segment.thickness,
                                                       segment.source.x + segment.thickness, segment.source.y + segment.thickness);
            } else {
                PaintTools::PaintLine(h_dc, segment.source.x, segment.source.y, 
                                            segment.target.x, segment.target.y);
            }

            PaintTools::ResetPen(h_dc, old_pen_segment, pen_segment);
        }
    }

};

#endif // NET_HPP
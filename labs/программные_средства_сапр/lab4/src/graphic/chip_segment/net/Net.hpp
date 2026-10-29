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
    std::deque<Macros<LayoutT>>          _macroses;

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

    u32 get_wire_thickness(const std::string& layer, ChipData chip_data) const {
        LefDataPackage lef_data = chip_data->get_lef_data();
        
        f64 t = lef_data.layers[layer].thickness;

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
    Net(LayoutT layout, 
        DefNet def_net, 
        ChipData chip_data,
        const std::deque<Macros<LayoutT>>& macroses) : IgraphicLayout<LayoutT>(layout, RGB(0, 0, 0)),
                                                       _name(def_net.name),
                                                       _segments(),
                                                       _macroses() {
        for (const auto& segment : def_net.segments) {
            if (std::holds_alternative<DefNet::Routing>(segment)) {
                const auto& routing = std::get<DefNet::Routing>(segment);
                _segments.push_back(NetSegmentGraphicPackage {
                    .source = routing.source,
                    .target = routing.target,
                    .layer = this->get_layer_color(routing.layer),
                    .thickness = this->get_wire_thickness(routing.layer, chip_data)
                });
            } else if (std::holds_alternative<DefNet::Contact>(segment)) {
                const auto& contact = std::get<DefNet::Contact>(segment);
                _segments.push_back(NetSegmentGraphicPackage {
                    .source = contact.source,
                    .target = contact.source,
                    .layer = this->get_layer_color(contact.layer),
                    .thickness = this->get_wire_thickness(contact.layer, chip_data)
                });
            }
        }

        for (const auto& macro : macroses) {
            for (const auto& connection : def_net.connections) {
                if (macro.check_name(connection.instance_name)) {
                    _macroses.push_back(macro);
                }
            }
        }
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        for (const auto& segment : _segments) {
            HPEN old_pen_segment;
            HPEN pen_segment = PaintTools::TakePen(h_dc, segment.layer, old_pen_segment, segment.thickness);

            PaintTools::PaintLine(h_dc, position_extent.pos_x + segment.source.x, 
                                        position_extent.pos_y + segment.source.y, 
                                        position_extent.pos_x + segment.target.x, 
                                        position_extent.pos_y + segment.target.y);

            PaintTools::ResetPen(h_dc, old_pen_segment, pen_segment);
        }
    }

};

#endif // NET_HPP
#ifndef MACROS_HPP
#define MACROS_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class Component : public IgraphicLayout<LayoutT> {
    std::string _instance_name;
    bool        _is_placed;
    bool        _is_highlighted;

    static constexpr void apply_orientation(const std::string& orientation, Point<f64>& size) {
        if (orientation == "W"  || orientation == "E" ||
            orientation == "FW" || orientation == "FE") {
            std::swap(size.x, size.y);
        }
    }

    void draw_instance_name(HDC h_dc, const GraphicPositionExtentPackage& position_extent) {
        if (_instance_name.empty()) return;

        std::string text = _instance_name;
        SIZE text_size;
        GetTextExtentPoint32A(h_dc, text.c_str(), sc<i32>(text.size()), &text_size);

        Point<u32> text_position = Point<u32> {
            .x = position_extent.pos_x + sc<u32>((position_extent.width - text_size.cx) * 0.5),
            .y = position_extent.pos_y + sc<u32>((position_extent.height - text_size.cy) * 0.5)
        };

        if (position_extent.width < sc<u32>(text_size.cx) || position_extent.height < sc<u32>(text_size.cy)) return;

        SetBkMode(h_dc, TRANSPARENT);
        SetTextColor(h_dc, RGB(255, 255, 255));
        TextOutA(h_dc, text_position.x, text_position.y, text.c_str(), sc<i32>(text.size()));
    }

public:
    Component(const LayoutT& parent_layout,
           const DieArea& die_area,
           const DefComponent& component, 
           const LefDataPackage& lef_data) : IgraphicLayout<LayoutT>([parent_layout, die_area, component, lef_data]() -> LayoutT {
                                                LefMacro macro = lef_data.macros.at(component.macro_name);

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
                                             
                                                Point<f64> macro_size = Point<f64> {
                                                    .x = macro.width  * lef_data.database_microns,
                                                    .y = macro.height * lef_data.database_microns
                                                };

                                                Component::apply_orientation(component.orientation, macro_size);

                                                Point<f64> screen_pos = Point<f64> {
                                                    .x = offset.x + (component.position.x - die_area.top_left.x) * scale,
                                                    .y = offset.y + (die_area.bottom_right.y - (component.position.y + macro_size.y)) * scale
                                                };

                                                Point<f64> screen_size = Point<f64> {
                                                    .x = std::max(sc<f64>(2.0), macro_size.x * scale),
                                                    .y = std::max(sc<f64>(2.0), macro_size.y * scale)
                                                };

                                                return GraphicPositionExtentPackage {
                                                    .pos_x = sc<u32>(std::round(screen_pos.x)),
                                                    .pos_y = sc<u32>(std::round(screen_pos.y)),
                                                    .width = sc<u32>(std::round(screen_size.x)),
                                                    .height = sc<u32>(std::round(screen_size.y))
                                                };
                                             }(), RGB(0, 0, 0)),
                                             _instance_name(component.instance_name),
                                             _is_placed(component.status == "PLACED" || component.status == "FIXED"),
                                             _is_highlighted(false) {}

    bool check_name(const std::string& name) const {
        return _instance_name == name;
    }

    void set_highlighted(bool highlighted) {
        _is_highlighted = highlighted;
        this->_color = _is_highlighted ? RGB(255, 0, 0) : RGB(0, 0, 0);
    }

    void send_message(Point<u32> mouse_pos, Message message, const WindowExtentPackage& window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        if ((mouse_pos.x < position_extent.pos_x || mouse_pos.x >= position_extent.pos_x + position_extent.width ||
             mouse_pos.y < position_extent.pos_y || mouse_pos.y >= position_extent.pos_y + position_extent.height) && 
            (mouse_pos.x != UINT32_MAX && mouse_pos.y != UINT32_MAX)) return;

        if (message.message_id == MESSAGE_ID_CLICK) {
            
        }
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        if (!_is_placed) return;
        
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, this->_color, old_pen, 1);
        HBRUSH old_brush;
        HBRUSH brush = PaintTools::TakeBrush(h_dc, this->_color, old_brush);
        
        PaintTools::PaintFilledRectangle(h_dc, position_extent.pos_x, 
                                               position_extent.pos_y, 
                                               position_extent.pos_x + position_extent.width, 
                                               position_extent.pos_y + position_extent.height);
                                                       
        PaintTools::ResetPen(h_dc, old_pen, pen);
        PaintTools::ResetBrush(h_dc, old_brush, brush);
        
        this->draw_instance_name(h_dc, position_extent);
    }
};

#endif // MACROS_HPP
#ifndef PLOT_MANAGER_HPP
#define PLOT_MANAGER_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class PlotManager : public IgraphicLayout<LayoutT> {
    std::string                                        _element_title;
    std::deque<PlotBox<GraphicShiftPackage>*>          _plot_boxes;
    std::deque<TextBox<GraphicPositionExtentPackage>*> _text_boxes;
    MathVisualFeatures                                 _math_visual_features;
    u32                                                _active_plot_index;

    void text_boxes_resize_handle(const GraphicPositionExtentPackage& position_extent, const WindowExtentPackage& window_extent) {
        for (auto* text_box : _text_boxes) {
            const auto& text_box_position_extent = text_box->template get_position_extent<GraphicPositionExtentPackage>(window_extent);
            
            text_box->set_layout(GraphicPositionExtentPackage {
                .pos_x  = text_box_position_extent.pos_x,
                .pos_y  = position_extent.pos_y + position_extent.height - (text_box_position_extent.height + 1),
                .width  = text_box_position_extent.width,
                .height = text_box_position_extent.height
            }, window_extent);
        }
    }

public:
    constexpr PlotManager(const LayoutT& layout,
                          const std::string& element_title,
                          MathVisualFeatures math_visual_features = MathVisualFeatures {},
                          COLORREF color = RGB(0, 0, 0)) : IgraphicLayout<LayoutT>(layout, color), 
                                                           _element_title(element_title),
                                                           _math_visual_features(math_visual_features),
                                                           _active_plot_index(0) {}

    PlotManager<LayoutT>* add_plot_box(Window, HWND h_wnd, PlotData plot_data, COLORREF color = RGB(0, 0, 0));

    void set_active_plot(u32 index) {
        if (index < _plot_boxes.size()) {
            _active_plot_index = index;
            this->_update = true;
        }
    }

    void init_runtime(HWND h_wnd, const WindowExtentPackage& window_extent) override {
        u32 text_box_height = 0;
        u32 text_box_width_sum = 0;

        HDC h_dc = GetDC(h_wnd);
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);
        
        for (auto* text_box : _text_boxes) {
            text_box->init_runtime(h_wnd, window_extent);

            SIZE text_size;
            GetTextExtentPoint32A(h_dc, text_box->get_text().c_str(), sc<int>(text_box->get_text().size()), &text_size);
            text_box_height = std::max(text_box_height, sc<u32>(text_size.cy));
        }   

        for (auto* text_box : _text_boxes) {
            SIZE text_size;
            GetTextExtentPoint32A(h_dc, text_box->get_text().c_str(), sc<int>(text_box->get_text().size()), &text_size);

            text_box->set_layout(GraphicPositionExtentPackage {
                .pos_x  = position_extent.pos_x + text_box_width_sum + 1,
                .pos_y  = position_extent.pos_y + position_extent.height - text_box_height - 1,
                .width  = sc<u32>(text_size.cx),
                .height = text_box_height
            }, window_extent);

            text_box_width_sum += sc<u32>(text_size.cx + 1);
        }

        ReleaseDC(h_wnd, h_dc);

        const auto& shift = this->template get_position_extent<GraphicShiftPackage>(window_extent);
        
        for (auto* plot_box : _plot_boxes) {
            plot_box->set_layout(GraphicShiftPackage {
                .left_border_shift   = shift.left_border_shift,
                .right_border_shift  = shift.right_border_shift,
                .top_border_shift    = shift.top_border_shift,
                .bottom_border_shift = shift.bottom_border_shift + text_box_height + 4
            }, window_extent);

            plot_box->init_runtime(h_wnd, window_extent);
        }
    }   

    void send_message(Point<u32> mouse_position, Message message, const WindowExtentPackage& window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        if ((mouse_position.x < position_extent.pos_x || mouse_position.x >= position_extent.pos_x + position_extent.width ||
             mouse_position.y < position_extent.pos_y || mouse_position.y >= position_extent.pos_y + position_extent.height) && 
            (mouse_position.x != UINT32_MAX && mouse_position.y != UINT32_MAX)) return;
        

        switch (message.message_id) {
            case MESSAGE_ID_UPDATE:
                for (auto* plot_box : _plot_boxes) {
                    plot_box->send_message(mouse_position, message, window_extent);
                }
                for (auto* text_box : _text_boxes) {
                    text_box->send_message(mouse_position, message, window_extent);
                }

                this->text_boxes_resize_handle(position_extent, window_extent);

                this->_update = true;
                return;

            case MESSAGE_ID_CLICK: {
                if (_active_plot_index < _plot_boxes.size()) {
                    _plot_boxes[_active_plot_index]->send_message(mouse_position, message, window_extent);
                }
                
                const auto& data = *reinterpret_cast<ClickMessagePackage*>(message.message_data);

                if (data.key != MK_LBUTTON) return;
                
                for (auto* text_box : _text_boxes) {
                    text_box->send_message(mouse_position, message, window_extent);
                }
                return;
            }

            case MESSAGE_ID_ZOOM:
                if (_active_plot_index < _plot_boxes.size()) {
                    _plot_boxes[_active_plot_index]->send_message(mouse_position, message, window_extent);
                }
                return;
        }
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        HPEN old_pen;
        HPEN pen;     

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        if (_active_plot_index < _plot_boxes.size()) {
            _plot_boxes[_active_plot_index]->draw(h_dc, actual_paint_window_extent);
        }

        for (auto* text_box : _text_boxes) {
            text_box->draw(h_dc, actual_paint_window_extent);
        }

        if (_plot_boxes.empty()) return;
        
        pen = PaintTools::TakePen(h_dc, this->_color, old_pen);

        PaintTools::PaintRectangle(h_dc, position_extent.pos_x, position_extent.pos_y, 
                                         position_extent.pos_x + position_extent.width, 
                                         position_extent.pos_y + position_extent.height);
        
        PaintTools::ResetPen(h_dc, old_pen, pen);
    }

    ~PlotManager() override {
        for (auto* plot_box : _plot_boxes) {
            delete plot_box;
        }
        for (auto* text_box : _text_boxes) {
            delete text_box;
        }
    }

    PlotManager(const PlotManager&&) = delete;
    PlotManager& operator=(const PlotManager&&) = delete;
    PlotManager(const PlotManager&) = delete;
    PlotManager& operator=(const PlotManager&) = delete;
};

#endif // PLOT_MANAGER_HPP
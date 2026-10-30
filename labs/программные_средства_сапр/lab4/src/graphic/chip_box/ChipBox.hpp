#ifndef CHIP_BOX_HPP
#define CHIP_BOX_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class ChipBox : public IgraphicLayout<LayoutT> {
    ChipData                                             _chip_data;
    std::deque<Net<GraphicPositionExtentPackage>*>       _nets;
    std::deque<Component<GraphicPositionExtentPackage>*> _components;
    bool                                                 _is_selected;

    void draw_text(HDC h_dc, const GraphicPositionExtentPackage& position_extent) {
        const auto& def_data = _chip_data->get_def_data();
        const auto& stat_data = _chip_data->get_stats();

        SetBkMode(h_dc, TRANSPARENT);
        SetTextColor(h_dc, RGB(40, 40, 40));

        std::string info = "Компонентов: " + std::to_string(stat_data.total_components) +
                           " | Сигнальных цепей: " + std::to_string(stat_data.signal_net_names.size()) +
                           " | Кристалл: " + std::to_string(sc<i32>(def_data.die_area.width())) + "x" + std::to_string(sc<i32>(def_data.die_area.height()));

        PaintTools::PaintText(h_dc, info, sc<i32>(position_extent.pos_x + 5), sc<i32>(position_extent.pos_y + position_extent.height + 6));
    }

public:
    constexpr ChipBox(const LayoutT& layout,
                      ChipData chip_data,
                      COLORREF color = RGB(0, 0, 0)) : IgraphicLayout<LayoutT>(layout, color),
                                                       _chip_data(chip_data),
                                                       _is_selected(false) {}

    void set_selected(bool selected) {
        _is_selected = selected;
    }

    void send_message(Point<u32> mouse_pos, Message msg, const WindowExtentPackage& window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        if ((mouse_pos.x < position_extent.pos_x || mouse_pos.x >= position_extent.pos_x + position_extent.width ||
             mouse_pos.y < position_extent.pos_y || mouse_pos.y >= position_extent.pos_y + position_extent.height) && 
            (mouse_pos.x != UINT32_MAX && mouse_pos.y != UINT32_MAX)) return;

        ClickMessagePackage click_data = {
            .key = VK_LBUTTON,
            .self = this
        };

        if (msg.message_id == MESSAGE_ID_CLICK) {
            for (auto& net_item : this->_nets) {
                if (_is_selected) {
                    std::cout << "Net selected" << std::endl;
                    break;
                }

                net_item->send_message(mouse_pos, Message {
                    .message_id = MESSAGE_ID_CLICK,
                    .message_data = &click_data
                }, window_extent);
            }
        }

        this->_is_selected = false;
    }

    void init_runtime(HWND hwnd, const WindowExtentPackage& window_extent) override {
        if (_chip_data == nullptr) return;

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        const auto& def_data = _chip_data->get_def_data();
        const auto& lef_data = _chip_data->get_lef_data();
        
        this->_components.clear();
        this->_nets.clear();

        for (const auto& component : def_data.components) {
            this->_components.push_back(new Component(position_extent, def_data.die_area, component, lef_data));
        }
        for (const auto& net : def_data.nets) {
            this->_nets.push_back(new Net(position_extent, def_data.die_area, net, lef_data, this->_components));
        }

        for (auto& macro_item : this->_components) {
            macro_item->init_runtime(hwnd, window_extent);
        }
        for (auto& net_item : this->_nets) {
            net_item->init_runtime(hwnd, window_extent);
        }
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        if (_chip_data == nullptr) return;

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        HPEN old_pen;
        HPEN h_pen = PaintTools::TakePen(h_dc, RGB(0, 0, 0), old_pen, 1);
        HBRUSH old_brush;
        HBRUSH h_brush = PaintTools::TakeBrush(h_dc, this->_color, old_brush);

        PaintTools::PaintFilledRectangle(h_dc, position_extent.pos_x, position_extent.pos_y, 
                                               position_extent.pos_x + position_extent.width, position_extent.pos_y + position_extent.height);

        PaintTools::ResetBrush(h_dc, old_brush, h_brush);
        PaintTools::ResetPen(h_dc, old_pen, h_pen);

        this->draw_text(h_dc, position_extent);

        for (auto& macro_item : this->_components) {
            macro_item->draw(h_dc, actual_paint_window_extent);
        }

        for (auto& net_item : this->_nets) {
            net_item->draw(h_dc, actual_paint_window_extent);
        }
        
    }

    ~ChipBox() {
        for (auto& macro_item : this->_components) {
            delete macro_item;
        }
        for (auto& net_item : this->_nets) {
            delete net_item;
        }
    }

    ChipBox(const ChipBox&&) = delete;
    ChipBox& operator=(const ChipBox&&) = delete;
    ChipBox(const ChipBox&) = delete;
    ChipBox& operator=(const ChipBox&) = delete;
};

#endif // CHIP_BOX_HPP

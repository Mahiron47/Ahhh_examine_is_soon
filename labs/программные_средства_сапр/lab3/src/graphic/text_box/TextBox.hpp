#ifndef TEXT_BOX_HPP
#define TEXT_BOX_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class TextBox : public IgraphicLayout<LayoutT> {
    std::string           _text;
    std::function<void()> _l_click_event_handler;
    std::function<void()> _r_click_event_handler;

public:
    constexpr TextBox(const LayoutT& layout,
                      const std::string& text,
                      std::function<void()> l_click_event_handler = []() -> void {},
                      std::function<void()> r_click_event_handler = []() -> void {},
                      COLORREF color = RGB(0, 0, 0)) : IgraphicLayout<LayoutT>(layout, color),
                                                       _text(text),
                                                       _l_click_event_handler(l_click_event_handler),
                                                       _r_click_event_handler(r_click_event_handler) {}

    constexpr const std::string& get_text() const noexcept {
        return _text;
    }

    void send_message(Point<u32> mouse_pos, Message msg, const WindowExtentPackage& window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        if ((mouse_pos.x < position_extent.pos_x || mouse_pos.x > position_extent.pos_x + position_extent.width ||
             mouse_pos.y < position_extent.pos_y || mouse_pos.y > position_extent.pos_y + position_extent.height) && 
            (mouse_pos.x != UINT32_MAX && mouse_pos.y != UINT32_MAX)) return;

        switch (msg.message_id) {
            case MESSAGE_ID_UPDATE:
                this->_update = true;
                return;
            case MESSAGE_ID_CLICK: {
                const auto& click_data = *reinterpret_cast<ClickMessagePackage*>(msg.message_data);

                switch (click_data.key) {
                    case MK_LBUTTON:
                        this->_l_click_event_handler();
                        break;
                    case MK_RBUTTON:
                        this->_r_click_event_handler();
                        break;
                }

                return;
        }   }
    }

    void draw(HDC h_dc, const WindowExtentPackage& window_extent) override {
        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, this->_color, old_pen);
        HBRUSH old_brush;
        HBRUSH brush = PaintTools::TakeBrush(h_dc, this->_color, old_brush);

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        PaintTools::PaintTextFilledRectangle(h_dc, _text, position_extent.pos_x, position_extent.pos_y, 
                                                          position_extent.pos_x + position_extent.width, 
                                                          position_extent.pos_y + position_extent.height);
        PaintTools::PaintRectangle(h_dc, position_extent.pos_x, position_extent.pos_y, 
                                         position_extent.pos_x + position_extent.width, 
                                         position_extent.pos_y + position_extent.height);

        PaintTools::ResetPen(h_dc, old_pen, pen);
        PaintTools::ResetBrush(h_dc, old_brush, brush);
    }

    TextBox(const TextBox&&) = delete;
    TextBox& operator=(const TextBox&&) = delete;
    TextBox(const TextBox&) = delete;
    TextBox& operator=(const TextBox&) = delete;
};

#endif // TEXT_BOX_HPP
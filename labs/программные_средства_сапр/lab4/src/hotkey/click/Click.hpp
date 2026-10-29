#ifndef CLICK_HPP
#define CLICK_HPP

class Click : public IhotKey {
    std::deque<Igraphic*> _graphic;

public:
    template<class... Args> requires (std::derived_from<Args, Igraphic> && ...)
    constexpr Click(u16 key, 
                    Args*... args) : IhotKey(key), 
                                     _graphic({args...}) {}

    void execute(u32, HWND h_wnd, WPARAM w_param, LPARAM l_param) override {
        if (w_param != _key) return;

        RECT window_rect;
        GetWindowRect(h_wnd, &window_rect);

        auto click_data = ClickMessagePackage {
            .key = _key,
        };

        for (auto& graphic : _graphic) {
            graphic->send_message(Point<u32> {
                .x = sc<u32>(l_param & 0xFFFF),
                .y = sc<u32>(l_param >> 16)
            }, Message {
                .message_id = MESSAGE_ID_CLICK,
                .message_data = &click_data
            }, WindowExtentPackage {
                .width = sc<u32>(window_rect.right - window_rect.left),
                .height = sc<u32>(window_rect.bottom - window_rect.top)
            });
        }
    }
};

#endif // CLICK_HPP


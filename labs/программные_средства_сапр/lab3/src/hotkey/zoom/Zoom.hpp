#ifndef ZOOM_HPP
#define ZOOM_HPP

#define APP_ZOOM 0x0002

class Zoom : public IhotKey {
    Igraphic* _graphic;
    f32       _scale;

public:
    constexpr Zoom(f32 scale,
                   Igraphic* graphic) : IhotKey(0x0000),
                                        _graphic(graphic),
                                        _scale(scale) {}
    constexpr Zoom(u16 additional_key,
                   f32 scale,
                   Igraphic* graphic) : IhotKey(additional_key),
                                        _graphic(graphic),
                                        _scale(scale) {}

    void execute(u32 msg, HWND h_wnd, WPARAM w_param, LPARAM l_param) {
        if (msg != WM_MOUSEWHEEL) return;

        i16 wheel_delta = w_param >> 16;
        u16 additional_key = w_param & 0xFFFF;

        if (wheel_delta == 0) return;
        if ((additional_key & this->_key) == 0 && this->_key != 0) return;

        POINT mouse_position = POINT {
            .x = sc<LONG>(l_param & 0xFFFF),
            .y = sc<LONG>(l_param >> 16)
        };

        ScreenToClient(h_wnd, &mouse_position);

        RECT client_rect;

        GetClientRect(h_wnd, &client_rect);

        constexpr f32 zoom_factor = 1.0f / WHEEL_DELTA;

        ZoomMessagePackage zoom_data = ZoomMessagePackage {
            .h_wnd = h_wnd,
            .bidirectional_strength = wheel_delta * zoom_factor * _scale,
            .additional_key = additional_key,
        };

        _graphic->send_message(Point<u32> { 
            .x = sc<u32>(mouse_position.x), 
            .y = sc<u32>(mouse_position.y)
        }, Message { 
            .message_id = MESSAGE_ID_ZOOM, 
            .message_data = &zoom_data,
        }, WindowExtentPackage {
            .width = sc<u32>(client_rect.right - client_rect.left),
            .height = sc<u32>(client_rect.bottom - client_rect.top)
        });
    }
};

#endif // ZOOM_HPP
#ifndef INIT_MAIN_WINDOW
#define INIT_MAIN_WINDOW

class InitMainWindow : public IinitWindowStrategy {
    int _n_cmd_show;
    COLORREF _window_background_color;

public: 
    template<class... Args> requires ((GET_HANDLER(Igraphic) || GET_HANDLER(IhotKey)) && ...)
    constexpr InitMainWindow(int n_cmd_show, 
                             COLORREF color = RGB(0, 0, 0), 
                             Args*... args) : IinitWindowStrategy(args...), 
                                              _n_cmd_show(n_cmd_show), 
                                              _window_background_color(color) {}

    int get_n_cmd_show() override {
        return _n_cmd_show;
    }
    COLORREF get_window_background_color() override {
        return _window_background_color;
    }
    LPCTSTR get_class_name() override {
        return TEXT("MainWindow");
    }
    constexpr UINT get_window_style() override {
        return CS_HREDRAW | CS_VREDRAW;
    }
    UINT get_instance_style() override {
        return WS_OVERLAPPEDWINDOW;
    }
    constexpr WindowPositionExtentPackage get_window_position_package() override {
        return WindowPositionExtentPackage {
            .pos_x = CW_USEDEFAULT,
            .pos_y = CW_USEDEFAULT,
            .width = 1200,
            .height = 800
        };
    }
    WNDPROC get_window_protocol() override;
};

inline WNDPROC InitMainWindow::get_window_protocol() {
    return IinitWindowStrategy::construct_window_protocol<[](Window_T* window, HWND h_wnd, 
                                                             UINT msg, WPARAM w_param, LPARAM l_param) -> LRESULT {
        switch (msg) {
        case WM_APP | MESSAGE_ID_INIT: {
            for (auto& graphic : window->get_graphics()) {
                graphic->init_runtime(h_wnd, window->get_extent());
            }
            break;
        }
        case WM_CLOSE:
            DestroyWindow(h_wnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            if (w_param == SIZE_MINIMIZED) break;

            for (auto& graphic : window->get_graphics()) {
                graphic->send_message(Point<u32> { 
                    .x = MAXUINT32, 
                    .y = MAXUINT32
                }, Message { 
                    .message_id = MESSAGE_ID_UPDATE, 
                    .message_data = nullptr
                }, window->get_extent());
            }

            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        case WM_KEYDOWN:
            for (auto& hotkey : window->get_hot_key()) {
                hotkey->execute(WM_KEYDOWN, h_wnd, w_param, l_param);
            }

            break;
        case WM_MOUSEWHEEL:
            for (auto& hotkey : window->get_hot_key()) {
                hotkey->execute(WM_MOUSEWHEEL, h_wnd, w_param, l_param);
            }

            break;
        case WM_LBUTTONDOWN:
            for (auto& hotkey : window->get_hot_key()) {
                hotkey->execute(WM_LBUTTONDOWN, h_wnd, w_param, l_param);
            }

            break;
        case WM_PAINT: {
            HDC h_dc;
            PAINTSTRUCT ps;
        
            h_dc = BeginPaint(h_wnd, &ps);

            HBRUSH old_brush;
            HBRUSH h_brush = PaintTools::TakeBrush(h_dc, window->get_window_background_color(), old_brush);

            PaintTools::PaintFilledRectangle(h_dc, 0, 0, 
                                             window->get_extent().width, 
                                             window->get_extent().height);


            PaintTools::ResetBrush(h_dc, old_brush, h_brush);

            for (auto& graphic : window->get_graphics()) {
                graphic->draw(h_dc, window->get_extent());
            }

            EndPaint(h_wnd, &ps);

            return 0;
        }   }

        return DefWindowProc(h_wnd, msg, w_param, l_param);
    }>();
}

#endif // INIT_MAIN_WINDOW
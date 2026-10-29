#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "window/strategy/IinitWindowStrategy.hpp"

class Window_T {
    static const LPCTSTR window_class_name;
    static bool window_class_registred;

    HINSTANCE            _h_instance;
    HWND                 _h_wnd;
    IinitWindowStrategy* _i_strategy;

    Window_T(HINSTANCE h_instance, 
             IinitWindowStrategy* init_window_strategy) : _h_instance(h_instance),
                                                          _i_strategy(init_window_strategy) {}

public: 
    static Window init(HINSTANCE, IinitWindowStrategy*);
    static void destroy(Window);

    constexpr void set_h_wnd(HWND h_wnd) noexcept {
        _h_wnd = h_wnd;
    }
    constexpr COLORREF get_window_background_color() const noexcept {
        return _i_strategy->get_window_background_color();
    } 

    constexpr std::deque<Igraphic*>& get_graphics() const noexcept {
        return _i_strategy->get_graphics();
    }
    constexpr std::deque<Igraphic*>& get_graphics() noexcept {
        return _i_strategy->get_graphics();
    }
    constexpr std::deque<IhotKey*>& get_hot_key() const noexcept {
        return _i_strategy->get_hot_key();
    }
    constexpr std::deque<IhotKey*>& get_hot_key() noexcept {
        return _i_strategy->get_hot_key();
    }
    WindowExtentPackage get_extent() const noexcept {
        RECT client_rect = {};
        GetClientRect(_h_wnd, &client_rect);

        return WindowExtentPackage {
            .width  = sc<u32>(client_rect.right - client_rect.left),
            .height = sc<u32>(client_rect.bottom - client_rect.top)
        };
    }

    void execute_window(std::function<void(HWND, const MSG&)> func = [](HWND, const MSG&) -> void {}) {
        ShowWindow(_h_wnd, _i_strategy->get_n_cmd_show());
        UpdateWindow(_h_wnd);

        MSG msg;
        BOOL b_ret;

        while ((b_ret = GetMessage(&msg, NULL, 0, 0)) != 0) {
            if (b_ret == -1) throw std::runtime_error("Window_T::execute_window : return error of window.");
            
            func(_h_wnd, msg);

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    template<typename... Args> requires (GET_HANDLER(Igraphic) && ...)
    Window add_graphic(Args*... graphic_stack) {
        ([this](auto* arg) -> void {
            _i_strategy->get_graphics().push_back(arg);
        }(graphic_stack), ...);    
        PostMessage(_h_wnd, WM_APP | MESSAGE_ID_INIT, 0, 0);
        return this;
    }
    template<typename... Args> requires (GET_HANDLER(IhotKey) && ...)
    Window add_hotkey(Args*... hotkey_stack) {
        ([this](auto* arg) -> void {
            _i_strategy->get_hot_key().push_back(arg);
        }(hotkey_stack), ...);      
        PostMessage(_h_wnd, WM_APP | MESSAGE_ID_INIT, 0, 0);
        return this;
    }
    Igraphic* pop_graphic() {
        if (_i_strategy->get_graphics().empty()) return nullptr;
        auto* graphic = _i_strategy->get_graphics().back();
        _i_strategy->get_graphics().pop_back();
        return graphic;
    }
    IhotKey* pop_hotkey() {
        if (_i_strategy->get_hot_key().empty()) return nullptr;
        auto* hotkey = _i_strategy->get_hot_key().back();
        _i_strategy->get_hot_key().pop_back();
        return hotkey;
    }

   ~Window_T() {
        if (_h_wnd) DestroyWindow(_h_wnd);
        if (_i_strategy) delete _i_strategy;
    }
    Window_T(Window_T&& other) noexcept : _h_instance(other._h_instance),
                                          _h_wnd(other._h_wnd),
                                          _i_strategy(other._i_strategy) {
        other._i_strategy = nullptr;
        other._h_instance = nullptr;
        other._h_wnd = nullptr;
    }
    Window_T& operator=(Window_T&& other) noexcept = delete;
    Window_T(const Window_T&) = delete;
    Window_T& operator=(const Window_T&) = delete;
};

inline const LPCTSTR Window_T::window_class_name = TEXT("ChipLayoutVisualizer");
inline bool Window_T::window_class_registred = false;

inline Window Window_T::init(HINSTANCE h_instance, IinitWindowStrategy* i_strategy) {
    if (!window_class_registred) {
        WNDCLASSEX wcex = {
            .cbSize = sizeof(WNDCLASSEX),
            .style = i_strategy->get_window_style(),
            .lpfnWndProc = i_strategy->get_window_protocol(),
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = h_instance,
            .hIcon = LoadIcon(h_instance, IDI_APPLICATION),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
            .lpszMenuName = NULL,
            .lpszClassName = window_class_name,
            .hIconSm = LoadIcon(h_instance, IDI_APPLICATION)
        };
    
        if (!RegisterClassEx(&wcex)) {
            MessageBox(NULL, "Can’t register window class!", "Win32 API Test", 0);
            throw std::runtime_error("Window::Init : can’t register window class.");
        }

        window_class_registred = true;
    }

    Window window = new Window_T(h_instance, i_strategy);

    HWND hwnd = CreateWindow(window_class_name, i_strategy->get_class_name(), 
                             i_strategy->get_instance_style(), 
                             i_strategy->get_window_position_package().pos_x, i_strategy->get_window_position_package().pos_y, 
                             i_strategy->get_window_position_package().width, i_strategy->get_window_position_package().height, 
                             NULL, NULL, h_instance, window);

    window->set_h_wnd(hwnd);

    if (!hwnd) {
        MessageBox(NULL, "Can’t create window!", "Win32 API Test", 0);
        throw std::runtime_error("Window::Init : can’t create window.");
    }

    return window;
}

inline void Window_T::destroy(Window instance) {
    delete instance;
}

#include "window/strategy/realization/InitMainWindow.hpp"

#endif // WINDOW_HPP


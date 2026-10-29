#ifndef INIT_WINDOW_STRATEGY_HPP
#define INIT_WINDOW_STRATEGY_HPP

class Window_T;

struct WindowPositionExtentPackage {
    i32 pos_x; i32 pos_y;
    u32 width; u32 height;
};

class IinitWindowStrategy {
    std::deque<Igraphic*> _graphics;
    std::deque<IhotKey*>  _hot_keys;

protected:
    template<auto Func> requires requires(Window_T* a, HWND b, UINT c, WPARAM d, LPARAM f) { 
        { Func(a, b, c, d, f) } -> std::convertible_to<LRESULT>;
    } constexpr static WNDPROC construct_window_protocol() noexcept;
    
public:
    #define GET_HANDLER(type) std::derived_from<std::remove_pointer_t<Args>, type>

    template<class... Args> requires ((GET_HANDLER(Igraphic) || GET_HANDLER(IhotKey)) && ...)
    constexpr IinitWindowStrategy(Args*... args) {
        ([this](auto* arg) -> void {
            using Arg = std::remove_pointer_t<decltype(arg)>;
            #define PICK_HANDLER(type_upper_case, type_downer_case) if constexpr (std::derived_from<Arg, type_upper_case>) type_downer_case.push_back(arg)
            
            PICK_HANDLER(Igraphic, _graphics);
            PICK_HANDLER(IhotKey,  _hot_keys);
        }(args), ...);
    }

    virtual int get_n_cmd_show() = 0;
    virtual COLORREF get_window_background_color() = 0;
    virtual LPCTSTR get_class_name() = 0;
    virtual UINT get_window_style() = 0;
    virtual WNDPROC get_window_protocol() = 0;
    virtual UINT get_instance_style() = 0;
    virtual WindowPositionExtentPackage get_window_position_package() = 0;

    virtual std::deque<Igraphic*>& get_graphics() {
        return _graphics;
    }
    virtual std::deque<IhotKey*>& get_hot_key() {
        return _hot_keys;
    }

    virtual ~IinitWindowStrategy() {
        for (auto& graphic : _graphics) {
            delete graphic;
        }
        for (auto& hot_key : _hot_keys) {
            delete hot_key;
        }
    };
};

template<auto Func> requires requires(Window_T* a, HWND b, UINT c, WPARAM d, LPARAM f) { 
    { Func(a, b, c, d, f) } -> std::convertible_to<LRESULT>;
} inline constexpr WNDPROC IinitWindowStrategy::construct_window_protocol() noexcept {
    return [](HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) -> LRESULT CALLBACK {
        Window_T* window = nullptr;

        if (msg == WM_NCCREATE) {
            auto* create_struct = reinterpret_cast<CREATESTRUCT*>(l_param);
            window = reinterpret_cast<Window_T*>(create_struct->lpCreateParams);
            SetWindowLongPtr(h_wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        } else {
            window = reinterpret_cast<Window_T*>(GetWindowLongPtr(h_wnd, GWLP_USERDATA));
        }

        return Func(window, h_wnd, msg, w_param, l_param);
    };
}

#endif // INIT_WINDOW_STRATEGY_HPP


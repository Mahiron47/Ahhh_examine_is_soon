#ifndef WINDOWHPP
#define WINDOWHPP

#include "window/strategy/InitWindowStrategy.hpp"

#define Window Window_T*

class Window_T {
    static bool window_class_registred;

    HWND      _hwnd;
    HINSTANCE _hinstance;
    int       _n_cmd_show;
    bool      _is_showed;

    static int64_t __stdcall WindowProtocol(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

    }
public: 
    static Window Init(HINSTANCE hinstance);
    static void Destroy(Window instance);

    void show(std::function<void(const MSG&)> func = [](const MSG&) -> void {}) {
        ShowWindow(_hwnd, _n_cmd_show);
        UpdateWindow(_hwnd);

        MSG msg;

        while (GetMessage(&msg, NULL, 0, 0)) {
            func(msg);

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Window_T(Window_T&& other) noexcept {

    }
    Window_T& operator=(Window_T&& other) noexcept {
        if (this == &other) return *this;

        return *this;
    }
    Window_T(const Window_T&) = delete;
    Window_T& operator=(const Window_T&) = delete;
};

inline bool Window_T::window_class_registred = false;

inline Window Window_T::Init(HINSTANCE hinstance) {
    if (!window_class_registred) {
        WNDCLASSEX wcex = {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = Window_T::WindowProtocol,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = hinstance,
            .hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_APPLICATION)),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
            .lpszMenuName = NULL,
            .lpszClassName = "ClassName",
            .hIconSm = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_APPLICATION))
        };
    
        if (!RegisterClassEx(&wcex)) {
            MessageBox(NULL, "Can’t register window class!", "Win32 API Test", NULL);
            throw std::runtime_error("Window::Init : can’t register window class.");
        }

        window_class_registred = true;
    }

    HWND hwnd = CreateWindow("ClassName", "WindowName", 
                             WS_OVERLAPPEDWINDOW, 
                             CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, 
                             NULL, NULL, hinstance, NULL);

    if (!hwnd) {
      MessageBox(NULL, "Can’t create window!", "Win32 API Test", NULL);
      throw std::runtime_error("Window::Init : can’t create window.");
    }

    return nullptr;
}

inline void Window_T::Destroy(Window instance) {
    delete instance;
}

#endif // WINDOWHPP
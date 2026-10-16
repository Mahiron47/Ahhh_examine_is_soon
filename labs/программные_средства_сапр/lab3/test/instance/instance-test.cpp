#include <catch2/catch_all.hpp>

#include "app/app.hpp"

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {
    Window window = Window_T::init(h_instance, new InitMainWindow(n_cmd_show));
    window->execute_window();

    Window_T::destroy(window);
    return 0;
}
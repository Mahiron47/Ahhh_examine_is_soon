#include "app/app.hpp"

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE, LPSTR, int n_cmd_show) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    FileHandler file_handler = FileHandler_T::init(std::filesystem::current_path().parent_path().parent_path().parent_path() / L"labs" 
                                                   / L"программные_средства_сапр" / L"lab4" / L"instance" / L"solution-maximum");

    Window window = Window_T::init(h_instance, 
                                   new InitMainWindow(n_cmd_show, RGB(255, 255, 255),
                                                      new FilePicker(file_handler, VK_INSERT)));

    window->execute_window([file_handler, window](HWND h_wnd, const MSG& msg) -> void {
        if (msg.hwnd != h_wnd) return;

        switch (msg.message) {
        case WM_APP | APP_FILE_PICKER: {
            window->pop_graphic();
            window->pop_hotkey();

            auto* chip_box = new ChipBox(GraphicShiftPackage {
                .left_border_shift   = 30,
                .right_border_shift  = 30,
                .top_border_shift    = 30,
                .bottom_border_shift = 50
            }, file_handler->get_chip_data());

            window->add_graphic(chip_box)
                  ->add_hotkey(new Click(MK_LBUTTON, chip_box));

            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        }   }
    });

    Window_T::destroy(window);
    FileHandler_T::destroy(file_handler);

    return 0;
}

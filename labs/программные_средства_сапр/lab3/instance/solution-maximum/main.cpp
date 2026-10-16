#include "app/app.hpp"

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE, LPSTR, int n_cmd_show) {
    FileHandler file_handler = FileHandler_T::init(std::filesystem::current_path().parent_path().parent_path().parent_path() / L"labs" 
                                                   / L"программные_средства_сапр" / L"lab3" / L"instance" / L"solution-maximum" / L"simple");
    
    auto* plot_manager = new PlotManager(GraphicShiftPackage {
        .left_border_shift = 30,
        .right_border_shift = 30,
        .top_border_shift = 30,
        .bottom_border_shift = 30
    }, "Plot Manager", RGB(0, 0, 0));

    Window window = Window_T::init(h_instance, 
                                   new InitMainWindow(n_cmd_show, 
                                                      new FilePicker(file_handler, VK_INSERT),
                                                      new Grid(GraphicShiftPackage {
                                                          .left_border_shift = 30,
                                                          .right_border_shift = 30,
                                                          .top_border_shift = 30,
                                                          .bottom_border_shift = 30
                                                      }, CellCountType { 
                                                          .cell_count = 10 
                                                      }, false, RGB(0, 0, 0), new DotGrid(3)),
                                                      plot_manager,
                                                      new Click(MK_LBUTTON, plot_manager)));

    window->execute_window([file_handler, window, plot_manager](HWND h_wnd, const MSG& msg) -> void {
        if (msg.hwnd != h_wnd) return;

        switch (msg.message) {
        case WM_APP | APP_FILE_PICKER: {
            while (!file_handler->get_plot_data().empty()) {
                PlotData plot_data = file_handler->get_plot_data().back();

                plot_manager->add_plot_box(window, h_wnd, plot_data, RGB(0, 0, 0));

                file_handler->get_plot_data().pop_back();
            }

            InvalidateRect(h_wnd, NULL, TRUE);
            break;
        }   }
    });

    Window_T::destroy(window);
    FileHandler_T::destroy(file_handler);
 
    return 0;
}
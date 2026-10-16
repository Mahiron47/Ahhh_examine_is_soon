#ifndef FILE_PICKER_HPP
#define FILE_PICKER_HPP

#define APP_FILE_PICKER 0x0001

class FilePicker : public IhotKey {
    FileHandler _file_handler;

public:
    constexpr FilePicker(FileHandler file_handler,
                         u16 key) : IhotKey(key),
                                    _file_handler(file_handler) {}

    void execute(u32 msg, HWND h_wnd, WPARAM w_param, LPARAM) override {
        if (msg != WM_KEYDOWN) return;

        if (w_param == _key) {
            _file_handler->execute_file_peeker(h_wnd);

            PostMessage(h_wnd, WM_APP | APP_FILE_PICKER, 0, 0);
        }
    }

};

#endif // FILE_PICKER_HPP
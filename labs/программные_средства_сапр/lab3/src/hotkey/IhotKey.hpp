#ifndef I_HOT_KEY_HPP
#define I_HOT_KEY_HPP

class IhotKey {
protected:
    u16 _key;

public:
    constexpr IhotKey(u16 key) : _key(key) {}

    virtual void execute(u32 msg, HWND h_wnd, WPARAM w_param, LPARAM l_param) = 0;

    virtual ~IhotKey() = default;
};  

#include "hotkey/file_picker/FilePicker.hpp"
#include "hotkey/zoom/Zoom.hpp"
#include "hotkey/click/Click.hpp"

#endif // I_HOT_KEY_HPP
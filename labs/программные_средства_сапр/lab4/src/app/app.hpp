#ifndef APPHPP
#define APPHPP

#include <functional>
#include <filesystem>
#include <windows.h>
#include <stdint.h>
#include <iostream>
#include <stdfloat>
#include <concepts>
#include <fstream>
#include <string>
#include <deque>
#include <cmath>
#include <ranges>
#include <map>
#include <set>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef std::float16_t f16; 
typedef std::float32_t f32;
typedef std::float64_t f64; 

#define MESSAGE_ID_UPDATE  0
#define MESSAGE_ID_RESIZE  1
#define MESSAGE_ID_INIT    2
#define MESSAGE_ID_CLICK   4

typedef struct ClickMessagePackage {
    u16 key;
} ClickMessagePackage;

#define MESSAGE_ID_MOVE    5
#define MESSAGE_ID_ZOOM    6

typedef struct ZoomMessagePackage {
    HWND h_wnd;
    f32 bidirectional_strength;
    u16 additional_key;
} ZoomMessagePackage;

template <typename To, typename From>
constexpr To sc(From&& value) noexcept {
    return static_cast<To>(std::forward<From>(value));
}

struct WindowExtentPackage {
    u32 width;
    u32 height;
};

template<typename T> 
struct Point {
    T x;
    T y;
};

class Window_T;

#define Window Window_T*

#include "paint/PaintTools.hpp"
#include "input/chip_data/ChipData.hpp"
#include "input/FileHandler.hpp"

#include "graphic/Igraphic.hpp"
#include "hotkey/IhotKey.hpp"

#include "window/Window.hpp"

#endif // APPHPP


#ifndef IGRAPHIC_HPP
#define IGRAPHIC_HPP

typedef struct GraphicPositionExtentPackage {
    u32 pos_x = 0;
    u32 pos_y = 0;
    u32 width = MAXUINT32;
    u32 height = MAXUINT32;
} GraphicPositionExtentPackage;

typedef struct GraphicShiftPackage {
    u32 left_border_shift = 0;
    u32 right_border_shift = 0;
    u32 top_border_shift = 0;
    u32 bottom_border_shift = 0;
} GraphicShiftPackage;

typedef struct Message {
    u32 message_id;
    void* message_data;
} Message;

class Igraphic {
protected:
    bool     _update = true;
    COLORREF _color;

public:
    constexpr Igraphic(COLORREF color) : _color(color) {}

    virtual void init_runtime(HWND, const WindowExtentPackage&) {
        return;
    }
    virtual void send_message(Point<u32> mouse_position, Message message, const WindowExtentPackage& actual_paint_window_extent) = 0;
    virtual void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) = 0;

    virtual ~Igraphic() = default;
};

#define SUPPORT_PACKAGE(package_type) std::same_as<package_type, std::remove_reference_t<LayoutT>>
#define SUPPORT_PACKAGE_N(package_type) std::same_as<package_type, std::remove_reference_t<NewLayoutT>>
#define HANDLE_PACKAGE(package_type, ...) if constexpr (std::same_as<package_type, LayoutT>) { \
            __VA_ARGS__ \
        } // HANDLE_PACKAGE
#define HANDLE_PACKAGE_N(package_type, ...) if constexpr (std::same_as<package_type, NewLayoutT>) { \
            __VA_ARGS__ \
        } // HANDLE_PACKAGE

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class IgraphicLayout : public Igraphic {
    LayoutT _layout;

protected:
    constexpr IgraphicLayout(const LayoutT& layout,
                             COLORREF color) : Igraphic(color),
                                               _layout(layout) {}

public:
    template<typename NewLayoutT> requires SUPPORT_PACKAGE_N(GraphicPositionExtentPackage) || SUPPORT_PACKAGE_N(GraphicShiftPackage)
    void set_layout(const NewLayoutT& layout, const WindowExtentPackage& actual_paint_window_extent) {
        HANDLE_PACKAGE_N(GraphicPositionExtentPackage, 
        HANDLE_PACKAGE(GraphicPositionExtentPackage,
            if (_layout.width == MAXUINT32) _layout.width = actual_paint_window_extent.width - layout.pos_x;
            if (_layout.height == MAXUINT32) _layout.height = actual_paint_window_extent.height - layout.pos_y;

            _layout = layout;
        )
        HANDLE_PACKAGE(GraphicShiftPackage,
            _layout = GraphicShiftPackage {
                .left_border_shift   = layout.pos_x,
                .right_border_shift  = actual_paint_window_extent.width  - (layout.width  + layout.pos_x),
                .top_border_shift    = layout.pos_y,
                .bottom_border_shift = actual_paint_window_extent.height - (layout.height + layout.pos_y)
            };
        )   )
        HANDLE_PACKAGE_N(GraphicShiftPackage, 
        HANDLE_PACKAGE(GraphicShiftPackage,
            _layout = layout;
        )
        HANDLE_PACKAGE(GraphicPositionExtentPackage,
            if (_layout.width == MAXUINT32) layout.width = actual_paint_window_extent.width - layout.pos_x;
            if (_layout.height == MAXUINT32) layout.height = actual_paint_window_extent.height - layout.pos_y;

            _layout = GraphicShiftPackage {
                .left_border_shift   = layout.pos_x,
                .top_border_shift    = layout.pos_y,
                .right_border_shift  = actual_paint_window_extent.width  - (layout.pos_x + layout.width),
                .bottom_border_shift = actual_paint_window_extent.height - (layout.pos_y + layout.height)
            };
        )   )
    }
    template<typename NewLayoutT> requires SUPPORT_PACKAGE_N(GraphicPositionExtentPackage) || SUPPORT_PACKAGE_N(GraphicShiftPackage)
    NewLayoutT get_position_extent(const WindowExtentPackage& actual_paint_window_extent) {
        HANDLE_PACKAGE_N(GraphicPositionExtentPackage, 
        HANDLE_PACKAGE(GraphicPositionExtentPackage,
            if (_layout.width == MAXUINT32) _layout.width = actual_paint_window_extent.width - _layout.pos_x;
            if (_layout.height == MAXUINT32) _layout.height = actual_paint_window_extent.height - _layout.pos_y;

            return _layout;
        )
        HANDLE_PACKAGE(GraphicShiftPackage,
            return GraphicPositionExtentPackage {
                .pos_x = _layout.left_border_shift,
                .pos_y = _layout.top_border_shift,
                .width = actual_paint_window_extent.width   - (_layout.left_border_shift + _layout.right_border_shift),
                .height = actual_paint_window_extent.height - (_layout.top_border_shift  + _layout.bottom_border_shift)
            };
        )   )
        HANDLE_PACKAGE_N(GraphicShiftPackage, 
        HANDLE_PACKAGE(GraphicShiftPackage,
            return _layout;
        )
        HANDLE_PACKAGE(GraphicPositionExtentPackage,
            if (_layout.width == MAXUINT32) _layout.width = actual_paint_window_extent.width - _layout.pos_x;
            if (_layout.height == MAXUINT32) _layout.height = actual_paint_window_extent.height - _layout.pos_y;
 
            return GraphicShiftPackage {
                .left_border_shift = _layout.pos_x,
                .top_border_shift = _layout.pos_y,
                .right_border_shift = actual_paint_window_extent.width   - (_layout.pos_x + _layout.width),
                .bottom_border_shift = actual_paint_window_extent.height - (_layout.pos_y + _layout.height)
            };
        )   )
    }

};

#include "chip_segment/macros/Macros.hpp"
#include "chip_segment/net/Net.hpp"
#include "chip_box/ChipBox.hpp"

#endif // IGRAPHIC_HPP
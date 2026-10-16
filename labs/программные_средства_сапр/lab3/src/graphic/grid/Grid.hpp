#ifndef GRID_HPP
#define GRID_HPP

struct CellSizeType {
    u32 cell_size;
};
struct CellCountType {
    u32 cell_count;
};

#include "graphic/grid/strategy/IgridPaintStrategy.hpp"

#include "graphic/grid/strategy/dot_grid/DotGrid.hpp"
#include "graphic/grid/strategy/line_grid/LineGrid.hpp"

template<typename LayoutT, typename CellSizeT> requires (SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)) &&
                                                        (std::same_as<CellSizeT, CellSizeType> || std::same_as<CellSizeT, CellCountType>)
class Grid : public IgraphicLayout<LayoutT> {
    bool                _is_erase;
    CellSizeT           _cell_factor;
    IgridPaintStrategy* _paint_strategy;

public:
    constexpr Grid(LayoutT layout, 
                   CellSizeT cell_factor, 
                   COLORREF color,
                   bool is_erase,
                   IgridPaintStrategy* paint_strategy) : IgraphicLayout<LayoutT>(layout, color),
                                                         _is_erase(is_erase),
                                                         _cell_factor(cell_factor), 
                                                         _paint_strategy(paint_strategy) {}

    void send_message(Point<u32>, Message, const WindowExtentPackage&) override {
        return;
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, RGB(255, 255, 255), old_pen);
        HBRUSH old_brush;
        HBRUSH brush = PaintTools::TakeBrush(h_dc, RGB(255, 255, 255), old_brush);

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);
        
        if (_is_erase) PaintTools::PaintFilledRectangle(h_dc, position_extent.pos_x, position_extent.pos_y, 
                                                              position_extent.pos_x + position_extent.width, 
                                                              position_extent.pos_y + position_extent.height);
                                               
        PaintTools::ResetPen(h_dc, old_pen, pen);
        PaintTools::ResetBrush(h_dc, old_brush, brush);
        
        _paint_strategy->paint_grid(h_dc, position_extent, _cell_factor, this->_color);
    }

    ~Grid() {
        delete _paint_strategy;
    }

    Grid(const Grid&&) = delete;
    Grid& operator=(const Grid&&) = delete;
    Grid(const Grid&) = delete;
    Grid& operator=(const Grid&) = delete;
};

#endif // GRID_HPP
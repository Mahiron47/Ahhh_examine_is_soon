#ifndef I_GRID_PAINT_STRATEGY_HPP
#define I_GRID_PAINT_STRATEGY_HPP

class IgridPaintStrategy {
protected:
    u16 _line_width;
    COLORREF _color;

public:
    constexpr IgridPaintStrategy(u16 line_width = 1, COLORREF color = RGB(0, 0, 0)) : _line_width(line_width), _color(color) {}
    
    virtual void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, 
                            CellSizeType cell_factor) = 0;
    virtual void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, 
                            CellCountType cell_factor) = 0;

    virtual ~IgridPaintStrategy() = default;
};

#endif // I_GRID_PAINT_STRATEGY_HPP
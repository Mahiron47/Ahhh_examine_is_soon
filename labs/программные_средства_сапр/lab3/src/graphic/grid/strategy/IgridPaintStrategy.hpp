#ifndef I_GRID_PAINT_STRATEGY_HPP
#define I_GRID_PAINT_STRATEGY_HPP

class IgridPaintStrategy {
protected:
    u16 _line_width;

public:
    constexpr IgridPaintStrategy(u16 line_width = 1) : _line_width(line_width) {}
    
    virtual void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, 
                            CellSizeType cell_factor, COLORREF color) = 0;
    virtual void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, 
                            CellCountType cell_factor, COLORREF color) = 0;

    virtual ~IgridPaintStrategy() = default;
};

#endif // I_GRID_PAINT_STRATEGY_HPP
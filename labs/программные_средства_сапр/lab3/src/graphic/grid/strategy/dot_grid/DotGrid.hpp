#ifndef DOT_GRID_HPP
#define DOT_GRID_HPP

class DotGrid : public IgridPaintStrategy {
public:
    constexpr DotGrid(u16 line_width) : IgridPaintStrategy(line_width) {}

    void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, CellSizeType cell_factor, COLORREF color) override {
        for (f64 x = position_extent_package.pos_x; sc<u32>(x) <= position_extent_package.pos_x + position_extent_package.width - this->_line_width; x += cell_factor.cell_size) {
            for (f64 y = position_extent_package.pos_y; sc<u32>(y) <= position_extent_package.pos_y + position_extent_package.height - this->_line_width; y += cell_factor.cell_size) {
                PaintTools::PaintDot(h_dc, sc<u32>(x), sc<u32>(y), _line_width, color);
        }   }
    }
    void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, CellCountType cell_factor, COLORREF color) override {
        u32 width = position_extent_package.width - this->_line_width;
        u32 height = position_extent_package.height - this->_line_width;

        f64 cell_size_x = width / sc<f64>(cell_factor.cell_count);
        f64 cell_size_y = height / sc<f64>(cell_factor.cell_count);

        for (f64 x = position_extent_package.pos_x; sc<u32>(x) <= position_extent_package.pos_x + width; x += cell_size_x) {
            for (f64 y = position_extent_package.pos_y; sc<u32>(y) <= position_extent_package.pos_y + height; y += cell_size_y) {
                PaintTools::PaintDot(h_dc, sc<u32>(x), sc<u32>(y), _line_width, color);
        }   }
    }

    DotGrid(const DotGrid&&) = delete;
    DotGrid& operator=(const DotGrid&&) = delete;
    DotGrid(const DotGrid&) = delete;
    DotGrid& operator=(const DotGrid&) = delete;
};

#endif // DOT_GRID_HPP
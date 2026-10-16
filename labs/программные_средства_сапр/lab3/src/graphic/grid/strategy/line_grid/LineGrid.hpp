#ifndef LINE_GRID_HPP
#define LINE_GRID_HPP

class LineGrid : public IgridPaintStrategy {
public:
    constexpr LineGrid(u16 line_width) : IgridPaintStrategy(line_width) {}

    void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, CellSizeType cell_factor, COLORREF color) override {
        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, color, old_pen);

        f64 width  = position_extent_package.width - 0.5f;
        f64 height = position_extent_package.height - 0.5f;

        for (f32 x = position_extent_package.pos_x; sc<u32>(x) <= position_extent_package.pos_x + width; x += cell_factor.cell_size) {
            PaintTools::PaintLine(h_dc, sc<u32>(x), position_extent_package.pos_y, 
                                        sc<u32>(x), position_extent_package.pos_y + height);
        }

        for (f32 y = position_extent_package.pos_y; sc<u32>(y) <= position_extent_package.pos_y + height; y += cell_factor.cell_size) {
            PaintTools::PaintLine(h_dc, position_extent_package.pos_x, sc<u32>(y), 
                                        position_extent_package.pos_x + width, sc<u32>(y));
        }
     
        PaintTools::ResetPen(h_dc, old_pen, pen);
    }
    void paint_grid(HDC h_dc, const GraphicPositionExtentPackage& position_extent_package, CellCountType cell_factor, COLORREF color) override {
        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, color, old_pen);

        f64 width  = position_extent_package.width - 0.5f;
        f64 height = position_extent_package.height - 0.5f;

        f64 cell_size_x = width / sc<f64>(cell_factor.cell_count);
        f64 cell_size_y = height / sc<f64>(cell_factor.cell_count);

        for (f64 x = position_extent_package.pos_x; sc<u32>(x) <= position_extent_package.pos_x + width; x += cell_size_x) {
            PaintTools::PaintLine(h_dc, sc<u32>(x), position_extent_package.pos_y, 
                                        sc<u32>(x), position_extent_package.pos_y + height);
        }

        for (f64 y = position_extent_package.pos_y; sc<u32>(y) <= position_extent_package.pos_y + height; y += cell_size_y) {
            PaintTools::PaintLine(h_dc, position_extent_package.pos_x, sc<u32>(y), 
                                        position_extent_package.pos_x + width, sc<u32>(y));
        }

        PaintTools::ResetPen(h_dc, old_pen, pen);
    }

    LineGrid(const LineGrid&&) = delete;
    LineGrid& operator=(const LineGrid&&) = delete;
    LineGrid(const LineGrid&) = delete;
    LineGrid& operator=(const LineGrid&) = delete;
};

#endif // LINE_GRID_HPP
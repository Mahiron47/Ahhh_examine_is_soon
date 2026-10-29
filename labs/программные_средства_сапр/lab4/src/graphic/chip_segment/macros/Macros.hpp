#ifndef MACROS_HPP
#define MACROS_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class Macros : public IgraphicLayout<LayoutT> {
    std::string     _instance_name;
    bool            _is_placed;
    Point<u32>      _size;

public:
    static constexpr i32 apply_orientation(const std::string& orientation, Point<u32>& size) {
        if (macro.orientation == "W"  || macro.orientation == "E" ||
            macro.orientation == "FW" || macro.orientation == "FE") {
            std::swap(size.x, size.y);
        }
    }

    Macros(LayoutT layout,
           const DefComponent& component, 
           const LefMacro& macro, 
           ChipData chip_data) : IgraphicLayout<LayoutT>(layout, RGB(0, 0, 0)),
                                 _instance_name(component.instance_name),
                                 _is_placed(component.status == "PLACED" || component.status == "FIXED"),
                                 _size(Point<u32> {
                                     .x = sc<u32>(macro.width * chip_data->get_lef_data().database_microns),
                                     .y = sc<u32>(macro.height * chip_data->get_lef_data().database_microns)
                                 }) {}

    bool check_name(const std::string& name) const {
        return _instance_name == name;
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, this->get_color(), old_pen, 1);
        HBRUSH old_brush;
        HBRUSH brush = PaintTools::TakeBrush(h_dc, this->get_color(), old_brush);

        if (_is_placed) {
            PaintTools::PaintFilledRectangle(h_dc, position_extent.pos_x, 
                                                   position_extent.pos_y, 
                                                   position_extent.pos_x + _size.x, 
                                                   position_extent.pos_y + _size.y);
        } 
        
        PaintTools::ReturnPen(h_dc, old_pen);
        PaintTools::ReturnBrush(h_dc, old_brush);
    }
};

#endif // MACROS_HPP
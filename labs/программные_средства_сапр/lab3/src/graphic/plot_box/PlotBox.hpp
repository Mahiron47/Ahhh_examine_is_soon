#ifndef PLOT_BOX_HPP
#define PLOT_BOX_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class PlotBox : public IgraphicLayout<LayoutT> {
    Grid<LayoutT, CellCountType>* _grid;
    Plot<LayoutT>*                _plot;

    void draw_name(HDC h_dc, const GraphicPositionExtentPackage& position_extent) {
        SIZE ts;

        GetTextExtentPoint32A(h_dc, _plot->get_data_original().name.c_str(), sc<int>(_plot->get_data_original().name.size()), &ts);

        PaintTools::PaintText(h_dc, _plot->get_data_original().name, position_extent.pos_x + 0.5f * (position_extent.width - ts.cx), position_extent.pos_y + 2);
    }

    void draw_marks(HDC h_dc, const GraphicPositionExtentPackage& position_extent) {
        const POINT* __restrict points            = _plot->get_data_original().points;
        const POINT* __restrict normalised_points = _plot->get_data_normalised().points;

        u32 line_width = 1;
        u32 line_lenght = 3;

        for (u32 i = 0; i < _plot->get_data_original().num_points; ++i) {
            SIZE ts1;
            SIZE ts2;

            GetTextExtentPoint32A(h_dc, std::to_string(points[i].x).c_str(), sc<int>(std::to_string(points[i].x).size()), &ts1);
            GetTextExtentPoint32A(h_dc, std::to_string(points[i].y).c_str(), sc<int>(std::to_string(points[i].y).size()), &ts2);
            
            PaintTools::PaintFilledRectangle(h_dc, normalised_points[i].x + line_width, position_extent.pos_y + position_extent.height - ts1.cy - 10 + line_lenght,
                                                   normalised_points[i].x - line_width, position_extent.pos_y + position_extent.height - ts1.cy - 10 - line_lenght);
            PaintTools::PaintText(h_dc, std::to_string(points[i].x), normalised_points[i].x - 0.5f * ts1.cx, position_extent.pos_y + position_extent.height - 0.5f * ts1.cy - 12);
            PaintTools::PaintFilledRectangle(h_dc, position_extent.pos_x + position_extent.width - ts2.cx - 10 + line_lenght, normalised_points[i].y + line_width,
                                                   position_extent.pos_x + position_extent.width - ts2.cx - 10 - line_lenght, normalised_points[i].y - line_width);
            PaintTools::PaintText(h_dc, std::to_string(points[i].y), position_extent.pos_x + position_extent.width - 0.5f * ts2.cx - 8, normalised_points[i].y - 0.5f * ts2.cy);
    }   }

public:
    constexpr PlotBox(const LayoutT& layout,
                      PlotData plot_data,
                      COLORREF color = RGB(0, 0, 0)) : IgraphicLayout<LayoutT>(layout, color) {
        this->_grid = new Grid(layout, CellCountType {
            .cell_count = plot_data->get_data().num_points + 1
        }, false, RGB(0, 0, 0), new LineGrid(3));
        this->_plot = new Plot(layout, plot_data, Point<f32> {
            .x = 0.05f,
            .y = 0.05f
        }, color);
    }

    void init_runtime(HWND h_wnd, const WindowExtentPackage& window_extent) override {
        HDC h_dc;

        const auto& plot_data = _plot->get_data_original();
        const auto& shifts = this->template get_position_extent<GraphicShiftPackage>(window_extent);
        
        h_dc = GetDC(h_wnd);

        SIZE ts_name;

        Point<SIZE> ts = Point<SIZE> {
            .x = SIZE {
                .cx = 0,
                .cy = 0
            },
            .y = SIZE {
                .cx = 0,
                .cy = 0
            }
        };

        GetTextExtentPoint32A(h_dc, plot_data.name.c_str(), sc<int>(plot_data.name.size()), &ts_name);
        GetTextExtentPoint32A(h_dc, std::to_string(plot_data.points[0].x).c_str(), sc<int>(std::to_string(plot_data.points[0].x).size()), &ts.x);
        GetTextExtentPoint32A(h_dc, std::to_string(plot_data.points[0].y).c_str(), sc<int>(std::to_string(plot_data.points[0].y).size()), &ts.y);
        
        Point<u32> max_lenght = Point<u32> {
            .x = sc<u32>(ts.y.cx),
            .y = sc<u32>(ts.x.cy)
        };

        for (u32 i = 1; i < plot_data.num_points; ++i) {
            GetTextExtentPoint32A(h_dc, std::to_string(plot_data.points[i].x).c_str(), sc<int>(std::to_string(plot_data.points[i].x).size()), &ts.x);
            GetTextExtentPoint32A(h_dc, std::to_string(plot_data.points[i].y).c_str(), sc<int>(std::to_string(plot_data.points[i].y).size()), &ts.y);
            
            max_lenght = Point<u32> {
                .x = std::max(max_lenght.x, sc<u32>(ts.y.cx)),
                .y = std::max(max_lenght.y, sc<u32>(ts.x.cy))
            };
        }

        ReleaseDC(h_wnd, h_dc);

        _grid->set_layout(GraphicShiftPackage {
            .left_border_shift = shifts.left_border_shift,
            .right_border_shift = shifts.right_border_shift + max_lenght.x + 10,
            .top_border_shift = shifts.top_border_shift + ts_name.cy + 6,
            .bottom_border_shift = shifts.bottom_border_shift + max_lenght.y + 10
        }, window_extent);
        _plot->set_layout(GraphicShiftPackage {
            .left_border_shift = shifts.left_border_shift,
            .right_border_shift = shifts.right_border_shift + max_lenght.x + 10,
            .top_border_shift = shifts.top_border_shift + ts_name.cy + 6,
            .bottom_border_shift = shifts.bottom_border_shift + max_lenght.y + 10
        }, window_extent);

        _grid->init_runtime(h_wnd, window_extent);
        _plot->init_runtime(h_wnd, window_extent);
    }

    void send_message(Point<u32> mouse_pos, Message msg, const WindowExtentPackage& window_extent) override {
        _plot->send_message(mouse_pos, msg, window_extent);

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        if ((mouse_pos.x < position_extent.pos_x || mouse_pos.x > position_extent.pos_x + position_extent.width ||
             mouse_pos.y < position_extent.pos_y || mouse_pos.y > position_extent.pos_y + position_extent.height) && 
            (mouse_pos.x != UINT32_MAX && mouse_pos.y != UINT32_MAX)) return;

        switch (msg.message_id) {
        case MESSAGE_ID_UPDATE:
            this->_update = true;   
            break;
        case MESSAGE_ID_CLICK:
            break;
        case MESSAGE_ID_ZOOM:
            break;
        }
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        int save_dc = SaveDC(h_dc);

        IntersectClipRect(h_dc, position_extent.pos_x, position_extent.pos_y, 
                                position_extent.pos_x + position_extent.width, 
                                position_extent.pos_y + position_extent.height);

        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, RGB(255, 255, 255), old_pen);
        HBRUSH old_brush;
        HBRUSH brush = PaintTools::TakeBrush(h_dc, RGB(255, 255, 255), old_brush);

        PaintTools::PaintFilledRectangle(h_dc, position_extent.pos_x, position_extent.pos_y, 
                                               position_extent.pos_x + position_extent.width, 
                                               position_extent.pos_y + position_extent.height);

        PaintTools::ResetBrush(h_dc, old_brush, brush);
        PaintTools::ResetPen(h_dc, old_pen, pen);

        _grid->draw(h_dc, actual_paint_window_extent);
        _plot->draw(h_dc, actual_paint_window_extent);

        pen = PaintTools::TakePen(h_dc, RGB(0, 0, 0), old_pen);
        brush = PaintTools::TakeBrush(h_dc, RGB(0, 0, 0), old_brush);

        this->draw_name(h_dc, position_extent);
        this->draw_marks(h_dc, position_extent);

        PaintTools::ResetBrush(h_dc, old_brush, brush);
        PaintTools::ResetPen(h_dc, old_pen, pen);

        RestoreDC(h_dc, save_dc);
    }

   ~PlotBox() {
        if (_grid) delete _grid;
        if (_plot) delete _plot;
    }

    PlotBox(const PlotBox&&) = delete;
    PlotBox& operator=(const PlotBox&&) = delete;
    PlotBox(const PlotBox&) = delete;
    PlotBox& operator=(const PlotBox&) = delete;
};

#endif // PLOT_BOX_HPP
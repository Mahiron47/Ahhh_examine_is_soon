#ifndef PLOT_HPP
#define PLOT_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class Plot : public IgraphicLayout<LayoutT> {
    PlotData   _data;
    PlotData   _normalised_data;
    Point<f32> _shift_percent;
    Point<f32> _scale;
    Point<f32> _shift;
    Point<i32> _min;
    Point<f32> _inv_range; 

    void update_coordinates(const GraphicPositionExtentPackage& position_extent) {
        Point<f32> factor = Point<f32> {
            .x = position_extent.width  * (1.0f - 2.0f * _shift_percent.x) * _scale.x * _inv_range.x,
            .y = position_extent.height * (1.0f - 2.0f * _shift_percent.y) * _scale.y * _inv_range.y
        };
        Point<f32> offset = Point<f32> {
            .x = sc<f32>(position_extent.pos_x) + _shift_percent.x * position_extent.width,
            .y = sc<f32>(position_extent.pos_y) + (1.0f - _shift_percent.y) * position_extent.height
        };
        const POINT* __restrict src = _data->get_data().points;
        POINT*       __restrict dst = _normalised_data->get_data().points;
        const u32               n   = _data->get_data().num_points;

        for (u32 i = 0; i < n; ++i) {
            dst[i] = POINT {
                .x = sc<LONG>((src[i].x - _min.x) * factor.x + offset.x + _shift.x),
                .y = sc<LONG>(offset.y - (src[i].y - _min.y) * factor.y + _shift.y)
            };
        }
    }
public:
    constexpr Plot(const LayoutT& layout,
                   PlotData data,
                   Point<f32> shift_percent, 
                   COLORREF color = RGB(0, 0, 0)) : IgraphicLayout<LayoutT>(layout, color), 
                                                    _data(data),
                                                    _shift_percent(shift_percent),
                                                    _scale(Point<f32> {
                                                        .x = 1.0f, 
                                                        .y = 1.0f 
                                                    }),
                                                    _shift(Point<f32> {
                                                        .x = 0.0f, 
                                                        .y = 0.0f 
                                                    }) {
        const auto raw = _data->get_data();
        Point<i32> max = { 
            .x = sc<i32>(raw.points[0].x), 
            .y = sc<i32>(raw.points[0].y) 
        };
        _min = max;

        for (u32 i = 1; i < raw.num_points; ++i) {
            max = Point<i32> {
                .x = std::max(sc<i32>(max.x), sc<i32>(raw.points[i].x)),
                .y = std::max(sc<i32>(max.y), sc<i32>(raw.points[i].y))
            };
            _min = Point<i32> {
                .x = std::min(sc<i32>(_min.x), sc<i32>(raw.points[i].x)),
                .y = std::min(sc<i32>(_min.y), sc<i32>(raw.points[i].y))
            };
        }

        Point<u32> range = { 
            .x = sc<u32>(max.x - _min.x), 
            .y = sc<u32>(max.y - _min.y) 
        };

        _inv_range = Point<f32> {
            .x = (max.x != _min.x) ? 1.0f / sc<f32>(range.x) : 1.0f,
            .y = (max.y != _min.y) ? 1.0f / sc<f32>(range.y) : 1.0f
        };
        _normalised_data = PlotData_T::init(_data);
    }
    
    constexpr PlotDataPack get_data_original() const noexcept {
        return _data->get_data();
    }
    constexpr PlotDataPack get_data_normalised() const noexcept {
        return _normalised_data->get_data();
    }

    void init_runtime(HWND, const WindowExtentPackage& window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        this->update_coordinates(position_extent);
    }

    void send_message(Point<u32> mouse_position, Message message, const WindowExtentPackage& actual_paint_window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);
        
        if ((mouse_position.x < position_extent.pos_x || mouse_position.x > position_extent.pos_x + position_extent.width ||
             mouse_position.y < position_extent.pos_y || mouse_position.y > position_extent.pos_y + position_extent.height) && 
            (mouse_position.x != MAXUINT32 && mouse_position.y != MAXUINT32)) return;

        switch (message.message_id) {
        case MESSAGE_ID_UPDATE: 
            this->_update = true;
            return;
        case MESSAGE_ID_ZOOM: {
            const auto& zoom_data = *reinterpret_cast<ZoomMessagePackage*>(message.message_data);
            
            f32 target_zoom = (zoom_data.bidirectional_strength > 0)
                ? (1.0f + zoom_data.bidirectional_strength)
                : (1.0f / (1.0f - zoom_data.bidirectional_strength));

            Point<f32> old_scale = _scale;

            _scale = Point<f32> {
                .x = std::max(sc<f32>(1.0f), _scale.x * target_zoom),
                .y = std::max(sc<f32>(1.0f), _scale.y * target_zoom)
            };

            if (_scale.x <= 1.0f && _scale.y <= 1.0f) {
                _shift = { 0.0f, 0.0f };
                this->_update = true;
                InvalidateRect(zoom_data.h_wnd, nullptr, TRUE);
                return;
            }

            f32 k_x = _scale.x / old_scale.x;
            f32 k_y = _scale.y / old_scale.y;

            Point<f32> offset = Point<f32> {
                .x = sc<f32>(position_extent.pos_x) + position_extent.width * _shift_percent.x,
                .y = sc<f32>(position_extent.pos_y) + (1.0f - _shift_percent.y) * position_extent.height
            };
            Point<f32> rel = Point<f32> {
                .x = sc<f32>(mouse_position.x) - offset.x,
                .y = sc<f32>(mouse_position.y) - offset.y
            };

            _shift = Point<f32> {
                .x = rel.x - (rel.x - _shift.x) * k_x,
                .y = rel.y - (rel.y - _shift.y) * k_y
            };

            f32 usable_width  = position_extent.width  * (1.0f - 2.0f * _shift_percent.x);
            f32 usable_height = position_extent.height * (1.0f - 2.0f * _shift_percent.y);
            f32 max_shift_x = usable_width  * (_scale.x - 1.0f);
            f32 max_shift_y = usable_height * (_scale.y - 1.0f);

            _shift.x = std::clamp(_shift.x, -max_shift_x, sc<f32>(0.0f));
            _shift.y = std::clamp(_shift.y, sc<f32>(0.0f), max_shift_y);

            this->_update = true;
            InvalidateRect(zoom_data.h_wnd, nullptr, TRUE);
            return;
        }   }
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        this->update_coordinates(position_extent);
    
        HPEN old_pen;
        HPEN pen = PaintTools::TakePen(h_dc, this->_color, old_pen);
        HBRUSH old_brush;
        HBRUSH brush = PaintTools::TakeBrush(h_dc, this->_color, old_brush);
        
        int save_dc = SaveDC(h_dc);

        IntersectClipRect(h_dc, position_extent.pos_x, position_extent.pos_y, 
                                position_extent.pos_x + position_extent.width, 
                                position_extent.pos_y + position_extent.height);

        PaintTools::PaintPolyLine(h_dc, _normalised_data->get_data().points, _normalised_data->get_data().num_points); 

        RestoreDC(h_dc, save_dc);

        PaintTools::ResetPen(h_dc, old_pen, pen);
        PaintTools::ResetBrush(h_dc, old_brush, brush);
    }

   ~Plot() {
        if (_data) PlotData_T::destroy(_data);
        if (_normalised_data) PlotData_T::destroy(_normalised_data);
    }
    Plot(const Plot&&) = delete;
    Plot& operator=(const Plot&&) = delete;
    Plot(const Plot&) = delete;
    Plot& operator=(const Plot&) = delete;
};

#endif // PLOT_HPP
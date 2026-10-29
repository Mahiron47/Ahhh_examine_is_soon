#ifndef CHIP_BOX_HPP
#define CHIP_BOX_HPP

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
class ChipBox : public IgraphicLayout<LayoutT> {
    ChipData                    _chip_data;
    std::deque<Net<LayoutT>>    _nets;
    std::deque<Macros<LayoutT>> _macroses;

public:
    constexpr ChipBox(const LayoutT& layout,
                      ChipData chip_data,
                      COLORREF color = RGB(0, 0, 0)) : IgraphicLayout<LayoutT>(layout, color),
                                                       _chip_data(chip_data) {}

    void send_message(Point<u32> mouse_pos, Message msg, const WindowExtentPackage& window_extent) override {
        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);

        if ((mouse_pos.x < position_extent.pos_x || mouse_pos.x >= position_extent.pos_x + position_extent.width ||
             mouse_pos.y < position_extent.pos_y || mouse_pos.y >= position_extent.pos_y + position_extent.height) && 
            (mouse_pos.x != UINT32_MAX && mouse_pos.y != UINT32_MAX)) return;

        if (msg.message_id == MESSAGE_ID_CLICK) {
            
        }
    }

    void init_runtime(HWND hwnd, const WindowExtentPackage& window_extent) override {
        if (_chip_data == nullptr) return;

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(window_extent);
        
        const auto& def_data = _chip_data->get_def_data();
        const auto& lef_data = _chip_data->get_lef_data();

        Point<f64> die_size = Point<f64> {
            .x = sc<f64>(def_data.die_area.width()),
            .y = sc<f64>(def_data.die_area.height())
        };
        if (die_size.x <= 0.0 || die_size.y <= 0.0) return;

        f64 scale = std::min(sc<f64>(position_extent.width)  / die_size.x, 
                             sc<f64>(position_extent.height) / die_size.y);

        Point<f64> fit_size = Point<f64> {
            .x = die_size.x * scale,
            .y = die_size.y * scale
        };

        Point<f64> offset = Point<f64> {
            .x = position_extent.pos_x + (sc<f64>(position_extent.width)  - fit_size.x) * 0.5,
            .y = position_extent.pos_y + (sc<f64>(position_extent.height) - fit_size.y) * 0.5
        };

        this->_macroses.clear();
        this->_nets.clear();

        for (const auto& macro : def_data.components) {
            auto it = lef_data.macros.find(macro.macro_name);
            if (it == lef_data.macros.end()) continue;
            const auto& lef_macro = it->second;

            Point<f64> macro_size = Point<f64> {
                .x = lef_macro.width  * lef_data.database_microns,
                .y = lef_macro.height * lef_data.database_microns
            };

            Macros::apply_orientation(macro.orientation, macro_size);

            Point<f64> screen_pos = Point<f64> {
                .x = offset.x + (macro.position.x - def_data.die_area.top_left.x) * scale,
                .y = offset.y + (def_data.die_area.bottom_right.y - (macro.position.y + macro_size.y)) * scale
            };

            Point<f64> screen_size = Point<f64> {
                .x = std::max(2.0, macro_size.x * scale),
                .y = std::max(2.0, macro_size.y * scale)
            };

            this->_macroses.emplace_back(GraphicPositionExtentPackage {
                .pos_x  = sc<u32>(std::max(0.0, std::round(screen_pos.x))),
                .pos_y  = sc<u32>(std::max(0.0, std::round(screen_pos.y))),
                .width  = sc<u32>(std::max(1.0, std::round(screen_size.x))),
                .height = sc<u32>(std::max(1.0, std::round(screen_size.y)))
            }, macro, lef_macro, _chip_data);
        }

        for (const auto& net : def_data.nets) {
            _nets.emplace_back(GraphicPositionExtentPackage {
                .pos_x  = sc<u32>(std::max(0.0, std::round(offset.x))),
                .pos_y  = sc<u32>(std::max(0.0, std::round(offset.y))),
                .width  = sc<u32>(std::max(1.0, std::round(fit_size.x))),
                .height = sc<u32>(std::max(1.0, std::round(fit_size.y)))
            }, net, _chip_data, _macroses);
        }

        for (auto& macro_item : this->_macroses) {
            macro_item.init_runtime(hwnd, window_extent);
        }
        for (auto& net_item : this->_nets) {
            net_item.init_runtime(hwnd, window_extent);
        }
    }

    void draw(HDC h_dc, const WindowExtentPackage& actual_paint_window_extent) override {
        if (_chip_data == nullptr) return;

        const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

        const auto& def_data = _chip_data->get_def_data();
        const auto& lef_data = _chip_data->get_lef_data();

        f64 die_w = sc<f64>(def_data.die_area.width());
        f64 die_h = sc<f64>(def_data.die_area.height());
        if (die_w <= 0.0 || die_h <= 0.0) return;

        f64 scale = std::min(sc<f64>(position_extent.width) / die_w, 
                             sc<f64>(position_extent.height) / die_h);
        f64 fit_w = die_w * scale;
        f64 fit_h = die_h * scale;
    
        Point<f64> offset = Point<f64> {
            .x = position_extent.pos_x + (sc<f64>(position_extent.width)  - fit_w) * 0.5,
            .y = position_extent.pos_y + (sc<f64>(position_extent.height) - fit_h) * 0.5
        };

        // 1. Отрисовка области кристалла: светло-серый фон (RGB(225, 225, 225))
        HPEN old_pen;
        HPEN border_pen = PaintTools::TakePen(h_dc, RGB(160, 160, 160), old_pen, 2);
        HBRUSH old_brush;
        HBRUSH die_brush = PaintTools::TakeBrush(h_dc, RGB(225, 225, 225), old_brush);

        PaintTools::PaintFilledRectangle(h_dc, sc<i32>(offset.x), sc<i32>(offset.y), 
                                               sc<i32>(offset.x + def_data.die_area.width() * scale), sc<i32>(offset.y + def_data.die_area.height() * scale));

        PaintTools::ResetBrush(h_dc, old_brush, die_brush);
        PaintTools::ResetPen(h_dc, old_pen, border_pen);

        // 2. Отрисовка ячеек (чёрные прямоугольники в координатах кристалла)
        HPEN normal_cell_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH normal_cell_brush = CreateSolidBrush(RGB(20, 20, 20));

        HPEN highlight_cell_pen = CreatePen(PS_SOLID, 2, RGB(255, 69, 0));
        HBRUSH highlight_cell_brush = CreateSolidBrush(RGB(255, 215, 0));

        for (const auto& comp : _chip_data->get_components()) {
            if (!comp.is_placed) continue;

            f64 cell_w_dbu = 1000.0;
            f64 cell_h_dbu = 1000.0;

            auto it = _chip_data->get_lef().macros.find(comp.macro_name);
            if (it != _chip_data->get_lef().macros.end()) {
                cell_w_dbu = it->second.width * _chip_data->get_def_units();
                cell_h_dbu = it->second.height * _chip_data->get_def_units();
            }

            if (comp.orientation == "W" || comp.orientation == "E" || 
                comp.orientation == "FW" || comp.orientation == "FE") {
                std::swap(cell_w_dbu, cell_h_dbu);
            }

            f64 sx = offset.x + (comp.x - _chip_data->get_die_area().x1) * scale;
            f64 sy = offset.y + (_chip_data->get_die_area().y2 - (comp.y + cell_h_dbu)) * scale;
            f64 sw = std::max(sc<f64>(2.0), cell_w_dbu * scale);
            f64 sh = std::max(sc<f64>(2.0), cell_h_dbu * scale);

            bool is_highlighted = _highlighted_components.contains(comp.instance_name);

            HPEN cur_pen = is_highlighted ? highlight_cell_pen : normal_cell_pen;
            HBRUSH cur_brush = is_highlighted ? highlight_cell_brush : normal_cell_brush;

            HPEN prev_p = reinterpret_cast<HPEN>(SelectObject(h_dc, cur_pen));
            HBRUSH prev_b = reinterpret_cast<HBRUSH>(SelectObject(h_dc, cur_brush));

            PaintTools::PaintFilledRectangle(h_dc, sc<i32>(sx), sc<i32>(sy), sc<i32>(sx + sw), sc<i32>(sy + sh));

            SelectObject(h_dc, prev_p);
            SelectObject(h_dc, prev_b);

            // If zoomed in enough, draw instance label
            if (sw > 35 && sh > 14) {
                SetBkMode(h_dc, TRANSPARENT);
                SetTextColor(h_dc, is_highlighted ? RGB(0, 0, 0) : RGB(255, 255, 255));
                PaintTools::PaintText(h_dc, comp.instance_name, sc<i32>(sx + 2), sc<i32>(sy + 2));
            }
        }

        DeleteObject(normal_cell_pen);
        DeleteObject(normal_cell_brush);
        DeleteObject(highlight_cell_pen);
        DeleteObject(highlight_cell_brush);

        // 3. Отрисовка трассировки цепей с цветом и толщиной в соответствии со слоем металла
        auto draw_net_segments = [&](const DefNet& net, bool is_selected) {
            for (const auto& seg : net.segments) {
                f64 sx1 = offset.x + (seg.start.x - _chip_data->get_die_area().x1) * scale;
                f64 sy1 = offset.y + (_chip_data->get_die_area().y2 - seg.start.y) * scale;
                f64 sx2 = offset.x + (seg.end.x   - _chip_data->get_die_area().x1) * scale;
                f64 sy2 = offset.y + (_chip_data->get_die_area().y2 - seg.end.y) * scale;

                COLORREF color = is_selected ? RGB(255, 215, 0) : get_layer_color(seg.layer);
                u32 thickness = get_wire_thickness(seg.layer, is_selected);

                HPEN seg_pen;
                HPEN h_pen = PaintTools::TakePen(h_dc, color, seg_pen, thickness);

                PaintTools::PaintLine(h_dc, sc<i32>(sx1), sc<i32>(sy1), 
                                            sc<i32>(sx2), sc<i32>(sy2));

                PaintTools::ResetPen(h_dc, seg_pen, h_pen);
            }
        };

        for (const auto& net : _chip_data->get_nets()) {
            if (net.name != _selected_net) {
                draw_net_segments(net, false);
            }
        }
        if (!_selected_net.empty()) {
            for (const auto& net : _chip_data->get_nets()) {
                if (net.name == _selected_net) {
                    draw_net_segments(net, true);
                    break;
                }
            }
        }

        // 4. Информационная строка состояния на русском языке
        SetBkMode(h_dc, TRANSPARENT);
        SetTextColor(h_dc, RGB(40, 40, 40));

        std::string info = "Компонентов: " + std::to_string(_chip_data->get_stats().total_components) +
                           " | Сигнальных цепей: " + std::to_string(_chip_data->get_stats().signal_net_names.size()) +
                           " | Кристалл: " + std::to_string(sc<i32>(die_w)) + "x" + std::to_string(sc<i32>(die_h));

        if (!_selected_net.empty()) {
            info += " | Выбрана цепь: " + _selected_net + " (Ячеек: " + std::to_string(_highlighted_components.size()) + ")";
        }

        PaintTools::PaintText(h_dc, info, sc<i32>(offset.x + 5), sc<i32>(offset.y + fit_h + 6));
    }

    ~ChipBox() override = default;

    ChipBox(const ChipBox&&) = delete;
    ChipBox& operator=(const ChipBox&&) = delete;
    ChipBox(const ChipBox&) = delete;
    ChipBox& operator=(const ChipBox&) = delete;
};

#endif // CHIP_BOX_HPP

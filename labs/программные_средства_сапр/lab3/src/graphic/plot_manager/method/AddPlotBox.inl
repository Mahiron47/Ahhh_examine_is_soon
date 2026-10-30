#ifndef ADD_PLOT_BOX_INL
#define ADD_PLOT_BOX_INL

template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
inline PlotManager<LayoutT>* PlotManager<LayoutT>::add_plot_box(Window window, HWND h_wnd, PlotData plot_data, COLORREF color_background, COLORREF color_plot) {
    RECT window_rect;
    GetClientRect(h_wnd, &window_rect);

    auto client_extent = WindowExtentPackage {
        .width = sc<u32>(window_rect.right - window_rect.left),
        .height = sc<u32>(window_rect.bottom - window_rect.top)
    };

    const auto& shift_extent = this->template get_position_extent<GraphicShiftPackage>(client_extent);

    auto* plot_box = new PlotBox(shift_extent, plot_data, _math_visual_features, color_background, color_plot);
    _plot_boxes.push_back(plot_box);
    const u32 plot_idx = _plot_boxes.size() - 1;

    _text_boxes.push_back(new TextBox(GraphicPositionExtentPackage {
        .pos_x = 0,
        .pos_y = 0,
        .width = 100,
        .height = 50
    }, plot_data->get_data().name,
    [this, plot_idx, h_wnd]() -> void {
        this->set_active_plot(plot_idx);
        InvalidateRect(h_wnd, nullptr, TRUE);
    }, []() -> void {}, color_plot));

    if (_plot_boxes.size() == 1) {
        window->add_hotkey(new Zoom(0.1f, this));
    }

    this->init_runtime(h_wnd, client_extent);

    return this;
}

#endif // ADD_PLOT_BOX_INL
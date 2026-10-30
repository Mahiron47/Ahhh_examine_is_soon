template<typename LayoutT> requires SUPPORT_PACKAGE(GraphicPositionExtentPackage) || SUPPORT_PACKAGE(GraphicShiftPackage)
void Net<LayoutT>::send_message(Point<u32> mouse_position, Message message, const WindowExtentPackage& actual_paint_window_extent) {
    const auto& position_extent = this->template get_position_extent<GraphicPositionExtentPackage>(actual_paint_window_extent);

    for (auto* component : _components) {
        component->set_highlighted(false);
    }

    if ((mouse_position.x < position_extent.pos_x || mouse_position.x >= position_extent.pos_x + position_extent.width ||
         mouse_position.y < position_extent.pos_y || mouse_position.y >= position_extent.pos_y + position_extent.height) && 
        (mouse_position.x != UINT32_MAX && mouse_position.y != UINT32_MAX)) return;

    if (message.message_id == MESSAGE_ID_CLICK) {
        std::cout << "Net: " << _name << std::endl;
        const auto& click_data = *reinterpret_cast<ClickMessagePackage*>(message.message_data);
        if (click_data.self != nullptr) {
            auto* chip_box = reinterpret_cast<ChipBox<GraphicPositionExtentPackage>*>(click_data.self);
            
            chip_box->set_selected(true);

            for (auto* component : _components) {
                component->set_highlighted(true);
            }
        }
    }
}
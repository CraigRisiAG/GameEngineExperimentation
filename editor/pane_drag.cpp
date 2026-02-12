

/// @class PaneDrag
/// @brief A custom UI control for dragging panes with visual feedback.
///
/// PaneDrag is an editor UI component that allows users to drag panes by clicking and dragging.
/// It provides visual feedback by displaying different icons based on mouse hover state.
///
/// @signal dragged(Vector2 amount) - Emitted when the pane is being dragged, with the relative
///                                    mouse movement as the amount parameter.
///
/// @details
/// The control responds to mouse input and emits a "dragged" signal with the relative movement
/// when the left mouse button is held down. It displays an icon that changes appearance when
/// the mouse hovers over it, providing visual feedback to the user.
///
/// @see _gui_input() - Handles mouse motion input events
/// @see _notification() - Handles drawing and mouse enter/exit events
/// @see get_minimum_size() - Returns the size of the pane drag icon
#include "pane_drag.h"

void PaneDrag::_gui_input(const Ref<InputEvent> &p_input) {

	Ref<InputEventMouseMotion> mm = p_input;
	if (mm.is_valid() && mm->get_button_mask() & BUTTON_MASK_LEFT) {

		emit_signal("dragged", Point2(mm->get_relative().x, mm->get_relative().y));
	}
}

void PaneDrag::_notification(int p_what) {

	switch (p_what) {

		case NOTIFICATION_DRAW: {

			Ref<Texture2D> icon = mouse_over ? get_icon("PaneDragHover", "EditorIcons") : get_icon("PaneDrag", "EditorIcons");
			if (!icon.is_null())
				icon->draw(get_canvas_item(), Point2(0, 0));

		} break;
		case NOTIFICATION_MOUSE_ENTER:
			mouse_over = true;
			update();
			break;
		case NOTIFICATION_MOUSE_EXIT:
			mouse_over = false;
			update();
			break;
	}
}
Size2 PaneDrag::get_minimum_size() const {

	Ref<Texture2D> icon = get_icon("PaneDrag", "EditorIcons");
	if (!icon.is_null())
		return icon->get_size();
	return Size2();
}

void PaneDrag::_bind_methods() {

	ClassDB::bind_method("_gui_input", &PaneDrag::_gui_input);
	ADD_SIGNAL(MethodInfo("dragged", PropertyInfo(Variant::VECTOR2, "amount")));
}

PaneDrag::PaneDrag() {

	mouse_over = false;
}

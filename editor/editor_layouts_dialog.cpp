

/// @class EditorLayoutsDialog
/// @brief A dialog for managing editor layouts with selection and creation options.
///
/// This dialog allows users to select existing layouts from a list or create new ones
/// by entering a name. It supports multi-selection of layouts and handles keyboard input
/// for confirmation and cancellation.
///
/// @signal name_confirmed(String name) - Emitted when a layout name is confirmed,
///         either from selection or text input.

/// @brief Handles keyboard input in the name line edit field.
/// @param p_event The input event to process.
/// @details Processes Enter/Return keys to confirm selection and Escape to cancel.
void EditorLayoutsDialog::_line_gui_input(const Ref<InputEvent> &p_event);

/// @brief Binds signals for the dialog.
/// @details Registers the "name_confirmed" signal with a String parameter.
void EditorLayoutsDialog::_bind_methods();

/// @brief Handles the OK button press or Enter key confirmation.
/// @details Emits the "name_confirmed" signal with either the selected layout names
///          or the text entered in the name field.
void EditorLayoutsDialog::ok_pressed();

/// @brief Populates the dialog after it becomes visible.
/// @details Loads available layouts from the editor configuration file and populates
///          the layout_names ItemList.
void EditorLayoutsDialog::_post_popup();

/// @brief Constructs the EditorLayoutsDialog and initializes its UI components.
/// @details Sets up the VBoxContainer, ItemList for layout selection, and LineEdit for
///          custom layout names. Connects input signals for interaction handling.
EditorLayoutsDialog::EditorLayoutsDialog();

/// @brief Toggles the visibility of the name input line.
/// @param p_enabled Whether to show (true) or hide (false) the name line edit.
void EditorLayoutsDialog::set_name_line_enabled(bool p_enabled);
#include "editor_layouts_dialog.h"

#include "core/class_db.h"
#include "core/io/config_file.h"
#include "core/os/keyboard.h"
#include "editor/editor_settings.h"
#include "scene/gui/item_list.h"
#include "scene/gui/line_edit.h"

void EditorLayoutsDialog::_line_gui_input(const Ref<InputEvent> &p_event) {
	Ref<InputEventKey> k = p_event;

	if (k.is_valid()) {

		if (!k->is_pressed())
			return;

		switch (k->get_keycode()) {
			case KEY_KP_ENTER:
			case KEY_ENTER: {

				if (get_hide_on_ok())
					hide();
				ok_pressed();
				accept_event();
			} break;
			case KEY_ESCAPE: {

				hide();
				accept_event();
			} break;
		}
	}
}

void EditorLayoutsDialog::_bind_methods() {

	ADD_SIGNAL(MethodInfo("name_confirmed", PropertyInfo(Variant::STRING, "name")));
}

void EditorLayoutsDialog::ok_pressed() {

	if (layout_names->is_anything_selected()) {

		Vector<int> const selected_items = layout_names->get_selected_items();
		for (int i = 0; i < selected_items.size(); ++i) {

			emit_signal("name_confirmed", layout_names->get_item_text(selected_items[i]));
		}
	} else if (name->is_visible() && name->get_text() != "") {

		emit_signal("name_confirmed", name->get_text());
	}
}

void EditorLayoutsDialog::_post_popup() {

	ConfirmationDialog::_post_popup();
	name->clear();
	layout_names->clear();

	Ref<ConfigFile> config;
	config.instance();
	Error err = config->load(EditorSettings::get_singleton()->get_editor_layouts_config());
	if (err != OK) {

		return;
	}

	List<String> layouts;
	config.ptr()->get_sections(&layouts);

	for (List<String>::Element *E = layouts.front(); E; E = E->next()) {

		layout_names->add_item(**E);
	}
}

EditorLayoutsDialog::EditorLayoutsDialog() {

	makevb = memnew(VBoxContainer);
	add_child(makevb);
	makevb->set_anchor_and_margin(MARGIN_LEFT, ANCHOR_BEGIN, 5);
	makevb->set_anchor_and_margin(MARGIN_RIGHT, ANCHOR_END, -5);

	layout_names = memnew(ItemList);
	makevb->add_child(layout_names);
	layout_names->set_visible(true);
	layout_names->set_margin(MARGIN_TOP, 5);
	layout_names->set_anchor_and_margin(MARGIN_LEFT, ANCHOR_BEGIN, 5);
	layout_names->set_anchor_and_margin(MARGIN_RIGHT, ANCHOR_END, -5);
	layout_names->set_v_size_flags(Control::SIZE_EXPAND_FILL);
	layout_names->set_select_mode(ItemList::SELECT_MULTI);
	layout_names->set_allow_rmb_select(true);

	name = memnew(LineEdit);
	makevb->add_child(name);
	name->set_margin(MARGIN_TOP, 5);
	name->set_anchor_and_margin(MARGIN_LEFT, ANCHOR_BEGIN, 5);
	name->set_anchor_and_margin(MARGIN_RIGHT, ANCHOR_END, -5);
	name->connect("gui_input", callable_mp(this, &EditorLayoutsDialog::_line_gui_input));
	name->connect("focus_entered", callable_mp(layout_names, &ItemList::unselect_all));
}

void EditorLayoutsDialog::set_name_line_enabled(bool p_enabled) {

	name->set_visible(p_enabled);
}

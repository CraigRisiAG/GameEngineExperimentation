

/// @class EditorLayoutsDialog
/// @brief Dialog for managing editor layouts with name input and layout selection.
///
/// EditorLayoutsDialog extends ConfirmationDialog to provide a user interface for creating,
/// selecting, and managing editor layout configurations. It allows users to input a layout name
/// via a LineEdit widget and choose from existing layouts in an ItemList.
///
/// @details
/// The dialog manages three main UI components:
/// - A LineEdit for entering layout names
/// - An ItemList displaying available layout names
/// - A VBoxContainer for organizing the layout UI elements
///
/// Input events on the name field trigger custom handling through _line_gui_input(),
/// and the dialog responds to confirmation and popup events through ok_pressed() and
/// _post_popup() respectively.
///
/// @note This class is registered with Godot's class system via GDCLASS macro.
#ifndef EDITOR_LAYOUTS_DIALOG_H
#define EDITOR_LAYOUTS_DIALOG_H

#include "scene/gui/dialogs.h"

class LineEdit;
class ItemList;

class EditorLayoutsDialog : public ConfirmationDialog {

	GDCLASS(EditorLayoutsDialog, ConfirmationDialog);

	LineEdit *name;
	ItemList *layout_names;
	VBoxContainer *makevb;

	void _line_gui_input(const Ref<InputEvent> &p_event);

protected:
	static void _bind_methods();
	virtual void ok_pressed();
	virtual void _post_popup();

public:
	EditorLayoutsDialog();

	void set_name_line_enabled(bool p_enabled);
};

#endif // EDITOR_LAYOUTS_DIALOG_H

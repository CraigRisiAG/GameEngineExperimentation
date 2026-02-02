

/**
 * @class EditorPath
 * @brief A menu button that displays and navigates through the editor's object history path.
 * 
 * EditorPath extends MenuButton to provide a visual representation of the current selection
 * hierarchy in the editor. It maintains a history of objects and allows navigation through
 * previously selected items.
 * 
 * @member history Pointer to EditorHistory that tracks object selection history.
 * @member objects Vector of ObjectIDs representing the current path hierarchy.
 * 
 * @method EditorPath() Default constructor (private).
 * @method EditorPath(EditorHistory *p_history) Constructor that initializes with a history reference.
 * @method void update_path() Updates the displayed path based on current selection.
 * @method void _id_pressed(int p_idx) Internal callback when a path item is selected.
 * @method void _about_to_show() Internal callback triggered before the popup menu is displayed.
 * @method void _add_children_to_popup(Object *p_obj, int p_depth) Recursively adds child objects to the popup menu.
 * @method void _notification(int p_what) Handles engine notifications.
 * @method void _bind_methods() Binds methods to the Godot scripting API.
 */
#ifndef EDITOR_PATH_H
#define EDITOR_PATH_H

#include "editor_data.h"
#include "scene/gui/menu_button.h"

class EditorPath : public MenuButton {

	GDCLASS(EditorPath, MenuButton);

	EditorHistory *history;

	Vector<ObjectID> objects;
	EditorPath();

	void _id_pressed(int p_idx);
	void _about_to_show();
	void _add_children_to_popup(Object *p_obj, int p_depth = 0);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void update_path();

	EditorPath(EditorHistory *p_history);
};

#endif // EDITOR_PATH_H

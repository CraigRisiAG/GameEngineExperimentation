

/// @class EditorSubScene
/// @brief A dialog for managing sub-scene selection and reparenting in the editor.
/// 
/// EditorSubScene provides a confirmation dialog interface that allows users to:
/// - Browse and select nodes from a scene hierarchy tree
/// - Choose a destination parent node for moving selected nodes
/// - Handle multi-selection of nodes
/// - Manage node ownership reassignment
/// 
/// The dialog displays a file browser for selecting the sub-scene path and a tree
/// view for selecting target nodes within the scene.
/// 
/// @member selection List of currently selected nodes
/// @member path LineEdit widget for displaying/entering the sub-scene file path
/// @member tree Tree widget displaying the scene hierarchy
/// @member scene The root node of the scene being edited
/// @member is_root Flag indicating if the current selection is at the scene root
/// @member file_dialog File dialog for browsing sub-scene files
#ifndef EDITOR_SUB_SCENE_H
#define EDITOR_SUB_SCENE_H

#include "editor/editor_file_dialog.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"

class EditorSubScene : public ConfirmationDialog {

	GDCLASS(EditorSubScene, ConfirmationDialog);

	List<Node *> selection;
	LineEdit *path;
	Tree *tree;
	Node *scene;
	bool is_root;

	EditorFileDialog *file_dialog;

	void _fill_tree(Node *p_node, TreeItem *p_parent);
	void _selected_changed();
	void _item_multi_selected(Object *p_object, int p_cell, bool p_selected);
	void _item_activated();
	void _remove_selection_child(Node *p_node);
	void _reown(Node *p_node, List<Node *> *p_to_reown);

	void ok_pressed();

protected:
	void _notification(int p_what);
	static void _bind_methods();
	void _path_browse();
	void _path_selected(const String &p_path);
	void _path_changed(const String &p_path);

public:
	void move(Node *p_new_parent, Node *p_new_owner);
	void clear();
	EditorSubScene();
};

#endif // EDITOR_SUB_SCENE_H

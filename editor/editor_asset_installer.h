

/// @class EditorAssetInstaller
/// @brief A dialog for managing and installing editor assets from packages.
///
/// EditorAssetInstaller extends ConfirmationDialog to provide a user interface
/// for selecting and installing assets from a package file. It displays a tree
/// view of available assets with checkboxes for selection and tracks their
/// installation status.
///
/// @note Inherits from ConfirmationDialog and uses the Godot GDCLASS system.
#ifndef EDITORASSETINSTALLER_H
#define EDITORASSETINSTALLER_H

#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"
class EditorAssetInstaller : public ConfirmationDialog {

	GDCLASS(EditorAssetInstaller, ConfirmationDialog);

	Tree *tree;
	String package_path;
	AcceptDialog *error;
	Map<String, TreeItem *> status_map;
	bool updating;
	void _update_subitems(TreeItem *p_item, bool p_check, bool p_first = false);
	void _item_edited();
	virtual void ok_pressed();

protected:
	static void _bind_methods();

public:
	void open(const String &p_path, int p_depth = 0);
	EditorAssetInstaller();
};

#endif // EDITORASSETINSTALLER_H

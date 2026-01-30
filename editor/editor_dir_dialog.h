

/// @class EditorDirDialog
/// @brief A dialog widget for selecting directories in the editor.
///
/// EditorDirDialog is a confirmation dialog that displays a tree view of the file system,
/// allowing users to navigate and select directories. It supports creating new directories
/// and tracks the state of expanded paths.
///
/// @details
/// The dialog maintains a tree structure synchronized with the EditorFileSystem and allows
/// users to expand/collapse directory nodes, create new directories, and confirm their selection.
/// It caches opened paths to preserve the tree expansion state across reloads.
///
/// @members
/// - @c makedialog: Dialog for confirming new directory creation
/// - @c makedirname: Text input field for new directory name
/// - @c mkdirerr: Error dialog for directory creation failures
/// - @c makedir: Button to trigger directory creation
/// - @c opened_paths: Set of paths that have been expanded in the tree
/// - @c tree: Main tree widget displaying the directory structure
/// - @c updating: Flag indicating if the tree is currently being updated
/// - @c must_reload: Flag to track if the dialog needs to reload its contents
///
/// @methods
/// - @c reload(): Refreshes the directory tree, optionally selecting a specific path
/// - @c _update_dir(): Recursively populates tree items from EditorFileSystemDirectory
/// - @c _make_dir(): Initiates directory creation workflow
/// - @c _make_dir_confirm(): Confirms and executes directory creation
/// - @c _item_collapsed(): Handles tree item collapse events
/// - @c _item_activated(): Handles tree item activation/selection
/// - @c ok_pressed(): Confirms the selected directory
#ifndef EDITOR_DIR_DIALOG_H
#define EDITOR_DIR_DIALOG_H

#include "core/os/dir_access.h"
#include "editor/editor_file_system.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"

class EditorDirDialog : public ConfirmationDialog {
	GDCLASS(EditorDirDialog, ConfirmationDialog);

	ConfirmationDialog *makedialog;
	LineEdit *makedirname;
	AcceptDialog *mkdirerr;

	Button *makedir;
	Set<String> opened_paths;

	Tree *tree;
	bool updating;

	void _item_collapsed(Object *p_item);
	void _item_activated();
	void _update_dir(TreeItem *p_item, EditorFileSystemDirectory *p_dir, const String &p_select_path = String());

	void _make_dir();
	void _make_dir_confirm();

	void ok_pressed();

	bool must_reload;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void reload(const String &p_path = "");
	EditorDirDialog();
};

#endif // EDITOR_DIR_DIALOG_H

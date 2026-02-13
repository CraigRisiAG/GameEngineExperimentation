

/**
 * @file rename_dialog.h
 * @brief Dialog for batch renaming nodes in the scene tree.
 * @author Blazej Floch
 * 
 * RenameDialog provides a comprehensive interface for renaming multiple nodes
 * in the scene tree with various options including search/replace, prefixes,
 * suffixes, regular expressions, and automatic counters.
 */

/**
 * @class RenameDialog
 * @brief A dialog window for batch renaming scene tree nodes with advanced features.
 * 
 * This dialog extends ConfirmationDialog and provides functionality to rename
 * multiple nodes at once with features such as:
 * - Search and replace text
 * - Add prefixes and suffixes
 * - Use regular expressions for pattern matching
 * - Insert special tags (node name, parent name, type, etc.)
 * - Auto-increment counters with customizable start, step, and padding
 * - Text case conversion and styling options
 * - Real-time preview of changes
 * 
 * The dialog integrates with the editor's undo/redo system to allow reverting changes.
 */

/**
 * @brief Executes the rename operation on all selected nodes.
 * 
 * This is called when the user confirms the dialog. It applies the configured
 * rename rules to all selected nodes in the scene tree.
 */

/**
 * @brief Handles the cancel button press.
 * 
 * Called when the user cancels the rename operation.
 */

/**
 * @brief Callback when feature checkboxes are toggled.
 * @param pressed Whether the checkbox is now pressed/checked.
 */

/**
 * @brief Inserts text at the current cursor position in the active field.
 * @param text The text to insert (e.g., special tags like {name}, {parent}).
 */

/**
 * @brief Updates the substitution preview based on current settings.
 */

/**
 * @brief Checks if the given LineEdit is one of the main renaming fields.
 * @param line_edit The LineEdit widget to check.
 * @return true if it's a main field (search, replace, prefix, or suffix).
 */

/**
 * @brief Recursively iterates through the scene tree starting from a node.
 * @param node The starting node for iteration.
 * @param selection Array of selected nodes to process.
 * @param count Pointer to counter for tracking processed nodes.
 */

/**
 * @brief Applies the configured rename pattern to a single node.
 * @param node The node to generate a new name for.
 * @param count The current counter value for this node.
 * @return The new name for the node.
 */

/**
 * @brief Substitutes special tags in the subject string with actual values.
 * @param subject The string containing tags to substitute.
 * @param node The node whose properties will be used for substitution.
 * @param count The current counter value.
 * @return The string with all tags replaced.
 */

/**
 * @brief Applies regular expression pattern matching and replacement.
 * @param pattern The regex pattern to match.
 * @param subject The string to search in.
 * @param replacement The replacement string.
 * @return The string after regex replacement.
 */

/**
 * @brief Post-processes the renamed string (applies case conversion, style, etc.).
 * @param subject The string to post-process.
 * @return The final processed string.
 */

/**
 * @brief Updates the preview display with new text.
 * @param new_text Optional text to preview. If empty, regenerates from current settings.
 */

/**
 * @brief Updates the preview when integer spin box values change.
 * @param new_value The new integer value.
 */

/**
 * @brief Custom error handler for catching errors during rename operations.
 * @param p_self Pointer to the RenameDialog instance.
 * @param p_func Function name where error occurred.
 * @param p_file File name where error occurred.
 * @param p_line Line number where error occurred.
 * @param p_error Error message.
 * @param p_errorexp Error expression.
 * @param p_type Type of error.
 */

/**
 * @brief Reference to the scene tree editor.
 */

/**
 * @brief Undo/redo manager for reverting rename operations.
 */

/**
 * @brief Global counter for tracking rename operations across all nodes.
 */

/**
 * @brief Line edit for entering search text.
 */

/**
 * @brief Line edit for entering replacement text.
 */

/**
 * @brief Line edit for entering prefix to add.
 */

/**
 * @brief Line edit for entering suffix to add.
 */

/**
 * @brief Tab container for organizing feature options.
 */

/**
 * @brief Checkbox to enable/disable substitution.
 */

/**
 * @brief Check button to enable/disable regex mode.
 */

/**
 * @brief Checkbox to enable/disable post-processing.
 */

/**
 * @brief Checkbox to enable per-level counter (resets counter at each hierarchy level).
 */

/**
 * @brief Button to insert {name} tag.
 */

/**
 * @brief Button to insert {parent} tag.
 */

/**
 * @brief Button to insert {type} tag.
 */

/**
 * @brief Button to insert {scene} tag.
 */

/**
 * @brief Button to insert {root} tag.
 */

/**
 * @brief Button to insert {count} tag.
 */

/**
 * @brief Spin box for setting counter start value.
 */

/**
 * @brief Spin box for setting counter step/increment value.
 */

/**
 * @brief Spin box for setting counter padding (leading zeros).
 */

/**
 * @brief Option button for selecting name style (PascalCase, snake_case, etc.).
 */

/**
 * @brief Option button for selecting case conversion (lowercase, UPPERCASE, etc.).
 */

/**
 * @brief Label showing the preview section title.
 */

/**
 * @brief Label showing the preview of renamed nodes.
 */

/**
 * @brief List of node paths and their new names to be applied.
 */

/**
 * @brief Node used for generating preview.
 */

/**
 * @brief Flag to prevent recursive preview updates.
 */

/**
 * @brief Custom error handler list for tracking errors.
 */

/**
 * @brief Flag indicating if errors occurred during rename operation.
 */

/**
 * @brief Handles notifications for the dialog.
 * @param p_what The notification type.
 */

/**
 * @brief Binds methods for Godot's reflection system.
 */

/**
 * @brief Called after the popup is displayed.
 * 
 * Used for initialization that needs to occur after the dialog is shown.
 */

/**
 * @brief Resets the dialog to default state.
 * 
 * Clears all fields and settings to their initial values.
 */

/**
 * @brief Performs the actual rename operation on selected nodes.
 * 
 * Applies all configured rename rules and uses the undo/redo system
 * to make the operation reversible.
 */

/**
 * @brief Constructs a new RenameDialog.
 * @param p_scene_tree_editor Pointer to the scene tree editor.
 * @param p_undo_redo Pointer to the undo/redo manager (optional).
 */

/**
 * @brief Destructor for RenameDialog.
 */
#ifndef RENAME_DIALOG_H
#define RENAME_DIALOG_H

#include "scene/gui/check_box.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/option_button.h"
#include "scene/gui/spin_box.h"

#include "core/undo_redo.h"
#include "editor/scene_tree_editor.h"

/**
@author Blazej Floch
*/

class RenameDialog : public ConfirmationDialog {

	GDCLASS(RenameDialog, ConfirmationDialog);

	virtual void ok_pressed() { rename(); };
	void _cancel_pressed(){};
	void _features_toggled(bool pressed);
	void _insert_text(String text);
	void _update_substitute();
	bool _is_main_field(LineEdit *line_edit);

	void _iterate_scene(const Node *node, const Array &selection, int *count);
	String _apply_rename(const Node *node, int count);
	String _substitute(const String &subject, const Node *node, int count);
	String _regex(const String &pattern, const String &subject, const String &replacement);
	String _postprocess(const String &subject);
	void _update_preview(String new_text = "");
	void _update_preview_int(int new_value = 0);
	static void _error_handler(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, ErrorHandlerType p_type);

	SceneTreeEditor *scene_tree_editor;
	UndoRedo *undo_redo;
	int global_count;

	LineEdit *lne_search;
	LineEdit *lne_replace;
	LineEdit *lne_prefix;
	LineEdit *lne_suffix;

	TabContainer *tabc_features;

	CheckBox *cbut_substitute;
	CheckButton *cbut_regex;
	CheckBox *cbut_process;
	CheckBox *chk_per_level_counter;

	Button *but_insert_name;
	Button *but_insert_parent;
	Button *but_insert_type;
	Button *but_insert_scene;
	Button *but_insert_root;
	Button *but_insert_count;

	SpinBox *spn_count_start;
	SpinBox *spn_count_step;
	SpinBox *spn_count_padding;

	OptionButton *opt_style;
	OptionButton *opt_case;

	Label *lbl_preview_title;
	Label *lbl_preview;

	List<Pair<NodePath, String>> to_rename;
	Node *preview_node;
	bool lock_preview_update;
	ErrorHandlerList eh;
	bool has_errors;

protected:
	void _notification(int p_what){};
	static void _bind_methods();
	virtual void _post_popup();

public:
	void reset();
	void rename();

	RenameDialog(SceneTreeEditor *p_scene_tree_editor, UndoRedo *p_undo_redo = NULL);
	~RenameDialog(){};
};

#endif

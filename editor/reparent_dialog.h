

/**
 * @class ReparentDialog
 * @brief A dialog window that allows users to reparent selected nodes in the scene tree.
 *
 * ReparentDialog provides a user interface for changing the parent of one or more nodes
 * in the scene hierarchy. It displays a scene tree editor where users can select a new
 * parent node and includes an option to preserve the node's global transform during
 * reparenting.
 *
 * @inherits ConfirmationDialog
 *
 * Key features:
 * - Interactive scene tree selection for choosing the new parent
 * - Optional transform preservation during reparent operation
 * - Support for reparenting multiple nodes simultaneously
 *
 * @author Juan Linietsky <reduzio@gmail.com>
 */
#ifndef REPARENT_DIALOG_H
#define REPARENT_DIALOG_H

#include "editor/scene_tree_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/check_button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"
/**
@author Juan Linietsky <reduzio@gmail.com>
*/
class ReparentDialog : public ConfirmationDialog {

	GDCLASS(ReparentDialog, ConfirmationDialog);

	SceneTreeEditor *tree;
	CheckBox *keep_transform;

	void update_tree();
	void _reparent();
	void _cancel();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_current(const Set<Node *> &p_selection);
	String get_selected_type();

	ReparentDialog();
	~ReparentDialog();
};

#endif

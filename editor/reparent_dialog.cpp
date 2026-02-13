

/**
 * @file reparent_dialog.cpp
 * @brief Implementation of the ReparentDialog class for reparenting nodes in the scene tree.
 *
 * This file contains the implementation of a dialog window that allows users to select
 * a new parent node for one or more selected nodes in the scene hierarchy. It provides
 * a tree view for navigation and an option to preserve global transforms during reparenting.
 */

/**
 * @brief Handles notification events for the ReparentDialog.
 * @param p_what The notification type to handle.
 *
 * Responds to NOTIFICATION_ENTER_TREE by connecting the confirmed signal,
 * NOTIFICATION_EXIT_TREE by disconnecting it, and NOTIFICATION_DRAW for
 * custom drawing operations (currently commented out).
 */

/**
 * @brief Cancels the reparent operation and closes the dialog.
 *
 * Called when the user cancels the operation. Simply hides the dialog
 * without performing any reparenting.
 */

/**
 * @brief Performs the reparent operation with the selected parent node.
 *
 * Emits the "reparent" signal with the selected node's path and the
 * keep_transform setting if a valid node is selected in the tree.
 * Hides the dialog after emitting the signal.
 */

/**
 * @brief Sets the current selection to be marked in the tree view.
 * @param p_selection A set of Node pointers representing the current selection.
 *
 * Marks the provided nodes in the scene tree editor to indicate which
 * nodes are being reparented.
 */

/**
 * @brief Binds methods and signals for the ReparentDialog class.
 *
 * Registers the "_cancel" method and defines the "reparent" signal with
 * parameters for the target node path and keep global transform flag.
 */

/**
 * @brief Constructs a new ReparentDialog.
 *
 * Initializes the dialog with a scene tree editor for selecting the new parent,
 * a checkbox for preserving global transforms, and connects necessary signals.
 * Sets up the UI layout using VBoxContainer.
 */

/**
 * @brief Destructor for ReparentDialog.
 *
 * Cleans up resources used by the ReparentDialog instance.
 */
#include "reparent_dialog.h"

#include "core/print_string.h"
#include "scene/gui/box_container.h"
#include "scene/gui/label.h"

void ReparentDialog::_notification(int p_what) {

	if (p_what == NOTIFICATION_ENTER_TREE) {

		connect("confirmed", callable_mp(this, &ReparentDialog::_reparent));
	}

	if (p_what == NOTIFICATION_EXIT_TREE) {

		disconnect("confirmed", callable_mp(this, &ReparentDialog::_reparent));
	}

	if (p_what == NOTIFICATION_DRAW) {

		//RID ci = get_canvas_item();
		//get_stylebox("panel","PopupMenu")->draw(ci,Rect2(Point2(),get_size()));
	}
}

void ReparentDialog::_cancel() {

	hide();
}
void ReparentDialog::_reparent() {

	if (tree->get_selected()) {

		emit_signal("reparent", tree->get_selected()->get_path(), keep_transform->is_pressed());
		hide();
	}
}

void ReparentDialog::set_current(const Set<Node *> &p_selection) {

	tree->set_marked(p_selection, false, false);
	//tree->set_selected(p_node->get_parent());
}

void ReparentDialog::_bind_methods() {

	ClassDB::bind_method("_cancel", &ReparentDialog::_cancel);

	ADD_SIGNAL(MethodInfo("reparent", PropertyInfo(Variant::NODE_PATH, "path"), PropertyInfo(Variant::BOOL, "keep_global_xform")));
}

ReparentDialog::ReparentDialog() {

	set_title(TTR("Reparent Node"));

	VBoxContainer *vbc = memnew(VBoxContainer);
	add_child(vbc);
	//set_child_rect(vbc);

	tree = memnew(SceneTreeEditor(false));
	tree->set_show_enabled_subscene(true);

	vbc->add_margin_child(TTR("Reparent Location (Select new Parent):"), tree, true);

	tree->get_scene_tree()->connect("item_activated", callable_mp(this, &ReparentDialog::_reparent));

	//Label *label = memnew( Label );
	//label->set_position( Point2( 15,8) );
	//label->set_text("Reparent Location (Select new Parent):");

	keep_transform = memnew(CheckBox);
	keep_transform->set_text(TTR("Keep Global Transform"));
	keep_transform->set_pressed(true);
	vbc->add_child(keep_transform);

	//vbc->add_margin_child("Options:",node_only);

	//cancel->connect("pressed", this,"_cancel");

	get_ok()->set_text(TTR("Reparent"));
}

ReparentDialog::~ReparentDialog() {
}

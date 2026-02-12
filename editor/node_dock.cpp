

/**
 * @brief Dock panel for editing a selected node's Signals and Groups in the editor.
 *
 * The NodeDock manages two sub-editors:
 * - ConnectionsDock (shown as "Signals")
 * - GroupsEditor (shown as "Groups")
 *
 * It provides a toggle UI to switch between these views, updates button icons on
 * theme/tree notifications, and synchronizes visibility based on whether a valid
 * node is currently selected.
 *
 * Behavior summary:
 * - show_connections(): Activates the Signals tab and hides Groups.
 * - show_groups(): Activates the Groups tab and hides Signals.
 * - set_node(Node *): Assigns the current node to child editors and shows either
 *   the active tab or a placeholder message when no node is selected.
 * - update_lists(): Refreshes the connections tree content.
 *
 * Notes:
 * - Maintains a singleton instance via NodeDock::singleton.
 * - Uses editor undo/redo from EditorNode for child editor operations.
 */
#include "node_dock.h"

#include "editor_node.h"
#include "editor_scale.h"

void NodeDock::show_groups() {

	groups_button->set_pressed(true);
	connections_button->set_pressed(false);
	groups->show();
	connections->hide();
}

void NodeDock::show_connections() {

	groups_button->set_pressed(false);
	connections_button->set_pressed(true);
	groups->hide();
	connections->show();
}

void NodeDock::_bind_methods() {
}

void NodeDock::_notification(int p_what) {

	if (p_what == NOTIFICATION_ENTER_TREE || p_what == NOTIFICATION_THEME_CHANGED) {
		connections_button->set_icon(get_icon("Signals", "EditorIcons"));
		groups_button->set_icon(get_icon("Groups", "EditorIcons"));
	}
}

NodeDock *NodeDock::singleton = NULL;

void NodeDock::update_lists() {

	connections->update_tree();
}

void NodeDock::set_node(Node *p_node) {

	connections->set_node(p_node);
	groups->set_current(p_node);

	if (p_node) {
		if (connections_button->is_pressed())
			connections->show();
		else
			groups->show();

		mode_hb->show();
		select_a_node->hide();
	} else {
		connections->hide();
		groups->hide();
		mode_hb->hide();
		select_a_node->show();
	}
}

NodeDock::NodeDock() {

	singleton = this;

	set_name("Node");
	mode_hb = memnew(HBoxContainer);
	add_child(mode_hb);
	mode_hb->hide();

	connections_button = memnew(ToolButton);
	connections_button->set_text(TTR("Signals"));
	connections_button->set_toggle_mode(true);
	connections_button->set_pressed(true);
	connections_button->set_h_size_flags(SIZE_EXPAND_FILL);
	connections_button->set_clip_text(true);
	mode_hb->add_child(connections_button);
	connections_button->connect("pressed", callable_mp(this, &NodeDock::show_connections));

	groups_button = memnew(ToolButton);
	groups_button->set_text(TTR("Groups"));
	groups_button->set_toggle_mode(true);
	groups_button->set_pressed(false);
	groups_button->set_h_size_flags(SIZE_EXPAND_FILL);
	groups_button->set_clip_text(true);
	mode_hb->add_child(groups_button);
	groups_button->connect("pressed", callable_mp(this, &NodeDock::show_groups));

	connections = memnew(ConnectionsDock(EditorNode::get_singleton()));
	connections->set_undoredo(EditorNode::get_undo_redo());
	add_child(connections);
	connections->set_v_size_flags(SIZE_EXPAND_FILL);
	connections->hide();

	groups = memnew(GroupsEditor);
	groups->set_undo_redo(EditorNode::get_undo_redo());
	add_child(groups);
	groups->set_v_size_flags(SIZE_EXPAND_FILL);
	groups->hide();

	select_a_node = memnew(Label);
	select_a_node->set_text(TTR("Select a single node to edit its signals and groups."));
	select_a_node->set_custom_minimum_size(Size2(100 * EDSCALE, 0));
	select_a_node->set_v_size_flags(SIZE_EXPAND_FILL);
	select_a_node->set_valign(Label::VALIGN_CENTER);
	select_a_node->set_align(Label::ALIGN_CENTER);
	select_a_node->set_autowrap(true);
	add_child(select_a_node);
}

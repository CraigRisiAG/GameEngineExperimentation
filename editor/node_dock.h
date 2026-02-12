

/// @class NodeDock
/// @brief A docking panel for managing and editing node properties, connections, and groups.
///
/// NodeDock provides a tabbed interface for inspecting and modifying various aspects of a selected node.
/// It displays two main editors: ConnectionsDock for viewing/editing node connections and GroupsEditor
/// for managing node group membership. The dock automatically updates when the selected node changes.
///
/// @members
/// - connections_button: Button to switch to connections view
/// - groups_button: Button to switch to groups view
/// - connections: The connections editor dock
/// - groups: The groups editor panel
/// - mode_hb: Container for the mode selection buttons
/// - select_a_node: Label displayed when no node is selected
/// - singleton: Static instance of the NodeDock
///
/// @methods
/// - set_node(Node*): Sets the current node to inspect and display
/// - show_groups(): Switches the view to the groups editor
/// - show_connections(): Switches the view to the connections editor
/// - update_lists(): Refreshes the displayed information for the current node
#ifndef NODE_DOCK_H
#define NODE_DOCK_H

#include "connections_dialog.h"
#include "groups_editor.h"

class NodeDock : public VBoxContainer {

	GDCLASS(NodeDock, VBoxContainer);

	ToolButton *connections_button;
	ToolButton *groups_button;

	ConnectionsDock *connections;
	GroupsEditor *groups;

	HBoxContainer *mode_hb;

	Label *select_a_node;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	static NodeDock *singleton;

	void set_node(Node *p_node);

	void show_groups();
	void show_connections();

	void update_lists();

	NodeDock();
};

#endif // NODE_DOCK_H

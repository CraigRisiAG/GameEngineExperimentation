


/// @class EditorDebuggerTree
/// @brief A tree view for displaying and interacting with remote scene nodes during debugging.
///
/// EditorDebuggerTree extends TreeView to provide a hierarchical visualization of nodes
/// from a remote debugged scene. It allows inspecting node properties, saving branches as scenes,
/// and copying node paths. The tree maintains state across updates including node selection,
/// expansion/collapse state, and filtering.
///
/// @signals
/// - object_selected(int object_id, int debugger): Emitted when a tree item is selected.
///   Parameters: object_id - the ObjectID of the selected node, debugger - the debugger instance ID.
/// - save_node(int object_id, String filename, int debugger): Emitted when user saves a node branch.
///   Parameters: object_id - the ObjectID of the node to save, filename - target file path, debugger - debugger instance ID.
///
/// @see SceneDebuggerTree, EditorFileDialog, PopupMenu

/// @fn EditorDebuggerTree::EditorDebuggerTree()
/// @brief Constructs the debugger tree and initializes popup menu and file dialog child nodes.

/// @fn void EditorDebuggerTree::_notification(int p_what)
/// @brief Handles initialization notifications to connect tree selection signals.

/// @fn void EditorDebuggerTree::_bind_methods()
/// @brief Binds object_selected and save_node signals to the scripting system.

/// @fn void EditorDebuggerTree::_scene_tree_selected()
/// @brief Callback when a tree item is selected; emits object_selected signal with the node's ID.

/// @fn void EditorDebuggerTree::_scene_tree_folded(Object *p_obj)
/// @brief Callback when a tree item is collapsed/expanded; updates the unfold cache.

/// @fn void EditorDebuggerTree::_scene_tree_rmb_selected(const Vector2 &p_position)
/// @brief Callback for right-click on tree item; displays context menu with save and copy options.

/// @fn void EditorDebuggerTree::update_scene_tree(const SceneDebuggerTree *p_tree, int p_debugger)
/// @brief Populates the tree from a flattened depth-first list of remote nodes.
/// @param p_tree The scene debugger tree data containing flattened node list.
/// @param p_debugger The debugger instance identifier.
/// @details Reconstructs the hierarchy from flat node array, applies filters, maintains selection and expansion state.

/// @fn String EditorDebuggerTree::get_selected_path()
/// @brief Returns the node path of the currently selected tree item.
/// @return Node path as a string, or empty string if no item is selected.

/// @fn String EditorDebuggerTree::_get_path(TreeItem *p_item)
/// @brief Constructs the full node path from a tree item by traversing parent nodes.
/// @param p_item The tree item to get the path for.
/// @return The constructed path from root to item.

/// @fn void EditorDebuggerTree::_item_menu_id_pressed(int p_option)
/// @brief Handles context menu selections: save node as scene or copy node path to clipboard.
/// @param p_option The selected menu option ID.

/// @fn void EditorDebuggerTree::_file_selected(const String &p_file)
/// @brief Callback when user confirms file selection in save dialog; emits save_node signal.
/// @param p_file The selected file path for saving the node.
#include "editor_debugger_tree.h"

#include "editor/editor_node.h"
#include "scene/debugger/scene_debugger.h"
#include "scene/resources/packed_scene.h"

EditorDebuggerTree::EditorDebuggerTree() {
	set_v_size_flags(SIZE_EXPAND_FILL);
	set_allow_rmb_select(true);

	// Popup
	item_menu = memnew(PopupMenu);
	item_menu->connect("id_pressed", callable_mp(this, &EditorDebuggerTree::_item_menu_id_pressed));
	add_child(item_menu);

	// File Dialog
	file_dialog = memnew(EditorFileDialog);
	file_dialog->connect("file_selected", callable_mp(this, &EditorDebuggerTree::_file_selected));
	add_child(file_dialog);
}

void EditorDebuggerTree::_notification(int p_what) {
	if (p_what == NOTIFICATION_POSTINITIALIZE) {
		connect("cell_selected", callable_mp(this, &EditorDebuggerTree::_scene_tree_selected));
		connect("item_collapsed", callable_mp(this, &EditorDebuggerTree::_scene_tree_folded));
		connect("item_rmb_selected", callable_mp(this, &EditorDebuggerTree::_scene_tree_rmb_selected));
	}
}

void EditorDebuggerTree::_bind_methods() {
	ADD_SIGNAL(MethodInfo("object_selected", PropertyInfo(Variant::INT, "object_id"), PropertyInfo(Variant::INT, "debugger")));
	ADD_SIGNAL(MethodInfo("save_node", PropertyInfo(Variant::INT, "object_id"), PropertyInfo(Variant::STRING, "filename"), PropertyInfo(Variant::INT, "debugger")));
}

void EditorDebuggerTree::_scene_tree_selected() {

	if (updating_scene_tree) {
		return;
	}

	TreeItem *item = get_selected();
	if (!item) {
		return;
	}

	inspected_object_id = uint64_t(item->get_metadata(0));

	emit_signal("object_selected", inspected_object_id, debugger_id);
}

void EditorDebuggerTree::_scene_tree_folded(Object *p_obj) {

	if (updating_scene_tree) {

		return;
	}
	TreeItem *item = Object::cast_to<TreeItem>(p_obj);

	if (!item)
		return;

	ObjectID id = ObjectID(uint64_t(item->get_metadata(0)));
	if (unfold_cache.has(id)) {
		unfold_cache.erase(id);
	} else {
		unfold_cache.insert(id);
	}
}

void EditorDebuggerTree::_scene_tree_rmb_selected(const Vector2 &p_position) {

	TreeItem *item = get_item_at_position(p_position);
	if (!item)
		return;

	item->select(0);

	item_menu->clear();
	item_menu->add_icon_item(get_icon("CreateNewSceneFrom", "EditorIcons"), TTR("Save Branch as Scene"), ITEM_MENU_SAVE_REMOTE_NODE);
	item_menu->add_icon_item(get_icon("CopyNodePath", "EditorIcons"), TTR("Copy Node Path"), ITEM_MENU_COPY_NODE_PATH);
	item_menu->set_global_position(get_global_mouse_position());
	item_menu->popup();
}

/// Populates inspect_scene_tree given data in nodes as a flat list, encoded depth first.
///
/// Given a nodes array like [R,A,B,C,D,E] the following Tree will be generated, assuming
/// filter is an empty String, R and A child count are 2, B is 1 and C, D and E are 0.
///
/// R
/// |-A
/// | |-B
/// | | |-C
/// | |
/// | |-D
/// |
/// |-E
///
void EditorDebuggerTree::update_scene_tree(const SceneDebuggerTree *p_tree, int p_debugger) {
	updating_scene_tree = true;
	const String last_path = get_selected_path();
	const String filter = EditorNode::get_singleton()->get_scene_tree_dock()->get_filter();

	// Nodes are in a flatten list, depth first. Use a stack of parents, avoid recursion.
	List<Pair<TreeItem *, int>> parents;
	for (int i = 0; i < p_tree->nodes.size(); i++) {
		TreeItem *parent = NULL;
		if (parents.size()) { // Find last parent.
			Pair<TreeItem *, int> &p = parents[0];
			parent = p.first;
			if (!(--p.second)) { // If no child left, remove it.
				parents.pop_front();
			}
		}
		// Add this node.
		const SceneDebuggerTree::RemoteNode &node = p_tree->nodes[i];
		TreeItem *item = create_item(parent);
		item->set_text(0, node.name);
		item->set_tooltip(0, TTR("Type:") + " " + node.type_name);
		Ref<Texture2D> icon = EditorNode::get_singleton()->get_class_icon(node.type_name, "");
		if (icon.is_valid()) {
			item->set_icon(0, icon);
		}
		item->set_metadata(0, node.id);

		// Set current item as collapsed if necessary (root is never collapsed)
		if (parent) {
			if (!unfold_cache.has(node.id)) {
				item->set_collapsed(true);
			}
		}
		// Select previously selected node.
		if (debugger_id == p_debugger) { // Can use remote id.
			if (node.id == inspected_object_id) {
				item->select(0);
			}
		} else { // Must use path
			if (last_path == _get_path(item)) {
				updating_scene_tree = false; // Force emission of new selection
				item->select(0);
				updating_scene_tree = true;
			}
		}

		// Add in front of the parents stack if children are expected.
		if (node.child_count) {
			parents.push_front(Pair<TreeItem *, int>(item, node.child_count));
		} else {
			// Apply filters.
			while (parent) {
				const bool had_siblings = item->get_prev() || item->get_next();
				if (filter.is_subsequence_ofi(item->get_text(0)))
					break; // Filter matches, must survive.
				parent->remove_child(item);
				memdelete(item);
				if (had_siblings)
					break; // Parent must survive.
				item = parent;
				parent = item->get_parent();
				// Check if parent expects more children.
				for (int j = 0; j < parents.size(); j++) {
					if (parents[j].first == item) {
						parent = NULL;
						break; // Might have more children.
					}
				}
			}
		}
	}
	debugger_id = p_debugger; // Needed by hook, could be avoided if every debugger had its own tree
	updating_scene_tree = false;
}

String EditorDebuggerTree::get_selected_path() {
	if (!get_selected())
		return "";
	return _get_path(get_selected());
}

String EditorDebuggerTree::_get_path(TreeItem *p_item) {
	ERR_FAIL_COND_V(!p_item, "");

	if (p_item->get_parent() == NULL) {
		return "/root";
	}
	String text = p_item->get_text(0);
	TreeItem *cur = p_item->get_parent();
	while (cur) {
		text = cur->get_text(0) + "/" + text;
		cur = cur->get_parent();
	}
	return "/" + text;
}

void EditorDebuggerTree::_item_menu_id_pressed(int p_option) {

	switch (p_option) {

		case ITEM_MENU_SAVE_REMOTE_NODE: {

			file_dialog->set_access(EditorFileDialog::ACCESS_RESOURCES);
			file_dialog->set_mode(EditorFileDialog::MODE_SAVE_FILE);

			List<String> extensions;
			Ref<PackedScene> sd = memnew(PackedScene);
			ResourceSaver::get_recognized_extensions(sd, &extensions);
			file_dialog->clear_filters();
			for (int i = 0; i < extensions.size(); i++) {
				file_dialog->add_filter("*." + extensions[i] + " ; " + extensions[i].to_upper());
			}

			file_dialog->popup_centered_ratio();
		} break;
		case ITEM_MENU_COPY_NODE_PATH: {

			String text = get_selected_path();
			if (text.empty()) {
				return;
			} else if (text == "/root") {
				text = ".";
			} else {
				text = text.replace("/root/", "");
				int slash = text.find("/");
				if (slash < 0) {
					text = ".";
				} else {
					text = text.substr(slash + 1);
				}
			}
			OS::get_singleton()->set_clipboard(text);
		} break;
	}
}

void EditorDebuggerTree::_file_selected(const String &p_file) {
	if (inspected_object_id.is_null())
		return;
	emit_signal("save_node", inspected_object_id, p_file, debugger_id);
}

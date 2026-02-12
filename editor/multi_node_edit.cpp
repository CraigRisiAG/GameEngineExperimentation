

/**
 * @class MultiNodeEdit
 * @brief Handles simultaneous editing of properties across multiple selected nodes.
 * 
 * This class provides a unified interface for getting, setting, and querying properties
 * of multiple nodes at once. It manages undo/redo operations and ensures property changes
 * are applied consistently across all selected nodes.
 */

/**
 * @brief Sets a property value on all managed nodes.
 * 
 * @param p_name The property name to set
 * @param p_value The value to assign to the property
 * @return true if the property was successfully set, false otherwise
 */
bool MultiNodeEdit::_set(const StringName &p_name, const Variant &p_value);

/**
 * @brief Internal implementation for setting properties with optional field-wise assignment.
 * 
 * Handles property assignment for multiple nodes with support for:
 * - Whole value replacement
 * - Field-wise property updates (e.g., updating only one component of a Vector3)
 * - NodePath reference resolution relative to each node
 * - Undo/redo action creation and commit
 * 
 * @param p_name The property name to set
 * @param p_value The value to assign
 * @param p_field The specific field to update (empty string for whole value)
 * @return true if the operation succeeded, false if no edited scene exists
 */
bool MultiNodeEdit::_set_impl(const StringName &p_name, const Variant &p_value, const String &p_field);

/**
 * @brief Retrieves a property value from the first matching node.
 * 
 * @param p_name The property name to retrieve
 * @param r_ret Output parameter containing the retrieved property value
 * @return true if the property was found and retrieved, false otherwise
 */
bool MultiNodeEdit::_get(const StringName &p_name, Variant &r_ret) const;

/**
 * @brief Collects and filters properties common to all managed nodes.
 * 
 * Only includes properties that exist with identical PropertyInfo on all nodes.
 * Automatically adds a "scripts" property for batch script assignment.
 * 
 * @param p_list Output list of properties available for multi-node editing
 */
void MultiNodeEdit::_get_property_list(List<PropertyInfo> *p_list) const;

/**
 * @brief Clears all managed nodes from the editor.
 */
void MultiNodeEdit::clear_nodes();

/**
 * @brief Adds a node to the list of nodes being edited.
 * 
 * @param p_node The NodePath of the node to add
 */
void MultiNodeEdit::add_node(const NodePath &p_node);

/**
 * @brief Returns the number of nodes currently being edited.
 * 
 * @return The count of managed nodes
 */
int MultiNodeEdit::get_node_count() const;

/**
 * @brief Retrieves the NodePath of a managed node by index.
 * 
 * @param p_index The index of the node to retrieve
 * @return The NodePath at the given index, or empty NodePath if index is invalid
 */
NodePath MultiNodeEdit::get_node(int p_index) const;

/**
 * @brief Sets a specific field of a property on all managed nodes.
 * 
 * Used for updating individual components of complex properties without
 * affecting other fields of the same property.
 * 
 * @param p_property The property name containing the field
 * @param p_value The value to assign to the field
 * @param p_field The specific field name within the property
 */
void MultiNodeEdit::set_property_field(const StringName &p_property, const Variant &p_value, const String &p_field);

/**
 * @brief Constructs a new MultiNodeEdit instance.
 */
MultiNodeEdit::MultiNodeEdit();
#include "multi_node_edit.h"

#include "core/math/math_fieldwise.h"
#include "editor_node.h"

bool MultiNodeEdit::_set(const StringName &p_name, const Variant &p_value) {
	return _set_impl(p_name, p_value, "");
}

bool MultiNodeEdit::_set_impl(const StringName &p_name, const Variant &p_value, const String &p_field) {
	Node *es = EditorNode::get_singleton()->get_edited_scene();
	if (!es)
		return false;

	String name = p_name;

	if (name == "scripts") { // script set is intercepted at object level (check Variant Object::get() ) ,so use a different name
		name = "script";
	}

	UndoRedo *ur = EditorNode::get_undo_redo();

	ur->create_action(TTR("MultiNode Set") + " " + String(name), UndoRedo::MERGE_ENDS);
	for (const List<NodePath>::Element *E = nodes.front(); E; E = E->next()) {

		if (!es->has_node(E->get()))
			continue;

		Node *n = es->get_node(E->get());
		if (!n)
			continue;

		if (p_value.get_type() == Variant::NODE_PATH) {
			Node *tonode = n->get_node(p_value);
			NodePath p_path = n->get_path_to(tonode);
			ur->add_do_property(n, name, p_path);
		} else {
			Variant new_value;
			if (p_field == "") {
				// whole value
				new_value = p_value;
			} else {
				// only one field
				new_value = fieldwise_assign(n->get(name), p_value, p_field);
			}
			ur->add_do_property(n, name, new_value);
		}

		ur->add_undo_property(n, name, n->get(name));
	}
	ur->add_do_method(EditorNode::get_singleton()->get_inspector(), "refresh");
	ur->add_undo_method(EditorNode::get_singleton()->get_inspector(), "refresh");

	ur->commit_action();
	return true;
}

bool MultiNodeEdit::_get(const StringName &p_name, Variant &r_ret) const {
	Node *es = EditorNode::get_singleton()->get_edited_scene();
	if (!es)
		return false;

	String name = p_name;
	if (name == "scripts") { // script set is intercepted at object level (check Variant Object::get() ) ,so use a different name
		name = "script";
	}

	for (const List<NodePath>::Element *E = nodes.front(); E; E = E->next()) {

		if (!es->has_node(E->get()))
			continue;

		const Node *n = es->get_node(E->get());
		if (!n)
			continue;

		bool found;
		r_ret = n->get(name, &found);
		if (found)
			return true;
	}

	return false;
}

void MultiNodeEdit::_get_property_list(List<PropertyInfo> *p_list) const {
	HashMap<String, PLData> usage;

	Node *es = EditorNode::get_singleton()->get_edited_scene();
	if (!es)
		return;

	int nc = 0;

	List<PLData *> data_list;

	for (const List<NodePath>::Element *E = nodes.front(); E; E = E->next()) {

		if (!es->has_node(E->get()))
			continue;

		Node *n = es->get_node(E->get());
		if (!n)
			continue;

		List<PropertyInfo> plist;
		n->get_property_list(&plist, true);

		for (List<PropertyInfo>::Element *F = plist.front(); F; F = F->next()) {

			if (F->get().name == "script")
				continue; //added later manually, since this is intercepted before being set (check Variant Object::get() )
			if (!usage.has(F->get().name)) {
				PLData pld;
				pld.uses = 0;
				pld.info = F->get();
				usage[F->get().name] = pld;
				data_list.push_back(usage.getptr(F->get().name));
			}

			// Make sure only properties with the same exact PropertyInfo data will appear
			if (usage[F->get().name].info == F->get())
				usage[F->get().name].uses++;
		}

		nc++;
	}

	for (List<PLData *>::Element *E = data_list.front(); E; E = E->next()) {

		if (nc == E->get()->uses) {
			p_list->push_back(E->get()->info);
		}
	}

	p_list->push_back(PropertyInfo(Variant::OBJECT, "scripts", PROPERTY_HINT_RESOURCE_TYPE, "Script"));
}

void MultiNodeEdit::clear_nodes() {
	nodes.clear();
}

void MultiNodeEdit::add_node(const NodePath &p_node) {
	nodes.push_back(p_node);
}

int MultiNodeEdit::get_node_count() const {
	return nodes.size();
}

NodePath MultiNodeEdit::get_node(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, nodes.size(), NodePath());
	return nodes[p_index];
}

void MultiNodeEdit::set_property_field(const StringName &p_property, const Variant &p_value, const String &p_field) {
	_set_impl(p_property, p_value, p_field);
}

MultiNodeEdit::MultiNodeEdit() {
}

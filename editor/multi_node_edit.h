

/**
 * @class MultiNodeEdit
 * @brief A reference-counted class for editing properties across multiple selected nodes.
 * 
 * This class allows simultaneous modification of properties on multiple nodes.
 * It maintains a list of node paths and provides methods to manage them and set
 * properties that apply to all selected nodes.
 * 
 * @note Inherits from Reference, so instances are automatically freed when no longer referenced.
 */

/**
 * @struct PLData
 * @brief Property list data structure for tracking property information.
 * 
 * @member uses - Number of nodes that use this property
 * @member info - PropertyInfo containing metadata about the property
 */

/**
 * @fn bool _set_impl(const StringName &p_name, const Variant &p_value, const String &p_field)
 * @brief Internal implementation for setting property values.
 * 
 * @param p_name - The name of the property to set
 * @param p_value - The value to set
 * @param p_field - Optional field specifier for nested properties
 * @return bool - True if property was successfully set
 */

/**
 * @fn void clear_nodes()
 * @brief Removes all nodes from the edit list.
 */

/**
 * @fn void add_node(const NodePath &p_node)
 * @brief Adds a node to the multi-edit list.
 * 
 * @param p_node - The path to the node to add
 */

/**
 * @fn int get_node_count() const
 * @brief Returns the number of nodes in the edit list.
 * 
 * @return int - Count of managed nodes
 */

/**
 * @fn NodePath get_node(int p_index) const
 * @brief Retrieves a node path by index.
 * 
 * @param p_index - The index of the node to retrieve
 * @return NodePath - The path to the requested node
 */

/**
 * @fn void set_property_field(const StringName &p_property, const Variant &p_value, const String &p_field)
 * @brief Sets a property field on all managed nodes.
 * 
 * @param p_property - The property name to modify
 * @param p_value - The value to set
 * @param p_field - The specific field within the property
 */
#ifndef MULTI_NODE_EDIT_H
#define MULTI_NODE_EDIT_H

#include "scene/main/node.h"

class MultiNodeEdit : public Reference {

	GDCLASS(MultiNodeEdit, Reference);

	List<NodePath> nodes;
	struct PLData {
		int uses;
		PropertyInfo info;
	};

	bool _set_impl(const StringName &p_name, const Variant &p_value, const String &p_field);

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

public:
	void clear_nodes();
	void add_node(const NodePath &p_node);

	int get_node_count() const;
	NodePath get_node(int p_index) const;

	void set_property_field(const StringName &p_property, const Variant &p_value, const String &p_field);

	MultiNodeEdit();
};

#endif // MULTI_NODE_EDIT_H

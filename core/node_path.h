

/**
 * @class NodePath
 * @brief Represents a path to a node in a scene tree hierarchy.
 * 
 * NodePath is used to reference nodes in the scene tree using a path-like notation.
 * It can represent both absolute paths (from root) and relative paths, with support
 * for property access through subpaths.
 * 
 * The class uses reference counting for memory management and includes hash caching
 * for efficient lookups and comparisons.
 * 
 * @note NodePath uses copy-on-write semantics through the shared Data structure.
 */

/**
 * @brief Checks if the path is absolute (starts from root).
 * @return true if the path is absolute, false if relative.
 */

/**
 * @brief Gets the number of node names in the path.
 * @return The count of node names in the main path.
 */

/**
 * @brief Retrieves a node name at the specified index.
 * @param p_idx The index of the node name to retrieve.
 * @return The StringName at the given index.
 */

/**
 * @brief Gets the number of subpath components (property access).
 * @return The count of subpath components.
 */

/**
 * @brief Retrieves a subpath component at the specified index.
 * @param p_idx The index of the subpath component to retrieve.
 * @return The StringName subpath component at the given index.
 */

/**
 * @brief Gets all node names in the path.
 * @return A vector containing all StringName components of the main path.
 */

/**
 * @brief Gets all subpath components.
 * @return A vector containing all StringName subpath components.
 */

/**
 * @brief Gets the concatenated representation of all subpaths.
 * @return A StringName containing the concatenated subpath.
 */

/**
 * @brief Computes a relative path from this path to another path.
 * @param p_np The target NodePath to compute the relative path to.
 * @return A new NodePath representing the relative path.
 */

/**
 * @brief Converts the path to a property path format.
 * @return A new NodePath formatted as a property path.
 */

/**
 * @brief Prepends a period to the path for property access.
 */

/**
 * @brief Gets the parent path by removing the last node component.
 * @return A new NodePath representing the parent node.
 */

/**
 * @brief Computes a 32-bit hash value for the path.
 * @return A uint32_t hash value, cached for efficiency.
 */

/**
 * @brief Converts the NodePath to its string representation.
 * @return A String representation of the path.
 */

/**
 * @brief Checks if the path is empty.
 * @return true if the path contains no components, false otherwise.
 */

/**
 * @brief Equality comparison operator.
 * @param p_path The NodePath to compare with.
 * @return true if paths are equal, false otherwise.
 */

/**
 * @brief Inequality comparison operator.
 * @param p_path The NodePath to compare with.
 * @return true if paths are not equal, false otherwise.
 */

/**
 * @brief Assignment operator.
 * @param p_path The NodePath to assign from.
 */

/**
 * @brief Simplifies the path in-place by removing unnecessary components.
 */

/**
 * @brief Returns a simplified copy of the path.
 * @return A new NodePath with redundant components removed.
 */

/**
 * @brief Constructs a NodePath from a vector of node names.
 * @param p_path Vector of StringName components forming the path.
 * @param p_absolute Whether the path is absolute or relative.
 */

/**
 * @brief Constructs a NodePath from node names and subpath components.
 * @param p_path Vector of StringName node components.
 * @param p_subpath Vector of StringName subpath components.
 * @param p_absolute Whether the path is absolute or relative.
 */

/**
 * @brief Copy constructor.
 * @param p_path The NodePath to copy from.
 */

/**
 * @brief Constructs a NodePath from its string representation.
 * @param p_path A String containing the path in text format.
 */

/**
 * @brief Default constructor. Creates an empty NodePath.
 */

/**
 * @brief Destructor. Releases the internal data reference.
 */
#ifndef NODE_PATH_H
#define NODE_PATH_H

#include "core/string_name.h"
#include "core/ustring.h"

class NodePath {

	struct Data {

		SafeRefCount refcount;
		Vector<StringName> path;
		Vector<StringName> subpath;
		StringName concatenated_subpath;
		bool absolute;
		bool has_slashes;
		mutable bool hash_cache_valid;
		mutable uint32_t hash_cache;
	};

	mutable Data *data;
	void unref();

	void _update_hash_cache() const;

public:
	bool is_absolute() const;
	int get_name_count() const;
	StringName get_name(int p_idx) const;
	int get_subname_count() const;
	StringName get_subname(int p_idx) const;
	Vector<StringName> get_names() const;
	Vector<StringName> get_subnames() const;
	StringName get_concatenated_subnames() const;

	NodePath rel_path_to(const NodePath &p_np) const;
	NodePath get_as_property_path() const;

	void prepend_period();

	NodePath get_parent() const;

	_FORCE_INLINE_ uint32_t hash() const {
		if (!data)
			return 0;
		if (!data->hash_cache_valid) {
			_update_hash_cache();
		}
		return data->hash_cache;
	}

	operator String() const;
	bool is_empty() const;

	bool operator==(const NodePath &p_path) const;
	bool operator!=(const NodePath &p_path) const;
	void operator=(const NodePath &p_path);

	void simplify();
	NodePath simplified() const;

	NodePath(const Vector<StringName> &p_path, bool p_absolute);
	NodePath(const Vector<StringName> &p_path, const Vector<StringName> &p_subpath, bool p_absolute);
	NodePath(const NodePath &p_path);
	NodePath(const String &p_path);
	NodePath();
	~NodePath();
};

#endif // NODE_PATH_H

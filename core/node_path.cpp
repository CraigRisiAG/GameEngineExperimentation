

/**
 * @class NodePath
 * @brief Represents a path to a node in a scene tree with support for absolute/relative paths and subpaths.
 * 
 * NodePath provides a way to reference nodes in a hierarchical structure using path-like syntax.
 * Paths can be absolute (starting with '/') or relative, and can include subpaths (properties) 
 * separated by colons. The class uses reference counting for memory management.
 * 
 * Example paths:
 * - "/root/child/grandchild" (absolute path)
 * - "sibling/cousin" (relative path)
 * - "/root/child:property" (path with subpath)
 * - "../other_node" (relative path with parent reference)
 */

/**
 * @brief Updates the cached hash value for this NodePath.
 * @details Recalculates the hash by combining hashes of all path and subpath components.
 * Sets hash_cache_valid to true after computation.
 */
void _update_hash_cache() const;

/**
 * @brief Prepends a period (".") to the beginning of the path if not already present.
 * @details This makes a relative path explicitly relative. Invalidates the hash cache.
 */
void prepend_period();

/**
 * @brief Checks if this NodePath is an absolute path.
 * @return true if the path is absolute (starts with '/'), false otherwise or if uninitialized.
 */
bool is_absolute() const;

/**
 * @brief Gets the number of components in the main path.
 * @return Number of path segments (directories/nodes).
 */
int get_name_count() const;

/**
 * @brief Retrieves a specific path component by index.
 * @param p_idx Index of the path component to retrieve.
 * @return The StringName at the given index, or empty StringName if invalid.
 */
StringName get_name(int p_idx) const;

/**
 * @brief Gets the number of subpath components (properties).
 * @return Number of subpath segments.
 */
int get_subname_count() const;

/**
 * @brief Retrieves a specific subpath component by index.
 * @param p_idx Index of the subpath component to retrieve.
 * @return The StringName at the given index, or empty StringName if invalid.
 */
StringName get_subname(int p_idx) const;

/**
 * @brief Decrements the reference count and deletes the internal data if count reaches zero.
 */
void unref();

/**
 * @brief Compares two NodePath objects for equality.
 * @param p_path The NodePath to compare with.
 * @return true if both paths represent the same path, false otherwise.
 */
bool operator==(const NodePath &p_path) const;

/**
 * @brief Compares two NodePath objects for inequality.
 * @param p_path The NodePath to compare with.
 * @return true if paths are different, false if equal.
 */
bool operator!=(const NodePath &p_path) const;

/**
 * @brief Assigns another NodePath to this one with reference counting.
 * @param p_path The NodePath to assign from.
 */
void operator=(const NodePath &p_path);

/**
 * @brief Converts the NodePath to its string representation.
 * @return String representation of the path (e.g., "/root/child:property").
 */
operator String() const;

/**
 * @brief Copy constructor.
 * @param p_path The NodePath to copy.
 */
NodePath(const NodePath &p_path);

/**
 * @brief Gets all path components as a Vector of StringNames.
 * @return Vector containing all path segments.
 */
Vector<StringName> get_names() const;

/**
 * @brief Gets all subpath components as a Vector of StringNames.
 * @return Vector containing all subpath segments.
 */
Vector<StringName> get_subnames() const;

/**
 * @brief Gets concatenated subpath as a single StringName with ':' separators.
 * @return Concatenated subpath string, cached for performance.
 */
StringName get_concatenated_subnames() const;

/**
 * @brief Calculates the relative path from this path to another absolute path.
 * @param p_np The destination NodePath (must be absolute).
 * @return A new NodePath representing the relative path from this to p_np.
 * @note Both paths must be absolute for this to work correctly.
 */
NodePath rel_path_to(const NodePath &p_np) const;

/**
 * @brief Converts this NodePath to a property path format.
 * @details Moves path components into the subpath with '/' separators.
 * @return A new NodePath with restructured path/subpath components.
 */
NodePath get_as_property_path() const;

/**
 * @brief Constructor from a vector of StringNames representing the path.
 * @param p_path Vector of path components.
 * @param p_absolute Whether the path is absolute.
 */
NodePath(const Vector<StringName> &p_path, bool p_absolute);

/**
 * @brief Constructor from path and subpath vectors.
 * @param p_path Vector of path components.
 * @param p_subpath Vector of subpath (property) components.
 * @param p_absolute Whether the path is absolute.
 */
NodePath(const Vector<StringName> &p_path, const Vector<StringName> &p_subpath, bool p_absolute);

/**
 * @brief Simplifies the path by removing '.' and resolving '..' references.
 * @details Modifies the path in-place, removing unnecessary components.
 * Invalidates the hash cache.
 */
void simplify();

/**
 * @brief Returns a simplified copy of this NodePath.
 * @return A new NodePath with simplified path.
 */
NodePath simplified() const;

/**
 * @brief Constructor from a string representation of a path.
 * @param p_path String representation (e.g., "/root/child:property:subproperty").
 * @details Parses the string to extract path, subpath, and absolute/relative nature.
 */
NodePath(const String &p_path);

/**
 * @brief Checks if this NodePath is empty (uninitialized).
 * @return true if the NodePath contains no data, false otherwise.
 */
bool is_empty() const;

/**
 * @brief Default constructor creating an empty NodePath.
 */
NodePath();

/**
 * @brief Destructor that releases the internal data reference.
 */
~NodePath();
#include "node_path.h"

#include "core/print_string.h"

void NodePath::_update_hash_cache() const {

	uint32_t h = data->absolute ? 1 : 0;
	int pc = data->path.size();
	const StringName *sn = data->path.ptr();
	for (int i = 0; i < pc; i++) {
		h = h ^ sn[i].hash();
	}
	int spc = data->subpath.size();
	const StringName *ssn = data->subpath.ptr();
	for (int i = 0; i < spc; i++) {
		h = h ^ ssn[i].hash();
	}

	data->hash_cache_valid = true;
	data->hash_cache = h;
}

void NodePath::prepend_period() {

	if (data->path.size() && data->path[0].operator String() != ".") {
		data->path.insert(0, ".");
		data->hash_cache_valid = false;
	}
}

bool NodePath::is_absolute() const {

	if (!data)
		return false;

	return data->absolute;
}
int NodePath::get_name_count() const {

	if (!data)
		return 0;

	return data->path.size();
}
StringName NodePath::get_name(int p_idx) const {

	ERR_FAIL_COND_V(!data, StringName());
	ERR_FAIL_INDEX_V(p_idx, data->path.size(), StringName());
	return data->path[p_idx];
}

int NodePath::get_subname_count() const {

	if (!data)
		return 0;

	return data->subpath.size();
}
StringName NodePath::get_subname(int p_idx) const {

	ERR_FAIL_COND_V(!data, StringName());
	ERR_FAIL_INDEX_V(p_idx, data->subpath.size(), StringName());
	return data->subpath[p_idx];
}

void NodePath::unref() {

	if (data && data->refcount.unref()) {

		memdelete(data);
	}
	data = NULL;
}

bool NodePath::operator==(const NodePath &p_path) const {

	if (data == p_path.data)
		return true;

	if (!data || !p_path.data)
		return false;

	if (data->absolute != p_path.data->absolute)
		return false;

	int path_size = data->path.size();

	if (path_size != p_path.data->path.size()) {
		return false;
	}

	int subpath_size = data->subpath.size();

	if (subpath_size != p_path.data->subpath.size()) {
		return false;
	}

	const StringName *l_path_ptr = data->path.ptr();
	const StringName *r_path_ptr = p_path.data->path.ptr();

	for (int i = 0; i < path_size; i++) {

		if (l_path_ptr[i] != r_path_ptr[i])
			return false;
	}

	const StringName *l_subpath_ptr = data->subpath.ptr();
	const StringName *r_subpath_ptr = p_path.data->subpath.ptr();

	for (int i = 0; i < subpath_size; i++) {

		if (l_subpath_ptr[i] != r_subpath_ptr[i])
			return false;
	}

	return true;
}
bool NodePath::operator!=(const NodePath &p_path) const {

	return (!(*this == p_path));
}

void NodePath::operator=(const NodePath &p_path) {

	if (this == &p_path)
		return;

	unref();

	if (p_path.data && p_path.data->refcount.ref()) {

		data = p_path.data;
	}
}

NodePath::operator String() const {

	if (!data)
		return String();

	String ret;
	if (data->absolute)
		ret = "/";

	for (int i = 0; i < data->path.size(); i++) {

		if (i > 0)
			ret += "/";
		ret += data->path[i].operator String();
	}

	for (int i = 0; i < data->subpath.size(); i++) {

		ret += ":" + data->subpath[i].operator String();
	}

	return ret;
}

NodePath::NodePath(const NodePath &p_path) {

	data = NULL;

	if (p_path.data && p_path.data->refcount.ref()) {

		data = p_path.data;
	}
}

Vector<StringName> NodePath::get_names() const {

	if (data)
		return data->path;
	return Vector<StringName>();
}

Vector<StringName> NodePath::get_subnames() const {

	if (data)
		return data->subpath;
	return Vector<StringName>();
}

StringName NodePath::get_concatenated_subnames() const {
	ERR_FAIL_COND_V(!data, StringName());

	if (!data->concatenated_subpath) {
		int spc = data->subpath.size();
		String concatenated;
		const StringName *ssn = data->subpath.ptr();
		for (int i = 0; i < spc; i++) {
			concatenated += i == 0 ? ssn[i].operator String() : ":" + ssn[i];
		}
		data->concatenated_subpath = concatenated;
	}
	return data->concatenated_subpath;
}

NodePath NodePath::rel_path_to(const NodePath &p_np) const {

	ERR_FAIL_COND_V(!is_absolute(), NodePath());
	ERR_FAIL_COND_V(!p_np.is_absolute(), NodePath());

	Vector<StringName> src_dirs = get_names();
	Vector<StringName> dst_dirs = p_np.get_names();

	//find common parent
	int common_parent = 0;

	while (true) {
		if (src_dirs.size() == common_parent)
			break;
		if (dst_dirs.size() == common_parent)
			break;
		if (src_dirs[common_parent] != dst_dirs[common_parent])
			break;
		common_parent++;
	}

	common_parent--;

	Vector<StringName> relpath;

	for (int i = src_dirs.size() - 1; i > common_parent; i--) {

		relpath.push_back("..");
	}

	for (int i = common_parent + 1; i < dst_dirs.size(); i++) {

		relpath.push_back(dst_dirs[i]);
	}

	if (relpath.size() == 0)
		relpath.push_back(".");

	return NodePath(relpath, p_np.get_subnames(), false);
}

NodePath NodePath::get_as_property_path() const {

	if (!data || !data->path.size()) {
		return *this;
	} else {
		Vector<StringName> new_path = data->subpath;

		String initial_subname = data->path[0];

		for (int i = 1; i < data->path.size(); i++) {
			initial_subname += "/" + data->path[i];
		}
		new_path.insert(0, initial_subname);

		return NodePath(Vector<StringName>(), new_path, false);
	}
}

NodePath::NodePath(const Vector<StringName> &p_path, bool p_absolute) {

	data = NULL;

	if (p_path.size() == 0)
		return;

	data = memnew(Data);
	data->refcount.init();
	data->absolute = p_absolute;
	data->path = p_path;
	data->has_slashes = true;
	data->hash_cache_valid = false;
}

NodePath::NodePath(const Vector<StringName> &p_path, const Vector<StringName> &p_subpath, bool p_absolute) {

	data = NULL;

	if (p_path.size() == 0 && p_subpath.size() == 0)
		return;

	data = memnew(Data);
	data->refcount.init();
	data->absolute = p_absolute;
	data->path = p_path;
	data->subpath = p_subpath;
	data->has_slashes = true;
	data->hash_cache_valid = false;
}

void NodePath::simplify() {

	if (!data)
		return;
	for (int i = 0; i < data->path.size(); i++) {
		if (data->path.size() == 1)
			break;
		if (data->path[i].operator String() == ".") {
			data->path.remove(i);
			i--;
		} else if (data->path[i].operator String() == ".." && i > 0 && data->path[i - 1].operator String() != "." && data->path[i - 1].operator String() != "..") {
			//remove both
			data->path.remove(i - 1);
			data->path.remove(i - 1);
			i -= 2;
			if (data->path.size() == 0) {
				data->path.push_back(".");
				break;
			}
		}
	}
	data->hash_cache_valid = false;
}

NodePath NodePath::simplified() const {

	NodePath np = *this;
	np.simplify();
	return np;
}

NodePath::NodePath(const String &p_path) {

	data = NULL;

	if (p_path.length() == 0)
		return;

	String path = p_path;
	Vector<StringName> subpath;

	bool absolute = (path[0] == '/');
	bool last_is_slash = true;
	bool has_slashes = false;
	int slices = 0;
	int subpath_pos = path.find(":");

	if (subpath_pos != -1) {

		int from = subpath_pos + 1;

		for (int i = from; i <= path.length(); i++) {

			if (path[i] == ':' || path[i] == 0) {

				String str = path.substr(from, i - from);
				if (str == "") {
					if (path[i] == 0) continue; // Allow end-of-path :

					ERR_FAIL_MSG("Invalid NodePath '" + p_path + "'.");
				}
				subpath.push_back(str);

				from = i + 1;
			}
		}

		path = path.substr(0, subpath_pos);
	}

	for (int i = (int)absolute; i < path.length(); i++) {

		if (path[i] == '/') {

			last_is_slash = true;
			has_slashes = true;
		} else {

			if (last_is_slash)
				slices++;

			last_is_slash = false;
		}
	}

	if (slices == 0 && !absolute && !subpath.size())
		return;

	data = memnew(Data);
	data->refcount.init();
	data->absolute = absolute;
	data->has_slashes = has_slashes;
	data->subpath = subpath;
	data->hash_cache_valid = false;

	if (slices == 0)
		return;
	data->path.resize(slices);
	last_is_slash = true;
	int from = (int)absolute;
	int slice = 0;

	for (int i = (int)absolute; i < path.length() + 1; i++) {

		if (path[i] == '/' || path[i] == 0) {

			if (!last_is_slash) {

				String name = path.substr(from, i - from);
				ERR_FAIL_INDEX(slice, data->path.size());
				data->path.write[slice++] = name;
			}
			from = i + 1;
			last_is_slash = true;
		} else {
			last_is_slash = false;
		}
	}
}

bool NodePath::is_empty() const {

	return !data;
}
NodePath::NodePath() {

	data = NULL;
}

NodePath::~NodePath() {

	unref();
}

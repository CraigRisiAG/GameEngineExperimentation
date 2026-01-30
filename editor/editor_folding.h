

/// @class EditorFolding
/// @brief Manages UI folding state for editor nodes and resources.
///
/// This class handles the persistence and restoration of folding states for nodes
/// and resources in the editor, allowing users to collapse/expand hierarchies and
/// have their preferences saved and restored.
///
/// @note Folding data is stored externally and loaded/saved on demand.

/// @brief Retrieves the list of unfolded paths for an object.
/// @param p_object Pointer to the object to query.
/// @return Vector of strings representing unfolded node/property paths.
Vector<String> _get_unfolds(const Object *p_object);

/// @brief Sets the unfolded state for an object.
/// @param p_object Pointer to the object to update.
/// @param p_unfolds Vector of paths that should be unfolded.
void _set_unfolds(Object *p_object, const Vector<String> &p_unfolds);

/// @brief Recursively collects fold states from a node hierarchy.
/// @param p_root The root node of the scene.
/// @param p_node Current node being processed.
/// @param p_folds Output array for general fold data.
/// @param resource_folds Output array for resource-specific fold data.
/// @param nodes_folded Output array for folded node states.
/// @param resources Set of resources encountered during traversal.
void _fill_folds(const Node *p_root, const Node *p_node, Array &p_folds, Array &resource_folds, Array &nodes_folded, Set<RES> &resources);

/// @brief Applies unfold state to an object and its properties.
/// @param p_object Pointer to the object to unfold.
/// @param resources Set of resources to track.
void _do_object_unfolds(Object *p_object, Set<RES> &resources);

/// @brief Applies unfold state to a node and its children.
/// @param p_root The root node of the scene.
/// @param p_node Current node to unfold.
/// @param resources Set of resources to track.
void _do_node_unfolds(Node *p_root, Node *p_node, Set<RES> &resources);

/// @brief Saves the folding state of a resource to a file.
/// @param p_resource The resource whose folding state should be saved.
/// @param p_path File path where the folding data will be stored.
void save_resource_folding(const RES &p_resource, const String &p_path);

/// @brief Loads previously saved folding state into a resource.
/// @param p_resource The resource to restore folding state for.
/// @param p_path File path containing the folding data.
void load_resource_folding(RES p_resource, const String &p_path);

/// @brief Saves the folding state of a scene to a file.
/// @param p_scene The root node of the scene whose state should be saved.
/// @param p_path File path where the folding data will be stored.
void save_scene_folding(const Node *p_scene, const String &p_path);

/// @brief Loads previously saved folding state into a scene.
/// @param p_scene The root node of the scene to restore state for.
/// @param p_path File path containing the folding data.
void load_scene_folding(Node *p_scene, const String &p_path);

/// @brief Unfolds all nodes in a scene hierarchy.
/// @param p_scene The root node of the scene to unfold.
void unfold_scene(Node *p_scene);

/// @brief Checks if folding data exists for a given path.
/// @param p_path The file path to check.
/// @return True if folding data file exists at the path, false otherwise.
bool has_folding_data(const String &p_path);

/// @brief Constructs an EditorFolding instance.
EditorFolding();
#ifndef EDITOR_FOLDING_H
#define EDITOR_FOLDING_H

#include "scene/main/node.h"

class EditorFolding {

	Vector<String> _get_unfolds(const Object *p_object);
	void _set_unfolds(Object *p_object, const Vector<String> &p_unfolds);

	void _fill_folds(const Node *p_root, const Node *p_node, Array &p_folds, Array &resource_folds, Array &nodes_folded, Set<RES> &resources);

	void _do_object_unfolds(Object *p_object, Set<RES> &resources);
	void _do_node_unfolds(Node *p_root, Node *p_node, Set<RES> &resources);

public:
	void save_resource_folding(const RES &p_resource, const String &p_path);
	void load_resource_folding(RES p_resource, const String &p_path);

	void save_scene_folding(const Node *p_scene, const String &p_path);
	void load_scene_folding(Node *p_scene, const String &p_path);

	void unfold_scene(Node *p_scene);

	bool has_folding_data(const String &p_path);

	EditorFolding();
};

#endif // EDITOR_FOLDING_H

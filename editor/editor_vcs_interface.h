


/**
 * @class EditorVCSInterface
 * @brief Interface for Version Control System integration in the Godot editor.
 * 
 * EditorVCSInterface provides a proxy pattern for communicating with VCS addons.
 * It acts as a singleton that manages initialization, file staging, committing,
 * and other VCS operations through virtual methods implemented by addon plugins.
 * 
 * The class maintains two sets of methods:
 * - Protected virtual methods (prefixed with '_') implemented by addons
 * - Public proxy methods that call the corresponding addon implementations
 * 
 * This design allows the editor to interact with VCS systems without direct
 * dependency on specific implementation details.
 * 
 * @note This is a singleton class. Use get_singleton() to access the instance.
 * 
 * @see set_singleton()
 * @see get_singleton()
 */

/**
 * @brief Initializes the VCS addon with the project root path.
 * @param p_project_root_path The root directory path of the project.
 * @return true if initialization was successful, false otherwise.
 */

/**
 * @brief Checks if a VCS has been initialized.
 * @return true if VCS is initialized, false otherwise.
 */

/**
 * @brief Retrieves data about all modified files in the project.
 * @return Dictionary containing modified files and their metadata.
 */

/**
 * @brief Stages a file for commit.
 * @param p_file_path Path to the file to stage.
 */

/**
 * @brief Unstages a file from the commit staging area.
 * @param p_file_path Path to the file to unstage.
 */

/**
 * @brief Commits staged changes with a message.
 * @param p_msg The commit message.
 */

/**
 * @brief Retrieves the diff for a specific file.
 * @param p_file_path Path to the file.
 * @return Array containing diff information.
 */

/**
 * @brief Shuts down the VCS addon.
 * @return true if shutdown was successful, false otherwise.
 */

/**
 * @brief Gets the name of the current project.
 * @return String containing the project name.
 */

/**
 * @brief Gets the name of the VCS system in use.
 * @return String containing the VCS name (e.g., "Git").
 */
#ifndef EDITOR_VCS_INTERFACE_H
#define EDITOR_VCS_INTERFACE_H

#include "core/object.h"
#include "core/ustring.h"
#include "scene/gui/panel_container.h"

class EditorVCSInterface : public Object {

	GDCLASS(EditorVCSInterface, Object)

	bool is_initialized;

protected:
	static EditorVCSInterface *singleton;

	static void _bind_methods();

	// Implemented by addons as end points for the proxy functions
	bool _initialize(String p_project_root_path);
	bool _is_vcs_initialized();
	Dictionary _get_modified_files_data();
	void _stage_file(String p_file_path);
	void _unstage_file(String p_file_path);
	void _commit(String p_msg);
	Array _get_file_diff(String p_file_path);
	bool _shut_down();
	String _get_project_name();
	String _get_vcs_name();

public:
	static EditorVCSInterface *get_singleton();
	static void set_singleton(EditorVCSInterface *p_singleton);

	bool is_addon_ready();

	// Proxy functions to the editor for use
	bool initialize(String p_project_root_path);
	bool is_vcs_initialized();
	Dictionary get_modified_files_data();
	void stage_file(String p_file_path);
	void unstage_file(String p_file_path);
	void commit(String p_msg);
	Array get_file_diff(String p_file_path);
	bool shut_down();
	String get_project_name();
	String get_vcs_name();

	EditorVCSInterface();
	virtual ~EditorVCSInterface();
};

#endif // !EDITOR_VCS_INTERFACE_H



/// @class EditorVCSInterface
/// @brief Interface for Version Control System (VCS) addon integration in the editor
/// 
/// This class provides a bridge between the editor and VCS addon implementations using a proxy pattern.
/// It defines both internal proxy methods (prefixed with '_') that addons should override, and public API
/// methods that handle addon availability checks before delegating to the proxy methods.
/// 
/// The class uses a singleton pattern for global access and tracks initialization state to ensure
/// the VCS addon is ready before executing operations.
/// 
/// @note Proxy methods provide default fallback implementations that addons can override.
/// @note All file operations (stage, unstage, diff) require the addon to be initialized via initialize().

/// @method void _bind_methods()
/// @brief Binds GDScript methods to C++ implementations for Godot's reflection system
/// @details Registers both proxy endpoint methods and public API methods with their respective
///          C++ function pointers for dynamic invocation via call()

/// @method bool _initialize(String p_project_root_path)
/// @brief Proxy method for VCS initialization (default implementation - addon should override)
/// @param p_project_root_path Root directory path of the project
/// @return true (default fallback behavior)

/// @method bool _is_vcs_initialized()
/// @brief Proxy method to check VCS initialization status
/// @return false (default fallback behavior)

/// @method Dictionary _get_modified_files_data()
/// @brief Proxy method to retrieve modified files information
/// @return Empty Dictionary (default fallback behavior)

/// @method void _stage_file(String p_file_path)
/// @brief Proxy method to stage a file for commit
/// @param p_file_path Path to the file to stage

/// @method void _unstage_file(String p_file_path)
/// @brief Proxy method to unstage a file
/// @param p_file_path Path to the file to unstage

/// @method void _commit(String p_msg)
/// @brief Proxy method to commit staged changes
/// @param p_msg Commit message

/// @method Array _get_file_diff(String p_file_path)
/// @brief Proxy method to get file differences
/// @param p_file_path Path to the file
/// @return Empty Array (default fallback behavior)

/// @method bool _shut_down()
/// @brief Proxy method to shut down VCS interface
/// @return false (default fallback behavior)

/// @method String _get_project_name()
/// @brief Proxy method to retrieve project name
/// @return Empty String (default fallback behavior)

/// @method String _get_vcs_name()
/// @brief Proxy method to retrieve VCS system name
/// @return Empty String (default fallback behavior)

/// @method bool initialize(String p_project_root_path)
/// @brief Public API: Initialize the VCS addon for the given project
/// @param p_project_root_path Root directory path of the project
/// @return true if initialization succeeded

/// @method bool is_vcs_initialized()
/// @brief Public API: Check if VCS has been initialized
/// @return true if VCS is initialized

/// @method Dictionary get_modified_files_data()
/// @brief Public API: Get data about modified files in the repository
/// @return Dictionary containing file modification information

/// @method void stage_file(String p_file_path)
/// @brief Public API: Stage a file for commit (requires addon to be ready)
/// @param p_file_path Path to the file to stage

/// @method void unstage_file(String p_file_path)
/// @brief Public API: Unstage a file (requires addon to be ready)
/// @param p_file_path Path to the file to unstage

/// @method bool is_addon_ready()
/// @brief Check if the VCS addon is initialized and ready for operations
/// @return true if addon is initialized

/// @method void commit(String p_msg)
/// @brief Public API: Commit staged changes (requires addon to be ready)
/// @param p_msg Commit message

/// @method Array get_file_diff(String p_file_path)
/// @brief Public API: Get file differences (requires addon to be ready)
/// @param p_file_path Path to the file
/// @return Array of diff data or empty Array if addon not ready

/// @method bool shut_down()
/// @brief Public API: Shut down the VCS interface
/// @return true if shutdown succeeded

/// @method String get_project_name()
/// @brief Public API: Get the project name from VCS
/// @return Project name string

/// @method String get_vcs_name()
/// @brief Public API: Get the VCS system name (e.g., "Git")
/// @return VCS system name

/// @method EditorVCSInterface()
/// @brief Constructor - initializes the interface in uninitialized state

/// @method ~EditorVCSInterface()
/// @brief Destructor

/// @method static EditorVCSInterface* get_singleton()
/// @brief Get the singleton instance of EditorVCSInterface
/// @return Pointer to the singleton instance

/// @method static void set_singleton(EditorVCSInterface* p_singleton)
/// @brief Set the singleton instance of EditorVCSInterface
/// @param p_singleton Pointer to the instance to set as singleton

/// @var static EditorVCSInterface* singleton
/// @brief Singleton instance pointer

/// @var bool is_initialized
/// @brief Tracks whether the VCS addon has been successfully initialized
#include "editor_vcs_interface.h"

EditorVCSInterface *EditorVCSInterface::singleton = NULL;

void EditorVCSInterface::_bind_methods() {

	// Proxy end points that act as fallbacks to unavailability of a function in the VCS addon
	ClassDB::bind_method(D_METHOD("_initialize", "project_root_path"), &EditorVCSInterface::_initialize);
	ClassDB::bind_method(D_METHOD("_is_vcs_initialized"), &EditorVCSInterface::_is_vcs_initialized);
	ClassDB::bind_method(D_METHOD("_get_vcs_name"), &EditorVCSInterface::_get_vcs_name);
	ClassDB::bind_method(D_METHOD("_shut_down"), &EditorVCSInterface::_shut_down);
	ClassDB::bind_method(D_METHOD("_get_project_name"), &EditorVCSInterface::_get_project_name);
	ClassDB::bind_method(D_METHOD("_get_modified_files_data"), &EditorVCSInterface::_get_modified_files_data);
	ClassDB::bind_method(D_METHOD("_commit", "msg"), &EditorVCSInterface::_commit);
	ClassDB::bind_method(D_METHOD("_get_file_diff", "file_path"), &EditorVCSInterface::_get_file_diff);
	ClassDB::bind_method(D_METHOD("_stage_file", "file_path"), &EditorVCSInterface::_stage_file);
	ClassDB::bind_method(D_METHOD("_unstage_file", "file_path"), &EditorVCSInterface::_unstage_file);

	ClassDB::bind_method(D_METHOD("is_addon_ready"), &EditorVCSInterface::is_addon_ready);

	// API methods that redirect calls to the proxy end points
	ClassDB::bind_method(D_METHOD("initialize", "project_root_path"), &EditorVCSInterface::initialize);
	ClassDB::bind_method(D_METHOD("is_vcs_initialized"), &EditorVCSInterface::is_vcs_initialized);
	ClassDB::bind_method(D_METHOD("get_modified_files_data"), &EditorVCSInterface::get_modified_files_data);
	ClassDB::bind_method(D_METHOD("stage_file", "file_path"), &EditorVCSInterface::stage_file);
	ClassDB::bind_method(D_METHOD("unstage_file", "file_path"), &EditorVCSInterface::unstage_file);
	ClassDB::bind_method(D_METHOD("commit", "msg"), &EditorVCSInterface::commit);
	ClassDB::bind_method(D_METHOD("get_file_diff", "file_path"), &EditorVCSInterface::get_file_diff);
	ClassDB::bind_method(D_METHOD("shut_down"), &EditorVCSInterface::shut_down);
	ClassDB::bind_method(D_METHOD("get_project_name"), &EditorVCSInterface::get_project_name);
	ClassDB::bind_method(D_METHOD("get_vcs_name"), &EditorVCSInterface::get_vcs_name);
}

bool EditorVCSInterface::_initialize(String p_project_root_path) {

	WARN_PRINT("Selected VCS addon does not implement an initialization function. This warning will be suppressed.");
	return true;
}

bool EditorVCSInterface::_is_vcs_initialized() {

	return false;
}

Dictionary EditorVCSInterface::_get_modified_files_data() {

	return Dictionary();
}

void EditorVCSInterface::_stage_file(String p_file_path) {
}

void EditorVCSInterface::_unstage_file(String p_file_path) {
}

void EditorVCSInterface::_commit(String p_msg) {
}

Array EditorVCSInterface::_get_file_diff(String p_file_path) {

	return Array();
}

bool EditorVCSInterface::_shut_down() {

	return false;
}

String EditorVCSInterface::_get_project_name() {

	return String();
}

String EditorVCSInterface::_get_vcs_name() {

	return "";
}

bool EditorVCSInterface::initialize(String p_project_root_path) {

	is_initialized = call("_initialize", p_project_root_path);
	return is_initialized;
}

bool EditorVCSInterface::is_vcs_initialized() {

	return call("_is_vcs_initialized");
}

Dictionary EditorVCSInterface::get_modified_files_data() {

	return call("_get_modified_files_data");
}

void EditorVCSInterface::stage_file(String p_file_path) {

	if (is_addon_ready()) {

		call("_stage_file", p_file_path);
	}
}

void EditorVCSInterface::unstage_file(String p_file_path) {

	if (is_addon_ready()) {

		call("_unstage_file", p_file_path);
	}
}

bool EditorVCSInterface::is_addon_ready() {

	return is_initialized;
}

void EditorVCSInterface::commit(String p_msg) {

	if (is_addon_ready()) {

		call("_commit", p_msg);
	}
}

Array EditorVCSInterface::get_file_diff(String p_file_path) {

	if (is_addon_ready()) {

		return call("_get_file_diff", p_file_path);
	}
	return Array();
}

bool EditorVCSInterface::shut_down() {

	return call("_shut_down");
}

String EditorVCSInterface::get_project_name() {

	return call("_get_project_name");
}

String EditorVCSInterface::get_vcs_name() {

	return call("_get_vcs_name");
}

EditorVCSInterface::EditorVCSInterface() {

	is_initialized = false;
}

EditorVCSInterface::~EditorVCSInterface() {
}

EditorVCSInterface *EditorVCSInterface::get_singleton() {

	return singleton;
}

void EditorVCSInterface::set_singleton(EditorVCSInterface *p_singleton) {

	singleton = p_singleton;
}

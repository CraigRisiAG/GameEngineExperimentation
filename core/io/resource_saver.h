

/// \class ResourceFormatSaver
/// \brief Base class for custom resource format savers.
/// 
/// ResourceFormatSaver is an abstract base class that defines the interface for saving
/// resources in custom formats. Derived classes should override the virtual methods to
/// provide format-specific saving functionality.
///
/// \see ResourceSaver

/// \fn virtual Error ResourceFormatSaver::save(const String &p_path, const RES &p_resource, uint32_t p_flags)
/// \brief Saves a resource to the specified path.
/// \param p_path The file path where the resource should be saved.
/// \param p_resource The resource to save.
/// \param p_flags Optional flags controlling save behavior (default: 0).
/// \return Error code indicating success or failure.

/// \fn virtual bool ResourceFormatSaver::recognize(const RES &p_resource) const
/// \brief Determines if this saver can handle the given resource.
/// \param p_resource The resource to check.
/// \return True if this saver can save the resource, false otherwise.

/// \fn virtual void ResourceFormatSaver::get_recognized_extensions(const RES &p_resource, List<String> *p_extensions) const
/// \brief Returns the file extensions this saver recognizes for the given resource.
/// \param p_resource The resource to get extensions for.
/// \param p_extensions Pointer to a list where recognized extensions will be added.

/// \typedef ResourceSavedCallback
/// \brief Callback function type invoked when a resource is successfully saved.
/// \param p_resource The saved resource.
/// \param p_path The file path where the resource was saved.

/// \class ResourceSaver
/// \brief Static class managing resource saving and format savers.
///
/// ResourceSaver is a singleton-like static class that manages a collection of resource
/// format savers and provides a unified interface for saving resources in various formats.

/// \enum ResourceSaver::SaverFlags
/// \brief Flags controlling resource save behavior.
/// 
/// FLAG_RELATIVE_PATHS - Use relative paths in saved resources.
/// FLAG_BUNDLE_RESOURCES - Bundle resources together.
/// FLAG_CHANGE_PATH - Update the resource path.
/// FLAG_OMIT_EDITOR_PROPERTIES - Exclude editor-specific properties.
/// FLAG_SAVE_BIG_ENDIAN - Save in big-endian format.
/// FLAG_COMPRESS - Compress the saved resource.
/// FLAG_REPLACE_SUBRESOURCE_PATHS - Replace subresource paths.

/// \fn static Error ResourceSaver::save(const String &p_path, const RES &p_resource, uint32_t p_flags)
/// \brief Saves a resource using the appropriate registered format saver.
/// \param p_path The file path where the resource should be saved.
/// \param p_resource The resource to save.
/// \param p_flags Optional flags controlling save behavior (default: 0).
/// \return Error code indicating success or failure.

/// \fn static void ResourceSaver::get_recognized_extensions(const RES &p_resource, List<String> *p_extensions)
/// \brief Gets all file extensions recognized for saving the given resource.
/// \param p_resource The resource to get extensions for.
/// \param p_extensions Pointer to a list where recognized extensions will be added.

/// \fn static void ResourceSaver::add_resource_format_saver(Ref<ResourceFormatSaver> p_format_saver, bool p_at_front)
/// \brief Registers a new resource format saver.
/// \param p_format_saver The saver to register.
/// \param p_at_front If true, adds saver to the front of the list (checked first).

/// \fn static void ResourceSaver::remove_resource_format_saver(Ref<ResourceFormatSaver> p_format_saver)
/// \brief Unregisters a resource format saver.
/// \param p_format_saver The saver to remove.

/// \fn static void ResourceSaver::set_timestamp_on_save(bool p_timestamp)
/// \brief Enables or disables automatic timestamp updating when saving.
/// \param p_timestamp True to enable timestamps, false to disable.

/// \fn static bool ResourceSaver::get_timestamp_on_save()
/// \brief Checks if automatic timestamp updating is enabled.
/// \return True if timestamps are enabled, false otherwise.

/// \fn static void ResourceSaver::set_save_callback(ResourceSavedCallback p_callback)
/// \brief Sets a callback to be invoked when resources are saved.
/// \param p_callback The callback function to invoke, or null to disable.

/// \fn static bool ResourceSaver::add_custom_resource_format_saver(String script_path)
/// \brief Loads and registers a custom resource format saver from a script.
/// \param script_path Path to the script file.
/// \return True if successfully added, false otherwise.

/// \fn static void ResourceSaver::remove_custom_resource_format_saver(String script_path)
/// \brief Unregisters a custom resource format saver loaded from a script.
/// \param script_path Path to the script file.

/// \fn static void ResourceSaver::add_custom_savers()
/// \brief Loads all custom resource format savers.

/// \fn static void ResourceSaver::remove_custom_savers()
/// \brief Unloads all custom resource format savers.
#ifndef RESOURCE_SAVER_H
#define RESOURCE_SAVER_H

#include "core/resource.h"

class ResourceFormatSaver : public Reference {
	GDCLASS(ResourceFormatSaver, Reference);

protected:
	static void _bind_methods();

public:
	virtual Error save(const String &p_path, const RES &p_resource, uint32_t p_flags = 0);
	virtual bool recognize(const RES &p_resource) const;
	virtual void get_recognized_extensions(const RES &p_resource, List<String> *p_extensions) const;

	virtual ~ResourceFormatSaver() {}
};

typedef void (*ResourceSavedCallback)(Ref<Resource> p_resource, const String &p_path);

class ResourceSaver {

	enum {
		MAX_SAVERS = 64
	};

	static Ref<ResourceFormatSaver> saver[MAX_SAVERS];
	static int saver_count;
	static bool timestamp_on_save;
	static ResourceSavedCallback save_callback;

	static Ref<ResourceFormatSaver> _find_custom_resource_format_saver(String path);

public:
	enum SaverFlags {

		FLAG_RELATIVE_PATHS = 1,
		FLAG_BUNDLE_RESOURCES = 2,
		FLAG_CHANGE_PATH = 4,
		FLAG_OMIT_EDITOR_PROPERTIES = 8,
		FLAG_SAVE_BIG_ENDIAN = 16,
		FLAG_COMPRESS = 32,
		FLAG_REPLACE_SUBRESOURCE_PATHS = 64,
	};

	static Error save(const String &p_path, const RES &p_resource, uint32_t p_flags = 0);
	static void get_recognized_extensions(const RES &p_resource, List<String> *p_extensions);
	static void add_resource_format_saver(Ref<ResourceFormatSaver> p_format_saver, bool p_at_front = false);
	static void remove_resource_format_saver(Ref<ResourceFormatSaver> p_format_saver);

	static void set_timestamp_on_save(bool p_timestamp) { timestamp_on_save = p_timestamp; }
	static bool get_timestamp_on_save() { return timestamp_on_save; }

	static void set_save_callback(ResourceSavedCallback p_callback);

	static bool add_custom_resource_format_saver(String script_path);
	static void remove_custom_resource_format_saver(String script_path);
	static void add_custom_savers();
	static void remove_custom_savers();
};

#endif // RESOURCE_SAVER_H

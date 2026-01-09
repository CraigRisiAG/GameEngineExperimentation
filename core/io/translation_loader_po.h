

/**
 * @class TranslationLoaderPO
 * @brief Loader for PO (Portable Object) translation files.
 * 
 * This class handles loading and parsing of PO format translation files,
 * which are commonly used for internationalization (i18n) in applications.
 * It extends the ResourceFormatLoader base class to integrate with the
 * engine's resource loading system.
 */

/**
 * @brief Loads a translation from a PO file.
 * 
 * @param f Pointer to the FileAccess object to read from.
 * @param r_error Optional pointer to store error status.
 * @return RES The loaded Translation resource, or null if loading fails.
 */

/**
 * @brief Loads a translation resource from the specified file path.
 * 
 * @param p_path The file path to load.
 * @param p_original_path The original path of the resource (optional).
 * @param r_error Optional pointer to store error status.
 * @param p_use_sub_threads Whether to use sub-threads for loading.
 * @param r_progress Optional pointer to store loading progress (0.0 to 1.0).
 * @return RES The loaded Translation resource.
 */

/**
 * @brief Retrieves the file extensions recognized by this loader.
 * 
 * @param p_extensions Pointer to a list that will be populated with recognized extensions.
 */

/**
 * @brief Checks if this loader handles the specified resource type.
 * 
 * @param p_type The resource type to check.
 * @return bool True if this loader can handle the given type, false otherwise.
 */

/**
 * @brief Gets the resource type for the file at the given path.
 * 
 * @param p_path The file path to check.
 * @return String The resource type identifier.
 */

/**
 * @brief Constructs a TranslationLoaderPO instance.
 */
#ifndef TRANSLATION_LOADER_PO_H
#define TRANSLATION_LOADER_PO_H

#include "core/io/resource_loader.h"
#include "core/os/file_access.h"
#include "core/translation.h"

class TranslationLoaderPO : public ResourceFormatLoader {
public:
	static RES load_translation(FileAccess *f, Error *r_error = NULL);
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_use_sub_threads = false, float *r_progress = nullptr);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;

	TranslationLoaderPO();
};

#endif // TRANSLATION_LOADER_PO_H

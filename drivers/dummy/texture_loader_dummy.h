

/**
 * @class ResourceFormatDummyTexture
 * @brief A dummy texture loader resource format handler for testing and development purposes.
 * 
 * This class extends ResourceFormatLoader and provides the necessary interface
 * to load dummy texture resources. It handles recognition of texture file extensions,
 * type validation, and resource instantiation.
 * 
 * @note This is a dummy implementation intended for experimentation and testing.
 */

/**
 * @brief Loads a texture resource from the specified file path.
 * 
 * @param p_path The file path of the texture to load.
 * @param p_original_path The original resource path (optional).
 * @param r_error Pointer to store error status if loading fails (optional).
 * @param p_use_sub_threads Whether to use sub-threads for loading (default: false).
 * @param r_progress Pointer to store loading progress (optional).
 * 
 * @return RES The loaded texture resource, or null on failure.
 */

/**
 * @brief Retrieves the list of file extensions recognized by this loader.
 * 
 * @param p_extensions Pointer to a list where recognized extensions will be stored.
 */

/**
 * @brief Checks if this loader handles the specified resource type.
 * 
 * @param p_type The resource type to check.
 * @return bool True if this loader handles the specified type, false otherwise.
 */

/**
 * @brief Determines the resource type based on the file path.
 * 
 * @param p_path The file path to analyze.
 * @return String The resource type corresponding to the file.
 */
#ifndef TEXTURE_LOADER_DUMMY_H
#define TEXTURE_LOADER_DUMMY_H

#include "core/io/resource_loader.h"
#include "scene/resources/texture.h"

class ResourceFormatDummyTexture : public ResourceFormatLoader {
public:
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_use_sub_threads = false, float *r_progress = nullptr);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;

	virtual ~ResourceFormatDummyTexture() {}
};

#endif // TEXTURE_LOADER_DUMMY_H

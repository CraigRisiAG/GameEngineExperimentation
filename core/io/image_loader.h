

/**
 * @class ImageFormatLoader
 * @brief Abstract base class for image format loaders.
 * 
 * ImageFormatLoader is a virtual interface that defines the contract for loading
 * images of a specific format. Subclasses must implement the load_image method
 * to handle their respective image formats.
 */

/**
 * @fn virtual Error ImageFormatLoader::load_image(Ref<Image> p_image, FileAccess *p_fileaccess, bool p_force_linear, float p_scale)
 * @brief Loads an image from file access into the provided Image reference.
 * @param p_image Reference to the Image object to populate.
 * @param p_fileaccess Pointer to the FileAccess object for reading the file.
 * @param p_force_linear If true, forces the image to be loaded in linear color space.
 * @param p_scale Scale factor for the image (1.0 = no scaling).
 * @return Error code indicating success or failure.
 */

/**
 * @fn virtual void ImageFormatLoader::get_recognized_extensions(List<String> *p_extensions) const
 * @brief Retrieves the list of file extensions recognized by this loader.
 * @param p_extensions Pointer to a list that will be populated with recognized extensions.
 */

/**
 * @class ImageLoader
 * @brief Manager class for image loading across multiple format loaders.
 * 
 * ImageLoader maintains a registry of ImageFormatLoader instances and provides
 * a unified interface for loading images regardless of their format. It automatically
 * detects the appropriate loader based on file extension.
 */

/**
 * @fn static Error ImageLoader::load_image(String p_file, Ref<Image> p_image, FileAccess *p_custom, bool p_force_linear, float p_scale)
 * @brief Loads an image file into the provided Image reference.
 * @param p_file Path to the image file to load.
 * @param p_image Reference to the Image object to populate.
 * @param p_custom Optional custom FileAccess object; if NULL, a new one is created.
 * @param p_force_linear If true, forces the image to be loaded in linear color space.
 * @param p_scale Scale factor for the image (1.0 = no scaling).
 * @return Error code indicating success or failure.
 */

/**
 * @fn static void ImageLoader::get_recognized_extensions(List<String> *p_extensions)
 * @brief Retrieves all file extensions recognized by any registered loader.
 * @param p_extensions Pointer to a list that will be populated with all recognized extensions.
 */

/**
 * @fn static ImageFormatLoader *ImageLoader::recognize(const String &p_extension)
 * @brief Finds and returns the appropriate loader for the given file extension.
 * @param p_extension The file extension to match.
 * @return Pointer to the matching ImageFormatLoader, or NULL if no loader recognizes the extension.
 */

/**
 * @fn static void ImageLoader::add_image_format_loader(ImageFormatLoader *p_loader)
 * @brief Registers a new image format loader.
 * @param p_loader Pointer to the ImageFormatLoader to register.
 */

/**
 * @fn static void ImageLoader::remove_image_format_loader(ImageFormatLoader *p_loader)
 * @brief Unregisters an image format loader.
 * @param p_loader Pointer to the ImageFormatLoader to remove.
 */

/**
 * @fn static const Vector<ImageFormatLoader *> &ImageLoader::get_image_format_loaders()
 * @brief Returns a reference to the list of all registered image format loaders.
 * @return Const reference to the vector of registered loaders.
 */

/**
 * @fn static void ImageLoader::cleanup()
 * @brief Cleans up and releases resources used by the ImageLoader.
 */

/**
 * @class ResourceFormatLoaderImage
 * @brief Resource loader implementation for image files.
 * 
 * ResourceFormatLoaderImage integrates image loading into the engine's resource
 * loading system, allowing images to be loaded as engine resources.
 */

/**
 * @fn virtual RES ResourceFormatLoaderImage::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress)
 * @brief Loads an image resource from the specified path.
 * @param p_path The path to the image file.
 * @param p_original_path The original path (used for reimporting).
 * @param r_error Pointer to an Error variable to receive any error code.
 * @param p_use_sub_threads If true, uses sub-threads for loading.
 * @param r_progress Pointer to a float to track loading progress.
 * @return A reference to the loaded Image resource.
 */

/**
 * @fn virtual void ResourceFormatLoaderImage::get_recognized_extensions(List<String> *p_extensions) const
 * @brief Retrieves the list of file extensions recognized as image resources.
 * @param p_extensions Pointer to a list that will be populated with recognized extensions.
 */

/**
 * @fn virtual bool ResourceFormatLoaderImage::handles_type(const String &p_type) const
 * @brief Checks if this loader handles the specified resource type.
 * @param p_type The resource type to check.
 * @return True if this loader handles the specified type, false otherwise.
 */

/**
 * @fn virtual String ResourceFormatLoaderImage::get_resource_type(const String &p_path) const
 * @brief Retrieves the resource type for the file at the given path.
 * @param p_path The path to the file.
 * @return The resource type as a string.
 */
#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include "core/image.h"
#include "core/io/resource_loader.h"
#include "core/list.h"
#include "core/os/file_access.h"
#include "core/ustring.h"

class ImageLoader;

class ImageFormatLoader {
	friend class ImageLoader;
	friend class ResourceFormatLoaderImage;

protected:
	virtual Error load_image(Ref<Image> p_image, FileAccess *p_fileaccess, bool p_force_linear, float p_scale) = 0;
	virtual void get_recognized_extensions(List<String> *p_extensions) const = 0;
	bool recognize(const String &p_extension) const;

public:
	virtual ~ImageFormatLoader() {}
};

class ImageLoader {

	static Vector<ImageFormatLoader *> loader;
	friend class ResourceFormatLoaderImage;

protected:
public:
	static Error load_image(String p_file, Ref<Image> p_image, FileAccess *p_custom = NULL, bool p_force_linear = false, float p_scale = 1.0);
	static void get_recognized_extensions(List<String> *p_extensions);
	static ImageFormatLoader *recognize(const String &p_extension);

	static void add_image_format_loader(ImageFormatLoader *p_loader);
	static void remove_image_format_loader(ImageFormatLoader *p_loader);

	static const Vector<ImageFormatLoader *> &get_image_format_loaders();

	static void cleanup();
};

class ResourceFormatLoaderImage : public ResourceFormatLoader {
public:
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_use_sub_threads = false, float *r_progress = nullptr);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};

#endif // IMAGE_LOADER_H



/// \file image_loader.cpp
/// \brief Implementation of image loading functionality for the game engine.
///
/// This file provides the core image loading system that manages multiple image format loaders
/// and handles the loading of image files in various formats. It includes:
/// - ImageFormatLoader: Base class for format-specific image loaders
/// - ImageLoader: Manager for multiple image format loaders
/// - ResourceFormatLoaderImage: Resource loader for image files with custom format detection

/// \class ImageFormatLoader
/// \brief Abstract base class for image format loaders.
/// 
/// Provides interface for recognizing and loading specific image formats.

/// \fn bool ImageFormatLoader::recognize(const String &p_extension) const
/// \brief Determines if this loader can handle the given file extension.
/// \param p_extension The file extension to check
/// \return true if this loader recognizes the extension, false otherwise

/// \class ImageLoader
/// \brief Manager class for registering and using multiple image format loaders.
/// 
/// Maintains a collection of image format loaders and coordinates loading operations
/// across different supported image formats.

/// \fn Error ImageLoader::load_image(String p_file, Ref<Image> p_image, FileAccess *p_custom, bool p_force_linear, float p_scale)
/// \brief Loads an image file into an Image object using the appropriate format loader.
/// \param p_file Path to the image file
/// \param p_image Reference to the Image object to populate
/// \param p_custom Optional custom FileAccess object; if null, a new one is opened
/// \param p_force_linear Whether to force linear color space
/// \param p_scale Scale factor for the image
/// \return Error code indicating success or failure

/// \fn void ImageLoader::get_recognized_extensions(List<String> *p_extensions)
/// \brief Collects all recognized file extensions from all registered loaders.
/// \param p_extensions Pointer to list that will be populated with extensions

/// \fn ImageFormatLoader *ImageLoader::recognize(const String &p_extension)
/// \brief Finds a loader that recognizes the given file extension.
/// \param p_extension The file extension to match
/// \return Pointer to the appropriate ImageFormatLoader, or NULL if none found

/// \fn void ImageLoader::add_image_format_loader(ImageFormatLoader *p_loader)
/// \brief Registers a new image format loader.
/// \param p_loader Pointer to the loader to register

/// \fn void ImageLoader::remove_image_format_loader(ImageFormatLoader *p_loader)
/// \brief Unregisters an image format loader.
/// \param p_loader Pointer to the loader to remove

/// \fn const Vector<ImageFormatLoader *> &ImageLoader::get_image_format_loaders()
/// \brief Returns the collection of all registered loaders.
/// \return Const reference to the vector of loaders

/// \fn void ImageLoader::cleanup()
/// \brief Removes all registered image format loaders.

/// \class ResourceFormatLoaderImage
/// \brief Resource loader for loading image files with engine-specific format detection.
/// 
/// Handles loading of image resources that are wrapped in a custom container format
/// with a "GDIM" header followed by the image format identifier and data.

/// \fn RES ResourceFormatLoaderImage::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress)
/// \brief Loads an image resource from disk.
/// \param p_path Path to the image file
/// \param p_original_path Original path before any processing
/// \param r_error Pointer to store error code
/// \param p_use_sub_threads Whether sub-threads can be used for loading
/// \param r_progress Pointer to store loading progress
/// \return Resource handle to the loaded Image

/// \fn void ResourceFormatLoaderImage::get_recognized_extensions(List<String> *p_extensions) const
/// \brief Returns the file extensions this resource loader handles.
/// \param p_extensions Pointer to list to populate with recognized extensions

/// \fn bool ResourceFormatLoaderImage::handles_type(const String &p_type) const
/// \brief Checks if this loader handles the specified resource type.
/// \param p_type Resource type name
/// \return true if the loader handles this type

/// \fn String ResourceFormatLoaderImage::get_resource_type(const String &p_path) const
/// \brief Determines the resource type of a file at the given path.
/// \param p_path Path to the file
/// \return Resource type name or empty string if not recognized
#include "image_loader.h"

#include "core/print_string.h"

bool ImageFormatLoader::recognize(const String &p_extension) const {

	List<String> extensions;
	get_recognized_extensions(&extensions);
	for (List<String>::Element *E = extensions.front(); E; E = E->next()) {

		if (E->get().nocasecmp_to(p_extension) == 0)
			return true;
	}

	return false;
}

Error ImageLoader::load_image(String p_file, Ref<Image> p_image, FileAccess *p_custom, bool p_force_linear, float p_scale) {
	ERR_FAIL_COND_V_MSG(p_image.is_null(), ERR_INVALID_PARAMETER, "It's not a reference to a valid Image object.");

	FileAccess *f = p_custom;
	if (!f) {
		Error err;
		f = FileAccess::open(p_file, FileAccess::READ, &err);
		if (!f) {
			ERR_PRINT("Error opening file '" + p_file + "'.");
			return err;
		}
	}

	String extension = p_file.get_extension();

	for (int i = 0; i < loader.size(); i++) {

		if (!loader[i]->recognize(extension))
			continue;
		Error err = loader[i]->load_image(p_image, f, p_force_linear, p_scale);
		if (err != OK) {
			ERR_PRINT("Error loading image: " + p_file);
		}

		if (err != ERR_FILE_UNRECOGNIZED) {

			if (!p_custom)
				memdelete(f);

			return err;
		}
	}

	if (!p_custom)
		memdelete(f);

	return ERR_FILE_UNRECOGNIZED;
}

void ImageLoader::get_recognized_extensions(List<String> *p_extensions) {

	for (int i = 0; i < loader.size(); i++) {

		loader[i]->get_recognized_extensions(p_extensions);
	}
}

ImageFormatLoader *ImageLoader::recognize(const String &p_extension) {

	for (int i = 0; i < loader.size(); i++) {

		if (loader[i]->recognize(p_extension))
			return loader[i];
	}

	return NULL;
}

Vector<ImageFormatLoader *> ImageLoader::loader;

void ImageLoader::add_image_format_loader(ImageFormatLoader *p_loader) {

	loader.push_back(p_loader);
}

void ImageLoader::remove_image_format_loader(ImageFormatLoader *p_loader) {

	loader.erase(p_loader);
}

const Vector<ImageFormatLoader *> &ImageLoader::get_image_format_loaders() {

	return loader;
}

void ImageLoader::cleanup() {

	while (loader.size()) {
		remove_image_format_loader(loader[0]);
	}
}

/////////////////

RES ResourceFormatLoaderImage::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress) {

	FileAccess *f = FileAccess::open(p_path, FileAccess::READ);
	if (!f) {
		if (r_error) {
			*r_error = ERR_CANT_OPEN;
		}
		return RES();
	}

	uint8_t header[4] = { 0, 0, 0, 0 };
	f->get_buffer(header, 4);

	bool unrecognized = header[0] != 'G' || header[1] != 'D' || header[2] != 'I' || header[3] != 'M';
	if (unrecognized) {
		memdelete(f);
		if (r_error) {
			*r_error = ERR_FILE_UNRECOGNIZED;
		}
		ERR_FAIL_V(RES());
	}

	String extension = f->get_pascal_string();

	int idx = -1;

	for (int i = 0; i < ImageLoader::loader.size(); i++) {
		if (ImageLoader::loader[i]->recognize(extension)) {
			idx = i;
			break;
		}
	}

	if (idx == -1) {
		memdelete(f);
		if (r_error) {
			*r_error = ERR_FILE_UNRECOGNIZED;
		}
		ERR_FAIL_V(RES());
	}

	Ref<Image> image;
	image.instance();

	Error err = ImageLoader::loader[idx]->load_image(image, f, false, 1.0);

	memdelete(f);

	if (err != OK) {
		if (r_error) {
			*r_error = err;
		}
		return RES();
	}

	if (r_error) {
		*r_error = OK;
	}

	return image;
}

void ResourceFormatLoaderImage::get_recognized_extensions(List<String> *p_extensions) const {

	p_extensions->push_back("image");
}

bool ResourceFormatLoaderImage::handles_type(const String &p_type) const {

	return p_type == "Image";
}

String ResourceFormatLoaderImage::get_resource_type(const String &p_path) const {

	return p_path.get_extension().to_lower() == "image" ? "Image" : String();
}

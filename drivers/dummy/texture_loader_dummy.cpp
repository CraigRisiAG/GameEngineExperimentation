

/// @class ResourceFormatDummyTexture
/// @brief A dummy texture loader resource format handler for testing purposes.
///
/// This class implements a texture loader that creates a simple 8x8 black dummy texture
/// regardless of the input file path. It's designed for testing and development scenarios
/// where actual texture loading is not required.
///
/// @method RES load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress)
/// @brief Loads a texture resource and returns a dummy 8x8 black ImageTexture.
/// @param p_path The path to the texture file to load.
/// @param p_original_path The original path before any remapping.
/// @param r_error Pointer to store any error codes that occur during loading.
/// @param p_use_sub_threads Whether to use sub-threads for loading (unused in dummy implementation).
/// @param r_progress Pointer to store loading progress (unused in dummy implementation).
/// @return A Ref<ImageTexture> containing the dummy 8x8 black texture, or null on failure.
///
/// @method void get_recognized_extensions(List<String> *p_extensions) const
/// @brief Registers the file extensions that this loader can handle.
/// @param p_extensions Pointer to the list where recognized extensions are added (png, hdr, jpg, tga).
///
/// @method bool handles_type(const String &p_type) const
/// @brief Checks if this loader can handle the specified resource type.
/// @param p_type The resource type to check.
/// @return True if the type is Texture2D or a derived class, false otherwise.
///
/// @method String get_resource_type(const String &p_path) const
/// @brief Determines the resource type based on file extension.
/// @param p_path The file path to analyze.
/// @return "ImageTexture" if the extension matches supported formats, empty string otherwise.
#include "texture_loader_dummy.h"

#include "core/os/file_access.h"
#include "core/print_string.h"

#include <string.h>

RES ResourceFormatDummyTexture::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress) {
	unsigned int width = 8;
	unsigned int height = 8;

	//We just use some format
	Image::Format fmt = Image::FORMAT_RGB8;
	int rowsize = 3 * width;

	Vector<uint8_t> dstbuff;

	dstbuff.resize(rowsize * height);

	uint8_t **row_p = memnew_arr(uint8_t *, height);

	for (unsigned int i = 0; i < height; i++) {
		row_p[i] = 0; //No colors any more, I want them to turn black
	}

	memdelete_arr(row_p);

	Ref<Image> img = memnew(Image(width, height, 0, fmt, dstbuff));

	Ref<ImageTexture> texture = memnew(ImageTexture);
	texture->create_from_image(img);

	if (r_error)
		*r_error = OK;

	return texture;
}

void ResourceFormatDummyTexture::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("png");
	p_extensions->push_back("hdr");
	p_extensions->push_back("jpg");
	p_extensions->push_back("tga");
}

bool ResourceFormatDummyTexture::handles_type(const String &p_type) const {
	return ClassDB::is_parent_class(p_type, "Texture2D");
}

String ResourceFormatDummyTexture::get_resource_type(const String &p_path) const {
	String extension = p_path.get_extension().to_lower();
	if (extension == "png" || extension == "hdr" || extension == "jpg" || extension == "tga")
		return "ImageTexture";
	return "";
}

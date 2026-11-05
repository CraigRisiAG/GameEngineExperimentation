// This file contains the declaration of the PNGDriverCommon namespace,
// which provides common functionality for handling PNG images, including
// loading PNG images into the engine and converting engine images to PNG
// format.
#ifndef PNG_DRIVER_COMMON_H
#define PNG_DRIVER_COMMON_H

#include "core/image.h"

namespace PNGDriverCommon {

// Attempt to load png from buffer (p_source, p_size) into p_image
Error png_to_image(const uint8_t *p_source, size_t p_size, Ref<Image> p_image);

// Append p_image, as a png, to p_buffer.
// Contents of p_buffer is unspecified if error returned.
Error image_to_png(const Ref<Image> &p_image, Vector<uint8_t> &p_buffer);

} // namespace PNGDriverCommon

#endif

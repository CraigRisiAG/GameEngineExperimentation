// This file declares the functions for image compression and decompression
// using the CVTT library within the Godot engine. It includes the
// image_compress_cvtt function for compressing an image with a specified lossy
// quality and used channels, and the image_decompress_cvtt function for
// decompressing an image.
#ifndef IMAGE_COMPRESS_CVTT_H
#define IMAGE_COMPRESS_CVTT_H

#include "core/image.h"

void image_compress_cvtt(Image *p_image, float p_lossy_quality,
                         Image::UsedChannels p_channels);
void image_decompress_cvtt(Image *p_image);

#endif // IMAGE_COMPRESS_CVTT_H

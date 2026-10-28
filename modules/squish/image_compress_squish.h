
#ifndef IMAGE_COMPRESS_SQUISH_H
#define IMAGE_COMPRESS_SQUISH_H

#include "core/image.h"

void image_compress_squish(Image *p_image, float p_lossy_quality,
                           Image::UsedChannels p_channels);
void image_decompress_squish(Image *p_image);

#endif // IMAGE_COMPRESS_SQUISH_H


#ifndef IMAGE_COMPRESS_CVTT_H
#define IMAGE_COMPRESS_CVTT_H

#include "core/image.h"

void image_compress_cvtt(Image *p_image, float p_lossy_quality,
                         Image::UsedChannels p_channels);
void image_decompress_cvtt(Image *p_image);

#endif // IMAGE_COMPRESS_CVTT_H

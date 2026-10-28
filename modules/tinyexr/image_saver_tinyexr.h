
#ifndef IMAGE_SAVER_TINYEXR_H
#define IMAGE_SAVER_TINYEXR_H

#include "core/os/os.h"

Error save_exr(const String &p_path, const Ref<Image> &p_img, bool p_grayscale);

#endif // IMAGE_SAVER_TINYEXR_H

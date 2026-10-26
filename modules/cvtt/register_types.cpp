
#include "register_types.h"

#ifdef TOOLS_ENABLED

#include "image_compress_cvtt.h"

void register_cvtt_types() {

  Image::set_compress_bptc_func(image_compress_cvtt);
  Image::_image_decompress_bptc = image_decompress_cvtt;
}

void unregister_cvtt_types() {}

#endif

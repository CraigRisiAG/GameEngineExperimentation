
#include "register_types.h"
#include "image_compress_squish.h"

void register_squish_types() {

  Image::set_compress_bc_func(image_compress_squish);
  Image::_image_decompress_bc = image_decompress_squish;
}

void unregister_squish_types() {}

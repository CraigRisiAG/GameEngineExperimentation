
#include "register_types.h"

#include "image_loader_tinyexr.h"
#include "image_saver_tinyexr.h"

static ImageLoaderTinyEXR *image_loader_tinyexr = NULL;

void register_tinyexr_types() {

  image_loader_tinyexr = memnew(ImageLoaderTinyEXR);
  ImageLoader::add_image_format_loader(image_loader_tinyexr);

  Image::save_exr_func = save_exr;
}

void unregister_tinyexr_types() {

  memdelete(image_loader_tinyexr);

  Image::save_exr_func = NULL;
}

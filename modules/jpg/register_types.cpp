
#include "register_types.h"

#include "image_loader_jpegd.h"

static ImageLoaderJPG *image_loader_jpg = NULL;

void register_jpg_types() {

  image_loader_jpg = memnew(ImageLoaderJPG);
  ImageLoader::add_image_format_loader(image_loader_jpg);
}

void unregister_jpg_types() { memdelete(image_loader_jpg); }

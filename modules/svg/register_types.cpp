
#include "register_types.h"

#include "image_loader_svg.h"

static ImageLoaderSVG *image_loader_svg = NULL;

void register_svg_types() {

  image_loader_svg = memnew(ImageLoaderSVG);
  ImageLoader::add_image_format_loader(image_loader_svg);
}

void unregister_svg_types() { memdelete(image_loader_svg); }

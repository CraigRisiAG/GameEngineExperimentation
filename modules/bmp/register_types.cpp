// This file registers and unregisters the BMP module types within the Godot
// engine. It handles the integration of the ImageLoaderBMP class, providing the
// necessary hooks for loading BMP images in both the editor and runtime.
#include "register_types.h"

#include "image_loader_bmp.h"

static ImageLoaderBMP *image_loader_bmp = NULL;

void register_bmp_types() {
  image_loader_bmp = memnew(ImageLoaderBMP);
  ImageLoader::add_image_format_loader(image_loader_bmp);
}

void unregister_bmp_types() { memdelete(image_loader_bmp); }

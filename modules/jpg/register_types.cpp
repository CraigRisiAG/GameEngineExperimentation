// This file registers and unregisters the JPG module types within the Godot
// engine. It includes the necessary headers and defines functions for
// integrating the ImageLoaderJPG with the engine.
#include "register_types.h"

#include "image_loader_jpegd.h"

static ImageLoaderJPG *image_loader_jpg = NULL;

void register_jpg_types() {

  image_loader_jpg = memnew(ImageLoaderJPG);
  ImageLoader::add_image_format_loader(image_loader_jpg);
}

void unregister_jpg_types() { memdelete(image_loader_jpg); }

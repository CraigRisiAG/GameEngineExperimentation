// This file registers and unregisters the ETC module types within the Godot
// engine. It includes the functions register_etc_types and
// unregister_etc_types, which are used to integrate the ETC compression
// functionality and the PKM resource loader with the Godot editor.
#include "register_types.h"

#include "image_etc.h"
#include "texture_loader_pkm.h"

static Ref<ResourceFormatPKM> resource_loader_pkm;

void register_etc_types() {

  resource_loader_pkm.instance();
  ResourceLoader::add_resource_format_loader(resource_loader_pkm);

  _register_etc_compress_func();
}

void unregister_etc_types() {

  ResourceLoader::remove_resource_format_loader(resource_loader_pkm);
  resource_loader_pkm.unref();
}

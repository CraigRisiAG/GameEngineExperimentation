
#include "register_types.h"

#include "texture_loader_pvr.h"

static Ref<ResourceFormatPVR> resource_loader_pvr;

void register_pvr_types() {

  resource_loader_pvr.instance();
  ResourceLoader::add_resource_format_loader(resource_loader_pvr);
}

void unregister_pvr_types() {

  ResourceLoader::remove_resource_format_loader(resource_loader_pvr);
  resource_loader_pvr.unref();
}

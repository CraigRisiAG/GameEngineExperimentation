
#include "register_types.h"

#include "texture_loader_dds.h"

static Ref<ResourceFormatDDS> resource_loader_dds;

void register_dds_types() {

  resource_loader_dds.instance();
  ResourceLoader::add_resource_format_loader(resource_loader_dds);
}

void unregister_dds_types() {

  ResourceLoader::remove_resource_format_loader(resource_loader_dds);
  resource_loader_dds.unref();
}

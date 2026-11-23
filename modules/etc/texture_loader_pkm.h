// This file declares the ResourceFormatPKM class for loading PKM textures
// within the Godot engine. It provides the interface for the load function to
// read PKM files, extract texture data, and create ImageTexture resources.
// Additionally, it defines functions to get recognized file extensions, handle
// resource types, and determine the resource type based on the file path.
#ifndef TEXTURE_LOADER_PKM_H
#define TEXTURE_LOADER_PKM_H

#include "core/io/resource_loader.h"
#include "scene/resources/texture.h"

class ResourceFormatPKM : public ResourceFormatLoader {
public:
  virtual RES load(const String &p_path, const String &p_original_path = "",
                   Error *r_error = NULL, bool p_use_sub_threads = false,
                   float *r_progress = nullptr);
  virtual void get_recognized_extensions(List<String> *p_extensions) const;
  virtual bool handles_type(const String &p_type) const;
  virtual String get_resource_type(const String &p_path) const;

  virtual ~ResourceFormatPKM() {}
};

#endif // TEXTURE_LOADER_PKM_H

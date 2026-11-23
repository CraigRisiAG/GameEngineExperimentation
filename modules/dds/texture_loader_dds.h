// This file declares the ResourceFormatDDS class, which is a resource format
// loader for DDS textures within the Godot engine. It provides the necessary
// functions to load DDS texture files, recognize supported extensions, and
// determine the resource type for DDS textures.
#ifndef TEXTURE_LOADER_DDS_H
#define TEXTURE_LOADER_DDS_H

#include "core/io/resource_loader.h"
#include "scene/resources/texture.h"

class ResourceFormatDDS : public ResourceFormatLoader {
public:
  virtual RES load(const String &p_path, const String &p_original_path = "",
                   Error *r_error = NULL, bool p_use_sub_threads = false,
                   float *r_progress = nullptr);
  virtual void get_recognized_extensions(List<String> *p_extensions) const;
  virtual bool handles_type(const String &p_type) const;
  virtual String get_resource_type(const String &p_path) const;

  virtual ~ResourceFormatDDS() {}
};

#endif // TEXTURE_LOADER_DDS_H

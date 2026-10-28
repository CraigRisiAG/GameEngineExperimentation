
#ifndef TEXTURE_LOADER_PVR_H
#define TEXTURE_LOADER_PVR_H

#include "core/io/resource_loader.h"
#include "scene/resources/texture.h"

class ResourceFormatPVR : public ResourceFormatLoader {
public:
  virtual RES load(const String &p_path, const String &p_original_path,
                   Error *r_error = NULL, bool p_use_sub_threads = false,
                   float *r_progress = nullptr);
  virtual void get_recognized_extensions(List<String> *p_extensions) const;
  virtual bool handles_type(const String &p_type) const;
  virtual String get_resource_type(const String &p_path) const;

  ResourceFormatPVR();
  virtual ~ResourceFormatPVR() {}
};

#endif // TEXTURE_LOADER_PVR_H

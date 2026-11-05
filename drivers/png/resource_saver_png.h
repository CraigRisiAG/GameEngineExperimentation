// This file contains the declaration of the ResourceSaverPNG class,
// which handles saving ImageTexture resources as PNG files, including
// support for saving to disk and to memory buffers.
#ifndef RESOURCE_SAVER_PNG_H
#define RESOURCE_SAVER_PNG_H

#include "core/image.h"
#include "core/io/resource_saver.h"

class ResourceSaverPNG : public ResourceFormatSaver {
public:
  static Error save_image(const String &p_path, const Ref<Image> &p_img);
  static Vector<uint8_t> save_image_to_buffer(const Ref<Image> &p_img);

  virtual Error save(const String &p_path, const RES &p_resource,
                     uint32_t p_flags = 0);
  virtual bool recognize(const RES &p_resource) const;
  virtual void get_recognized_extensions(const RES &p_resource,
                                         List<String> *p_extensions) const;

  ResourceSaverPNG();
};

#endif // RESOURCE_SAVER_PNG_H

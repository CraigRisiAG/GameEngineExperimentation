
#ifndef VULKAN_DEVICE_WIN_H
#define VULKAN_DEVICE_WIN_H

#include "drivers/vulkan/vulkan_context.h"
#include <windows.h>

class VulkanContextWindows : public VulkanContext {

  virtual const char *_get_platform_surface_extension() const;

public:
  int window_create(HWND p_window, HINSTANCE p_instance, int p_width,
                    int p_height);

  VulkanContextWindows();
  ~VulkanContextWindows();
};

#endif // VULKAN_DEVICE_WIN_H


#include "vulkan_context_osx.h"
#include <vulkan/vulkan_macos.h>

const char *VulkanContextOSX::_get_platform_surface_extension() const {
	return VK_MVK_MACOS_SURFACE_EXTENSION_NAME;
}

int VulkanContextOSX::window_create(id p_window, int p_width, int p_height) {

	VkMacOSSurfaceCreateInfoMVK createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.pView = p_window;

	VkSurfaceKHR surface;
	VkResult err = vkCreateMacOSSurfaceMVK(_get_instance(), &createInfo, NULL, &surface);
	ERR_FAIL_COND_V(err, -1);
	return _window_create(surface, p_width, p_height);
}

VulkanContextOSX::VulkanContextOSX() {
}

VulkanContextOSX::~VulkanContextOSX() {
}

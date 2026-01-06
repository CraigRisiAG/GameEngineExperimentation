
#include "vulkan_context_iphone.h"
#include <vulkan/vulkan_ios.h>

const char *VulkanContextIPhone::_get_platform_surface_extension() const {
	return VK_MVK_IOS_SURFACE_EXTENSION_NAME;
}

int VulkanContextIPhone::window_create(void *p_window, int p_width, int p_height) {

	VkIOSSurfaceCreateInfoMVK createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
	createInfo.pNext = NULL;
	createInfo.flags = 0;
	createInfo.pView = p_window;

	VkSurfaceKHR surface;
	VkResult err = vkCreateIOSSurfaceMVK(_get_instance(), &createInfo, NULL, &surface);
	ERR_FAIL_COND_V(err, -1);
	return _window_create(surface, p_width, p_height);
}

VulkanContextIPhone::VulkanContextIPhone() {
}

VulkanContextIPhone::~VulkanContextIPhone() {
}


/**
 * @class VulkanContextIPhone
 * @brief iPhone-specific Vulkan context implementation
 * 
 * Extends the base VulkanContext class to provide platform-specific
 * Vulkan initialization and surface creation for iOS applications.
 * 
 * @see VulkanContext
 */

/**
 * @brief Gets the platform-specific Vulkan surface extension name
 * 
 * @return const char* The name of the Vulkan surface extension required for iOS
 * @remarks This is a virtual override of the base VulkanContext method
 * @private
 */

/**
 * @brief Creates a Vulkan window surface on iOS
 * 
 * Initializes a Vulkan surface for rendering on an iOS window/view.
 * 
 * @param p_window void* Pointer to the native iOS window/view object
 * @param p_width int The width of the window in pixels
 * @param p_height int The height of the window in pixels
 * 
 * @return int Status code indicating success (0) or failure (non-zero)
 * 
 * @remarks The window pointer should be a valid UIView or CAMetalLayer on iOS
 */

/**
 * @brief Constructor
 * 
 * Initializes a new VulkanContextIPhone instance with default values.
 */

/**
 * @brief Destructor
 * 
 * Cleans up Vulkan resources and properly releases the iOS surface.
 */
#ifndef VULKAN_CONTEXT_IPHONE_H
#define VULKAN_CONTEXT_IPHONE_H

#include "drivers/vulkan/vulkan_context.h"
// #import <UIKit/UIKit.h>

class VulkanContextIPhone : public VulkanContext {

	virtual const char *_get_platform_surface_extension() const;

public:
	int window_create(void *p_window, int p_width, int p_height);

	VulkanContextIPhone();
	~VulkanContextIPhone();
};

#endif // VULKAN_CONTEXT_IPHONE_H

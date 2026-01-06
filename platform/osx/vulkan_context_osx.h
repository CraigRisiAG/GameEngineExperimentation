

/// \class VulkanContextOSX
/// \brief macOS-specific implementation of Vulkan context.
///
/// This class extends the base VulkanContext to provide platform-specific
/// Vulkan surface and context creation for macOS applications using AppKit.
///
/// \details
/// Handles Vulkan surface creation for macOS windows and manages the
/// platform-specific extensions required for Vulkan rendering on macOS.
///
/// \see VulkanContext
class VulkanContextOSX {
public:
	/// \brief Creates a Vulkan window surface for an AppKit window.
	///
	/// \param p_window The AppKit window (NSWindow) to create a surface for.
	/// \param p_width The width of the window in pixels.
	/// \param p_height The height of the window in pixels.
	///
	/// \return 0 on success, non-zero error code on failure.
	int window_create(id p_window, int p_width, int p_height);

	/// \brief Constructs a VulkanContextOSX instance.
	VulkanContextOSX();

	/// \brief Destroys the VulkanContextOSX instance and cleans up resources.
	~VulkanContextOSX();

private:
	/// \brief Retrieves the required Vulkan surface extension for macOS.
	///
	/// \return A string containing the macOS Vulkan surface extension name
	///         (typically "VK_MVK_macos_surface").
	virtual const char *_get_platform_surface_extension() const;
};

#endif // VULKAN_DEVICE_OSX_H

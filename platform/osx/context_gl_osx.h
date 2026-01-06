
/**
 * @class ContextGL_OSX
 * @brief OpenGL context management for macOS/OSX platform
 * 
 * Handles the creation, configuration, and management of OpenGL rendering contexts
 * on macOS. This class wraps NSOpenGLContext and provides platform-specific
 * functionality for OpenGL initialization, buffer swapping, and context switching.
 * 
 * @note This class is only compiled when OPENGL_ENABLED or GLES_ENABLED is defined.
 */

/**
 * @brief Releases the current OpenGL context
 */
void release_current();

/**
 * @brief Makes this context the active rendering context
 */
void make_current();

/**
 * @brief Updates the OpenGL context state
 */
void update();

/**
 * @brief Sets the opacity level of the rendering context
 * 
 * @param p_opacity The opacity value to set
 */
void set_opacity(GLint p_opacity);

/**
 * @brief Retrieves the width of the associated window
 * 
 * @return The window width in pixels
 */
int get_window_width();

/**
 * @brief Retrieves the height of the associated window
 * 
 * @return The window height in pixels
 */
int get_window_height();

/**
 * @brief Swaps the front and back buffers for display
 */
void swap_buffers();

/**
 * @brief Initializes the OpenGL context
 * 
 * @return Error code indicating success or failure
 */
Error initialize();

/**
 * @brief Enables or disables vertical sync (vsync)
 * 
 * @param p_use True to enable vsync, false to disable
 */
void set_use_vsync(bool p_use);

/**
 * @brief Checks if vertical sync is currently enabled
 * 
 * @return True if vsync is enabled, false otherwise
 */
bool is_using_vsync() const;

/**
 * @brief Constructs an OpenGL context for the specified view
 * 
 * @param p_view The NSView to attach this context to
 * @param p_opengl_3_context True to create an OpenGL 3.x context, false otherwise
 */
ContextGL_OSX(id p_view, bool p_opengl_3_context);

/**
 * @brief Destructs the OpenGL context and releases associated resources
 */
~ContextGL_OSX();
#ifndef CONTEXT_GL_OSX_H
#define CONTEXT_GL_OSX_H

#if defined(OPENGL_ENABLED) || defined(GLES_ENABLED)

#include "core/error_list.h"
#include "core/os/os.h"

#include <AppKit/AppKit.h>
#include <ApplicationServices/ApplicationServices.h>
#include <CoreVideo/CoreVideo.h>

class ContextGL_OSX {

	bool opengl_3_context;
	bool use_vsync;

	void *framework;
	id window_view;
	NSOpenGLPixelFormat *pixelFormat;
	NSOpenGLContext *context;

public:
	void release_current();

	void make_current();
	void update();

	void set_opacity(GLint p_opacity);

	int get_window_width();
	int get_window_height();
	void swap_buffers();

	Error initialize();

	void set_use_vsync(bool p_use);
	bool is_using_vsync() const;

	ContextGL_OSX(id p_view, bool p_opengl_3_context);
	~ContextGL_OSX();
};

#endif
#endif

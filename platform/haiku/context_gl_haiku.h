
/**
 * @class ContextGL_Haiku
 * @brief OpenGL context management for Haiku operating system.
 * 
 * Manages OpenGL rendering context for Haiku platform, handling window
 * association, buffer swapping, and vsync configuration.
 * 
 * @note Only available when OPENGL_ENABLED is defined.
 */

/**
 * @brief Initialize the OpenGL context.
 * @return Error code indicating success or failure of initialization.
 */

/**
 * @brief Release the current OpenGL context.
 */

/**
 * @brief Make this context the active OpenGL context.
 */

/**
 * @brief Swap the front and back buffers.
 */

/**
 * @brief Get the width of the associated window.
 * @return Window width in pixels.
 */

/**
 * @brief Get the height of the associated window.
 * @return Window height in pixels.
 */

/**
 * @brief Enable or disable vertical sync.
 * @param p_use True to enable vsync, false to disable.
 */

/**
 * @brief Check if vertical sync is currently enabled.
 * @return True if vsync is enabled, false otherwise.
 */

/**
 * @brief Construct a new ContextGL_Haiku instance.
 * @param p_window Pointer to the HaikuDirectWindow for rendering.
 */

/**
 * @brief Destroy the ContextGL_Haiku instance and cleanup resources.
 */
#ifndef CONTEXT_GL_HAIKU_H
#define CONTEXT_GL_HAIKU_H

#if defined(OPENGL_ENABLED)

#include "haiku_direct_window.h"
#include "haiku_gl_view.h"

class ContextGL_Haiku {
private:
	HaikuGLView *view;
	HaikuDirectWindow *window;

	bool use_vsync;

public:
	Error initialize();
	void release_current();
	void make_current();
	void swap_buffers();
	int get_window_width();
	int get_window_height();

	void set_use_vsync(bool p_use);
	bool is_using_vsync() const;

	ContextGL_Haiku(HaikuDirectWindow *p_window);
	~ContextGL_Haiku();
};

#endif
#endif

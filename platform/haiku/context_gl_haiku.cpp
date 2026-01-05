
/**
 * @file context_gl_haiku.cpp
 * @brief OpenGL context implementation for the Haiku operating system.
 * 
 * This file provides the platform-specific OpenGL context management for Haiku,
 * including window binding, buffer swapping, and vsync control.
 */

/**
 * @brief Constructs an OpenGL context for a Haiku window.
 * 
 * @param p_window Pointer to the HaikuDirectWindow that will host the OpenGL context.
 * 
 * Creates a new HaikuGLView with RGB double-buffered rendering and depth buffer support.
 * VSync is disabled by default.
 */

/**
 * @brief Destroys the OpenGL context and associated view.
 * 
 * Cleans up the HaikuGLView allocated during construction.
 */

/**
 * @brief Initializes the OpenGL context and attaches it to the window.
 * 
 * @return Error code (OK on success).
 * 
 * Adds the OpenGL view as a child to the window and registers it as the active
 * OpenGL view for the window.
 */

/**
 * @brief Releases the current OpenGL context from the calling thread.
 * 
 * Unlocks the OpenGL context, allowing other threads to use it.
 */

/**
 * @brief Makes this OpenGL context current for the calling thread.
 * 
 * Locks the OpenGL context for exclusive use by the current thread.
 */

/**
 * @brief Swaps the front and back buffers to display rendered content.
 * 
 * Respects the current vsync setting when performing the buffer swap.
 */

/**
 * @brief Retrieves the current window width in pixels.
 * 
 * @return The width of the window's bounds.
 */

/**
 * @brief Retrieves the current window height in pixels.
 * 
 * @return The height of the window's bounds.
 */

/**
 * @brief Sets whether vertical sync (vsync) should be enabled.
 * 
 * @param p_use True to enable vsync, false to disable it.
 */

/**
 * @brief Checks if vertical sync (vsync) is currently enabled.
 * 
 * @return True if vsync is enabled, false otherwise.
 */
#include "context_gl_haiku.h"

#if defined(OPENGL_ENABLED)

ContextGL_Haiku::ContextGL_Haiku(HaikuDirectWindow *p_window) {
	window = p_window;

	uint32 type = BGL_RGB | BGL_DOUBLE | BGL_DEPTH;
	view = new HaikuGLView(window->Bounds(), type);

	use_vsync = false;
}

ContextGL_Haiku::~ContextGL_Haiku() {
	delete view;
}

Error ContextGL_Haiku::initialize() {
	window->AddChild(view);
	window->SetHaikuGLView(view);

	return OK;
}

void ContextGL_Haiku::release_current() {
	view->UnlockGL();
}

void ContextGL_Haiku::make_current() {
	view->LockGL();
}

void ContextGL_Haiku::swap_buffers() {
	view->SwapBuffers(use_vsync);
}

int ContextGL_Haiku::get_window_width() {
	return window->Bounds().IntegerWidth();
}

int ContextGL_Haiku::get_window_height() {
	return window->Bounds().IntegerHeight();
}

void ContextGL_Haiku::set_use_vsync(bool p_use) {
	use_vsync = p_use;
}

bool ContextGL_Haiku::is_using_vsync() const {
	return use_vsync;
}

#endif

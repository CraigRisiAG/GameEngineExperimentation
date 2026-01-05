
/**
 * @class HaikuGLView
 * @brief OpenGL view implementation for Haiku operating system.
 * 
 * A custom OpenGL view that extends BGLView to provide rendering capabilities
 * for the Godot game engine on the Haiku platform.
 */

/**
 * @brief Constructs a HaikuGLView with specified frame and OpenGL type.
 * 
 * @param frame The rectangular frame defining the view's position and size.
 * @param type The OpenGL context type flags (e.g., BGL_RGB, BGL_DOUBLE).
 */

/**
 * @brief Called when the view is attached to a window.
 * 
 * Locks the OpenGL context, calls the parent class attachment handler,
 * unlocks the context, and sets focus to this view.
 */

/**
 * @brief Renders the view in response to a draw request.
 * 
 * Forces a complete redraw of the main application when this view region
 * needs to be updated.
 * 
 * @param updateRect The rectangular region that needs to be redrawn.
 */
#include "haiku_gl_view.h"
#include "main/main.h"

HaikuGLView::HaikuGLView(BRect frame, uint32 type) :
		BGLView(frame, "GodotGLView", B_FOLLOW_ALL_SIDES, 0, type) {
}

void HaikuGLView::AttachedToWindow(void) {
	LockGL();
	BGLView::AttachedToWindow();
	UnlockGL();
	MakeFocus();
}

void HaikuGLView::Draw(BRect updateRect) {
	Main::force_redraw();
}


/**
 * @brief OpenGL view implementation for the Haiku operating system
 * 
 * HaikuGLView is a specialized GLView class that provides OpenGL rendering
 * capabilities within the Haiku BeOS-compatible operating system. It manages
 * the creation and rendering of OpenGL contexts in Haiku windows.
 * 
 * @class HaikuGLView
 * @extends BGLView
 */

/**
 * @brief Constructs a HaikuGLView with the specified frame and type
 * 
 * Initializes a new OpenGL view that will be rendered within the given frame
 * rectangle with the specified OpenGL context type.
 * 
 * @param frame The rectangular area where this view will be displayed
 * @param type The OpenGL context type/mode for this view
 */

/**
 * @brief Called when the view is attached to a window
 * 
 * Performs initialization tasks when this view is attached to a parent window.
 * This is typically used to set up OpenGL context and any window-related state.
 */

/**
 * @brief Renders the view for the specified update rectangle
 * 
 * Called by the system to redraw the view. This method should contain all
 * OpenGL drawing commands needed to render the contents of the specified
 * update region.
 * 
 * @param updateRect The rectangular region that needs to be redrawn
 */
#ifndef HAIKU_GL_VIEW_H
#define HAIKU_GL_VIEW_H

#include <kernel/image.h> // needed for image_id

#include <GLView.h>

class HaikuGLView : public BGLView {
public:
	HaikuGLView(BRect frame, uint32 type);
	virtual void AttachedToWindow(void);
	virtual void Draw(BRect updateRect);
};

#endif

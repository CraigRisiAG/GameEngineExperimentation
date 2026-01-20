


/**
 * @class RasterizerGLES2
 * @brief OpenGL ES 2.0 implementation of the Rasterizer interface.
 * 
 * This class provides rendering capabilities using OpenGL ES 2.0 as the graphics backend.
 * It manages three main rendering subsystems: storage, canvas, and scene rendering.
 * 
 * @details
 * The RasterizerGLES2 class is responsible for:
 * - Managing the rendering pipeline for OpenGL ES 2.0 graphics
 * - Coordinating storage operations for graphics resources
 * - Handling canvas and scene rendering operations
 * - Managing render targets and frame rendering
 * - Processing boot images and lens distortion effects
 * 
 * @note This is a low-end renderer implementation as indicated by is_low_end() returning true.
 * 
 * @see RasterizerStorage, RasterizerCanvas, RasterizerScene
 */

/**
 * @brief Gets the storage subsystem for graphics resource management.
 * @return Pointer to RasterizerStorage implementation
 */

/**
 * @brief Gets the canvas rendering subsystem.
 * @return Pointer to RasterizerCanvas implementation
 */

/**
 * @brief Gets the scene rendering subsystem.
 * @return Pointer to RasterizerScene implementation
 */

/**
 * @brief Sets the boot image displayed during engine initialization.
 * @param p_image The image to display
 * @param p_color Background color
 * @param p_scale Whether to scale the image to fit the screen
 * @param p_use_filter Whether to apply filtering to the image (default: true)
 */

/**
 * @brief Initializes the rasterizer and graphics subsystems.
 */

/**
 * @brief Begins a new frame with the specified time step.
 * @param frame_step The time elapsed since the last frame in seconds
 */

/**
 * @brief Sets the current render target for subsequent drawing operations.
 * @param p_render_target RID of the render target
 */

/**
 * @brief Restores the previous render target after 3D rendering.
 * @param p_3d_was_drawn Whether 3D content was drawn to the render target
 */

/**
 * @brief Clears the current render target with the specified color.
 * @param p_color The color to clear with
 */

/**
 * @brief Blits a render target to the screen.
 * @param p_render_target RID of the source render target
 * @param p_screen_rect Rectangle defining the destination on screen
 * @param p_screen Screen index (default: 0)
 */

/**
 * @brief Outputs a render target to screen with lens distortion applied.
 * @param p_render_target RID of the source render target
 * @param p_screen_rect Rectangle defining the destination on screen
 * @param p_k1 First distortion coefficient
 * @param p_k2 Second distortion coefficient
 * @param p_eye_center Center point of the eye for distortion calculation
 * @param p_oversample Oversampling factor for quality
 */

/**
 * @brief Ends the current frame and optionally swaps buffers.
 * @param p_swap_buffers Whether to swap front and back buffers
 */

/**
 * @brief Finalizes and cleans up the rasterizer resources.
 */

/**
 * @brief Checks if the OpenGL ES 2.0 driver is viable on this system.
 * @return Error code indicating viability status
 */

/**
 * @brief Sets this rasterizer as the current rendering backend.
 */

/**
 * @brief Registers configuration options for the rasterizer.
 */

/**
 * @brief Determines if this is a low-end renderer.
 * @return true, as GLES2 is optimized for low-end devices
 */

/**
 * @brief Constructs a new RasterizerGLES2 instance.
 */

/**
 * @brief Destructs the RasterizerGLES2 instance and releases resources.
 */
#ifndef RASTERIZERGLES2_H
#define RASTERIZERGLES2_H

#include "rasterizer_canvas_gles2.h"
#include "rasterizer_scene_gles2.h"
#include "rasterizer_storage_gles2.h"
#include "servers/visual/rasterizer.h"

class RasterizerGLES2 : public Rasterizer {

	static Rasterizer *_create_current();

	RasterizerStorageGLES2 *storage;
	RasterizerCanvasGLES2 *canvas;
	RasterizerSceneGLES2 *scene;

	double time_total;

public:
	virtual RasterizerStorage *get_storage();
	virtual RasterizerCanvas *get_canvas();
	virtual RasterizerScene *get_scene();

	virtual void set_boot_image(const Ref<Image> &p_image, const Color &p_color, bool p_scale, bool p_use_filter = true);

	virtual void initialize();
	virtual void begin_frame(double frame_step);
	virtual void set_current_render_target(RID p_render_target);
	virtual void restore_render_target(bool p_3d_was_drawn);
	virtual void clear_render_target(const Color &p_color);
	virtual void blit_render_target_to_screen(RID p_render_target, const Rect2 &p_screen_rect, int p_screen = 0);
	virtual void output_lens_distorted_to_screen(RID p_render_target, const Rect2 &p_screen_rect, float p_k1, float p_k2, const Vector2 &p_eye_center, float p_oversample);
	virtual void end_frame(bool p_swap_buffers);
	virtual void finalize();

	static Error is_viable();
	static void make_current();
	static void register_config();

	virtual bool is_low_end() const { return true; }

	RasterizerGLES2();
	~RasterizerGLES2();
};

#endif // RASTERIZERGLES2_H

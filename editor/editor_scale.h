

/**
 * @file editor_scale.h
 * @brief Provides editor scaling functionality for UI and viewport rendering.
 * 
 * This header defines functions and macros to manage the editor's scale factor,
 * allowing dynamic scaling of editor UI elements and rendering based on user preferences
 * or system DPI settings.
 */

/**
 * @brief Sets the editor's scale factor.
 * 
 * @param p_scale The scale factor to apply. Typical values are >= 1.0 for zooming in,
 *                 and < 1.0 for zooming out.
 */

/**
 * @brief Retrieves the current editor scale factor.
 * 
 * @return The current scale factor as a floating-point value.
 */

/**
 * @def EDSCALE
 * @brief Convenience macro for accessing the current editor scale factor.
 * 
 * Equivalent to calling editor_get_scale() directly. Useful for concise
 * scaling operations throughout the editor codebase.
 */
#ifndef EDITOR_SCALE_H
#define EDITOR_SCALE_H

void editor_set_scale(float p_scale);
float editor_get_scale();

#define EDSCALE (editor_get_scale())
#endif // EDITOR_SCALE_H

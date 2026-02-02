

/**
 * @file editor_scale.cpp
 * @brief Manages the editor UI scaling factor.
 * 
 * This module provides functionality to set and retrieve the global editor scale,
 * which is used to adjust the UI size for different display resolutions and user preferences.
 */

/**
 * @brief Sets the editor scale factor.
 * 
 * Updates the global editor scale value used for UI rendering and layout calculations.
 * 
 * @param p_scale The scale factor to apply. Typically a value >= 1.0 for upscaling.
 */

/**
 * @brief Retrieves the current editor scale factor.
 * 
 * Returns the global editor scale value that is applied to UI elements.
 * 
 * @return float The current editor scale factor.
 */
#include "editor_scale.h"

#include "core/os/os.h"

static float scale = 1.0;

void editor_set_scale(float p_scale) {

	scale = p_scale;
}
float editor_get_scale() {

	return scale;
}

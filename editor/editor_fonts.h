
/// @file editor_fonts.h
/// @brief Font management and registration for the editor.
///
/// This header provides functionality for registering and managing fonts
/// used throughout the editor interface via a centralized theme resource.

/// @brief Registers all editor fonts to the given theme.
/// @param p_theme A reference to the theme resource where fonts will be
/// registered.
///                The theme will be modified to include all necessary editor
///                fonts.
#ifndef EDITOR_FONTS_H
#define EDITOR_FONTS_H

#include "scene/resources/theme.h"

void editor_register_fonts(Ref<Theme> p_theme);

#endif

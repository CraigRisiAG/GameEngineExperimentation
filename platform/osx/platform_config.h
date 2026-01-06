

/**
 * @file platform_config.h
 * @brief macOS platform-specific configuration and definitions
 * 
 * Configures platform-specific headers and settings for macOS builds.
 * Sets up OpenGL ES 2.0 header inclusion path and pthread naming capabilities.
 * 
 * @details
 * - Includes alloca.h for dynamic stack memory allocation
 * - Defines the path to the GLAD OpenGL loader library
 * - Enables pthread self-renaming functionality on macOS
 */
#include <alloca.h>

#define GLES2_INCLUDE_H "thirdparty/glad/glad/glad.h"
#define PTHREAD_RENAME_SELF


/**
 * @file platform_config.h
 * @brief Platform-specific configuration for Haiku OS
 * 
 * Defines platform-specific includes and preprocessor directives required for
 * building on the Haiku operating system.
 * 
 * @details
 * - Includes alloca.h for dynamic memory allocation on the stack
 * - Enables BSD source definitions required by ifaddrs.h, which is used in
 *   drivers/unix/ip_unix.cpp for network interface enumeration
 * - Specifies the OpenGL ES 2.0 header path pointing to the bundled GLAD
 *   loader library
 */
#include <alloca.h>

// for ifaddrs.h needed in drivers/unix/ip_unix.cpp
#define _BSD_SOURCE 1

#define GLES2_INCLUDE_H "thirdparty/glad/glad/glad.h"

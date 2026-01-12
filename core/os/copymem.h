

/**
 * @file copymem.h
 * @brief Memory operation abstraction layer for cross-platform compatibility.
 * 
 * This header provides a platform-agnostic interface for common memory operations
 * including copying, zeroing, and moving memory blocks. It allows for platform-specific
 * optimizations through conditional compilation.
 * 
 * When PLATFORM_COPYMEM is defined, platform-specific implementations are used via
 * platform_copymem.h. Otherwise, standard C library functions (memcpy, memset, memmove)
 * are used as fallbacks.
 * 
 * @macros
 *   - copymem(to, from, count): Copy 'count' bytes from 'from' to 'to'
 *   - zeromem(to, count): Zero out 'count' bytes starting at 'to'
 *   - movemem(to, from, count): Move 'count' bytes from 'from' to 'to' (handles overlapping regions)
 * 
 * @note Requires core/typedefs.h to be included for type definitions.
 * @note Platform-specific implementations should define PLATFORM_COPYMEM to enable custom memory operations.
 */
#ifndef COPYMEM_H
#define COPYMEM_H

#include "core/typedefs.h"

#ifdef PLATFORM_COPYMEM

#include "platform_copymem.h" // included from platform/<current_platform>/platform_copymem.h"

#else

#include <string.h>

#define copymem(to, from, count) memcpy(to, from, count)
#define zeromem(to, count) memset(to, 0, count)
#define movemem(to, from, count) memmove(to, from, count)

#endif

#endif // COPYMEM_H

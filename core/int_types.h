


/**
 * @file int_types.h
 * @brief Provides cross-platform fixed-width integer type definitions.
 * 
 * This header file ensures consistent integer type definitions across different
 * compilers and platforms by conditionally defining standard fixed-width integer
 * types (int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t).
 * 
 * The implementation follows three priority levels:
 * 1. For MSVC compiler (_MSC_VER), uses Microsoft-specific __int types
 * 2. If NO_STDINT_H is defined, uses fundamental C types (char, short, int, long long)
 * 3. Otherwise, includes the standard <stdint.h> header for ISO C99 compliant types
 * 
 * @note This header is typically used when portability across compilers and
 *       C standard versions is required, or when the standard <stdint.h> is unavailable.
 */
#ifndef INT_TYPES_H
#define INT_TYPES_H

#ifdef _MSC_VER

typedef signed __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef signed __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef signed __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else

#ifdef NO_STDINT_H
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
#else
#include <stdint.h>
#endif

#endif // _MSC_VER

#endif // INT_TYPES_H

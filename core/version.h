

/**
 * @file version.h
 * @brief Version information and macros for the Godot game engine.
 * 
 * This header file defines a comprehensive set of macros for managing and formatting
 * version information throughout the engine. Version numbers follow the semantic versioning
 * pattern of <major>.<minor> for initial releases and <major>.<minor>.<patch> for
 * subsequent bugfix releases where patch != 0.
 * 
 * @note Requires version_generated.gen.h to define VERSION_MAJOR, VERSION_MINOR,
 *       VERSION_PATCH, VERSION_STATUS, VERSION_MODULE_CONFIG, VERSION_BUILD, and VERSION_NAME.
 * 
 * Macro Definitions:
 * - VERSION_BRANCH: Main branch version string (e.g., "3.1")
 * - VERSION_NUMBER: Complete version string excluding patch if zero (e.g., "3.1" or "3.1.4")
 * - VERSION_HEX: Hexadecimal encoded version for easy compile-time comparison
 * - VERSION_FULL_CONFIG: Complete version with status and module-specific features
 * - VERSION_FULL_BUILD: Extended version including build description
 * - VERSION_FULL_NAME: Human-readable version string with engine name and "v" prefix
 */
#ifndef VERSION_H
#define VERSION_H

#include "core/version_generated.gen.h"

// Godot versions are of the form <major>.<minor> for the initial release,
// and then <major>.<minor>.<patch> for subsequent bugfix releases where <patch> != 0
// That's arbitrary, but we find it pretty and it's the current policy.

// Defines the main "branch" version. Patch versions in this branch should be
// forward-compatible.
// Example: "3.1"
#define VERSION_BRANCH "" _MKSTR(VERSION_MAJOR) "." _MKSTR(VERSION_MINOR)
#if VERSION_PATCH
// Example: "3.1.4"
#define VERSION_NUMBER "" VERSION_BRANCH "." _MKSTR(VERSION_PATCH)
#else // patch is 0, we don't include it in the "pretty" version number.
// Example: "3.1" instead of "3.1.0"
#define VERSION_NUMBER "" VERSION_BRANCH
#endif // VERSION_PATCH

// Version number encoded as hexadecimal int with one byte for each number,
// for easy comparison from code.
// Example: 3.1.4 will be 0x030104, making comparison easy from script.
#define VERSION_HEX 0x10000 * VERSION_MAJOR + 0x100 * VERSION_MINOR + VERSION_PATCH

// Describes the full configuration of that Godot version, including the version number,
// the status (beta, stable, etc.) and potential module-specific features (e.g. mono).
// Example: "3.1.4.stable.mono"
#define VERSION_FULL_CONFIG "" VERSION_NUMBER "." VERSION_STATUS VERSION_MODULE_CONFIG

// Similar to VERSION_FULL_CONFIG, but also includes the (potentially custom) VERSION_BUILD
// description (e.g. official, custom_build, etc.).
// Example: "3.1.4.stable.mono.official"
#define VERSION_FULL_BUILD "" VERSION_FULL_CONFIG "." VERSION_BUILD

// Same as above, but prepended with Godot's name and a cosmetic "v" for "version".
// Example: "Godot v3.1.4.stable.official.mono"
#define VERSION_FULL_NAME "" VERSION_NAME " v" VERSION_FULL_BUILD

#endif // VERSION_H

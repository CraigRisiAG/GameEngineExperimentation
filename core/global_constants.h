


/**
 * @class GlobalConstants
 * @brief Provides access to a collection of global constants and their metadata.
 * 
 * This class offers a static interface to query global constants, including their
 * enumeration groups, names, and values. It serves as a central registry for
 * application-wide constant definitions.
 */

/**
 * @brief Retrieves the total number of registered global constants.
 * @return The count of global constants available in the registry.
 */

/**
 * @brief Gets the enumeration group name associated with a global constant.
 * @param p_idx The index of the global constant.
 * @return A StringName representing the enum group this constant belongs to.
 */

/**
 * @brief Retrieves the name of a global constant by its index.
 * @param p_idx The index of the global constant.
 * @return A pointer to a C-string containing the constant's name.
 */

/**
 * @brief Retrieves the value of a global constant by its index.
 * @param p_idx The index of the global constant.
 * @return The integer value of the global constant.
 */
#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

#include "core/string_name.h"

class GlobalConstants {
public:
	static int get_global_constant_count();
	static StringName get_global_constant_enum(int p_idx);
	static const char *get_global_constant_name(int p_idx);
	static int get_global_constant_value(int p_idx);
};

#endif // GLOBAL_CONSTANTS_H

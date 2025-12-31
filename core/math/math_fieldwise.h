
/**
 * @file math_fieldwise.h
 * @brief Declares functions for assigning fields between Variant objects within
 * the math module.
 *
 * This header provides the interface for field-wise assignment functionality
 * used when tools are enabled.
 */

/// Assigns a specific field from a source Variant to a target Variant.
/**
 * Copies the value of the specified field from the given source variant into
 * the corresponding field of the target variant, typically used in tool-enabled
 * builds for manipulating variants by field name.
 *
 * @param p_target The Variant object whose field will be modified.
 * @param p_source The Variant object providing the field value.
 * @param p_field The name of the field to assign.
 * @return A new Variant reflecting the result of the assignment.
 */
#ifndef MATH_FIELDWISE_H
#define MATH_FIELDWISE_H

#ifdef TOOLS_ENABLED

#include "core/variant.h"

Variant fieldwise_assign(const Variant &p_target, const Variant &p_source,
                         const String &p_field);

#endif // TOOLS_ENABLED

#endif // MATH_FIELDWISE_H

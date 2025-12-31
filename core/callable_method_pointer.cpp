

/**
 * @class CallableCustomMethodPointerBase
 * @brief Base class for callable method pointers with comparison and hashing
 * capabilities.
 *
 * This class provides functionality to compare and hash method pointers by
 * comparing their underlying memory representation. It uses DJB2 hashing
 * algorithm for efficient hash computation.
 */

/**
 * @brief Compares two CallableCustomMethodPointer instances for equality.
 *
 * @param p_a First callable custom object to compare
 * @param p_b Second callable custom object to compare
 * @return true if both objects have equal size and identical component data,
 * false otherwise
 *
 * Performs a byte-by-byte comparison of the component pointers after size
 * validation.
 */

/**
 * @brief Compares two CallableCustomMethodPointer instances using less-than
 * operator.
 *
 * @param p_a First callable custom object to compare
 * @param p_b Second callable custom object to compare
 * @return true if p_a is less than p_b, false otherwise
 *
 * First compares by size, then performs lexicographic comparison of component
 * data.
 */

/**
 * @brief Returns the function pointer for equality comparison.
 *
 * @return CallableCustom::CompareEqualFunc Function pointer to compare_equal
 */

/**
 * @brief Returns the function pointer for less-than comparison.
 *
 * @return CallableCustom::CompareLessFunc Function pointer to compare_less
 */

/**
 * @brief Returns the precomputed hash value.
 *
 * @return uint32_t The DJB2 hash of the callable method pointer
 */

/**
 * @brief Initializes the callable method pointer with base pointer and size
 * information.
 *
 * @param p_base_ptr Pointer to the base memory location containing pointer data
 * @param p_ptr_size Total size in bytes of the pointer data
 *
 * Sets up the component pointer and size, then precomputes and caches the DJB2
 * hash for all components.
 */
#include "callable_method_pointer.h"

bool CallableCustomMethodPointerBase::compare_equal(const CallableCustom *p_a,
                                                    const CallableCustom *p_b) {
  const CallableCustomMethodPointerBase *a =
      static_cast<const CallableCustomMethodPointerBase *>(p_a);
  const CallableCustomMethodPointerBase *b =
      static_cast<const CallableCustomMethodPointerBase *>(p_b);

  if (a->comp_size != b->comp_size) {
    return false;
  }

  for (uint32_t i = 0; i < a->comp_size; i++) {
    if (a->comp_ptr[i] != b->comp_ptr[i]) {
      return false;
    }
  }

  return true;
}

bool CallableCustomMethodPointerBase::compare_less(const CallableCustom *p_a,
                                                   const CallableCustom *p_b) {

  const CallableCustomMethodPointerBase *a =
      static_cast<const CallableCustomMethodPointerBase *>(p_a);
  const CallableCustomMethodPointerBase *b =
      static_cast<const CallableCustomMethodPointerBase *>(p_b);

  if (a->comp_size != b->comp_size) {
    return a->comp_size < b->comp_size;
  }

  for (uint32_t i = 0; i < a->comp_size; i++) {
    if (a->comp_ptr[i] == b->comp_ptr[i]) {
      continue;
    }

    return a->comp_ptr[i] < b->comp_ptr[i];
  }

  return false;
}

CallableCustom::CompareEqualFunc
CallableCustomMethodPointerBase::get_compare_equal_func() const {
  return compare_equal;
}

CallableCustom::CompareLessFunc
CallableCustomMethodPointerBase::get_compare_less_func() const {
  return compare_less;
}

uint32_t CallableCustomMethodPointerBase::hash() const { return h; }

void CallableCustomMethodPointerBase::_setup(uint32_t *p_base_ptr,
                                             uint32_t p_ptr_size) {
  comp_ptr = p_base_ptr;
  comp_size = p_ptr_size / 4;

  // Precompute hash.
  for (uint32_t i = 0; i < comp_size; i++) {
    if (i == 0) {
      h = hash_djb2_one_32(comp_ptr[i]);
    } else {
      h = hash_djb2_one_32(comp_ptr[i], h);
    }
  }
}

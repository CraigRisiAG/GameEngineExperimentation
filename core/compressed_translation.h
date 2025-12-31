

/**
 * @class PHashTranslation
 * @brief A compressed translation class using a modified perfect hash
 * algorithm.
 *
 * This translation implementation uses a modified perfect hash algorithm to
 * store and retrieve translated strings efficiently. It employs double hashing
 * followed by binary search, which provides a slower lookup but with an
 * extremely high chance of catching untranslated strings.
 *
 * @details
 * The class uses load/store friendly data structures:
 * - hash_table: Primary hash table for quick lookups
 * - bucket_table: Secondary bucket table for collision resolution
 * - strings: Compressed string storage
 *
 * @note The implementation trades lookup speed for accuracy in detecting
 * missing translations.
 */

/**
 * @struct Bucket
 * @brief Represents a hash bucket for storing translated strings.
 *
 * @member size - Number of elements in this bucket
 * @member func - Hash function identifier for this bucket
 * @member elem - Array of bucket elements (variable size)
 */

/**
 * @struct Bucket::Elem
 * @brief Individual element within a hash bucket.
 *
 * @member key - Hash key for the source string
 * @member str_offset - Offset into the strings buffer
 * @member comp_size - Compressed size of the translated string
 * @member uncomp_size - Uncompressed size of the translated string
 */

/**
 * @fn hash
 * @brief Computes an FNV-1a hash of the given string with an optional seed.
 *
 * @param d - Initial hash seed (defaults to 0x1000193 if 0)
 * @param p_str - Null-terminated string to hash
 * @return uint32_t - Computed hash value
 */

/**
 * @fn get_message
 * @brief Retrieves the translated message for a given source text.
 *
 * @param p_src_text - The source text to look up
 * @return StringName - The translated text, or original if not found
 */

/**
 * @fn generate
 * @brief Generates the compressed translation data from another Translation
 * object.
 *
 * @param p_from - Reference to the source Translation to compress
 */
#ifndef COMPRESSED_TRANSLATION_H
#define COMPRESSED_TRANSLATION_H

#include "core/translation.h"

class PHashTranslation : public Translation {

  GDCLASS(PHashTranslation, Translation);

  // this translation uses a sort of modified perfect hash algorithm
  // it requires hashing strings twice and then does a binary search,
  // so it's slower, but at the same time it has an extreemly high chance
  // of catching untranslated strings

  // load/store friendly types
  Vector<int> hash_table;
  Vector<int> bucket_table;
  Vector<uint8_t> strings;

  struct Bucket {

    int size;
    uint32_t func;

    struct Elem {

      uint32_t key;
      uint32_t str_offset;
      uint32_t comp_size;
      uint32_t uncomp_size;
    };

    Elem elem[1];
  };

  _FORCE_INLINE_ uint32_t hash(uint32_t d, const char *p_str) const {

    if (d == 0)
      d = 0x1000193;
    while (*p_str) {

      d = (d * 0x1000193) ^ uint32_t(*p_str);
      p_str++;
    }

    return d;
  }

protected:
  bool _set(const StringName &p_name, const Variant &p_value);
  bool _get(const StringName &p_name, Variant &r_ret) const;
  void _get_property_list(List<PropertyInfo> *p_list) const;
  static void _bind_methods();

public:
  virtual StringName get_message(const StringName &p_src_text)
      const; // overridable for other implementations
  void generate(const Ref<Translation> &p_from);

  PHashTranslation();
};

#endif // COMPRESSED_TRANSLATION_H

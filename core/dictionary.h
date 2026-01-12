

/**
 * @class Dictionary
 * @brief A dynamic key-value container for storing and managing variant pairs.
 * 
 * Dictionary provides a flexible data structure for associating keys with values
 * of arbitrary types (Variant). It supports efficient lookup, insertion, deletion,
 * and iteration operations. The implementation uses copy-on-write semantics through
 * a private data pointer for memory efficiency.
 * 
 * @note This class uses reference counting internally through _ref() and _unref().
 */

/**
 * @brief Retrieves all keys in the dictionary.
 * @param p_keys Pointer to a List that will be populated with all keys.
 */

/**
 * @brief Gets the key at the specified index.
 * @param p_index The index position.
 * @return The key at the given index.
 */

/**
 * @brief Gets the value at the specified index.
 * @param p_index The index position.
 * @return The value at the given index.
 */

/**
 * @brief Accesses or inserts a value by key (mutable).
 * @param p_key The key to access.
 * @return Reference to the variant at the key.
 */

/**
 * @brief Accesses a value by key (immutable).
 * @param p_key The key to access.
 * @return Const reference to the variant at the key.
 */

/**
 * @brief Gets a pointer to the value for a key without creating it if absent.
 * @param p_key The key to find.
 * @return Const pointer to the value, or nullptr if key doesn't exist.
 */

/**
 * @brief Gets a mutable pointer to the value for a key.
 * @param p_key The key to find.
 * @return Pointer to the value, or nullptr if key doesn't exist.
 */

/**
 * @brief Gets a value by key, returning empty Variant if not found.
 * @param p_key The key to access.
 * @return The value at the key or an empty Variant.
 */

/**
 * @brief Gets a value by key with a default fallback.
 * @param p_key The key to access.
 * @param p_default The value to return if key is not found.
 * @return The value at the key or the default value.
 */

/**
 * @brief Returns the number of key-value pairs in the dictionary.
 * @return The number of elements.
 */

/**
 * @brief Checks if the dictionary is empty.
 * @return True if the dictionary contains no elements, false otherwise.
 */

/**
 * @brief Removes all key-value pairs from the dictionary.
 */

/**
 * @brief Checks if a key exists in the dictionary.
 * @param p_key The key to search for.
 * @return True if the key exists, false otherwise.
 */

/**
 * @brief Checks if all keys in the array exist in the dictionary.
 * @param p_keys An array of keys to check.
 * @return True if all keys exist, false otherwise.
 */

/**
 * @brief Removes a key-value pair from the dictionary.
 * @param p_key The key to remove.
 * @return True if the key was found and removed, false otherwise.
 */

/**
 * @brief Compares two dictionaries for equality.
 * @param p_dictionary The dictionary to compare with.
 * @return True if dictionaries are equal, false otherwise.
 */

/**
 * @brief Compares two dictionaries for inequality.
 * @param p_dictionary The dictionary to compare with.
 * @return True if dictionaries are not equal, false otherwise.
 */

/**
 * @brief Computes a hash value for the dictionary.
 * @return A 32-bit hash value.
 */

/**
 * @brief Assigns another dictionary to this one.
 * @param p_dictionary The source dictionary to copy from.
 */

/**
 * @brief Iterates to the next key in the dictionary.
 * @param p_key Pointer to the current key (nullptr to start iteration).
 * @return Pointer to the next key, or nullptr if iteration is complete.
 */

/**
 * @brief Retrieves all keys as an Array.
 * @return An Array containing all keys in the dictionary.
 */

/**
 * @brief Retrieves all values as an Array.
 * @return An Array containing all values in the dictionary.
 */

/**
 * @brief Creates a copy of the dictionary.
 * @param p_deep If true, performs a deep copy of nested structures; otherwise shallow copy.
 * @return A new Dictionary containing the copied data.
 */

/**
 * @brief Gets a unique identifier for this dictionary instance.
 * @return A pointer value uniquely identifying this dictionary.
 */

/**
 * @brief Copy constructor.
 * @param p_from The dictionary to copy from.
 */

/**
 * @brief Default constructor.
 */

/**
 * @brief Destructor.
 */
#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "core/array.h"
#include "core/list.h"
#include "core/ustring.h"

class Variant;

struct DictionaryPrivate;

class Dictionary {

	mutable DictionaryPrivate *_p;

	void _ref(const Dictionary &p_from) const;
	void _unref() const;

public:
	void get_key_list(List<Variant> *p_keys) const;
	Variant get_key_at_index(int p_index) const;
	Variant get_value_at_index(int p_index) const;

	Variant &operator[](const Variant &p_key);
	const Variant &operator[](const Variant &p_key) const;

	const Variant *getptr(const Variant &p_key) const;
	Variant *getptr(const Variant &p_key);

	Variant get_valid(const Variant &p_key) const;
	Variant get(const Variant &p_key, const Variant &p_default) const;

	int size() const;
	bool empty() const;
	void clear();

	bool has(const Variant &p_key) const;
	bool has_all(const Array &p_keys) const;

	bool erase(const Variant &p_key);

	bool operator==(const Dictionary &p_dictionary) const;
	bool operator!=(const Dictionary &p_dictionary) const;

	uint32_t hash() const;
	void operator=(const Dictionary &p_dictionary);

	const Variant *next(const Variant *p_key = NULL) const;

	Array keys() const;
	Array values() const;

	Dictionary duplicate(bool p_deep = false) const;

	const void *id() const;

	Dictionary(const Dictionary &p_from);
	Dictionary();
	~Dictionary();
};

#endif // DICTIONARY_H

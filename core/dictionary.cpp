

/**
 * @file dictionary.cpp
 * @brief Implementation of the Dictionary class for managing key-value pairs.
 * 
 * The Dictionary class provides a dynamic collection of variant key-value pairs
 * with ordered hash map storage. It supports reference counting for memory management
 * and offers various operations for accessing, modifying, and iterating over entries.
 */

/**
 * @struct DictionaryPrivate
 * @brief Private data structure holding the dictionary's internal state.
 * 
 * Contains reference counting and the ordered hash map that stores variant pairs.
 */

/**
 * @brief Populates a list with all keys in the dictionary.
 * @param p_keys Pointer to the list to be filled with keys.
 */

/**
 * @brief Retrieves the key at the specified index.
 * @param p_index The index of the key to retrieve.
 * @return The key at the given index, or an empty Variant if index is out of bounds.
 */

/**
 * @brief Retrieves the value at the specified index.
 * @param p_index The index of the value to retrieve.
 * @return The value at the given index, or an empty Variant if index is out of bounds.
 */

/**
 * @brief Accesses or creates a value by key (non-const version).
 * @param p_key The variant key to access.
 * @return A reference to the value associated with the key.
 */

/**
 * @brief Accesses a value by key (const version).
 * @param p_key The variant key to access.
 * @return A const reference to the value associated with the key.
 */

/**
 * @brief Retrieves a pointer to a value by key (const version).
 * @param p_key The variant key to search for.
 * @return A const pointer to the value, or NULL if the key is not found.
 */

/**
 * @brief Retrieves a pointer to a value by key (non-const version).
 * @param p_key The variant key to search for.
 * @return A pointer to the value, or NULL if the key is not found.
 */

/**
 * @brief Retrieves a value by key with validation.
 * @param p_key The variant key to search for.
 * @return The value if found, or an empty Variant otherwise.
 */

/**
 * @brief Retrieves a value by key with a default fallback.
 * @param p_key The variant key to search for.
 * @param p_default The value to return if the key is not found.
 * @return The value if found, otherwise the default value.
 */

/**
 * @brief Returns the number of key-value pairs in the dictionary.
 * @return The size of the dictionary.
 */

/**
 * @brief Checks if the dictionary is empty.
 * @return True if the dictionary contains no elements, false otherwise.
 */

/**
 * @brief Checks if a key exists in the dictionary.
 * @param p_key The variant key to search for.
 * @return True if the key exists, false otherwise.
 */

/**
 * @brief Checks if all keys in an array exist in the dictionary.
 * @param p_keys The array of keys to check.
 * @return True if all keys exist, false otherwise.
 */

/**
 * @brief Removes a key-value pair from the dictionary.
 * @param p_key The variant key to erase.
 * @return True if the key was found and erased, false otherwise.
 */

/**
 * @brief Compares two dictionaries for equality.
 * @param p_dictionary The dictionary to compare against.
 * @return True if both dictionaries reference the same internal data, false otherwise.
 */

/**
 * @brief Compares two dictionaries for inequality.
 * @param p_dictionary The dictionary to compare against.
 * @return True if the dictionaries reference different internal data, false otherwise.
 */

/**
 * @brief Increments the reference count for another dictionary's data.
 * @param p_from The source dictionary to reference.
 * 
 * Thread-safe reference assignment with automatic cleanup of previous data.
 */

/**
 * @brief Removes all key-value pairs from the dictionary.
 */

/**
 * @brief Decrements the reference count and deallocates memory if needed.
 * 
 * Thread-safe unreferencing with automatic deletion of private data when count reaches zero.
 */

/**
 * @brief Computes a hash value for the dictionary.
 * @return A 32-bit hash value representing the dictionary contents.
 */

/**
 * @brief Returns all keys as an array.
 * @return An array containing all keys in the dictionary.
 */

/**
 * @brief Returns all values as an array.
 * @return An array containing all values in the dictionary.
 */

/**
 * @brief Retrieves the next key in iteration order.
 * @param p_key Pointer to the current key, or NULL to get the first key.
 * @return A pointer to the next key, or NULL if at the end.
 */

/**
 * @brief Creates a duplicate of the dictionary.
 * @param p_deep If true, performs a deep copy of values; if false, performs a shallow copy.
 * @return A new Dictionary instance with copied data.
 */

/**
 * @brief Assignment operator for dictionary copying.
 * @param p_dictionary The source dictionary to assign.
 */

/**
 * @brief Returns the unique identifier of the internal hash map.
 * @return A const pointer to the hash map's ID.
 */

/**
 * @brief Copy constructor.
 * @param p_from The source dictionary to copy from.
 */

/**
 * @brief Default constructor.
 * 
 * Initializes an empty dictionary with reference counting.
 */

/**
 * @brief Destructor.
 * 
 * Safely unreferences and deallocates the dictionary's internal data.
 */
#include "dictionary.h"

#include "core/ordered_hash_map.h"
#include "core/safe_refcount.h"
#include "core/variant.h"

struct DictionaryPrivate {

	SafeRefCount refcount;
	OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator> variant_map;
};

void Dictionary::get_key_list(List<Variant> *p_keys) const {

	if (_p->variant_map.empty())
		return;

	for (OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.front(); E; E = E.next()) {
		p_keys->push_back(E.key());
	}
}

Variant Dictionary::get_key_at_index(int p_index) const {

	int index = 0;
	for (OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.front(); E; E = E.next()) {
		if (index == p_index) {
			return E.key();
		}
		index++;
	}

	return Variant();
}

Variant Dictionary::get_value_at_index(int p_index) const {

	int index = 0;
	for (OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.front(); E; E = E.next()) {
		if (index == p_index) {
			return E.value();
		}
		index++;
	}

	return Variant();
}

Variant &Dictionary::operator[](const Variant &p_key) {

	return _p->variant_map[p_key];
}

const Variant &Dictionary::operator[](const Variant &p_key) const {

	return _p->variant_map[p_key];
}
const Variant *Dictionary::getptr(const Variant &p_key) const {

	OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::ConstElement E = ((const OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator> *)&_p->variant_map)->find(p_key);

	if (!E)
		return NULL;
	return &E.get();
}

Variant *Dictionary::getptr(const Variant &p_key) {

	OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.find(p_key);

	if (!E)
		return NULL;
	return &E.get();
}

Variant Dictionary::get_valid(const Variant &p_key) const {

	OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::ConstElement E = ((const OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator> *)&_p->variant_map)->find(p_key);

	if (!E)
		return Variant();
	return E.get();
}

Variant Dictionary::get(const Variant &p_key, const Variant &p_default) const {
	const Variant *result = getptr(p_key);
	if (!result) {
		return p_default;
	}

	return *result;
}

int Dictionary::size() const {

	return _p->variant_map.size();
}
bool Dictionary::empty() const {

	return !_p->variant_map.size();
}

bool Dictionary::has(const Variant &p_key) const {

	return _p->variant_map.has(p_key);
}

bool Dictionary::has_all(const Array &p_keys) const {
	for (int i = 0; i < p_keys.size(); i++) {
		if (!has(p_keys[i])) {
			return false;
		}
	}
	return true;
}

bool Dictionary::erase(const Variant &p_key) {

	return _p->variant_map.erase(p_key);
}

bool Dictionary::operator==(const Dictionary &p_dictionary) const {

	return _p == p_dictionary._p;
}

bool Dictionary::operator!=(const Dictionary &p_dictionary) const {

	return _p != p_dictionary._p;
}

void Dictionary::_ref(const Dictionary &p_from) const {

	//make a copy first (thread safe)
	if (!p_from._p->refcount.ref())
		return; // couldn't copy

	//if this is the same, unreference the other one
	if (p_from._p == _p) {
		_p->refcount.unref();
		return;
	}
	if (_p)
		_unref();
	_p = p_from._p;
}

void Dictionary::clear() {

	_p->variant_map.clear();
}

void Dictionary::_unref() const {

	ERR_FAIL_COND(!_p);
	if (_p->refcount.unref()) {
		memdelete(_p);
	}
	_p = NULL;
}
uint32_t Dictionary::hash() const {

	uint32_t h = hash_djb2_one_32(Variant::DICTIONARY);

	for (OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.front(); E; E = E.next()) {
		h = hash_djb2_one_32(E.key().hash(), h);
		h = hash_djb2_one_32(E.value().hash(), h);
	}

	return h;
}

Array Dictionary::keys() const {

	Array varr;
	if (_p->variant_map.empty())
		return varr;

	varr.resize(size());

	int i = 0;
	for (OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.front(); E; E = E.next()) {
		varr[i] = E.key();
		i++;
	}

	return varr;
}

Array Dictionary::values() const {

	Array varr;
	if (_p->variant_map.empty())
		return varr;

	varr.resize(size());

	int i = 0;
	for (OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.front(); E; E = E.next()) {
		varr[i] = E.get();
		i++;
	}

	return varr;
}

const Variant *Dictionary::next(const Variant *p_key) const {

	if (p_key == NULL) {
		// caller wants to get the first element
		if (_p->variant_map.front())
			return &_p->variant_map.front().key();
		return NULL;
	}
	OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.find(*p_key);

	if (E && E.next())
		return &E.next().key();
	return NULL;
}

Dictionary Dictionary::duplicate(bool p_deep) const {

	Dictionary n;

	for (OrderedHashMap<Variant, Variant, VariantHasher, VariantComparator>::Element E = _p->variant_map.front(); E; E = E.next()) {
		n[E.key()] = p_deep ? E.value().duplicate(true) : E.value();
	}

	return n;
}

void Dictionary::operator=(const Dictionary &p_dictionary) {

	_ref(p_dictionary);
}

const void *Dictionary::id() const {
	return _p->variant_map.id();
}

Dictionary::Dictionary(const Dictionary &p_from) {
	_p = NULL;
	_ref(p_from);
}

Dictionary::Dictionary() {

	_p = memnew(DictionaryPrivate);
	_p->refcount.init();
}
Dictionary::~Dictionary() {

	_unref();
}

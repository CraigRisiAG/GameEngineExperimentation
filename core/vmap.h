

/**
 * @class VMap
 * @brief A template-based sorted map container using copy-on-write semantics.
 *
 * VMap is a generic key-value map that maintains its elements in sorted order
 * based on the key type. It uses binary search for efficient lookups and uses
 * copy-on-write (CowData) for memory efficiency when copying.
 *
 * @tparam T The key type. Must support operator< for comparison.
 * @tparam V The value type associated with each key.
 *
 * @note This container maintains sorted order, making it suitable for scenarios
 *       where elements need to be accessed in order or where range queries are needed.
 *
 * @note Uses copy-on-write semantics for efficient copying and assignment operations.
 *
 * Example usage:
 * @code
 * VMap<String, int> myMap;
 * myMap.insert("key1", 42);
 * if (myMap.has("key1")) {
 *     int value = myMap["key1"];
 * }
 * @endcode
 */

/**
 * @struct Pair
 * @brief A key-value pair structure.
 *
 * @member T key The key of the pair.
 * @member V value The value associated with the key.
 */

/**
 * @brief Finds the position of a key or where it should be inserted.
 *
 * @param p_val The value to search for.
 * @param r_exact Output parameter set to true if an exact match was found.
 * @return The index of the found element or the insertion position.
 */

/**
 * @brief Finds the exact position of a key.
 *
 * @param p_val The value to search for.
 * @return The index of the key if found, -1 otherwise.
 */

/**
 * @brief Inserts or updates a key-value pair.
 *
 * @param p_key The key to insert or update.
 * @param p_val The value to associate with the key.
 * @return The index where the pair was inserted or updated.
 */

/**
 * @brief Checks if a key exists in the map.
 *
 * @param p_val The key to search for.
 * @return true if the key exists, false otherwise.
 */

/**
 * @brief Removes a key-value pair from the map.
 *
 * @param p_val The key to remove. Does nothing if the key doesn't exist.
 */

/**
 * @brief Finds the index of a key.
 *
 * @param p_val The key to search for.
 * @return The index if found, -1 otherwise.
 */

/**
 * @brief Finds the nearest index to a given key.
 *
 * @param p_val The key to search for.
 * @return The index of the key if found, or the index where it would be inserted.
 */

/**
 * @brief Returns the number of elements in the map.
 *
 * @return The size of the map.
 */

/**
 * @brief Checks if the map is empty.
 *
 * @return true if the map contains no elements, false otherwise.
 */

/**
 * @brief Returns a const pointer to the internal array of pairs.
 *
 * @return Const pointer to the pair array.
 */

/**
 * @brief Returns a mutable pointer to the internal array of pairs.
 *
 * @return Mutable pointer to the pair array.
 */

/**
 * @brief Retrieves the value at a given index.
 *
 * @param p_index The index of the element.
 * @return Const reference to the value.
 */

/**
 * @brief Retrieves the mutable value at a given index.
 *
 * @param p_index The index of the element.
 * @return Mutable reference to the value.
 */

/**
 * @brief Retrieves the key at a given index.
 *
 * @param p_index The index of the element.
 * @return Const reference to the key.
 */

/**
 * @brief Retrieves the mutable key at a given index.
 *
 * @param p_index The index of the element.
 * @return Mutable reference to the key.
 */

/**
 * @brief Accesses a value by key (const version).
 *
 * @param p_key The key to access.
 * @return Const reference to the value.
 * @note Will crash if the key does not exist.
 */

/**
 * @brief Accesses or creates a value by key.
 *
 * @param p_key The key to access.
 * @return Mutable reference to the value. If the key doesn't exist, it is created with a default value.
 */
#ifndef VMAP_H
#define VMAP_H

#include "core/cowdata.h"
#include "core/typedefs.h"

template <class T, class V>
class VMap {
public:
	struct Pair {

		T key;
		V value;

		_FORCE_INLINE_ Pair() {}

		_FORCE_INLINE_ Pair(const T &p_key, const V &p_value) {

			key = p_key;
			value = p_value;
		}
	};

private:
	CowData<Pair> _cowdata;

	_FORCE_INLINE_ int _find(const T &p_val, bool &r_exact) const {

		r_exact = false;
		if (_cowdata.empty())
			return 0;

		int low = 0;
		int high = _cowdata.size() - 1;
		const Pair *a = _cowdata.ptr();
		int middle = 0;

#ifdef DEBUG_ENABLED
		if (low > high)
			ERR_PRINT("low > high, this may be a bug");
#endif
		while (low <= high) {
			middle = (low + high) / 2;

			if (p_val < a[middle].key) {
				high = middle - 1; //search low end of array
			} else if (a[middle].key < p_val) {
				low = middle + 1; //search high end of array
			} else {
				r_exact = true;
				return middle;
			}
		}

		//return the position where this would be inserted
		if (a[middle].key < p_val)
			middle++;
		return middle;
	}

	_FORCE_INLINE_ int _find_exact(const T &p_val) const {

		if (_cowdata.empty())
			return -1;

		int low = 0;
		int high = _cowdata.size() - 1;
		int middle;
		const Pair *a = _cowdata.ptr();

		while (low <= high) {
			middle = (low + high) / 2;

			if (p_val < a[middle].key) {
				high = middle - 1; //search low end of array
			} else if (a[middle].key < p_val) {
				low = middle + 1; //search high end of array
			} else {
				return middle;
			}
		}

		return -1;
	}

public:
	int insert(const T &p_key, const V &p_val) {

		bool exact;
		int pos = _find(p_key, exact);
		if (exact) {
			_cowdata.get_m(pos).value = p_val;
			return pos;
		}
		_cowdata.insert(pos, Pair(p_key, p_val));
		return pos;
	}

	bool has(const T &p_val) const {

		return _find_exact(p_val) != -1;
	}

	void erase(const T &p_val) {

		int pos = _find_exact(p_val);
		if (pos < 0)
			return;
		_cowdata.remove(pos);
	}

	int find(const T &p_val) const {

		return _find_exact(p_val);
	}

	int find_nearest(const T &p_val) const {

		bool exact;
		return _find(p_val, exact);
	}

	_FORCE_INLINE_ int size() const { return _cowdata.size(); }
	_FORCE_INLINE_ bool empty() const { return _cowdata.empty(); }

	const Pair *get_array() const {

		return _cowdata.ptr();
	}

	Pair *get_array() {

		return _cowdata.ptrw();
	}

	const V &getv(int p_index) const {

		return _cowdata.get(p_index).value;
	}

	V &getv(int p_index) {

		return _cowdata.get_m(p_index).value;
	}

	const T &getk(int p_index) const {

		return _cowdata.get(p_index).key;
	}

	T &getk(int p_index) {

		return _cowdata.get_m(p_index).key;
	}

	inline const V &operator[](const T &p_key) const {

		int pos = _find_exact(p_key);

		CRASH_COND(pos < 0);

		return _cowdata.get(pos).value;
	}

	inline V &operator[](const T &p_key) {

		int pos = _find_exact(p_key);
		if (pos < 0) {
			pos = insert(p_key, V());
		}

		return _cowdata.get_m(pos).value;
	}

	_FORCE_INLINE_ VMap(){};
	_FORCE_INLINE_ VMap(const VMap &p_from) { _cowdata._ref(p_from._cowdata); }
	inline VMap &operator=(const VMap &p_from) {
		_cowdata._ref(p_from._cowdata);
		return *this;
	}
};
#endif // VMAP_H

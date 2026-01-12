

/**
 * @class Array
 * @brief Dynamic array container for storing Variant objects
 * 
 * Array is a dynamic container class that manages a collection of Variant objects
 * with automatic memory management through reference counting. It provides various
 * operations including insertion, deletion, searching, and sorting capabilities.
 * 
 * The Array class uses copy-on-write semantics through the ArrayPrivate pointer
 * to optimize memory usage and performance.
 * 
 * @note This class uses mutable internal state and reference counting for efficient
 *       copying and assignment operations.
 */

/**
 * @brief Access array element by index (non-const version)
 * @param p_idx The index of the element to access
 * @return Reference to the Variant at the specified index
 */
Variant &operator[](int p_idx);

/**
 * @brief Access array element by index (const version)
 * @param p_idx The index of the element to access
 * @return Const reference to the Variant at the specified index
 */
const Variant &operator[](int p_idx) const;

/**
 * @brief Set element at specified index
 * @param p_idx The index where the element will be set
 * @param p_value The value to set
 */
void set(int p_idx, const Variant &p_value);

/**
 * @brief Get element at specified index
 * @param p_idx The index of the element to retrieve
 * @return Const reference to the Variant at the specified index
 */
const Variant &get(int p_idx) const;

/**
 * @brief Get the number of elements in the array
 * @return The size of the array
 */
int size() const;

/**
 * @brief Check if the array is empty
 * @return True if the array contains no elements, false otherwise
 */
bool empty() const;

/**
 * @brief Remove all elements from the array
 */
void clear();

/**
 * @brief Compare two arrays for equality
 * @param p_array The array to compare with
 * @return True if arrays are equal, false otherwise
 */
bool operator==(const Array &p_array) const;

/**
 * @brief Generate a hash value for the array
 * @return Hash value of the array
 */
uint32_t hash() const;

/**
 * @brief Assign another array to this array
 * @param p_array The array to assign from
 */
void operator=(const Array &p_array);

/**
 * @brief Add an element to the end of the array
 * @param p_value The value to add
 */
void push_back(const Variant &p_value);

/**
 * @brief Add an element to the end of the array (Python compatibility alias)
 * @param p_value The value to add
 */
_FORCE_INLINE_ void append(const Variant &p_value);

/**
 * @brief Resize the array to a new size
 * @param p_new_size The new size of the array
 * @return Error code indicating success or failure
 */
Error resize(int p_new_size);

/**
 * @brief Insert an element at the specified position
 * @param p_pos The position where the element will be inserted
 * @param p_value The value to insert
 */
void insert(int p_pos, const Variant &p_value);

/**
 * @brief Remove element at the specified position
 * @param p_pos The index of the element to remove
 */
void remove(int p_pos);

/**
 * @brief Get the first element in the array
 * @return A copy of the first element
 */
Variant front() const;

/**
 * @brief Get the last element in the array
 * @return A copy of the last element
 */
Variant back() const;

/**
 * @brief Sort the array in ascending order
 * @return Reference to this array for method chaining
 */
Array &sort();

/**
 * @brief Sort the array using a custom comparison function
 * @param p_obj The object containing the comparison function
 * @param p_function The name of the comparison function
 * @return Reference to this array for method chaining
 */
Array &sort_custom(Object *p_obj, const StringName &p_function);

/**
 * @brief Randomly shuffle the elements in the array
 */
void shuffle();

/**
 * @brief Binary search for a value in the array
 * @param p_value The value to search for
 * @param p_before If true, returns the index before the insertion point
 * @return The index of the value or insertion point
 */
int bsearch(const Variant &p_value, bool p_before = true);

/**
 * @brief Binary search using a custom comparison function
 * @param p_value The value to search for
 * @param p_obj The object containing the comparison function
 * @param p_function The name of the comparison function
 * @param p_before If true, returns the index before the insertion point
 * @return The index of the value or insertion point
 */
int bsearch_custom(const Variant &p_value, Object *p_obj, const StringName &p_function, bool p_before = true);

/**
 * @brief Reverse the order of elements in the array
 * @return Reference to this array for method chaining
 */
Array &invert();

/**
 * @brief Find the first index of a value in the array
 * @param p_value The value to search for
 * @param p_from The starting index for the search
 * @return The index of the first occurrence, or -1 if not found
 */
int find(const Variant &p_value, int p_from = 0) const;

/**
 * @brief Find the last index of a value in the array (searching backwards)
 * @param p_value The value to search for
 * @param p_from The starting index for the backward search
 * @return The index of the last occurrence, or -1 if not found
 */
int rfind(const Variant &p_value, int p_from = -1) const;

/**
 * @brief Find the last index of a value in the array
 * @param p_value The value to search for
 * @return The index of the last occurrence, or -1 if not found
 */
int find_last(const Variant &p_value) const;

/**
 * @brief Count the number of occurrences of a value in the array
 * @param p_value The value to count
 * @return The number of times the value appears in the array
 */
int count(const Variant &p_value) const;

/**
 * @brief Check if the array contains a specific value
 * @param p_value The value to search for
 * @return True if the value is found, false otherwise
 */
bool has(const Variant &p_value) const;

/**
 * @brief Remove all occurrences of a value from the array
 * @param p_value The value to erase
 */
void erase(const Variant &p_value);

/**
 * @brief Add an element to the beginning of the array
 * @param p_value The value to add
 */
void push_front(const Variant &p_value);

/**
 * @brief Remove and return the last element from the array
 * @return A copy of the last element
 */
Variant pop_back();

/**
 * @brief Remove and return the first element from the array
 * @return A copy of the first element
 */
Variant pop_front();

/**
 * @brief Create a copy of the array
 * @param p_deep If true, performs a deep copy of all elements; if false, performs a shallow copy
 * @return A new Array that is a duplicate of this array
 */
Array duplicate(bool p_deep = false) const;

/**
 * @brief Extract a slice of the array
 * @param p_begin The starting index (inclusive)
 * @param p_end The ending index (exclusive)
 * @param p_step The step size between elements (default 1)
 * @param p_deep If true, performs a deep copy of elements; if false, performs a shallow copy
 * @return A new Array containing the sliced elements
 */
Array slice(int p_begin, int p_end, int p_step = 1, bool p_deep = false) const;

/**
 * @brief Find the minimum value in the array
 * @return The minimum value as a Variant
 */
Variant min() const;

/**
 * @brief Find the maximum value in the array
 * @return The maximum value as a Variant
 */
Variant max() const;

/**
 * @brief Get a unique identifier for this array's internal data
 * @return A pointer representing the unique identifier
 */
const void *id() const;

/**
 * @brief Copy constructor
 * @param p_from The array to copy from
 */
Array(const Array &p_from);

/**
 * @brief Default constructor
 * Initializes an empty array
 */
Array();

/**
 * @brief Destructor
 * Releases the internal array data
 */
~Array();
#ifndef ARRAY_H
#define ARRAY_H

#include "core/typedefs.h"

class Variant;
class ArrayPrivate;
class Object;
class StringName;

class Array {

	mutable ArrayPrivate *_p;
	void _ref(const Array &p_from) const;
	void _unref() const;

	int _clamp_index(int p_index) const;
	static int _fix_slice_index(int p_index, int p_arr_len, int p_top_mod);

public:
	Variant &operator[](int p_idx);
	const Variant &operator[](int p_idx) const;

	void set(int p_idx, const Variant &p_value);
	const Variant &get(int p_idx) const;

	int size() const;
	bool empty() const;
	void clear();

	bool operator==(const Array &p_array) const;

	uint32_t hash() const;
	void operator=(const Array &p_array);

	void push_back(const Variant &p_value);
	_FORCE_INLINE_ void append(const Variant &p_value) { push_back(p_value); } //for python compatibility
	Error resize(int p_new_size);

	void insert(int p_pos, const Variant &p_value);
	void remove(int p_pos);

	Variant front() const;
	Variant back() const;

	Array &sort();
	Array &sort_custom(Object *p_obj, const StringName &p_function);
	void shuffle();
	int bsearch(const Variant &p_value, bool p_before = true);
	int bsearch_custom(const Variant &p_value, Object *p_obj, const StringName &p_function, bool p_before = true);
	Array &invert();

	int find(const Variant &p_value, int p_from = 0) const;
	int rfind(const Variant &p_value, int p_from = -1) const;
	int find_last(const Variant &p_value) const;
	int count(const Variant &p_value) const;
	bool has(const Variant &p_value) const;

	void erase(const Variant &p_value);

	void push_front(const Variant &p_value);
	Variant pop_back();
	Variant pop_front();

	Array duplicate(bool p_deep = false) const;

	Array slice(int p_begin, int p_end, int p_step = 1, bool p_deep = false) const;

	Variant min() const;
	Variant max() const;

	const void *id() const;

	Array(const Array &p_from);
	Array();
	~Array();
};

#endif // ARRAY_H

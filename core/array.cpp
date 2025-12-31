

/// @class Array
/// @brief A dynamic array container that holds Variant values with reference counting.
/// 
/// The Array class provides a flexible, dynamically-sized container for storing Variant objects.
/// It uses reference counting for memory management and supports standard array operations like
/// insertion, removal, searching, and sorting.
/// 
/// @note Array uses copy-on-write semantics through reference counting. Multiple Array instances
/// can share the same underlying data until one is modified.
/// 
/// @example
/// @code
/// Array arr;
/// arr.push_back(42);
/// arr.push_back("Hello");
/// arr.insert(1, 3.14);
/// int index = arr.find(3.14);
/// @endcode

/// @fn void Array::_ref(const Array &p_from) const
/// @brief Increases reference count and assigns internal pointer from another array.
/// @param p_from The source array to reference.
/// @note This is an internal method. Does nothing if already referencing the same array.

/// @fn void Array::_unref() const
/// @brief Decreases reference count and deletes internal data if no longer referenced.
/// @note This is an internal method.

/// @fn Variant &Array::operator[](int p_idx)
/// @brief Returns a mutable reference to the variant at the given index.
/// @param p_idx The index of the element.
/// @return Mutable reference to the Variant at the specified index.

/// @fn const Variant &Array::operator[](int p_idx) const
/// @brief Returns a const reference to the variant at the given index.
/// @param p_idx The index of the element.
/// @return Const reference to the Variant at the specified index.

/// @fn int Array::size() const
/// @brief Returns the number of elements in the array.
/// @return The size of the array.

/// @fn bool Array::empty() const
/// @brief Checks if the array is empty.
/// @return True if the array contains no elements, false otherwise.

/// @fn void Array::clear()
/// @brief Removes all elements from the array.

/// @fn bool Array::operator==(const Array &p_array) const
/// @brief Compares two arrays for equality based on reference identity.
/// @param p_array The array to compare with.
/// @return True if both arrays reference the same internal data, false otherwise.

/// @fn uint32_t Array::hash() const
/// @brief Computes a hash value for the array based on its contents.
/// @return A 32-bit hash value.

/// @fn void Array::operator=(const Array &p_array)
/// @brief Assigns another array to this array using reference counting.
/// @param p_array The source array to assign.

/// @fn void Array::push_back(const Variant &p_value)
/// @brief Appends a variant to the end of the array.
/// @param p_value The value to append.

/// @fn Error Array::resize(int p_new_size)
/// @brief Resizes the array to the specified size.
/// @param p_new_size The new size of the array.
/// @return Error code indicating success or failure.

/// @fn void Array::insert(int p_pos, const Variant &p_value)
/// @brief Inserts a variant at the specified position.
/// @param p_pos The position to insert at.
/// @param p_value The value to insert.

/// @fn void Array::erase(const Variant &p_value)
/// @brief Removes the first occurrence of a variant value.
/// @param p_value The value to erase.

/// @fn Variant Array::front() const
/// @brief Returns the first element in the array.
/// @return The first Variant, or an empty Variant if the array is empty.

/// @fn Variant Array::back() const
/// @brief Returns the last element in the array.
/// @return The last Variant, or an empty Variant if the array is empty.

/// @fn int Array::find(const Variant &p_value, int p_from) const
/// @brief Finds the index of the first occurrence of a value.
/// @param p_value The value to search for.
/// @param p_from The starting index for the search.
/// @return The index of the first match, or -1 if not found.

/// @fn int Array::rfind(const Variant &p_value, int p_from) const
/// @brief Finds the index of the last occurrence of a value.
/// @param p_value The value to search for.
/// @param p_from The starting index for reverse search (negative for offset from end).
/// @return The index of the last match, or -1 if not found.

/// @fn int Array::find_last(const Variant &p_value) const
/// @brief Finds the index of the last occurrence of a value.
/// @param p_value The value to search for.
/// @return The index of the last match, or -1 if not found.

/// @fn int Array::count(const Variant &p_value) const
/// @brief Counts occurrences of a specific value in the array.
/// @param p_value The value to count.
/// @return The number of times the value appears in the array.

/// @fn bool Array::has(const Variant &p_value) const
/// @brief Checks if the array contains a specific value.
/// @param p_value The value to search for.
/// @return True if the value is found, false otherwise.

/// @fn void Array::remove(int p_pos)
/// @brief Removes the element at the specified position.
/// @param p_pos The index of the element to remove.

/// @fn void Array::set(int p_idx, const Variant &p_value)
/// @brief Sets the value at the specified index.
/// @param p_idx The index to set.
/// @param p_value The new value.

/// @fn const Variant &Array::get(int p_idx) const
/// @brief Gets the value at the specified index.
/// @param p_idx The index to retrieve.
/// @return Const reference to the Variant at the specified index.

/// @fn Array Array::duplicate(bool p_deep) const
/// @brief Creates a copy of the array.
/// @param p_deep If true, recursively duplicates all contained Variants.
/// @return A new Array with duplicated elements.

/// @fn int Array::_fix_slice_index(int p_index, int p_arr_len, int p_top_mod)
/// @brief Internal method to normalize slice indices.
/// @param p_index The index to normalize.
/// @param p_arr_len The length of the array.
/// @param p_top_mod Modulo adjustment for boundary.
/// @return The normalized index.

/// @fn int Array::_clamp_index(int p_index) const
/// @brief Internal method to clamp an index within valid bounds.
/// @param p_index The index to clamp.
/// @return The clamped index.

/// @fn Array Array::slice(int p_begin, int p_end, int p_step, bool p_deep) const
/// @brief Extracts a slice of the array with optional step and deep copy.
/// @param p_begin The starting index (inclusive).
/// @param p_end The ending index (inclusive).
/// @param p_step The step between elements (default 1, can be negative).
/// @param p_deep If true, deeply copies all extracted elements.
/// @return A new Array containing the sliced elements.

/// @fn Array &Array::sort()
/// @brief Sorts the array using the default variant comparison operator.
/// @return Reference to this array for method chaining.

/// @fn Array &Array::sort_custom(Object *p_obj, const StringName &p_function)
/// @brief Sorts the array using a custom comparison function from an object.
/// @param p_obj The object containing the comparison method.
/// @param p_function The name of the comparison method.
/// @return Reference to this array for method chaining.

/// @fn void Array::shuffle()
/// @brief Randomly shuffles the elements of the array.
/// @note Uses pseudo-random number generation.

/// @fn int Array::bsearch(const Variant &p_value, bool p_before)
/// @brief Performs a binary search on a sorted array.
/// @param p_value The value to search for.
/// @param p_before If true, returns position before the value; otherwise, position after.
/// @return The index where the value is or should be inserted.

/// @fn int Array::bsearch_custom(const Variant &p_value, Object *p_obj, const StringName &p_function, bool p_before)
/// @brief Performs a binary search using a custom comparison function.
/// @param p_value The value to search for.
/// @param p_obj The object containing the comparison method.
/// @param p_function The name of the comparison method.
/// @param p_before If true, returns position before the value; otherwise, position after.
/// @return The index where the value is or should be inserted.

/// @fn Array &Array::invert()
/// @brief Reverses the order of elements in the array.
/// @return Reference to this array for method chaining.

/// @fn void Array::push_front(const Variant &p_value)
/// @brief Inserts a variant at the beginning of the array.
/// @param p_value The value to insert.

/// @fn Variant Array::pop_back()
/// @brief Removes and returns the last element.
/// @return The last element, or an empty Variant if the array is empty.

/// @fn Variant Array::pop_front()
/// @brief Removes and returns the first element.
/// @return The first element, or an empty Variant if the array is empty.

/// @fn Variant Array::min() const
/// @brief Finds the minimum value in the array using variant comparison.
/// @return The minimum Variant, or an empty Variant if comparison is invalid.

/// @fn Variant Array::max() const
/// @brief Finds the maximum value in the array using variant comparison.
/// @return The maximum Variant, or an empty Variant if comparison is invalid.

/// @fn const void *Array::id() const
/// @brief Returns the internal pointer ID of the underlying data.
/// @return A const void pointer to the internal array data.

/// @fn Array::Array(const Array &p_from)
/// @brief Copy constructor that creates an array referencing the same data.
/// @param p_from The source array to copy from.

/// @fn Array::Array()
/// @brief Default constructor that creates an empty array.

/// @fn Array::~Array()
/// @brief Destructor that unreferences the internal data.
#include "array.h"

#include "core/hashfuncs.h"
#include "core/object.h"
#include "core/variant.h"
#include "core/vector.h"

class ArrayPrivate {
public:
	SafeRefCount refcount;
	Vector<Variant> array;
};

void Array::_ref(const Array &p_from) const {

	ArrayPrivate *_fp = p_from._p;

	ERR_FAIL_COND(!_fp); // should NOT happen.

	if (_fp == _p)
		return; // whatever it is, nothing to do here move along

	bool success = _fp->refcount.ref();

	ERR_FAIL_COND(!success); // should really not happen either

	_unref();

	_p = p_from._p;
}

void Array::_unref() const {

	if (!_p)
		return;

	if (_p->refcount.unref()) {
		memdelete(_p);
	}
	_p = NULL;
}

Variant &Array::operator[](int p_idx) {

	return _p->array.write[p_idx];
}

const Variant &Array::operator[](int p_idx) const {

	return _p->array[p_idx];
}

int Array::size() const {

	return _p->array.size();
}
bool Array::empty() const {

	return _p->array.empty();
}
void Array::clear() {

	_p->array.clear();
}

bool Array::operator==(const Array &p_array) const {

	return _p == p_array._p;
}

uint32_t Array::hash() const {

	uint32_t h = hash_djb2_one_32(0);

	for (int i = 0; i < _p->array.size(); i++) {

		h = hash_djb2_one_32(_p->array[i].hash(), h);
	}
	return h;
}
void Array::operator=(const Array &p_array) {

	_ref(p_array);
}
void Array::push_back(const Variant &p_value) {

	_p->array.push_back(p_value);
}

Error Array::resize(int p_new_size) {

	return _p->array.resize(p_new_size);
}

void Array::insert(int p_pos, const Variant &p_value) {

	_p->array.insert(p_pos, p_value);
}

void Array::erase(const Variant &p_value) {

	_p->array.erase(p_value);
}

Variant Array::front() const {
	ERR_FAIL_COND_V_MSG(_p->array.size() == 0, Variant(), "Can't take value from empty array.");
	return operator[](0);
}

Variant Array::back() const {
	ERR_FAIL_COND_V_MSG(_p->array.size() == 0, Variant(), "Can't take value from empty array.");
	return operator[](_p->array.size() - 1);
}

int Array::find(const Variant &p_value, int p_from) const {

	return _p->array.find(p_value, p_from);
}

int Array::rfind(const Variant &p_value, int p_from) const {

	if (_p->array.size() == 0)
		return -1;

	if (p_from < 0) {
		// Relative offset from the end
		p_from = _p->array.size() + p_from;
	}
	if (p_from < 0 || p_from >= _p->array.size()) {
		// Limit to array boundaries
		p_from = _p->array.size() - 1;
	}

	for (int i = p_from; i >= 0; i--) {

		if (_p->array[i] == p_value) {
			return i;
		}
	}

	return -1;
}

int Array::find_last(const Variant &p_value) const {

	return rfind(p_value);
}

int Array::count(const Variant &p_value) const {

	if (_p->array.size() == 0)
		return 0;

	int amount = 0;
	for (int i = 0; i < _p->array.size(); i++) {

		if (_p->array[i] == p_value) {
			amount++;
		}
	}

	return amount;
}

bool Array::has(const Variant &p_value) const {
	return _p->array.find(p_value, 0) != -1;
}

void Array::remove(int p_pos) {

	_p->array.remove(p_pos);
}

void Array::set(int p_idx, const Variant &p_value) {

	operator[](p_idx) = p_value;
}

const Variant &Array::get(int p_idx) const {

	return operator[](p_idx);
}

Array Array::duplicate(bool p_deep) const {

	Array new_arr;
	int element_count = size();
	new_arr.resize(element_count);
	for (int i = 0; i < element_count; i++) {
		new_arr[i] = p_deep ? get(i).duplicate(p_deep) : get(i);
	}

	return new_arr;
}

int Array::_fix_slice_index(int p_index, int p_arr_len, int p_top_mod) {
	p_index = CLAMP(p_index, -p_arr_len, p_arr_len + p_top_mod);
	if (p_index < 0) {
		p_index = (p_index % p_arr_len + p_arr_len) % p_arr_len; // positive modulo
	}
	return p_index;
}

int Array::_clamp_index(int p_index) const {
	return CLAMP(p_index, -size() + 1, size() - 1);
}

#define ARRAY_GET_DEEP(idx, is_deep) is_deep ? get(idx).duplicate(is_deep) : get(idx)

Array Array::slice(int p_begin, int p_end, int p_step, bool p_deep) const { // like python, but inclusive on upper bound
	Array new_arr;

	if (empty()) // Don't try to slice empty arrays.
		return new_arr;

	p_begin = Array::_fix_slice_index(p_begin, size(), -1); // can't start out of range
	p_end = Array::_fix_slice_index(p_end, size(), 0);

	int x = p_begin;
	int new_arr_i = 0;

	ERR_FAIL_COND_V(p_step == 0, new_arr);
	if (Array::_clamp_index(p_begin) == Array::_clamp_index(p_end)) { // don't include element twice
		new_arr.resize(1);
		// new_arr[0] = 1;
		new_arr[0] = ARRAY_GET_DEEP(Array::_clamp_index(p_begin), p_deep);
		return new_arr;
	} else {
		int element_count = ceil((int)MAX(0, (p_end - p_begin) / p_step)) + 1;
		if (element_count == 1) { // delta going in wrong direction to reach end
			new_arr.resize(0);
			return new_arr;
		}
		new_arr.resize(element_count);
	}

	// if going backwards, have to have a different terminating condition
	if (p_step < 0) {
		while (x >= p_end) {
			new_arr[new_arr_i] = ARRAY_GET_DEEP(Array::_clamp_index(x), p_deep);
			x += p_step;
			new_arr_i += 1;
		}
	} else if (p_step > 0) {
		while (x <= p_end) {
			new_arr[new_arr_i] = ARRAY_GET_DEEP(Array::_clamp_index(x), p_deep);
			x += p_step;
			new_arr_i += 1;
		}
	}

	return new_arr;
}

struct _ArrayVariantSort {

	_FORCE_INLINE_ bool operator()(const Variant &p_l, const Variant &p_r) const {
		bool valid = false;
		Variant res;
		Variant::evaluate(Variant::OP_LESS, p_l, p_r, res, valid);
		if (!valid)
			res = false;
		return res;
	}
};

Array &Array::sort() {

	_p->array.sort_custom<_ArrayVariantSort>();
	return *this;
}

struct _ArrayVariantSortCustom {

	Object *obj;
	StringName func;

	_FORCE_INLINE_ bool operator()(const Variant &p_l, const Variant &p_r) const {

		const Variant *args[2] = { &p_l, &p_r };
		Callable::CallError err;
		bool res = obj->call(func, args, 2, err);
		if (err.error != Callable::CallError::CALL_OK)
			res = false;
		return res;
	}
};
Array &Array::sort_custom(Object *p_obj, const StringName &p_function) {

	ERR_FAIL_NULL_V(p_obj, *this);

	SortArray<Variant, _ArrayVariantSortCustom, true> avs;
	avs.compare.obj = p_obj;
	avs.compare.func = p_function;
	avs.sort(_p->array.ptrw(), _p->array.size());
	return *this;
}

void Array::shuffle() {

	const int n = _p->array.size();
	if (n < 2)
		return;
	Variant *data = _p->array.ptrw();
	for (int i = n - 1; i >= 1; i--) {
		const int j = Math::rand() % (i + 1);
		const Variant tmp = data[j];
		data[j] = data[i];
		data[i] = tmp;
	}
}

template <typename Less>
_FORCE_INLINE_ int bisect(const Vector<Variant> &p_array, const Variant &p_value, bool p_before, const Less &p_less) {

	int lo = 0;
	int hi = p_array.size();
	if (p_before) {
		while (lo < hi) {
			const int mid = (lo + hi) / 2;
			if (p_less(p_array.get(mid), p_value)) {
				lo = mid + 1;
			} else {
				hi = mid;
			}
		}
	} else {
		while (lo < hi) {
			const int mid = (lo + hi) / 2;
			if (p_less(p_value, p_array.get(mid))) {
				hi = mid;
			} else {
				lo = mid + 1;
			}
		}
	}
	return lo;
}

int Array::bsearch(const Variant &p_value, bool p_before) {

	return bisect(_p->array, p_value, p_before, _ArrayVariantSort());
}

int Array::bsearch_custom(const Variant &p_value, Object *p_obj, const StringName &p_function, bool p_before) {

	ERR_FAIL_NULL_V(p_obj, 0);

	_ArrayVariantSortCustom less;
	less.obj = p_obj;
	less.func = p_function;

	return bisect(_p->array, p_value, p_before, less);
}

Array &Array::invert() {

	_p->array.invert();
	return *this;
}

void Array::push_front(const Variant &p_value) {

	_p->array.insert(0, p_value);
}

Variant Array::pop_back() {

	if (!_p->array.empty()) {
		int n = _p->array.size() - 1;
		Variant ret = _p->array.get(n);
		_p->array.resize(n);
		return ret;
	}
	return Variant();
}

Variant Array::pop_front() {

	if (!_p->array.empty()) {
		Variant ret = _p->array.get(0);
		_p->array.remove(0);
		return ret;
	}
	return Variant();
}

Variant Array::min() const {

	Variant minval;
	for (int i = 0; i < size(); i++) {
		if (i == 0) {
			minval = get(i);
		} else {
			bool valid;
			Variant ret;
			Variant test = get(i);
			Variant::evaluate(Variant::OP_LESS, test, minval, ret, valid);
			if (!valid) {
				return Variant(); //not a valid comparison
			}
			if (bool(ret)) {
				//is less
				minval = test;
			}
		}
	}
	return minval;
}

Variant Array::max() const {

	Variant maxval;
	for (int i = 0; i < size(); i++) {
		if (i == 0) {
			maxval = get(i);
		} else {
			bool valid;
			Variant ret;
			Variant test = get(i);
			Variant::evaluate(Variant::OP_GREATER, test, maxval, ret, valid);
			if (!valid) {
				return Variant(); //not a valid comparison
			}
			if (bool(ret)) {
				//is less
				maxval = test;
			}
		}
	}
	return maxval;
}

const void *Array::id() const {
	return _p->array.ptr();
}

Array::Array(const Array &p_from) {

	_p = NULL;
	_ref(p_from);
}

Array::Array() {

	_p = memnew(ArrayPrivate);
	_p->refcount.init();
}
Array::~Array() {

	_unref();
}

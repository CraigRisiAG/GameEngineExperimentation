

/**
 * @class StringName
 * @brief Optimized string class for fast comparisons using interned strings.
 *
 * StringName implements a string interning system where identical strings share the same
 * internal data pointer. This allows O(1) equality comparisons using pointer comparison
 * instead of character-by-character comparison.
 *
 * The class uses a hash table with 2^12 (4096) buckets to store unique strings. Each
 * string is stored as a _Data structure containing:
 * - Reference counting for automatic memory management
 * - C-string pointer for constant strings
 * - String object for dynamic strings
 * - Hash value for quick lookups
 * - Doubly-linked list pointers for collision handling
 *
 * @note All StringName instances are thread-safe through mutex protection during
 * construction and destruction. The comparison operators are lock-free O(1) operations.
 *
 * @example
 * StringName name1("MyString");
 * StringName name2("MyString");
 * if (name1 == name2) { }  // O(1) pointer comparison, very fast
 *
 * @see String - For mutable string operations
 * @see StaticCString - For compile-time constant string optimization
 */
#ifndef STRING_NAME_H
#define STRING_NAME_H

#include "core/os/mutex.h"
#include "core/safe_refcount.h"
#include "core/ustring.h"

struct StaticCString {

	const char *ptr;
	static StaticCString create(const char *p_ptr);
};

class StringName {

	enum {

		STRING_TABLE_BITS = 12,
		STRING_TABLE_LEN = 1 << STRING_TABLE_BITS,
		STRING_TABLE_MASK = STRING_TABLE_LEN - 1
	};

	struct _Data {
		SafeRefCount refcount;
		const char *cname;
		String name;

		String get_name() const { return cname ? String(cname) : name; }
		int idx;
		uint32_t hash;
		_Data *prev;
		_Data *next;
		_Data() {
			cname = NULL;
			next = prev = NULL;
			idx = 0;
			hash = 0;
		}
	};

	static _Data *_table[STRING_TABLE_LEN];

	_Data *_data;

	union _HashUnion {

		_Data *ptr;
		uint32_t hash;
	};

	void unref();
	friend void register_core_types();
	friend void unregister_core_types();

	static Mutex mutex;
	static void setup();
	static void cleanup();
	static bool configured;

	StringName(_Data *p_data) { _data = p_data; }

public:
	operator const void *() const { return (_data && (_data->cname || !_data->name.empty())) ? (void *)1 : 0; }

	bool operator==(const String &p_name) const;
	bool operator==(const char *p_name) const;
	bool operator!=(const String &p_name) const;
	_FORCE_INLINE_ bool operator<(const StringName &p_name) const {

		return _data < p_name._data;
	}
	_FORCE_INLINE_ bool operator==(const StringName &p_name) const {
		// the real magic of all this mess happens here.
		// this is why path comparisons are very fast
		return _data == p_name._data;
	}
	_FORCE_INLINE_ uint32_t hash() const {

		if (_data)
			return _data->hash;
		else
			return 0;
	}
	_FORCE_INLINE_ const void *data_unique_pointer() const {
		return (void *)_data;
	}
	bool operator!=(const StringName &p_name) const;

	_FORCE_INLINE_ operator String() const {

		if (_data) {
			if (_data->cname)
				return String(_data->cname);
			else
				return _data->name;
		}

		return String();
	}

	static StringName search(const char *p_name);
	static StringName search(const CharType *p_name);
	static StringName search(const String &p_name);

	struct AlphCompare {

		_FORCE_INLINE_ bool operator()(const StringName &l, const StringName &r) const {

			const char *l_cname = l._data ? l._data->cname : "";
			const char *r_cname = r._data ? r._data->cname : "";

			if (l_cname) {

				if (r_cname)
					return is_str_less(l_cname, r_cname);
				else
					return is_str_less(l_cname, r._data->name.ptr());
			} else {

				if (r_cname)
					return is_str_less(l._data->name.ptr(), r_cname);
				else
					return is_str_less(l._data->name.ptr(), r._data->name.ptr());
			}
		}
	};

	void operator=(const StringName &p_name);
	StringName(const char *p_name);
	StringName(const StringName &p_name);
	StringName(const String &p_name);
	StringName(const StaticCString &p_static_string);
	StringName();
	~StringName();
};

StringName _scs_create(const char *p_chr);

#endif // STRING_NAME_H

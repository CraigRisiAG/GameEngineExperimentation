

/**
 * @class StringBuilder
 * @brief A utility class for efficiently concatenating multiple strings.
 * 
 * The StringBuilder class provides an efficient way to build strings by appending
 * multiple String or C-string fragments without creating intermediate copies.
 * It maintains separate storage for Godot Strings and C-strings, tracking their
 * total length for optimized final string construction.
 * 
 * @note Internally distinguishes between Godot String objects (marked as -1) and
 *       C strings (marked with their index) for efficient memory management.
 * 
 * @example
 *     StringBuilder sb;
 *     sb.append("Hello").append(" ").append("World");
 *     String result = sb.as_string();
 */
#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include "core/ustring.h"

#include "core/vector.h"

class StringBuilder {

	uint32_t string_length;

	Vector<String> strings;
	Vector<const char *> c_strings;

	// -1 means it's a Godot String
	// a natural number means C string.
	Vector<int32_t> appended_strings;

public:
	StringBuilder &append(const String &p_string);
	StringBuilder &append(const char *p_cstring);

	_FORCE_INLINE_ StringBuilder &operator+(const String &p_string) {
		return append(p_string);
	}

	_FORCE_INLINE_ StringBuilder &operator+(const char *p_cstring) {
		return append(p_cstring);
	}

	_FORCE_INLINE_ void operator+=(const String &p_string) {
		append(p_string);
	}

	_FORCE_INLINE_ void operator+=(const char *p_cstring) {
		append(p_cstring);
	}

	_FORCE_INLINE_ int num_strings_appended() const {
		return appended_strings.size();
	}

	_FORCE_INLINE_ uint32_t get_string_length() const {
		return string_length;
	}

	String as_string() const;

	_FORCE_INLINE_ operator String() const {
		return as_string();
	}

	StringBuilder() {
		string_length = 0;
	}
};

#endif // STRING_BUILDER_H

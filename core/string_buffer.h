

/// @class StringBuffer
/// @brief A flexible string buffer with small string optimization (SSO).
///
/// StringBuffer provides an efficient way to build strings by combining small string
/// optimization with dynamic allocation. Strings up to SHORT_BUFFER_SIZE characters
/// are stored in a stack-allocated buffer, while larger strings are dynamically allocated.
///
/// @tparam SHORT_BUFFER_SIZE The size of the inline buffer (default: 64 characters).
///
/// @details
/// The class manages two storage locations:
/// - short_buffer: Stack-allocated buffer for strings smaller than SHORT_BUFFER_SIZE
/// - buffer: Dynamically allocated String for larger content
///
/// This approach minimizes heap allocations for typical small strings while maintaining
/// scalability for larger content.
///
/// @example
/// StringBuffer<> sb;
/// sb.append("Hello").append(" ").append("World");
/// String result = sb.as_string();

/// @fn StringBuffer& StringBuffer::append(CharType p_char)
/// @brief Appends a single character to the buffer.
/// @param p_char The character to append.
/// @return Reference to this StringBuffer for method chaining.

/// @fn StringBuffer& StringBuffer::append(const String& p_string)
/// @brief Appends a String object to the buffer.
/// @param p_string The String to append.
/// @return Reference to this StringBuffer for method chaining.

/// @fn StringBuffer& StringBuffer::append(const char* p_str)
/// @brief Appends a C-style null-terminated string to the buffer.
/// @param p_str Pointer to the null-terminated string to append.
/// @return Reference to this StringBuffer for method chaining.

/// @fn StringBuffer& StringBuffer::append(const CharType* p_str, int p_clip_to_len)
/// @brief Appends a CharType string with optional length limit.
/// @param p_str Pointer to the string to append.
/// @param p_clip_to_len Maximum characters to append (-1 for unlimited).
/// @return Reference to this StringBuffer for method chaining.

/// @fn StringBuffer& StringBuffer::reserve(int p_size)
/// @brief Reserves capacity for at least p_size characters.
/// @param p_size The desired minimum capacity.
/// @return Reference to this StringBuffer for method chaining.
/// @details Promotes to dynamic allocation if needed, copying existing content.

/// @fn int StringBuffer::length() const
/// @brief Returns the current number of characters in the buffer.
/// @return The length of the accumulated string.

/// @fn String StringBuffer::as_string()
/// @brief Converts the buffer contents to a String object.
/// @return A String containing the accumulated data.

/// @fn double StringBuffer::as_double()
/// @brief Converts the buffer contents to a double value.
/// @return The buffer content parsed as a double.

/// @fn int64_t StringBuffer::as_int()
/// @brief Converts the buffer contents to a 64-bit integer.
/// @return The buffer content parsed as an int64_t.

/// @fn operator String()
/// @brief Implicit conversion operator to String.
/// @return A String containing the buffer contents.
#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include "core/ustring.h"

template <int SHORT_BUFFER_SIZE = 64>
class StringBuffer {

	CharType short_buffer[SHORT_BUFFER_SIZE];
	String buffer;
	int string_length;

	_FORCE_INLINE_ CharType *current_buffer_ptr() {
		return static_cast<String &>(buffer).empty() ? short_buffer : buffer.ptrw();
	}

public:
	StringBuffer &append(CharType p_char);
	StringBuffer &append(const String &p_string);
	StringBuffer &append(const char *p_str);
	StringBuffer &append(const CharType *p_str, int p_clip_to_len = -1);

	_FORCE_INLINE_ void operator+=(CharType p_char) {
		append(p_char);
	}

	_FORCE_INLINE_ void operator+=(const String &p_string) {
		append(p_string);
	}

	_FORCE_INLINE_ void operator+=(const char *p_str) {
		append(p_str);
	}

	_FORCE_INLINE_ void operator+=(const CharType *p_str) {
		append(p_str);
	}

	StringBuffer &reserve(int p_size);

	int length() const;

	String as_string();

	double as_double();
	int64_t as_int();

	_FORCE_INLINE_ operator String() {
		return as_string();
	}

	StringBuffer() {
		string_length = 0;
	}
};

template <int SHORT_BUFFER_SIZE>
StringBuffer<SHORT_BUFFER_SIZE> &StringBuffer<SHORT_BUFFER_SIZE>::append(CharType p_char) {
	reserve(string_length + 2);
	current_buffer_ptr()[string_length++] = p_char;
	return *this;
}

template <int SHORT_BUFFER_SIZE>
StringBuffer<SHORT_BUFFER_SIZE> &StringBuffer<SHORT_BUFFER_SIZE>::append(const String &p_string) {
	return append(p_string.c_str());
}

template <int SHORT_BUFFER_SIZE>
StringBuffer<SHORT_BUFFER_SIZE> &StringBuffer<SHORT_BUFFER_SIZE>::append(const char *p_str) {
	int len = strlen(p_str);
	reserve(string_length + len + 1);

	CharType *buf = current_buffer_ptr();
	for (const char *c_ptr = p_str; *c_ptr; ++c_ptr) {
		buf[string_length++] = *c_ptr;
	}
	return *this;
}

template <int SHORT_BUFFER_SIZE>
StringBuffer<SHORT_BUFFER_SIZE> &StringBuffer<SHORT_BUFFER_SIZE>::append(const CharType *p_str, int p_clip_to_len) {
	int len = 0;
	while ((p_clip_to_len < 0 || len < p_clip_to_len) && p_str[len]) {
		++len;
	}
	reserve(string_length + len + 1);
	memcpy(&(current_buffer_ptr()[string_length]), p_str, len * sizeof(CharType));
	string_length += len;

	return *this;
}

template <int SHORT_BUFFER_SIZE>
StringBuffer<SHORT_BUFFER_SIZE> &StringBuffer<SHORT_BUFFER_SIZE>::reserve(int p_size) {
	if (p_size < SHORT_BUFFER_SIZE || p_size < buffer.size())
		return *this;

	bool need_copy = string_length > 0 && buffer.empty();
	buffer.resize(next_power_of_2(p_size));
	if (need_copy) {
		memcpy(buffer.ptrw(), short_buffer, string_length * sizeof(CharType));
	}

	return *this;
}

template <int SHORT_BUFFER_SIZE>
int StringBuffer<SHORT_BUFFER_SIZE>::length() const {
	return string_length;
}

template <int SHORT_BUFFER_SIZE>
String StringBuffer<SHORT_BUFFER_SIZE>::as_string() {
	current_buffer_ptr()[string_length] = '\0';
	if (buffer.empty()) {
		return String(short_buffer);
	} else {
		buffer.resize(string_length + 1);
		return buffer;
	}
}

template <int SHORT_BUFFER_SIZE>
double StringBuffer<SHORT_BUFFER_SIZE>::as_double() {
	current_buffer_ptr()[string_length] = '\0';
	return String::to_double(current_buffer_ptr());
}

template <int SHORT_BUFFER_SIZE>
int64_t StringBuffer<SHORT_BUFFER_SIZE>::as_int() {
	current_buffer_ptr()[string_length] = '\0';
	return String::to_int(current_buffer_ptr());
}

#endif // STRING_BUFFER_H

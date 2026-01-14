

/**
 * @class StringBuilder
 * @brief A utility class for efficiently concatenating strings.
 * 
 * This class provides methods to append both Godot String objects and C-style
 * null-terminated strings, then combine them into a single String result.
 * It tracks the total length and maintains separate lists for each string type
 * to optimize memory usage and concatenation performance.
 */

/**
 * @brief Appends a Godot String to the builder.
 * 
 * @param p_string The String object to append. If empty, the operation is ignored.
 * @return A reference to this StringBuilder instance for method chaining.
 */
StringBuilder &append(const String &p_string);

/**
 * @brief Appends a C-style null-terminated string to the builder.
 * 
 * @param p_cstring The null-terminated C string to append.
 * @return A reference to this StringBuilder instance for method chaining.
 */
StringBuilder &append(const char *p_cstring);

/**
 * @brief Constructs and returns the final concatenated string.
 * 
 * Combines all appended strings (both Godot Strings and C strings) into a single
 * String object in the order they were appended. Allocates temporary buffer memory
 * which is freed after the String is constructed.
 * 
 * @return A new String containing all appended content. Returns an empty string if
 *         no strings have been appended.
 */
String as_string() const;
#include "string_builder.h"

#include <string.h>

StringBuilder &StringBuilder::append(const String &p_string) {

	if (p_string == String())
		return *this;

	strings.push_back(p_string);
	appended_strings.push_back(-1);

	string_length += p_string.length();

	return *this;
}

StringBuilder &StringBuilder::append(const char *p_cstring) {

	int32_t len = strlen(p_cstring);

	c_strings.push_back(p_cstring);
	appended_strings.push_back(len);

	string_length += len;

	return *this;
}

String StringBuilder::as_string() const {

	if (string_length == 0)
		return "";

	CharType *buffer = memnew_arr(CharType, string_length);

	int current_position = 0;

	int godot_string_elem = 0;
	int c_string_elem = 0;

	for (int i = 0; i < appended_strings.size(); i++) {
		if (appended_strings[i] == -1) {
			// Godot string
			const String &s = strings[godot_string_elem];

			memcpy(buffer + current_position, s.ptr(), s.length() * sizeof(CharType));

			current_position += s.length();

			godot_string_elem++;
		} else {

			const char *s = c_strings[c_string_elem];

			for (int32_t j = 0; j < appended_strings[i]; j++) {
				buffer[current_position + j] = s[j];
			}

			current_position += appended_strings[i];

			c_string_elem++;
		}
	}

	String final_string = String(buffer, string_length);

	memdelete_arr(buffer);

	return final_string;
}



/**
 * @class JSON
 * @brief A utility class for parsing and printing JSON data.
 *
 * This class provides static methods to serialize Variant objects to JSON strings
 * and deserialize JSON strings back into Variant objects. It handles all standard
 * JSON data types including objects, arrays, strings, numbers, and booleans.
 *
 * @details
 * The class uses a tokenizer-based approach to parse JSON strings. It validates
 * the JSON structure and provides detailed error reporting including line numbers
 * and error descriptions.
 *
 * @note All methods are static; this class should not be instantiated.
 *
 * @enum TokenType
 * Enumeration of all valid JSON tokens recognized by the parser.
 * - TK_CURLY_BRACKET_OPEN: Opening brace '{'
 * - TK_CURLY_BRACKET_CLOSE: Closing brace '}'
 * - TK_BRACKET_OPEN: Opening bracket '['
 * - TK_BRACKET_CLOSE: Closing bracket ']'
 * - TK_IDENTIFIER: Unquoted identifier (null, true, false)
 * - TK_STRING: Quoted string literal
 * - TK_NUMBER: Numeric literal
 * - TK_COLON: Colon separator ':'
 * - TK_COMMA: Comma separator ','
 * - TK_EOF: End of file marker
 *
 * @enum Expecting
 * Enumeration of parser states indicating what token is expected next.
 *
 * @struct Token
 * Represents a single parsed JSON token.
 * - type: The TokenType of this token
 * - value: The parsed value of the token
 */
#ifndef JSON_H
#define JSON_H

#include "core/variant.h"

class JSON {

	enum TokenType {
		TK_CURLY_BRACKET_OPEN,
		TK_CURLY_BRACKET_CLOSE,
		TK_BRACKET_OPEN,
		TK_BRACKET_CLOSE,
		TK_IDENTIFIER,
		TK_STRING,
		TK_NUMBER,
		TK_COLON,
		TK_COMMA,
		TK_EOF,
		TK_MAX
	};

	enum Expecting {

		EXPECT_OBJECT,
		EXPECT_OBJECT_KEY,
		EXPECT_COLON,
		EXPECT_OBJECT_VALUE,
	};

	struct Token {

		TokenType type;
		Variant value;
	};

	static const char *tk_name[TK_MAX];

	static String _print_var(const Variant &p_var, const String &p_indent, int p_cur_indent, bool p_sort_keys);

	static Error _get_token(const CharType *p_str, int &index, int p_len, Token &r_token, int &line, String &r_err_str);
	static Error _parse_value(Variant &value, Token &token, const CharType *p_str, int &index, int p_len, int &line, String &r_err_str);
	static Error _parse_array(Array &array, const CharType *p_str, int &index, int p_len, int &line, String &r_err_str);
	static Error _parse_object(Dictionary &object, const CharType *p_str, int &index, int p_len, int &line, String &r_err_str);

public:
	static String print(const Variant &p_var, const String &p_indent = "", bool p_sort_keys = true);
	static Error parse(const String &p_json, Variant &r_ret, String &r_err_str, int &r_err_line);
};

#endif // JSON_H

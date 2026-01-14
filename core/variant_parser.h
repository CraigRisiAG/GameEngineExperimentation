

/**
 * @class VariantParser
 * @brief Parser for deserializing Variant data from streams.
 * 
 * Provides functionality to parse Variant objects from different stream sources
 * (files and strings) into structured data formats including dictionaries, arrays,
 * and tagged objects. Supports custom resource parsing through callback functions.
 * 
 * @nested Stream
 * Abstract base class representing a character input stream.
 * Implementations must define character reading and EOF detection.
 * 
 * @nested StreamFile
 * Stream implementation for reading from file resources.
 * 
 * @nested StreamString
 * Stream implementation for reading from in-memory strings.
 * 
 * @nested ResourceParser
 * Callback structure for handling custom resource deserialization.
 * @field userdata Opaque pointer passed to parsing functions
 * @field func Main resource parsing function
 * @field ext_func External resource parsing function
 * @field sub_func Sub-resource parsing function
 * 
 * @enum TokenType
 * Enumeration of all recognized token types in the variant format.
 * 
 * @enum Expecting
 * Parser state expectations during object/dictionary parsing.
 * 
 * @nested Token
 * Represents a single parsed token with its type and value.
 * 
 * @nested Tag
 * Represents a named object with associated field data.
 */

/**
 * @class VariantWriter
 * @brief Serializer for converting Variant data to string representation.
 * 
 * Provides functionality to serialize Variant objects to strings with support
 * for custom resource encoding through callback functions.
 * 
 * @typedef StoreStringFunc
 * Callback for storing serialized string output.
 * 
 * @typedef EncodeResourceFunc
 * Callback for encoding resource references as strings.
 */
#ifndef VARIANT_PARSER_H
#define VARIANT_PARSER_H

#include "core/os/file_access.h"
#include "core/resource.h"
#include "core/variant.h"

class VariantParser {
public:
	struct Stream {

		virtual CharType get_char() = 0;
		virtual bool is_utf8() const = 0;
		virtual bool is_eof() const = 0;

		CharType saved;

		Stream() :
				saved(0) {}
		virtual ~Stream() {}
	};

	struct StreamFile : public Stream {

		FileAccess *f;

		virtual CharType get_char();
		virtual bool is_utf8() const;
		virtual bool is_eof() const;

		StreamFile() { f = NULL; }
	};

	struct StreamString : public Stream {

		String s;
		int pos;

		virtual CharType get_char();
		virtual bool is_utf8() const;
		virtual bool is_eof() const;

		StreamString() { pos = 0; }
	};

	typedef Error (*ParseResourceFunc)(void *p_self, Stream *p_stream, Ref<Resource> &r_res, int &line, String &r_err_str);

	struct ResourceParser {

		void *userdata = nullptr;
		ParseResourceFunc func;
		ParseResourceFunc ext_func;
		ParseResourceFunc sub_func;
	};

	enum TokenType {
		TK_CURLY_BRACKET_OPEN,
		TK_CURLY_BRACKET_CLOSE,
		TK_BRACKET_OPEN,
		TK_BRACKET_CLOSE,
		TK_PARENTHESIS_OPEN,
		TK_PARENTHESIS_CLOSE,
		TK_IDENTIFIER,
		TK_STRING,
		TK_STRING_NAME,
		TK_NUMBER,
		TK_COLOR,
		TK_COLON,
		TK_COMMA,
		TK_PERIOD,
		TK_EQUAL,
		TK_EOF,
		TK_ERROR,
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

	struct Tag {

		String name;
		Map<String, Variant> fields;
	};

private:
	static const char *tk_name[TK_MAX];

	template <class T>
	static Error _parse_construct(Stream *p_stream, Vector<T> &r_construct, int &line, String &r_err_str);
	static Error _parse_enginecfg(Stream *p_stream, Vector<String> &strings, int &line, String &r_err_str);
	static Error _parse_dictionary(Dictionary &object, Stream *p_stream, int &line, String &r_err_str, ResourceParser *p_res_parser = NULL);
	static Error _parse_array(Array &array, Stream *p_stream, int &line, String &r_err_str, ResourceParser *p_res_parser = NULL);
	static Error _parse_tag(Token &token, Stream *p_stream, int &line, String &r_err_str, Tag &r_tag, ResourceParser *p_res_parser = NULL, bool p_simple_tag = false);

public:
	static Error parse_tag(Stream *p_stream, int &line, String &r_err_str, Tag &r_tag, ResourceParser *p_res_parser = NULL, bool p_simple_tag = false);
	static Error parse_tag_assign_eof(Stream *p_stream, int &line, String &r_err_str, Tag &r_tag, String &r_assign, Variant &r_value, ResourceParser *p_res_parser = NULL, bool p_simple_tag = false);

	static Error parse_value(Token &token, Variant &value, Stream *p_stream, int &line, String &r_err_str, ResourceParser *p_res_parser = NULL);
	static Error get_token(Stream *p_stream, Token &r_token, int &line, String &r_err_str);
	static Error parse(Stream *p_stream, Variant &r_ret, String &r_err_str, int &r_err_line, ResourceParser *p_res_parser = NULL);
};

class VariantWriter {
public:
	typedef Error (*StoreStringFunc)(void *ud, const String &p_string);
	typedef String (*EncodeResourceFunc)(void *ud, const RES &p_resource);

	static Error write(const Variant &p_variant, StoreStringFunc p_store_string_func, void *p_store_string_ud, EncodeResourceFunc p_encode_res_func, void *p_encode_res_ud);
	static Error write_to_string(const Variant &p_variant, String &r_string, EncodeResourceFunc p_encode_res_func = NULL, void *p_encode_res_ud = NULL);
};

#endif // VARIANT_PARSER_H

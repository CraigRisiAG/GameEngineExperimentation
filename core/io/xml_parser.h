

/**
 * @class XMLParser
 * @brief XML parser for reading and processing XML documents.
 * 
 * XMLParser is a lightweight XML parser based on irrXML, designed primarily for
 * compatibility with Collada file loading. It provides functionality to read XML files,
 * parse nodes, and extract node attributes and data.
 * 
 * @note Based on irrXML (see their zlib license).
 * 
 * @section Usage
 * 1. Call open() or open_buffer() to load XML data
 * 2. Call read() to parse the next node
 * 3. Use getter methods to access node properties (name, type, attributes, etc.)
 * 4. Call close() when finished
 * 
 * @section Supported Formats
 * - ASCII
 * - UTF-8
 * - UTF-16 (Big Endian and Little Endian)
 * - UTF-32 (Big Endian and Little Endian)
 */

/**
 * @enum SourceFormat
 * @brief Enumeration of all supported source text file formats.
 */

/**
 * @enum NodeType
 * @brief Enumeration of XML node types that can be encountered during parsing.
 */

/**
 * @brief Reads and parses the next node in the XML document.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Returns the type of the current node.
 * @return The NodeType of the current node.
 */

/**
 * @brief Returns the name of the current node.
 * @return String containing the node name.
 */

/**
 * @brief Returns the text data contained within the current node.
 * @return String containing the node data.
 */

/**
 * @brief Returns the byte offset of the current node in the XML file.
 * @return The offset position as uint64_t.
 */

/**
 * @brief Returns the number of attributes in the current node.
 * @return Integer count of attributes.
 */

/**
 * @brief Returns the name of an attribute by index.
 * @param p_idx The attribute index.
 * @return String containing the attribute name.
 */

/**
 * @brief Returns the value of an attribute by index.
 * @param p_idx The attribute index.
 * @return String containing the attribute value.
 */

/**
 * @brief Checks if the current node has a specific attribute.
 * @param p_name The attribute name to search for.
 * @return True if the attribute exists, false otherwise.
 */

/**
 * @brief Returns the value of an attribute by name.
 * @param p_name The attribute name.
 * @return String containing the attribute value.
 */

/**
 * @brief Returns the value of an attribute by name without printing errors if not found.
 * @param p_name The attribute name.
 * @return String containing the attribute value, or empty string if not found.
 */

/**
 * @brief Checks if the current element node is self-closing (empty).
 * @return True if the element is empty, false otherwise.
 */

/**
 * @brief Returns the current line number in the XML document.
 * @return The line number as an integer.
 */

/**
 * @brief Skips the current XML section and advances to the next sibling node.
 */

/**
 * @brief Seeks to a specific position in the XML document.
 * @param p_pos The byte offset to seek to.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Opens and loads an XML file from the specified file path.
 * @param p_path The file path to the XML document.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Opens and parses XML data from a buffer.
 * @param p_buffer A vector of bytes containing the XML data.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Closes the XML document and frees associated resources.
 */

/**
 * @brief Constructs a new XMLParser instance.
 */

/**
 * @brief Destructs the XMLParser instance, freeing all resources.
 */
#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "core/os/file_access.h"
#include "core/reference.h"
#include "core/ustring.h"
#include "core/vector.h"

/*
  Based on irrXML (see their zlib license). Added mainly for compatibility with their Collada loader.
*/

class XMLParser : public Reference {

	GDCLASS(XMLParser, Reference);

public:
	//! Enumeration of all supported source text file formats
	enum SourceFormat {
		SOURCE_ASCII,
		SOURCE_UTF8,
		SOURCE_UTF16_BE,
		SOURCE_UTF16_LE,
		SOURCE_UTF32_BE,
		SOURCE_UTF32_LE
	};

	enum NodeType {
		NODE_NONE,
		NODE_ELEMENT,
		NODE_ELEMENT_END,
		NODE_TEXT,
		NODE_COMMENT,
		NODE_CDATA,
		NODE_UNKNOWN
	};

private:
	char *data;
	char *P;
	uint64_t length;
	void unescape(String &p_str);
	Vector<String> special_characters;
	String node_name;
	bool node_empty;
	NodeType node_type;
	uint64_t node_offset;

	struct Attribute {
		String name;
		String value;
	};

	Vector<Attribute> attributes;

	String _replace_special_characters(const String &origstr);
	bool _set_text(char *start, char *end);
	void _parse_closing_xml_element();
	void _ignore_definition();
	bool _parse_cdata();
	void _parse_comment();
	void _parse_opening_xml_element();
	void _parse_current_node();

	static void _bind_methods();

public:
	Error read();
	NodeType get_node_type();
	String get_node_name() const;
	String get_node_data() const;
	uint64_t get_node_offset() const;
	int get_attribute_count() const;
	String get_attribute_name(int p_idx) const;
	String get_attribute_value(int p_idx) const;
	bool has_attribute(const String &p_name) const;
	String get_attribute_value(const String &p_name) const;
	String get_attribute_value_safe(const String &p_name) const; // do not print error if doesn't exist
	bool is_empty() const;
	int get_current_line() const;

	void skip_section();
	Error seek(uint64_t p_pos);

	Error open(const String &p_path);
	Error open_buffer(const Vector<uint8_t> &p_buffer);

	void close();

	XMLParser();
	~XMLParser();
};

#endif // XML_PARSER_H

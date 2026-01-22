


/// @class DocData
/// @brief Manages and stores documentation data for classes, methods, properties, and constants.
///
/// DocData is responsible for loading, merging, and saving documentation information for engine classes.
/// It provides structures to represent class documentation including methods, properties, constants, and signals.
///
/// @struct ArgumentDoc
/// @brief Represents documentation for a method or function argument.
/// @member name - The name of the argument
/// @member type - The data type of the argument
/// @member enumeration - The enumeration type if applicable
/// @member default_value - The default value of the argument if any
///
/// @struct MethodDoc
/// @brief Represents documentation for a class method or function.
/// @member name - The name of the method
/// @member return_type - The return type of the method
/// @member return_enum - The enumeration type of the return value if applicable
/// @member qualifiers - Method qualifiers (e.g., const, static)
/// @member description - Detailed description of the method
/// @member arguments - Vector of arguments the method accepts
///
/// @struct ConstantDoc
/// @brief Represents documentation for a class constant.
/// @member name - The name of the constant
/// @member value - The constant's value
/// @member enumeration - The enumeration group it belongs to
/// @member description - Description of the constant's purpose
///
/// @struct PropertyDoc
/// @brief Represents documentation for a class property.
/// @member name - The property name
/// @member type - The property's data type
/// @member enumeration - Enumeration type if applicable
/// @member description - Description of the property
/// @member setter - Name of the setter method
/// @member getter - Name of the getter method
/// @member default_value - The default value of the property
/// @member overridden - Whether the property is overridden in a subclass
///
/// @struct ClassDoc
/// @brief Represents complete documentation for a class.
/// @member name - The class name
/// @member inherits - The parent class name
/// @member category - Documentation category
/// @member brief_description - Short description of the class
/// @member description - Detailed description of the class
/// @member tutorials - Vector of related tutorial references
/// @member methods - Vector of method documentation
/// @member signals - Vector of signal documentation
/// @member constants - Vector of constant documentation
/// @member properties - Vector of property documentation
/// @member theme_properties - Vector of theme property documentation
///
/// @member version - Documentation format version
/// @member class_list - Map storing ClassDoc entries indexed by class name
///
/// @fn Error load_classes(const String &p_dir)
/// @brief Loads documentation from XML files in the specified directory.
/// @param p_dir Directory path containing documentation files
/// @return Error code indicating success or failure
///
/// @fn Error save_classes(const String &p_default_path, const Map<String, String> &p_class_path)
/// @brief Saves documentation to XML files.
/// @param p_default_path Default output directory path
/// @param p_class_path Map of class names to custom output paths
/// @return Error code indicating success or failure
///
/// @fn void merge_from(const DocData &p_data)
/// @brief Merges documentation from another DocData instance into this one.
/// @param p_data The DocData to merge from
///
/// @fn void remove_from(const DocData &p_data)
/// @brief Removes documentation entries that exist in the provided DocData.
/// @param p_data The DocData containing entries to remove
///
/// @fn void generate(bool p_basic_types)
/// @brief Generates documentation for built-in types and classes.
/// @param p_basic_types If true, generates documentation for basic types
///
/// @fn Error load_compressed(const uint8_t *p_data, int p_compressed_size, int p_uncompressed_size)
/// @brief Loads documentation from compressed data.
/// @param p_data Pointer to compressed documentation data
/// @param p_compressed_size Size of the compressed data
/// @param p_uncompressed_size Expected size after decompression
/// @return Error code indicating success or failure
///
/// @fn static Error erase_classes(const String &p_dir)
/// @brief Erases documentation files from the specified directory.
/// @param p_dir Directory path containing documentation files to erase
/// @return Error code indicating success or failure
#ifndef DOC_DATA_H
#define DOC_DATA_H

#include "core/io/xml_parser.h"
#include "core/map.h"
#include "core/variant.h"

class DocData {
public:
	struct ArgumentDoc {

		String name;
		String type;
		String enumeration;
		String default_value;
	};

	struct MethodDoc {

		String name;
		String return_type;
		String return_enum;
		String qualifiers;
		String description;
		Vector<ArgumentDoc> arguments;
		bool operator<(const MethodDoc &p_md) const {
			return name < p_md.name;
		}
	};

	struct ConstantDoc {

		String name;
		String value;
		String enumeration;
		String description;
	};

	struct PropertyDoc {

		String name;
		String type;
		String enumeration;
		String description;
		String setter, getter;
		String default_value;
		bool overridden;
		bool operator<(const PropertyDoc &p_prop) const {
			return name < p_prop.name;
		}
		PropertyDoc() {
			overridden = false;
		}
	};

	struct ClassDoc {

		String name;
		String inherits;
		String category;
		String brief_description;
		String description;
		Vector<String> tutorials;
		Vector<MethodDoc> methods;
		Vector<MethodDoc> signals;
		Vector<ConstantDoc> constants;
		Vector<PropertyDoc> properties;
		Vector<PropertyDoc> theme_properties;
	};

	String version;

	Map<String, ClassDoc> class_list;
	Error _load(Ref<XMLParser> parser);

public:
	void merge_from(const DocData &p_data);
	void remove_from(const DocData &p_data);
	void generate(bool p_basic_types = false);
	Error load_classes(const String &p_dir);
	static Error erase_classes(const String &p_dir);
	Error save_classes(const String &p_default_path, const Map<String, String> &p_class_path);

	Error load_compressed(const uint8_t *p_data, int p_compressed_size, int p_uncompressed_size);
};

#endif // DOC_DATA_H



/// @class ConfigFile
/// @brief A utility class for reading and writing configuration files.
///
/// ConfigFile provides functionality to manage key-value pairs organized into sections,
/// similar to INI file format. It supports loading from and saving to files, with optional
/// encryption capabilities.
///
/// The configuration data is stored internally as a nested map structure where each section
/// contains a map of keys to variant values. This allows flexible storage of various data types.
///
/// @note ConfigFile extends the Reference class and is designed to be used with Godot's
/// reference counting system.
///
/// ## Features:
/// - Load and save configuration files from disk
/// - Parse configuration from string data
/// - Encrypt and decrypt configuration files
/// - Organize settings into named sections
/// - Store values of any Variant type
/// - Query sections and keys with existence checks
///
/// ## Usage Example:
/// @code
/// var config = ConfigFile.new()
/// config.set_value("player", "name", "John")
/// config.set_value("player", "level", 10)
/// config.save("user://config.cfg")
///
/// var config2 = ConfigFile.new()
/// config2.load("user://config.cfg")
/// var player_name = config2.get_value("player", "name", "Unknown")
/// @endcode
#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include "core/ordered_hash_map.h"
#include "core/os/file_access.h"
#include "core/reference.h"
#include "core/variant_parser.h"

class ConfigFile : public Reference {

	GDCLASS(ConfigFile, Reference);

	OrderedHashMap<String, OrderedHashMap<String, Variant>> values;

	PackedStringArray _get_sections() const;
	PackedStringArray _get_section_keys(const String &p_section) const;
	Error _internal_load(const String &p_path, FileAccess *f);
	Error _internal_save(FileAccess *file);

	Error _parse(const String &p_path, VariantParser::Stream *p_stream);

protected:
	static void _bind_methods();

public:
	void set_value(const String &p_section, const String &p_key, const Variant &p_value);
	Variant get_value(const String &p_section, const String &p_key, Variant p_default = Variant()) const;

	bool has_section(const String &p_section) const;
	bool has_section_key(const String &p_section, const String &p_key) const;

	void get_sections(List<String> *r_sections) const;
	void get_section_keys(const String &p_section, List<String> *r_keys) const;

	void erase_section(const String &p_section);
	void erase_section_key(const String &p_section, const String &p_key);

	Error save(const String &p_path);
	Error load(const String &p_path);
	Error parse(const String &p_data);

	Error load_encrypted(const String &p_path, const Vector<uint8_t> &p_key);
	Error load_encrypted_pass(const String &p_path, const String &p_pass);

	Error save_encrypted(const String &p_path, const Vector<uint8_t> &p_key);
	Error save_encrypted_pass(const String &p_path, const String &p_pass);
};

#endif // CONFIG_FILE_H

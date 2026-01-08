

/**
 * @file config_file.cpp
 * @brief Configuration file management implementation
 * 
 * Provides functionality for reading, writing, and manipulating configuration files
 * in INI-like format with support for encryption.
 */

/**
 * @brief Retrieves all section names as a packed string array
 * @return PackedStringArray containing all section names
 */

/**
 * @brief Retrieves all keys in a given section as a packed string array
 * @param p_section The section name to retrieve keys from
 * @return PackedStringArray containing all keys in the section
 */

/**
 * @brief Sets a configuration value
 * @param p_section The section name
 * @param p_key The key name
 * @param p_value The value to set (use Variant::NIL to erase)
 */

/**
 * @brief Retrieves a configuration value
 * @param p_section The section name
 * @param p_key The key name
 * @param p_default The default value if key is not found
 * @return The configuration value or default if not found
 */

/**
 * @brief Checks if a section exists
 * @param p_section The section name
 * @return true if section exists, false otherwise
 */

/**
 * @brief Checks if a key exists in a section
 * @param p_section The section name
 * @param p_key The key name
 * @return true if key exists in section, false otherwise
 */

/**
 * @brief Populates a list with all section names
 * @param r_sections Pointer to list to populate with section names
 */

/**
 * @brief Populates a list with all keys in a section
 * @param p_section The section name
 * @param r_keys Pointer to list to populate with key names
 */

/**
 * @brief Erases an entire section
 * @param p_section The section name to erase
 */

/**
 * @brief Erases a key from a section
 * @param p_section The section name
 * @param p_key The key name to erase
 */

/**
 * @brief Saves configuration to an unencrypted file
 * @param p_path The file path
 * @return Error code (OK on success)
 */

/**
 * @brief Saves configuration to an AES256-encrypted file using a key
 * @param p_path The file path
 * @param p_key The encryption key as a byte vector
 * @return Error code (OK on success)
 */

/**
 * @brief Saves configuration to an AES256-encrypted file using a password
 * @param p_path The file path
 * @param p_pass The encryption password
 * @return Error code (OK on success)
 */

/**
 * @brief Internal method to write configuration data to file
 * @param file Pointer to FileAccess object for writing
 * @return Error code (OK on success)
 */

/**
 * @brief Loads configuration from an unencrypted file
 * @param p_path The file path
 * @return Error code (OK on success)
 */

/**
 * @brief Loads configuration from an AES256-encrypted file using a key
 * @param p_path The file path
 * @param p_key The decryption key as a byte vector
 * @return Error code (OK on success)
 */

/**
 * @brief Loads configuration from an AES256-encrypted file using a password
 * @param p_path The file path
 * @param p_pass The decryption password
 * @return Error code (OK on success)
 */

/**
 * @brief Internal method to read configuration data from file
 * @param p_path The file path for error reporting
 * @param f Pointer to FileAccess object for reading
 * @return Error code (OK on success)
 */

/**
 * @brief Parses configuration from a string
 * @param p_data The configuration data as a string
 * @return Error code (OK on success)
 */

/**
 * @brief Internal parsing method for configuration data
 * @param p_path The data source path for error reporting
 * @param p_stream Pointer to variant parser stream
 * @return Error code (OK on success)
 */

/**
 * @brief Binds configuration file methods to the scripting engine
 */
#include "config_file.h"

#include "core/io/file_access_encrypted.h"
#include "core/os/keyboard.h"
#include "core/variant_parser.h"

PackedStringArray ConfigFile::_get_sections() const {

	List<String> s;
	get_sections(&s);
	PackedStringArray arr;
	arr.resize(s.size());
	int idx = 0;
	for (const List<String>::Element *E = s.front(); E; E = E->next()) {

		arr.set(idx++, E->get());
	}

	return arr;
}

PackedStringArray ConfigFile::_get_section_keys(const String &p_section) const {

	List<String> s;
	get_section_keys(p_section, &s);
	PackedStringArray arr;
	arr.resize(s.size());
	int idx = 0;
	for (const List<String>::Element *E = s.front(); E; E = E->next()) {

		arr.set(idx++, E->get());
	}

	return arr;
}

void ConfigFile::set_value(const String &p_section, const String &p_key, const Variant &p_value) {

	if (p_value.get_type() == Variant::NIL) {
		//erase
		if (!values.has(p_section))
			return; // ?
		values[p_section].erase(p_key);
		if (values[p_section].empty()) {
			values.erase(p_section);
		}

	} else {
		if (!values.has(p_section)) {
			values[p_section] = OrderedHashMap<String, Variant>();
		}

		values[p_section][p_key] = p_value;
	}
}
Variant ConfigFile::get_value(const String &p_section, const String &p_key, Variant p_default) const {

	if (!values.has(p_section) || !values[p_section].has(p_key)) {
		ERR_FAIL_COND_V_MSG(p_default.get_type() == Variant::NIL, Variant(),
				vformat("Couldn't find the given section \"%s\" and key \"%s\", and no default was given.", p_section, p_key));
		return p_default;
	}

	return values[p_section][p_key];
}

bool ConfigFile::has_section(const String &p_section) const {

	return values.has(p_section);
}
bool ConfigFile::has_section_key(const String &p_section, const String &p_key) const {

	if (!values.has(p_section))
		return false;
	return values[p_section].has(p_key);
}

void ConfigFile::get_sections(List<String> *r_sections) const {

	for (OrderedHashMap<String, OrderedHashMap<String, Variant>>::ConstElement E = values.front(); E; E = E.next()) {
		r_sections->push_back(E.key());
	}
}
void ConfigFile::get_section_keys(const String &p_section, List<String> *r_keys) const {

	ERR_FAIL_COND_MSG(!values.has(p_section), vformat("Cannot get keys from nonexistent section \"%s\".", p_section));

	for (OrderedHashMap<String, Variant>::ConstElement E = values[p_section].front(); E; E = E.next()) {
		r_keys->push_back(E.key());
	}
}

void ConfigFile::erase_section(const String &p_section) {

	ERR_FAIL_COND_MSG(!values.has(p_section), vformat("Cannot erase nonexistent section \"%s\".", p_section));
	values.erase(p_section);
}

void ConfigFile::erase_section_key(const String &p_section, const String &p_key) {

	ERR_FAIL_COND_MSG(!values.has(p_section), vformat("Cannot erase key \"%s\" from nonexistent section \"%s\".", p_key, p_section));
	ERR_FAIL_COND_MSG(!values[p_section].has(p_key), vformat("Cannot erase nonexistent key \"%s\" from section \"%s\".", p_key, p_section));

	values[p_section].erase(p_key);
}

Error ConfigFile::save(const String &p_path) {

	Error err;
	FileAccess *file = FileAccess::open(p_path, FileAccess::WRITE, &err);

	if (err) {
		if (file)
			memdelete(file);
		return err;
	}

	return _internal_save(file);
}

Error ConfigFile::save_encrypted(const String &p_path, const Vector<uint8_t> &p_key) {

	Error err;
	FileAccess *f = FileAccess::open(p_path, FileAccess::WRITE, &err);

	if (err)
		return err;

	FileAccessEncrypted *fae = memnew(FileAccessEncrypted);
	err = fae->open_and_parse(f, p_key, FileAccessEncrypted::MODE_WRITE_AES256);
	if (err) {
		memdelete(fae);
		memdelete(f);
		return err;
	}
	return _internal_save(fae);
}

Error ConfigFile::save_encrypted_pass(const String &p_path, const String &p_pass) {

	Error err;
	FileAccess *f = FileAccess::open(p_path, FileAccess::WRITE, &err);

	if (err)
		return err;

	FileAccessEncrypted *fae = memnew(FileAccessEncrypted);
	err = fae->open_and_parse_password(f, p_pass, FileAccessEncrypted::MODE_WRITE_AES256);
	if (err) {
		memdelete(fae);
		memdelete(f);
		return err;
	}

	return _internal_save(fae);
}

Error ConfigFile::_internal_save(FileAccess *file) {

	for (OrderedHashMap<String, OrderedHashMap<String, Variant>>::Element E = values.front(); E; E = E.next()) {

		if (E != values.front())
			file->store_string("\n");
		file->store_string("[" + E.key() + "]\n\n");

		for (OrderedHashMap<String, Variant>::Element F = E.get().front(); F; F = F.next()) {

			String vstr;
			VariantWriter::write_to_string(F.get(), vstr);
			file->store_string(F.key() + "=" + vstr + "\n");
		}
	}

	memdelete(file);

	return OK;
}

Error ConfigFile::load(const String &p_path) {

	Error err;
	FileAccess *f = FileAccess::open(p_path, FileAccess::READ, &err);

	if (!f)
		return err;

	return _internal_load(p_path, f);
}

Error ConfigFile::load_encrypted(const String &p_path, const Vector<uint8_t> &p_key) {

	Error err;
	FileAccess *f = FileAccess::open(p_path, FileAccess::READ, &err);

	if (err)
		return err;

	FileAccessEncrypted *fae = memnew(FileAccessEncrypted);
	err = fae->open_and_parse(f, p_key, FileAccessEncrypted::MODE_READ);
	if (err) {
		memdelete(fae);
		memdelete(f);
		return err;
	}
	return _internal_load(p_path, fae);
}

Error ConfigFile::load_encrypted_pass(const String &p_path, const String &p_pass) {

	Error err;
	FileAccess *f = FileAccess::open(p_path, FileAccess::READ, &err);

	if (err)
		return err;

	FileAccessEncrypted *fae = memnew(FileAccessEncrypted);
	err = fae->open_and_parse_password(f, p_pass, FileAccessEncrypted::MODE_READ);
	if (err) {
		memdelete(fae);
		memdelete(f);
		return err;
	}

	return _internal_load(p_path, fae);
}

Error ConfigFile::_internal_load(const String &p_path, FileAccess *f) {

	VariantParser::StreamFile stream;
	stream.f = f;

	Error err = _parse(p_path, &stream);

	memdelete(f);

	return err;
}

Error ConfigFile::parse(const String &p_data) {

	VariantParser::StreamString stream;
	stream.s = p_data;
	return _parse("<string>", &stream);
}

Error ConfigFile::_parse(const String &p_path, VariantParser::Stream *p_stream) {

	String assign;
	Variant value;
	VariantParser::Tag next_tag;

	int lines = 0;
	String error_text;

	String section;

	while (true) {

		assign = Variant();
		next_tag.fields.clear();
		next_tag.name = String();

		Error err = VariantParser::parse_tag_assign_eof(p_stream, lines, error_text, next_tag, assign, value, NULL, true);
		if (err == ERR_FILE_EOF) {
			return OK;
		} else if (err != OK) {
			ERR_PRINT(vformat("ConfigFile parse error at %s:%d: %s.", p_path, lines, error_text));
			return err;
		}

		if (assign != String()) {
			set_value(section, assign, value);
		} else if (next_tag.name != String()) {
			section = next_tag.name;
		}
	}

	return OK;
}

void ConfigFile::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_value", "section", "key", "value"), &ConfigFile::set_value);
	ClassDB::bind_method(D_METHOD("get_value", "section", "key", "default"), &ConfigFile::get_value, DEFVAL(Variant()));

	ClassDB::bind_method(D_METHOD("has_section", "section"), &ConfigFile::has_section);
	ClassDB::bind_method(D_METHOD("has_section_key", "section", "key"), &ConfigFile::has_section_key);

	ClassDB::bind_method(D_METHOD("get_sections"), &ConfigFile::_get_sections);
	ClassDB::bind_method(D_METHOD("get_section_keys", "section"), &ConfigFile::_get_section_keys);

	ClassDB::bind_method(D_METHOD("erase_section", "section"), &ConfigFile::erase_section);
	ClassDB::bind_method(D_METHOD("erase_section_key", "section", "key"), &ConfigFile::erase_section_key);

	ClassDB::bind_method(D_METHOD("load", "path"), &ConfigFile::load);
	ClassDB::bind_method(D_METHOD("parse", "data"), &ConfigFile::parse);
	ClassDB::bind_method(D_METHOD("save", "path"), &ConfigFile::save);

	ClassDB::bind_method(D_METHOD("load_encrypted", "path", "key"), &ConfigFile::load_encrypted);
	ClassDB::bind_method(D_METHOD("load_encrypted_pass", "path", "password"), &ConfigFile::load_encrypted_pass);

	ClassDB::bind_method(D_METHOD("save_encrypted", "path", "key"), &ConfigFile::save_encrypted);
	ClassDB::bind_method(D_METHOD("save_encrypted_pass", "path", "password"), &ConfigFile::save_encrypted_pass);
}

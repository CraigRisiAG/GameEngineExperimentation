

/**
 * @class TranslationLoaderPO
 * @brief Loads translation data from PO (Portable Object) files.
 * 
 * This class handles parsing and loading of PO format translation files,
 * which are commonly used for internationalization (i18n) in software projects.
 * It extracts message IDs and their corresponding translations, along with
 * language configuration metadata.
 */

/**
 * @function load_translation
 * @brief Parses a PO file and creates a Translation resource.
 * 
 * Reads a PO file line by line, extracting msgid/msgstr pairs and configuration
 * metadata. Handles multi-line strings, escape sequences, and fuzzy translations.
 * 
 * @param f FileAccess pointer to the opened PO file
 * @param r_error Pointer to Error variable to store result status
 * @return RES A Translation resource containing parsed messages and locale info
 * 
 * @note Sets r_error to ERR_FILE_CORRUPT on initialization, OK on success
 * @note Cleans up file handle internally
 */

/**
 * @function load
 * @brief Opens and loads a translation file from the specified path.
 * 
 * @param p_path Path to the PO file to load
 * @param p_original_path Original path (unused in implementation)
 * @param r_error Pointer to Error variable for status reporting
 * @param p_use_sub_threads Whether to use sub-threads (unused)
 * @param r_progress Pointer to progress float (unused)
 * @return RES Loaded Translation resource
 * 
 * @note Sets r_error to ERR_CANT_OPEN on file open failure
 */

/**
 * @function get_recognized_extensions
 * @brief Returns file extensions this loader handles.
 * 
 * @param p_extensions Pointer to list to populate with supported extensions
 * @note Currently supports "po" format
 */

/**
 * @function handles_type
 * @brief Checks if this loader handles the specified resource type.
 * 
 * @param p_type Resource type string to check
 * @return bool True if type is "Translation", false otherwise
 */

/**
 * @function get_resource_type
 * @brief Determines the resource type for a given file path.
 * 
 * @param p_path File path to check
 * @return String "Translation" if file has .po extension, empty string otherwise
 */
#include "translation_loader_po.h"

#include "core/os/file_access.h"
#include "core/translation.h"

RES TranslationLoaderPO::load_translation(FileAccess *f, Error *r_error) {

	enum Status {

		STATUS_NONE,
		STATUS_READING_ID,
		STATUS_READING_STRING,
	};

	Status status = STATUS_NONE;

	String msg_id;
	String msg_str;
	String config;

	if (r_error)
		*r_error = ERR_FILE_CORRUPT;

	Ref<Translation> translation = Ref<Translation>(memnew(Translation));
	int line = 1;
	bool skip_this = false;
	bool skip_next = false;
	bool is_eof = false;

	while (!is_eof) {

		String l = f->get_line().strip_edges();
		is_eof = f->eof_reached();

		// If we reached last line and it's not a content line, break, otherwise let processing that last loop
		if (is_eof && l.empty()) {

			if (status == STATUS_READING_ID) {
				memdelete(f);
				ERR_FAIL_V_MSG(RES(), f->get_path() + ":" + itos(line) + " Unexpected EOF while reading 'msgid' at file: ");
			} else {
				break;
			}
		}

		if (l.begins_with("msgid")) {

			if (status == STATUS_READING_ID) {

				memdelete(f);
				ERR_FAIL_V_MSG(RES(), f->get_path() + ":" + itos(line) + " Unexpected 'msgid', was expecting 'msgstr' while parsing: ");
			}

			if (msg_id != "") {
				if (!skip_this)
					translation->add_message(msg_id, msg_str);
			} else if (config == "")
				config = msg_str;

			l = l.substr(5, l.length()).strip_edges();
			status = STATUS_READING_ID;
			msg_id = "";
			msg_str = "";
			skip_this = skip_next;
			skip_next = false;
		}

		if (l.begins_with("msgstr")) {

			if (status != STATUS_READING_ID) {

				memdelete(f);
				ERR_FAIL_V_MSG(RES(), f->get_path() + ":" + itos(line) + " Unexpected 'msgstr', was expecting 'msgid' while parsing: ");
			}

			l = l.substr(6, l.length()).strip_edges();
			status = STATUS_READING_STRING;
		}

		if (l == "" || l.begins_with("#")) {
			if (l.find("fuzzy") != -1) {
				skip_next = true;
			}
			line++;
			continue; //nothing to read or comment
		}

		ERR_FAIL_COND_V_MSG(!l.begins_with("\"") || status == STATUS_NONE, RES(), f->get_path() + ":" + itos(line) + " Invalid line '" + l + "' while parsing: ");

		l = l.substr(1, l.length());
		// Find final quote, ignoring escaped ones (\").
		// The escape_next logic is necessary to properly parse things like \\"
		// where the blackslash is the one being escaped, not the quote.
		int end_pos = -1;
		bool escape_next = false;
		for (int i = 0; i < l.length(); i++) {
			if (l[i] == '\\' && !escape_next) {
				escape_next = true;
				continue;
			}

			if (l[i] == '"' && !escape_next) {
				end_pos = i;
				break;
			}

			escape_next = false;
		}

		ERR_FAIL_COND_V_MSG(end_pos == -1, RES(), f->get_path() + ":" + itos(line) + ": Expected '\"' at end of message while parsing file.");

		l = l.substr(0, end_pos);
		l = l.c_unescape();

		if (status == STATUS_READING_ID)
			msg_id += l;
		else
			msg_str += l;

		line++;
	}

	f->close();
	memdelete(f);

	if (status == STATUS_READING_STRING) {

		if (msg_id != "") {
			if (!skip_this)
				translation->add_message(msg_id, msg_str);
		} else if (config == "")
			config = msg_str;
	}

	ERR_FAIL_COND_V_MSG(config == "", RES(), "No config found in file: " + f->get_path() + ".");

	Vector<String> configs = config.split("\n");
	for (int i = 0; i < configs.size(); i++) {

		String c = configs[i].strip_edges();
		int p = c.find(":");
		if (p == -1)
			continue;
		String prop = c.substr(0, p).strip_edges();
		String value = c.substr(p + 1, c.length()).strip_edges();

		if (prop == "X-Language" || prop == "Language") {
			translation->set_locale(value);
		}
	}

	if (r_error)
		*r_error = OK;

	return translation;
}

RES TranslationLoaderPO::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress) {

	if (r_error)
		*r_error = ERR_CANT_OPEN;

	FileAccess *f = FileAccess::open(p_path, FileAccess::READ);
	ERR_FAIL_COND_V_MSG(!f, RES(), "Cannot open file '" + p_path + "'.");

	return load_translation(f, r_error);
}

void TranslationLoaderPO::get_recognized_extensions(List<String> *p_extensions) const {

	p_extensions->push_back("po");
	//p_extensions->push_back("mo"); //mo in the future...
}
bool TranslationLoaderPO::handles_type(const String &p_type) const {

	return (p_type == "Translation");
}

String TranslationLoaderPO::get_resource_type(const String &p_path) const {

	if (p_path.get_extension().to_lower() == "po")
		return "Translation";
	return "";
}

TranslationLoaderPO::TranslationLoaderPO() {
}

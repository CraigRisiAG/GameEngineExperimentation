

/**
 * @class ResourceFormatImporter
 * @brief Handles importing of resources from external files into the engine.
 * 
 * ResourceFormatImporter manages the resource import pipeline, coordinating between
 * various ResourceImporter implementations to load and process external files.
 * It reads import metadata from .import files and delegates actual loading to
 * appropriate importers based on file type and extension.
 * 
 * @note This class uses the singleton pattern for global access.
 */

/**
 * @brief Comparator for sorting importers by name.
 * @param p_a First importer to compare.
 * @param p_b Second importer to compare.
 * @return true if p_a's name is lexicographically less than p_b's name.
 */

/**
 * @brief Parses import metadata from a .import file.
 * @param p_path Path to the resource file (without .import extension).
 * @param r_path_and_type Output structure containing parsed path, type, importer, and metadata.
 * @param r_valid Optional pointer to validity flag set based on import file contents.
 * @return OK if parsing succeeded, error code otherwise.
 * @details Reads the .import file and extracts resource path, type, importer name,
 *          group file, and metadata. Feature-specific paths take priority over generic paths.
 */

/**
 * @brief Loads a resource using the import system.
 * @param p_path Path to the resource file.
 * @param p_original_path Original path of the resource.
 * @param r_error Output error code.
 * @param p_use_sub_threads Whether to use sub-threads for loading.
 * @param r_progress Optional pointer to progress value.
 * @return Loaded resource or null if loading failed.
 */

/**
 * @brief Gets all file extensions recognized by any importer.
 * @param p_extensions Output list to populate with recognized extensions.
 */

/**
 * @brief Gets file extensions recognized for a specific resource type.
 * @param p_type Resource type to filter by (empty string gets all extensions).
 * @param p_extensions Output list to populate with recognized extensions.
 */

/**
 * @brief Checks if an import file exists for the given path.
 * @param p_path Path to check.
 * @return true if .import file exists, false otherwise.
 */

/**
 * @brief Checks if the importer can handle the given path.
 * @param p_path Path to check.
 * @param p_for_type Unused type parameter.
 * @return true if .import file exists for the path.
 */

/**
 * @brief Checks if the resource can be imported.
 * @param p_path Path to the resource file.
 * @return true if the resource can be imported by any registered importer.
 */

/**
 * @brief Gets the import order priority for a resource.
 * @param p_path Path to the resource file.
 * @return Import priority value (higher = imported first).
 */

/**
 * @brief Checks if the importer handles a specific resource type.
 * @param p_type Resource type to check.
 * @return true if any importer handles this type.
 */

/**
 * @brief Gets the internal resource path from import metadata.
 * @param p_path Path to the resource file.
 * @return Internal resource path or empty string on error.
 */

/**
 * @brief Gets all internal resource paths for a file.
 * @param p_path Path to the resource file.
 * @param r_paths Output list to populate with internal resource paths.
 */

/**
 * @brief Gets the group file for an imported resource.
 * @param p_path Path to the resource file.
 * @return Group file path or empty string if not valid.
 */

/**
 * @brief Checks if import metadata is valid for a resource.
 * @param p_path Path to the resource file.
 * @return true if the import file is marked as valid.
 */

/**
 * @brief Gets the resource type from import metadata.
 * @param p_path Path to the resource file.
 * @return Resource type string or empty string on error.
 */

/**
 * @brief Gets custom metadata for an imported resource.
 * @param p_path Path to the resource file.
 * @return Metadata variant or empty variant on error.
 */

/**
 * @brief Gets dependencies of an imported resource.
 * @param p_path Path to the resource file.
 * @param p_dependencies Output list to populate with dependency paths.
 * @param p_add_types Whether to include type information in dependencies.
 */

/**
 * @brief Finds an importer by its name.
 * @param p_name Name of the importer to find.
 * @return Reference to the importer or null if not found.
 */

/**
 * @brief Gets all importers that handle a specific file extension.
 * @param p_extension File extension to search for.
 * @param r_importers Output list to populate with matching importers.
 */

/**
 * @brief Gets the best importer for a file extension based on priority.
 * @param p_extension File extension to search for.
 * @return Reference to the highest priority importer or null if none found.
 */

/**
 * @brief Generates the base path for imported resources.
 * @param p_for_file Source file path.
 * @return Base path where imported resources are stored.
 */

/**
 * @brief Validates import settings for a resource.
 * @param p_path Path to the resource file.
 * @return true if all import settings are valid according to the importer.
 */

/**
 * @brief Generates a hash of all import settings.
 * @return MD5 hash of all registered importers and their settings.
 */
#include "resource_importer.h"

#include "core/os/os.h"
#include "core/variant_parser.h"

bool ResourceFormatImporter::SortImporterByName::operator()(const Ref<ResourceImporter> &p_a, const Ref<ResourceImporter> &p_b) const {
	return p_a->get_importer_name() < p_b->get_importer_name();
}

Error ResourceFormatImporter::_get_path_and_type(const String &p_path, PathAndType &r_path_and_type, bool *r_valid) const {

	Error err;
	FileAccess *f = FileAccess::open(p_path + ".import", FileAccess::READ, &err);

	if (!f) {
		if (r_valid) {
			*r_valid = false;
		}
		return err;
	}

	VariantParser::StreamFile stream;
	stream.f = f;

	String assign;
	Variant value;
	VariantParser::Tag next_tag;

	if (r_valid) {
		*r_valid = true;
	}

	int lines = 0;
	String error_text;
	bool path_found = false; //first match must have priority
	while (true) {

		assign = Variant();
		next_tag.fields.clear();
		next_tag.name = String();

		err = VariantParser::parse_tag_assign_eof(&stream, lines, error_text, next_tag, assign, value, NULL, true);
		if (err == ERR_FILE_EOF) {
			memdelete(f);
			return OK;
		} else if (err != OK) {
			ERR_PRINT("ResourceFormatImporter::load - " + p_path + ".import:" + itos(lines) + " error: " + error_text);
			memdelete(f);
			return err;
		}

		if (assign != String()) {
			if (!path_found && assign.begins_with("path.") && r_path_and_type.path == String()) {
				String feature = assign.get_slicec('.', 1);
				if (OS::get_singleton()->has_feature(feature)) {
					r_path_and_type.path = value;
					path_found = true; //first match must have priority
				}

			} else if (!path_found && assign == "path") {
				r_path_and_type.path = value;
				path_found = true; //first match must have priority
			} else if (assign == "type") {
				r_path_and_type.type = value;
			} else if (assign == "importer") {
				r_path_and_type.importer = value;
			} else if (assign == "group_file") {
				r_path_and_type.group_file = value;
			} else if (assign == "metadata") {
				r_path_and_type.metadata = value;
			} else if (assign == "valid") {
				if (r_valid) {
					*r_valid = value;
				}
			}

		} else if (next_tag.name != "remap") {
			break;
		}
	}

	memdelete(f);

	if (r_path_and_type.path == String() || r_path_and_type.type == String()) {
		return ERR_FILE_CORRUPT;
	}
	return OK;
}

RES ResourceFormatImporter::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress) {

	PathAndType pat;
	Error err = _get_path_and_type(p_path, pat);

	if (err != OK) {

		if (r_error)
			*r_error = err;

		return RES();
	}

	RES res = ResourceLoader::_load(pat.path, p_path, pat.type, false, r_error, p_use_sub_threads, r_progress);

#ifdef TOOLS_ENABLED
	if (res.is_valid()) {
		res->set_import_last_modified_time(res->get_last_modified_time()); //pass this, if used
		res->set_import_path(pat.path);
	}
#endif

	return res;
}

void ResourceFormatImporter::get_recognized_extensions(List<String> *p_extensions) const {

	Set<String> found;

	for (int i = 0; i < importers.size(); i++) {
		List<String> local_exts;
		importers[i]->get_recognized_extensions(&local_exts);
		for (List<String>::Element *F = local_exts.front(); F; F = F->next()) {
			if (!found.has(F->get())) {
				p_extensions->push_back(F->get());
				found.insert(F->get());
			}
		}
	}
}

void ResourceFormatImporter::get_recognized_extensions_for_type(const String &p_type, List<String> *p_extensions) const {

	if (p_type == "") {
		get_recognized_extensions(p_extensions);
		return;
	}

	Set<String> found;

	for (int i = 0; i < importers.size(); i++) {
		String res_type = importers[i]->get_resource_type();
		if (res_type == String())
			continue;

		if (!ClassDB::is_parent_class(res_type, p_type))
			continue;

		List<String> local_exts;
		importers[i]->get_recognized_extensions(&local_exts);
		for (List<String>::Element *F = local_exts.front(); F; F = F->next()) {
			if (!found.has(F->get())) {
				p_extensions->push_back(F->get());
				found.insert(F->get());
			}
		}
	}
}

bool ResourceFormatImporter::exists(const String &p_path) const {

	return FileAccess::exists(p_path + ".import");
}

bool ResourceFormatImporter::recognize_path(const String &p_path, const String &p_for_type) const {

	return FileAccess::exists(p_path + ".import");
}

bool ResourceFormatImporter::can_be_imported(const String &p_path) const {

	return ResourceFormatLoader::recognize_path(p_path);
}

int ResourceFormatImporter::get_import_order(const String &p_path) const {

	Ref<ResourceImporter> importer;

	if (FileAccess::exists(p_path + ".import")) {

		PathAndType pat;
		Error err = _get_path_and_type(p_path, pat);

		if (err == OK) {
			importer = get_importer_by_name(pat.importer);
		}
	} else {

		importer = get_importer_by_extension(p_path.get_extension().to_lower());
	}

	if (importer.is_valid())
		return importer->get_import_order();

	return 0;
}

bool ResourceFormatImporter::handles_type(const String &p_type) const {

	for (int i = 0; i < importers.size(); i++) {

		String res_type = importers[i]->get_resource_type();
		if (res_type == String())
			continue;
		if (ClassDB::is_parent_class(res_type, p_type))
			return true;
	}

	return true;
}

String ResourceFormatImporter::get_internal_resource_path(const String &p_path) const {

	PathAndType pat;
	Error err = _get_path_and_type(p_path, pat);

	if (err != OK) {

		return String();
	}

	return pat.path;
}

void ResourceFormatImporter::get_internal_resource_path_list(const String &p_path, List<String> *r_paths) {

	Error err;
	FileAccess *f = FileAccess::open(p_path + ".import", FileAccess::READ, &err);

	if (!f)
		return;

	VariantParser::StreamFile stream;
	stream.f = f;

	String assign;
	Variant value;
	VariantParser::Tag next_tag;

	int lines = 0;
	String error_text;
	while (true) {

		assign = Variant();
		next_tag.fields.clear();
		next_tag.name = String();

		err = VariantParser::parse_tag_assign_eof(&stream, lines, error_text, next_tag, assign, value, NULL, true);
		if (err == ERR_FILE_EOF) {
			memdelete(f);
			return;
		} else if (err != OK) {
			ERR_PRINT("ResourceFormatImporter::get_internal_resource_path_list - " + p_path + ".import:" + itos(lines) + " error: " + error_text);
			memdelete(f);
			return;
		}

		if (assign != String()) {
			if (assign.begins_with("path.")) {
				r_paths->push_back(value);
			} else if (assign == "path") {
				r_paths->push_back(value);
			}
		} else if (next_tag.name != "remap") {
			break;
		}
	}
	memdelete(f);
}

String ResourceFormatImporter::get_import_group_file(const String &p_path) const {

	bool valid = true;
	PathAndType pat;
	_get_path_and_type(p_path, pat, &valid);
	return valid ? pat.group_file : String();
}

bool ResourceFormatImporter::is_import_valid(const String &p_path) const {

	bool valid = true;
	PathAndType pat;
	_get_path_and_type(p_path, pat, &valid);
	return valid;
}

String ResourceFormatImporter::get_resource_type(const String &p_path) const {

	PathAndType pat;
	Error err = _get_path_and_type(p_path, pat);

	if (err != OK) {

		return "";
	}

	return pat.type;
}

Variant ResourceFormatImporter::get_resource_metadata(const String &p_path) const {
	PathAndType pat;
	Error err = _get_path_and_type(p_path, pat);

	if (err != OK) {

		return Variant();
	}

	return pat.metadata;
}

void ResourceFormatImporter::get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types) {

	PathAndType pat;
	Error err = _get_path_and_type(p_path, pat);

	if (err != OK) {

		return;
	}

	ResourceLoader::get_dependencies(pat.path, p_dependencies, p_add_types);
}

Ref<ResourceImporter> ResourceFormatImporter::get_importer_by_name(const String &p_name) const {

	for (int i = 0; i < importers.size(); i++) {
		if (importers[i]->get_importer_name() == p_name) {
			return importers[i];
		}
	}

	return Ref<ResourceImporter>();
}

void ResourceFormatImporter::get_importers_for_extension(const String &p_extension, List<Ref<ResourceImporter>> *r_importers) {

	for (int i = 0; i < importers.size(); i++) {
		List<String> local_exts;
		importers[i]->get_recognized_extensions(&local_exts);
		for (List<String>::Element *F = local_exts.front(); F; F = F->next()) {
			if (p_extension.to_lower() == F->get()) {
				r_importers->push_back(importers[i]);
			}
		}
	}
}

Ref<ResourceImporter> ResourceFormatImporter::get_importer_by_extension(const String &p_extension) const {

	Ref<ResourceImporter> importer;
	float priority = 0;

	for (int i = 0; i < importers.size(); i++) {

		List<String> local_exts;
		importers[i]->get_recognized_extensions(&local_exts);
		for (List<String>::Element *F = local_exts.front(); F; F = F->next()) {
			if (p_extension.to_lower() == F->get() && importers[i]->get_priority() > priority) {
				importer = importers[i];
				priority = importers[i]->get_priority();
			}
		}
	}

	return importer;
}

String ResourceFormatImporter::get_import_base_path(const String &p_for_file) const {

	return "res://.import/" + p_for_file.get_file() + "-" + p_for_file.md5_text();
}

bool ResourceFormatImporter::are_import_settings_valid(const String &p_path) const {

	bool valid = true;
	PathAndType pat;
	_get_path_and_type(p_path, pat, &valid);

	if (!valid) {
		return false;
	}

	for (int i = 0; i < importers.size(); i++) {
		if (importers[i]->get_importer_name() == pat.importer) {
			if (!importers[i]->are_import_settings_valid(p_path)) { //importer thinks this is not valid
				return false;
			}
		}
	}

	return true;
}

String ResourceFormatImporter::get_import_settings_hash() const {

	Vector<Ref<ResourceImporter>> sorted_importers = importers;

	sorted_importers.sort_custom<SortImporterByName>();

	String hash;
	for (int i = 0; i < sorted_importers.size(); i++) {
		hash += ":" + sorted_importers[i]->get_importer_name() + ":" + sorted_importers[i]->get_import_settings_string();
	}
	return hash.md5_text();
}

ResourceFormatImporter *ResourceFormatImporter::singleton = NULL;

ResourceFormatImporter::ResourceFormatImporter() {
	singleton = this;
}

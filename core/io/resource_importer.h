

/**
 * @file resource_importer.h
 * @brief Resource importing system for loading and managing various file formats.
 * 
 * This header defines the core resource importing infrastructure, consisting of two main classes:
 * 
 * @class ResourceFormatImporter
 * A loader that manages multiple resource importers and handles the import pipeline.
 * It maintains a collection of ResourceImporter instances and provides methods to:
 * - Recognize and load resources by file path
 * - Query supported file extensions and types
 * - Retrieve resource metadata and import validity
 * - Manage importer registration and retrieval
 * - Compute import settings hashes for caching
 * 
 * @struct PathAndType
 * Internal structure containing metadata about an imported resource path:
 * - path: File system path to the resource
 * - type: The resource type
 * - importer: Name of the importer used
 * - group_file: Optional associated group file
 * - metadata: Additional resource metadata
 * 
 * @struct SortImporterByName
 * Comparator for sorting importers by name to maintain consistent settings hash computation.
 * 
 * @class ResourceImporter
 * Abstract base class that defines the interface for concrete resource importers.
 * Subclasses must implement:
 * - get_importer_name(): Unique identifier for the importer
 * - get_visible_name(): Human-readable name
 * - get_recognized_extensions(): File extensions this importer handles
 * - get_save_extension(): Output file extension
 * - get_resource_type(): Type of resources produced
 * - get_import_options(): Available import configuration options
 * - get_option_visibility(): Determines which options are visible based on settings
 * - import(): Performs the actual import operation
 * 
 * @struct ImportOption
 * Configuration option for the import process, containing:
 * - option: PropertyInfo describing the option
 * - default_value: Default value for the option
 * 
 * Optional methods allow importers to:
 * - Report priority and import order
 * - Support multiple presets
 * - Handle group file imports
 * - Validate import settings
 */
#ifndef RESOURCE_IMPORTER_H
#define RESOURCE_IMPORTER_H

#include "core/io/resource_loader.h"

class ResourceImporter;

class ResourceFormatImporter : public ResourceFormatLoader {

	struct PathAndType {
		String path;
		String type;
		String importer;
		String group_file;
		Variant metadata;
	};

	Error _get_path_and_type(const String &p_path, PathAndType &r_path_and_type, bool *r_valid = NULL) const;

	static ResourceFormatImporter *singleton;

	//need them to stay in order to compute the settings hash
	struct SortImporterByName {
		bool operator()(const Ref<ResourceImporter> &p_a, const Ref<ResourceImporter> &p_b) const;
	};

	Vector<Ref<ResourceImporter>> importers;

public:
	static ResourceFormatImporter *get_singleton() { return singleton; }
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_use_sub_threads = false, float *r_progress = nullptr);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual void get_recognized_extensions_for_type(const String &p_type, List<String> *p_extensions) const;
	virtual bool recognize_path(const String &p_path, const String &p_for_type = String()) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
	virtual Variant get_resource_metadata(const String &p_path) const;
	virtual bool is_import_valid(const String &p_path) const;
	virtual void get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types = false);
	virtual bool is_imported(const String &p_path) const { return recognize_path(p_path); }
	virtual String get_import_group_file(const String &p_path) const;
	virtual bool exists(const String &p_path) const;

	virtual bool can_be_imported(const String &p_path) const;
	virtual int get_import_order(const String &p_path) const;

	String get_internal_resource_path(const String &p_path) const;
	void get_internal_resource_path_list(const String &p_path, List<String> *r_paths);

	void add_importer(const Ref<ResourceImporter> &p_importer) {
		importers.push_back(p_importer);
	}
	void remove_importer(const Ref<ResourceImporter> &p_importer) { importers.erase(p_importer); }
	Ref<ResourceImporter> get_importer_by_name(const String &p_name) const;
	Ref<ResourceImporter> get_importer_by_extension(const String &p_extension) const;
	void get_importers_for_extension(const String &p_extension, List<Ref<ResourceImporter>> *r_importers);

	bool are_import_settings_valid(const String &p_path) const;
	String get_import_settings_hash() const;

	String get_import_base_path(const String &p_for_file) const;
	ResourceFormatImporter();
};

class ResourceImporter : public Reference {

	GDCLASS(ResourceImporter, Reference);

public:
	virtual String get_importer_name() const = 0;
	virtual String get_visible_name() const = 0;
	virtual void get_recognized_extensions(List<String> *p_extensions) const = 0;
	virtual String get_save_extension() const = 0;
	virtual String get_resource_type() const = 0;
	virtual float get_priority() const { return 1.0; }
	virtual int get_import_order() const { return 0; }

	struct ImportOption {
		PropertyInfo option;
		Variant default_value;

		ImportOption(const PropertyInfo &p_info, const Variant &p_default) :
				option(p_info),
				default_value(p_default) {
		}
		ImportOption() {}
	};

	virtual int get_preset_count() const { return 0; }
	virtual String get_preset_name(int p_idx) const { return String(); }

	virtual void get_import_options(List<ImportOption> *r_options, int p_preset = 0) const = 0;
	virtual bool get_option_visibility(const String &p_option, const Map<StringName, Variant> &p_options) const = 0;
	virtual String get_option_group_file() const { return String(); }

	virtual Error import(const String &p_source_file, const String &p_save_path, const Map<StringName, Variant> &p_options, List<String> *r_platform_variants, List<String> *r_gen_files = NULL, Variant *r_metadata = NULL) = 0;

	virtual Error import_group_file(const String &p_group_file, const Map<String, Map<StringName, Variant>> &p_source_file_options, const Map<String, String> &p_base_paths) { return ERR_UNAVAILABLE; }
	virtual bool are_import_settings_valid(const String &p_path) const { return true; }
	virtual String get_import_settings_string() const { return String(); }
};

#endif // RESOURCE_IMPORTER_H

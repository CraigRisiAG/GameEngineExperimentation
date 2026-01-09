

/// \file resource_format_binary.h
/// \brief Binary resource format loader and saver implementations.
///
/// This header defines classes for loading and saving resources in a binary format.
/// It provides functionality for:
/// - Parsing binary resource files with support for external and internal resources
/// - Managing resource dependencies and remapping
/// - Serializing resources to binary format with various options
/// - Handling string pooling and variant serialization

/// \class ResourceLoaderBinary
/// \brief Handles parsing and loading of binary resource files.
///
/// Manages the low-level reading of binary resource files, including:
/// - String decompression and management via string_map
/// - Variant parsing and reconstruction
/// - External and internal resource references
/// - Translation remapping and dependency tracking
/// - Progress tracking for multi-threaded loading

/// \class ResourceFormatLoaderBinary
/// \brief Factory class for creating ResourceLoaderBinary instances.
///
/// Implements the ResourceFormatLoader interface to provide:
/// - Resource file loading with optional multi-threading support
/// - File type recognition and validation
/// - Extension and type queries
/// - Dependency extraction and remapping

/// \class ResourceFormatSaverBinaryInstance
/// \brief Handles serialization of resources to binary format.
///
/// Manages the binary encoding of resources including:
/// - String pooling and compression
/// - External and internal resource management
/// - Property serialization with variant encoding
/// - Path resolution (relative/absolute)
/// - Endian conversion support

/// \class ResourceFormatSaverBinary
/// \brief Factory class for creating binary resource savers.
///
/// Implements the ResourceFormatSaver interface to provide:
/// - Resource serialization to binary format
/// - Format recognition for supported resource types
/// - File extension queries
#ifndef RESOURCE_FORMAT_BINARY_H
#define RESOURCE_FORMAT_BINARY_H

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"
#include "core/os/file_access.h"

class ResourceLoaderBinary {

	bool translation_remapped;
	String local_path;
	String res_path;
	String type;
	Ref<Resource> resource;
	uint32_t ver_format;

	FileAccess *f;

	uint64_t importmd_ofs;

	Vector<char> str_buf;
	List<RES> resource_cache;

	Vector<StringName> string_map;

	StringName _get_string();

	struct ExtResource {
		String path;
		String type;
		RES cache;
	};

	bool use_sub_threads;
	float *progress;
	Vector<ExtResource> external_resources;

	struct IntResource {
		String path;
		uint64_t offset;
	};

	Vector<IntResource> internal_resources;

	String get_unicode_string();
	void _advance_padding(uint32_t p_len);

	Map<String, String> remaps;
	Error error;

	friend class ResourceFormatLoaderBinary;

	Error parse_variant(Variant &r_v);

	Map<String, RES> dependency_cache;

public:
	void set_local_path(const String &p_local_path);
	Ref<Resource> get_resource();
	Error load();
	void set_translation_remapped(bool p_remapped);

	void set_remaps(const Map<String, String> &p_remaps) { remaps = p_remaps; }
	void open(FileAccess *p_f);
	String recognize(FileAccess *p_f);
	void get_dependencies(FileAccess *p_f, List<String> *p_dependencies, bool p_add_types);

	ResourceLoaderBinary();
	~ResourceLoaderBinary();
};

class ResourceFormatLoaderBinary : public ResourceFormatLoader {
public:
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_use_sub_threads = false, float *r_progress = nullptr);
	virtual void get_recognized_extensions_for_type(const String &p_type, List<String> *p_extensions) const;
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
	virtual void get_dependencies(const String &p_path, List<String> *p_dependencies, bool p_add_types = false);
	virtual Error rename_dependencies(const String &p_path, const Map<String, String> &p_map);
};

class ResourceFormatSaverBinaryInstance {

	String local_path;
	String path;

	bool relative_paths;
	bool bundle_resources;
	bool skip_editor;
	bool big_endian;
	bool takeover_paths;
	FileAccess *f;
	String magic;
	Set<RES> resource_set;

	struct NonPersistentKey { //for resource properties generated on the fly
		RES base;
		StringName property;
		bool operator<(const NonPersistentKey &p_key) const { return base == p_key.base ? property < p_key.property : base < p_key.base; }
	};

	Map<NonPersistentKey, RES> non_persistent_map;
	Map<StringName, int> string_map;
	Vector<StringName> strings;

	Map<RES, int> external_resources;
	List<RES> saved_resources;

	struct Property {
		int name_idx;
		Variant value;
		PropertyInfo pi;
	};

	struct ResourceData {

		String type;
		List<Property> properties;
	};

	static void _pad_buffer(FileAccess *f, int p_bytes);
	void _write_variant(const Variant &p_property, const PropertyInfo &p_hint = PropertyInfo());
	void _find_resources(const Variant &p_variant, bool p_main = false);
	static void save_unicode_string(FileAccess *f, const String &p_string, bool p_bit_on_len = false);
	int get_string_index(const String &p_string);

public:
	Error save(const String &p_path, const RES &p_resource, uint32_t p_flags = 0);
	static void write_variant(FileAccess *f, const Variant &p_property, Set<RES> &resource_set, Map<RES, int> &external_resources, Map<StringName, int> &string_map, const PropertyInfo &p_hint = PropertyInfo());
};

class ResourceFormatSaverBinary : public ResourceFormatSaver {
public:
	static ResourceFormatSaverBinary *singleton;
	virtual Error save(const String &p_path, const RES &p_resource, uint32_t p_flags = 0);
	virtual bool recognize(const RES &p_resource) const;
	virtual void get_recognized_extensions(const RES &p_resource, List<String> *p_extensions) const;

	ResourceFormatSaverBinary();
};

#endif // RESOURCE_FORMAT_BINARY_H



/**
 * @class PCKPacker
 * @brief Utility class for creating and packing files into PCK (Godot package) archives.
 * 
 * PCKPacker provides functionality to create PCK package files by adding individual files
 * and writing them to disk with proper alignment and offset management.
 * 
 * @note This class inherits from Reference and is part of the core I/O module.
 * 
 * @example
 * var packer = PCKPacker.new()
 * packer.pck_start("res://game.pck")
 * packer.add_file("res://data.txt", "user://data.txt")
 * packer.flush()
 */

/**
 * @brief Initializes a new PCK package file for writing.
 * 
 * @param p_file The output path where the PCK file will be created
 * @param p_alignment Optional alignment boundary for file offsets (default: 0)
 * 
 * @return Error code indicating success or failure of the operation
 */

/**
 * @brief Adds a file to the PCK package.
 * 
 * @param p_file The destination path within the PCK archive
 * @param p_src The source file path to be packed
 * 
 * @return Error code indicating success or failure of adding the file
 */

/**
 * @brief Writes all added files to the PCK package on disk.
 * 
 * @param p_verbose If true, outputs detailed information during the flush operation (default: false)
 * 
 * @return Error code indicating success or failure of the flush operation
 */
#ifndef PCK_PACKER_H
#define PCK_PACKER_H

#include "core/reference.h"

class FileAccess;

class PCKPacker : public Reference {

	GDCLASS(PCKPacker, Reference);

	FileAccess *file;
	int alignment;

	static void _bind_methods();

	struct File {

		String path;
		String src_path;
		int size;
		uint64_t offset_offset;
	};
	Vector<File> files;

public:
	Error pck_start(const String &p_file, int p_alignment = 0);
	Error add_file(const String &p_file, const String &p_src);
	Error flush(bool p_verbose = false);

	PCKPacker();
	~PCKPacker();
};

#endif // PCK_PACKER_H

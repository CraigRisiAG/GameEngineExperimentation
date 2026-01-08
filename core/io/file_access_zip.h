

/**
 * @file file_access_zip.h
 * @brief ZIP archive file access implementation using minizip library.
 * 
 * Provides classes for reading and accessing files within ZIP archives.
 * Requires MINIZIP_ENABLED to be defined for compilation.
 */

/**
 * @class ZipArchive
 * @brief Manages ZIP archive packages and file access.
 * 
 * Handles loading and managing multiple ZIP packages, maintaining a map of files
 * within those packages. Acts as a singleton for global ZIP archive access.
 */

/**
 * @struct ZipArchive::File
 * @brief Represents a file within a ZIP archive.
 * @member package Index of the package containing this file (-1 if uninitialized)
 * @member file_pos Position information for the file within the ZIP
 */

/**
 * @struct ZipArchive::Package
 * @brief Represents a loaded ZIP package.
 * @member filename Path to the ZIP file
 * @member zfile Handle to the opened ZIP file
 */

/**
 * @class FileAccessZip
 * @brief Provides file access interface for reading files within ZIP archives.
 * 
 * Implements FileAccess interface to allow reading operations on individual files
 * stored within ZIP archives. Supports seeking, reading, and EOF detection.
 * Write operations are not supported.
 */
#ifdef MINIZIP_ENABLED

#ifndef FILE_ACCESS_ZIP_H
#define FILE_ACCESS_ZIP_H

#include "core/io/file_access_pack.h"
#include "core/map.h"

#include "thirdparty/minizip/unzip.h"

#include <stdlib.h>

class ZipArchive : public PackSource {

public:
	struct File {

		int package;
		unz_file_pos file_pos;
		File() {

			package = -1;
		};
	};

private:
	struct Package {
		String filename;
		unzFile zfile;
	};
	Vector<Package> packages;

	Map<String, File> files;

	static ZipArchive *instance;

	FileAccess::CreateFunc fa_create_func;

public:
	void close_handle(unzFile p_file) const;
	unzFile get_file_handle(String p_file) const;

	Error add_package(String p_name);

	bool file_exists(String p_name) const;

	virtual bool try_open_pack(const String &p_path, bool p_replace_files);
	FileAccess *get_file(const String &p_path, PackedData::PackedFile *p_file);

	static ZipArchive *get_singleton();

	ZipArchive();
	~ZipArchive();
};

class FileAccessZip : public FileAccess {

	unzFile zfile;
	unz_file_info64 file_info;

	mutable bool at_eof;

public:
	virtual Error _open(const String &p_path, int p_mode_flags); ///< open a file
	virtual void close(); ///< close a file
	virtual bool is_open() const; ///< true when file is open

	virtual void seek(size_t p_position); ///< seek to a given position
	virtual void seek_end(int64_t p_position = 0); ///< seek from the end of file
	virtual size_t get_position() const; ///< get position in the file
	virtual size_t get_len() const; ///< get size of the file

	virtual bool eof_reached() const; ///< reading passed EOF

	virtual uint8_t get_8() const; ///< get a byte
	virtual int get_buffer(uint8_t *p_dst, int p_length) const;

	virtual Error get_error() const; ///< get last error

	virtual void flush();
	virtual void store_8(uint8_t p_dest); ///< store a byte
	virtual bool file_exists(const String &p_name); ///< return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file) { return 0; } // todo
	virtual uint32_t _get_unix_permissions(const String &p_file) { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) { return FAILED; }

	FileAccessZip(const String &p_path, const PackedData::PackedFile &p_file);
	~FileAccessZip();
};

#endif // FILE_ACCESS_ZIP_H

#endif // MINIZIP_ENABLED

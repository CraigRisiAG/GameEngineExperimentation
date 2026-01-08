

/**
 * @file file_access_memory.cpp
 * @brief Memory-based file access implementation for the game engine.
 * 
 * This file implements the FileAccessMemory class, which provides file-like access
 * to data stored in memory. It maintains a static map of registered memory files and
 * allows reading and writing operations on these in-memory buffers.
 * 
 * Key Features:
 * - Register virtual files with binary data
 * - Open and close memory-based files
 * - Read and write operations with position tracking
 * - Support for seeking within memory buffers
 * - EOF detection and error handling
 * 
 * The implementation uses a static Map to store file data globally, with methods to
 * register new files, retrieve them, and clean up resources. File paths are normalized
 * using ProjectSettings when available.
 * 
 * Dependencies:
 * - core/map.h: For file storage map data structure
 * - core/os/copymem.h: For memory copy operations
 * - core/os/dir_access.h: For directory path utilities
 * - core/project_settings.h: For path globalization
 */
#include "file_access_memory.h"

#include "core/map.h"
#include "core/os/copymem.h"
#include "core/os/dir_access.h"
#include "core/project_settings.h"

static Map<String, Vector<uint8_t>> *files = NULL;

void FileAccessMemory::register_file(String p_name, Vector<uint8_t> p_data) {

	if (!files) {
		files = memnew((Map<String, Vector<uint8_t>>));
	}

	String name;
	if (ProjectSettings::get_singleton())
		name = ProjectSettings::get_singleton()->globalize_path(p_name);
	else
		name = p_name;
	//name = DirAccess::normalize_path(name);

	(*files)[name] = p_data;
}

void FileAccessMemory::cleanup() {

	if (!files)
		return;

	memdelete(files);
}

FileAccess *FileAccessMemory::create() {

	return memnew(FileAccessMemory);
}

bool FileAccessMemory::file_exists(const String &p_name) {

	String name = fix_path(p_name);
	//name = DirAccess::normalize_path(name);

	return files && (files->find(name) != NULL);
}

Error FileAccessMemory::open_custom(const uint8_t *p_data, int p_len) {

	data = (uint8_t *)p_data;
	length = p_len;
	pos = 0;
	return OK;
}

Error FileAccessMemory::_open(const String &p_path, int p_mode_flags) {

	ERR_FAIL_COND_V(!files, ERR_FILE_NOT_FOUND);

	String name = fix_path(p_path);
	//name = DirAccess::normalize_path(name);

	Map<String, Vector<uint8_t>>::Element *E = files->find(name);
	ERR_FAIL_COND_V_MSG(!E, ERR_FILE_NOT_FOUND, "Can't find file '" + p_path + "'.");

	data = E->get().ptrw();
	length = E->get().size();
	pos = 0;

	return OK;
}

void FileAccessMemory::close() {

	data = NULL;
}

bool FileAccessMemory::is_open() const {

	return data != NULL;
}

void FileAccessMemory::seek(size_t p_position) {

	ERR_FAIL_COND(!data);
	pos = p_position;
}

void FileAccessMemory::seek_end(int64_t p_position) {

	ERR_FAIL_COND(!data);
	pos = length + p_position;
}

size_t FileAccessMemory::get_position() const {

	ERR_FAIL_COND_V(!data, 0);
	return pos;
}

size_t FileAccessMemory::get_len() const {

	ERR_FAIL_COND_V(!data, 0);
	return length;
}

bool FileAccessMemory::eof_reached() const {

	return pos > length;
}

uint8_t FileAccessMemory::get_8() const {

	uint8_t ret = 0;
	if (pos < length) {
		ret = data[pos];
	}
	++pos;

	return ret;
}

int FileAccessMemory::get_buffer(uint8_t *p_dst, int p_length) const {

	ERR_FAIL_COND_V(!data, -1);

	int left = length - pos;
	int read = MIN(p_length, left);

	if (read < p_length) {
		WARN_PRINT("Reading less data than requested");
	};

	copymem(p_dst, &data[pos], read);
	pos += p_length;

	return read;
}

Error FileAccessMemory::get_error() const {

	return pos >= length ? ERR_FILE_EOF : OK;
}

void FileAccessMemory::flush() {
	ERR_FAIL_COND(!data);
}

void FileAccessMemory::store_8(uint8_t p_byte) {

	ERR_FAIL_COND(!data);
	ERR_FAIL_COND(pos >= length);
	data[pos++] = p_byte;
}

void FileAccessMemory::store_buffer(const uint8_t *p_src, int p_length) {

	int left = length - pos;
	int write = MIN(p_length, left);
	if (write < p_length) {
		WARN_PRINT("Writing less data than requested");
	}

	copymem(&data[pos], p_src, write);
	pos += p_length;
}

FileAccessMemory::FileAccessMemory() {

	data = NULL;
}

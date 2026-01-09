

/**
 * @file pck_packer.cpp
 * @brief PCK package file packer implementation.
 * 
 * This file implements the PCKPacker class, which is responsible for creating
 * and writing PCK (pack) files. PCK files are binary archives that contain
 * multiple files with their metadata and content.
 * 
 * The packer handles:
 * - Creating a new PCK file with proper header and format version information
 * - Adding files to the pack queue with their source paths and destination paths
 * - Writing the complete pack file with proper alignment and offset management
 * - Storing file metadata (paths, sizes, offsets, MD5 hashes)
 * 
 * Key Features:
 * - Alignment support for optimal data layout
 * - Efficient buffered file writing (64KB buffer)
 * - Proper file offset tracking and index management
 * - Reserved header space for future extensions
 * - Progress reporting during flush operations
 * 
 * Usage:
 * 1. Call pck_start() to create and initialize a new PCK file
 * 2. Call add_file() multiple times to queue files for packing
 * 3. Call flush() to write all queued files to the PCK archive
 * 
 * @note This implementation assumes little-endian byte order for integer storage.
 * @note MD5 hash storage is reserved but currently filled with zeros.
 * @dependencies core/io/file_access_pack.h, core/os/file_access.h, core/version.h
 */
#include "pck_packer.h"

#include "core/io/file_access_pack.h" // PACK_HEADER_MAGIC, PACK_FORMAT_VERSION
#include "core/os/file_access.h"
#include "core/version.h"

static uint64_t _align(uint64_t p_n, int p_alignment) {

	if (p_alignment == 0)
		return p_n;

	uint64_t rest = p_n % p_alignment;
	if (rest == 0)
		return p_n;
	else
		return p_n + (p_alignment - rest);
};

static void _pad(FileAccess *p_file, int p_bytes) {

	for (int i = 0; i < p_bytes; i++) {

		p_file->store_8(0);
	};
};

void PCKPacker::_bind_methods() {

	ClassDB::bind_method(D_METHOD("pck_start", "pck_name", "alignment"), &PCKPacker::pck_start, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("add_file", "pck_path", "source_path"), &PCKPacker::add_file);
	ClassDB::bind_method(D_METHOD("flush", "verbose"), &PCKPacker::flush, DEFVAL(false));
};

Error PCKPacker::pck_start(const String &p_file, int p_alignment) {

	if (file != NULL) {
		memdelete(file);
	}

	file = FileAccess::open(p_file, FileAccess::WRITE);

	ERR_FAIL_COND_V_MSG(!file, ERR_CANT_CREATE, "Can't open file to write: " + String(p_file) + ".");

	alignment = p_alignment;

	file->store_32(PACK_HEADER_MAGIC);
	file->store_32(PACK_FORMAT_VERSION);
	file->store_32(VERSION_MAJOR);
	file->store_32(VERSION_MINOR);
	file->store_32(VERSION_PATCH);

	for (int i = 0; i < 16; i++) {

		file->store_32(0); // reserved
	};

	files.clear();

	return OK;
};

Error PCKPacker::add_file(const String &p_file, const String &p_src) {

	FileAccess *f = FileAccess::open(p_src, FileAccess::READ);
	if (!f) {
		return ERR_FILE_CANT_OPEN;
	};

	File pf;
	pf.path = p_file;
	pf.src_path = p_src;
	pf.size = f->get_len();
	pf.offset_offset = 0;

	files.push_back(pf);

	f->close();
	memdelete(f);

	return OK;
};

Error PCKPacker::flush(bool p_verbose) {

	ERR_FAIL_COND_V_MSG(!file, ERR_INVALID_PARAMETER, "File must be opened before use.");

	// write the index

	file->store_32(files.size());

	for (int i = 0; i < files.size(); i++) {

		file->store_pascal_string(files[i].path);
		files.write[i].offset_offset = file->get_position();
		file->store_64(0); // offset
		file->store_64(files[i].size); // size

		// # empty md5
		file->store_32(0);
		file->store_32(0);
		file->store_32(0);
		file->store_32(0);
	};

	uint64_t ofs = file->get_position();
	ofs = _align(ofs, alignment);

	_pad(file, ofs - file->get_position());

	const uint32_t buf_max = 65536;
	uint8_t *buf = memnew_arr(uint8_t, buf_max);

	int count = 0;
	for (int i = 0; i < files.size(); i++) {

		FileAccess *src = FileAccess::open(files[i].src_path, FileAccess::READ);
		uint64_t to_write = files[i].size;
		while (to_write > 0) {

			int read = src->get_buffer(buf, MIN(to_write, buf_max));
			file->store_buffer(buf, read);
			to_write -= read;
		};

		uint64_t pos = file->get_position();
		file->seek(files[i].offset_offset); // go back to store the file's offset
		file->store_64(ofs);
		file->seek(pos);

		ofs = _align(ofs + files[i].size, alignment);
		_pad(file, ofs - pos);

		src->close();
		memdelete(src);
		count += 1;
		if (p_verbose && files.size() > 0) {
			if (count % 100 == 0) {
				printf("%i/%i (%.2f)\r", count, files.size(), float(count) / files.size() * 100);
				fflush(stdout);
			};
		};
	};

	if (p_verbose)
		printf("\n");

	file->close();
	memdelete_arr(buf);

	return OK;
};

PCKPacker::PCKPacker() {

	file = NULL;
};

PCKPacker::~PCKPacker() {
	if (file != NULL) {
		memdelete(file);
	};
	file = NULL;
};

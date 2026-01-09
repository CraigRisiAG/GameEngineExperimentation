

/**
 * @file zip_io.cpp
 * @brief ZIP file I/O operations using zlib file function interface.
 *
 * This module provides a bridge between the engine's FileAccess class and zlib's
 * file operation callbacks, enabling reading and writing of ZIP archives.
 */

/**
 * @brief Opens a file for reading or writing in ZIP operations.
 * @param data Pointer to FileAccess pointer that will be set to the opened file.
 * @param p_fname UTF-8 encoded filename to open.
 * @param mode File access mode (ZLIB_FILEFUNC_MODE_WRITE for writing, else reading).
 * @return Pointer to data parameter on success, NULL on failure.
 */

/**
 * @brief Reads data from the currently open file.
 * @param data Pointer to FileAccess pointer.
 * @param fdata File data pointer (unused).
 * @param buf Buffer to store read data.
 * @param size Number of bytes to read.
 * @return Number of bytes actually read.
 */

/**
 * @brief Writes data to the currently open file.
 * @param opaque Pointer to FileAccess pointer.
 * @param stream Stream pointer (unused).
 * @param buf Buffer containing data to write.
 * @param size Number of bytes to write.
 * @return Number of bytes written.
 */

/**
 * @brief Gets the current position in the open file.
 * @param opaque Pointer to FileAccess pointer.
 * @param stream Stream pointer (unused).
 * @return Current file position.
 */

/**
 * @brief Seeks to a specific position in the open file.
 * @param opaque Pointer to FileAccess pointer.
 * @param stream Stream pointer (unused).
 * @param offset Offset in bytes.
 * @param origin Seek origin (ZLIB_FILEFUNC_SEEK_CUR, ZLIB_FILEFUNC_SEEK_END, or ZLIB_FILEFUNC_SEEK_SET).
 * @return 0 on success.
 */

/**
 * @brief Closes the currently open file and frees associated resources.
 * @param opaque Pointer to FileAccess pointer.
 * @param stream Stream pointer (unused).
 * @return 0 on success.
 */

/**
 * @brief Tests if an error occurred during file operations.
 * @param opaque Pointer to FileAccess pointer.
 * @param stream Stream pointer (unused).
 * @return 1 if an error occurred, 0 otherwise.
 */

/**
 * @brief Allocates and zero-initializes memory for zlib operations.
 * @param opaque Opaque pointer (unused).
 * @param items Number of items to allocate.
 * @param size Size of each item in bytes.
 * @return Pointer to allocated memory, or NULL if allocation fails.
 */

/**
 * @brief Frees memory allocated by zipio_alloc.
 * @param opaque Opaque pointer (unused).
 * @param address Pointer to memory to free.
 */

/**
 * @brief Creates a zlib file function definition structure for ZIP operations.
 * @param p_file Pointer to FileAccess pointer used for I/O operations.
 * @return zlib_filefunc_def structure initialized with ZIP I/O callbacks.
 */
#include "zip_io.h"

#include "core/os/copymem.h"

void *zipio_open(void *data, const char *p_fname, int mode) {

	FileAccess *&f = *(FileAccess **)data;

	String fname;
	fname.parse_utf8(p_fname);

	if (mode & ZLIB_FILEFUNC_MODE_WRITE) {
		f = FileAccess::open(fname, FileAccess::WRITE);
	} else {

		f = FileAccess::open(fname, FileAccess::READ);
	}

	if (!f)
		return NULL;

	return data;
}

uLong zipio_read(void *data, void *fdata, void *buf, uLong size) {

	FileAccess *f = *(FileAccess **)data;
	return f->get_buffer((uint8_t *)buf, size);
}

uLong zipio_write(voidpf opaque, voidpf stream, const void *buf, uLong size) {

	FileAccess *f = *(FileAccess **)opaque;
	f->store_buffer((uint8_t *)buf, size);
	return size;
}

long zipio_tell(voidpf opaque, voidpf stream) {

	FileAccess *f = *(FileAccess **)opaque;
	return f->get_position();
}

long zipio_seek(voidpf opaque, voidpf stream, uLong offset, int origin) {

	FileAccess *f = *(FileAccess **)opaque;

	int pos = offset;
	switch (origin) {

		case ZLIB_FILEFUNC_SEEK_CUR:
			pos = f->get_position() + offset;
			break;
		case ZLIB_FILEFUNC_SEEK_END:
			pos = f->get_len() + offset;
			break;
		default:
			break;
	};

	f->seek(pos);
	return 0;
}

int zipio_close(voidpf opaque, voidpf stream) {

	FileAccess *&f = *(FileAccess **)opaque;
	if (f) {
		f->close();
		memdelete(f);
		f = NULL;
	}
	return 0;
}

int zipio_testerror(voidpf opaque, voidpf stream) {

	FileAccess *f = *(FileAccess **)opaque;
	return (f && f->get_error() != OK) ? 1 : 0;
}

voidpf zipio_alloc(voidpf opaque, uInt items, uInt size) {

	voidpf ptr = memalloc(items * size);
	zeromem(ptr, items * size);
	return ptr;
}

void zipio_free(voidpf opaque, voidpf address) {

	memfree(address);
}

zlib_filefunc_def zipio_create_io_from_file(FileAccess **p_file) {

	zlib_filefunc_def io;
	io.opaque = p_file;
	io.zopen_file = zipio_open;
	io.zread_file = zipio_read;
	io.zwrite_file = zipio_write;
	io.ztell_file = zipio_tell;
	io.zseek_file = zipio_seek;
	io.zclose_file = zipio_close;
	io.zerror_file = zipio_testerror;
	io.alloc_mem = zipio_alloc;
	io.free_mem = zipio_free;
	return io;
}

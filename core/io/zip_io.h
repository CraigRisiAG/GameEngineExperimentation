

/**
 * @file zip_io.h
 * @brief ZIP I/O interface for minizip library integration
 * 
 * Provides a set of callback functions that bridge the minizip library with
 * the engine's FileAccess abstraction layer. These functions enable reading,
 * writing, and seeking operations within ZIP archives using the engine's
 * file system.
 * 
 * @note Third-party minizip headers are included here for use in downstream
 * compilation units (e.g., platform-specific export modules). It is recommended
 * to minimize the exposure of third-party headers in public APIs.
 */

/**
 * @brief Opens a file resource for ZIP operations
 * @param data User-defined data pointer passed to minizip
 * @param p_fname File name or path to open
 * @param mode File access mode (read/write)
 * @return Opaque file handle for subsequent operations, or nullptr on failure
 */

/**
 * @brief Reads data from an open ZIP file resource
 * @param data User-defined data pointer
 * @param fdata File data handle returned by zipio_open()
 * @param buf Buffer to store read data
 * @param size Number of bytes to read
 * @return Number of bytes actually read, or 0 on EOF/error
 */

/**
 * @brief Writes data to an open ZIP file resource
 * @param opaque User-defined data pointer
 * @param stream File handle returned by zipio_open()
 * @param buf Data buffer to write
 * @param size Number of bytes to write
 * @return Number of bytes actually written
 */

/**
 * @brief Gets the current file position in a ZIP file resource
 * @param opaque User-defined data pointer
 * @param stream File handle returned by zipio_open()
 * @return Current position in the file
 */

/**
 * @brief Seeks to a position in a ZIP file resource
 * @param opaque User-defined data pointer
 * @param stream File handle returned by zipio_open()
 * @param offset Position offset relative to origin
 * @param origin Seek origin (SEEK_SET, SEEK_CUR, SEEK_END)
 * @return New file position, or -1 on error
 */

/**
 * @brief Closes an open ZIP file resource
 * @param opaque User-defined data pointer
 * @param stream File handle returned by zipio_open()
 * @return 0 on success, non-zero on error
 */

/**
 * @brief Tests for errors on a ZIP file resource
 * @param opaque User-defined data pointer
 * @param stream File handle returned by zipio_open()
 * @return 0 if no error, non-zero if an error occurred
 */

/**
 * @brief Allocates memory for ZIP operations
 * @param opaque User-defined data pointer
 * @param items Number of items to allocate
 * @param size Size of each item in bytes
 * @return Pointer to allocated memory, or nullptr on failure
 */

/**
 * @brief Frees memory allocated by zipio_alloc()
 * @param opaque User-defined data pointer
 * @param address Pointer to memory to free
 */

/**
 * @brief Creates a minizip I/O function definition from a FileAccess instance
 * @param p_file Pointer to FileAccess pointer to use for ZIP operations
 * @return zlib_filefunc_def structure initialized with ZIP I/O callbacks
 */
#ifndef ZIP_IO_H
#define ZIP_IO_H

#include "core/os/file_access.h"

// Not directly used in this header, but assumed available in downstream users
// like platform/*/export/export.cpp. Could be fixed, but probably better to have
// thirdparty includes in as little headers as possible.
#include "thirdparty/minizip/unzip.h"
#include "thirdparty/minizip/zip.h"

void *zipio_open(void *data, const char *p_fname, int mode);
uLong zipio_read(void *data, void *fdata, void *buf, uLong size);
uLong zipio_write(voidpf opaque, voidpf stream, const void *buf, uLong size);

long zipio_tell(voidpf opaque, voidpf stream);
long zipio_seek(voidpf opaque, voidpf stream, uLong offset, int origin);

int zipio_close(voidpf opaque, voidpf stream);

int zipio_testerror(voidpf opaque, voidpf stream);

voidpf zipio_alloc(voidpf opaque, uInt items, uInt size);
void zipio_free(voidpf opaque, voidpf address);

zlib_filefunc_def zipio_create_io_from_file(FileAccess **p_file);

#endif // ZIP_IO_H



/**
 * @class FileAccessMemory
 * @brief In-memory file access implementation for reading and writing data stored in RAM.
 * 
 * This class provides a FileAccess interface for working with data buffers stored in memory
 * rather than on disk. It allows registering named memory buffers and accessing them through
 * the standard file access API.
 * 
 * @note This implementation does not support actual file system operations like checking
 * modification times or Unix permissions.
 * 
 * Usage:
 *   - Register a memory buffer with register_file() before opening
 *   - Open the registered buffer using _open() with the registered name
 *   - Use standard FileAccess methods to read/write data
 *   - Call cleanup() to free registered buffers when done
 */
#ifndef FILE_ACCESS_MEMORY_H
#define FILE_ACCESS_MEMORY_H

#include "core/os/file_access.h"

class FileAccessMemory : public FileAccess {

	uint8_t *data;
	int length;
	mutable int pos;

	static FileAccess *create();

public:
	static void register_file(String p_name, Vector<uint8_t> p_data);
	static void cleanup();

	virtual Error open_custom(const uint8_t *p_data, int p_len); ///< open a file
	virtual Error _open(const String &p_path, int p_mode_flags); ///< open a file
	virtual void close(); ///< close a file
	virtual bool is_open() const; ///< true when file is open

	virtual void seek(size_t p_position); ///< seek to a given position
	virtual void seek_end(int64_t p_position); ///< seek from the end of file
	virtual size_t get_position() const; ///< get position in the file
	virtual size_t get_len() const; ///< get size of the file

	virtual bool eof_reached() const; ///< reading passed EOF

	virtual uint8_t get_8() const; ///< get a byte

	virtual int get_buffer(uint8_t *p_dst, int p_length) const; ///< get an array of bytes

	virtual Error get_error() const; ///< get last error

	virtual void flush();
	virtual void store_8(uint8_t p_byte); ///< store a byte
	virtual void store_buffer(const uint8_t *p_src, int p_length); ///< store an array of bytes

	virtual bool file_exists(const String &p_name); ///< return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file) { return 0; }
	virtual uint32_t _get_unix_permissions(const String &p_file) { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) { return FAILED; }

	FileAccessMemory();
};

#endif // FILE_ACCESS_MEMORY_H

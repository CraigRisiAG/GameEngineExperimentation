

/**
 * @class FileAccessBuffered
 * @brief A buffered file access implementation that provides efficient reading with caching.
 * 
 * This class extends FileAccess to provide buffered I/O operations with configurable cache size.
 * It maintains an internal cache to reduce the number of actual file read operations and improve
 * performance when accessing file data sequentially or randomly.
 * 
 * @note This is an abstract class. Derived classes must implement the read_data_block() method.
 * 
 * @see FileAccess
 */

/**
 * @enum DEFAULT_CACHE_SIZE
 * @brief Default size of the read cache buffer in bytes (128 KB).
 */

/**
 * @var int cache_size
 * @brief The size of the cache buffer in bytes.
 */

/**
 * @var mutable Error last_error
 * @brief Stores the last error that occurred during file operations.
 */

/**
 * @struct File
 * @brief Metadata about the currently opened file.
 * @member bool open - Whether the file is currently open.
 * @member int size - Total size of the file in bytes.
 * @member int offset - Current read position in the file.
 * @member String name - The name/path of the file.
 * @member int access_flags - Flags specifying how the file is accessed.
 */

/**
 * @struct Cache
 * @brief Internal cache buffer for efficient file reading.
 * @member Vector<uint8_t> buffer - The buffer containing cached data.
 * @member int offset - Current position within the cache buffer.
 */

/**
 * @fn virtual int read_data_block(int p_offset, int p_size, uint8_t *p_dest = 0) const
 * @brief Pure virtual method to read a block of data from the file.
 * @param p_offset The offset in the file to read from.
 * @param p_size The number of bytes to read.
 * @param p_dest Optional pointer to destination buffer where read data should be stored.
 * @return The number of bytes actually read.
 * @note Must be implemented by derived classes.
 */

/**
 * @fn void set_cache_size(int p_size)
 * @brief Sets the size of the read cache buffer.
 * @param p_size The desired cache size in bytes.
 */

/**
 * @fn int get_cache_size()
 * @brief Retrieves the current cache buffer size.
 * @return The cache size in bytes.
 */

/**
 * @fn virtual size_t get_position() const
 * @brief Gets the current read position in the file.
 * @return The current position in bytes.
 */

/**
 * @fn virtual size_t get_len() const
 * @brief Gets the total size of the file.
 * @return The file size in bytes.
 */

/**
 * @fn virtual void seek(size_t p_position)
 * @brief Seeks to an absolute position in the file.
 * @param p_position The position to seek to, in bytes from the beginning.
 */

/**
 * @fn virtual void seek_end(int64_t p_position = 0)
 * @brief Seeks to a position relative to the end of the file.
 * @param p_position The offset from the end of the file (typically negative or zero).
 */

/**
 * @fn virtual bool eof_reached() const
 * @brief Checks if the end of file has been reached.
 * @return True if at end of file, false otherwise.
 */

/**
 * @fn virtual uint8_t get_8() const
 * @brief Reads a single byte from the file.
 * @return The byte value read from the current position.
 */

/**
 * @fn virtual int get_buffer(uint8_t *p_dest, int p_length) const
 * @brief Reads a buffer of bytes from the file.
 * @param p_dest Pointer to the destination buffer.
 * @param p_length Number of bytes to read.
 * @return The number of bytes actually read.
 */

/**
 * @fn virtual bool is_open() const
 * @brief Checks if a file is currently open.
 * @return True if a file is open, false otherwise.
 */

/**
 * @fn virtual Error get_error() const
 * @brief Retrieves the last error that occurred.
 * @return The last error code.
 */

/**
 * @fn FileAccessBuffered()
 * @brief Constructor for FileAccessBuffered.
 */

/**
 * @fn virtual ~FileAccessBuffered()
 * @brief Destructor for FileAccessBuffered.
 */
#ifndef FILE_ACCESS_BUFFERED_H
#define FILE_ACCESS_BUFFERED_H

#include "core/os/file_access.h"

#include "core/ustring.h"

class FileAccessBuffered : public FileAccess {

public:
	enum {
		DEFAULT_CACHE_SIZE = 128 * 1024,
	};

private:
	int cache_size;

	int cache_data_left() const;
	mutable Error last_error;

protected:
	Error set_error(Error p_error) const;

	mutable struct File {

		bool open;
		int size;
		int offset;
		String name;
		int access_flags;
	} file;

	mutable struct Cache {

		Vector<uint8_t> buffer;
		int offset;
	} cache;

	virtual int read_data_block(int p_offset, int p_size, uint8_t *p_dest = 0) const = 0;

	void set_cache_size(int p_size);
	int get_cache_size();

public:
	virtual size_t get_position() const; ///< get position in the file
	virtual size_t get_len() const; ///< get size of the file

	virtual void seek(size_t p_position); ///< seek to a given position
	virtual void seek_end(int64_t p_position = 0); ///< seek from the end of file

	virtual bool eof_reached() const;

	virtual uint8_t get_8() const;
	virtual int get_buffer(uint8_t *p_dest, int p_length) const; ///< get an array of bytes

	virtual bool is_open() const;

	virtual Error get_error() const;

	FileAccessBuffered();
	virtual ~FileAccessBuffered();
};

#endif

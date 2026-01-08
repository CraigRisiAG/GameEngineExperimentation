
/**
 * @class FileAccessBuffered
 * @brief A buffered file access utility class for efficient file reading with caching.
 * 
 * This class provides buffered access to files, implementing a cache mechanism to optimize
 * read operations. It maintains file position tracking and handles data caching to reduce
 * direct file I/O operations.
 * 
 * @method set_error(Error p_error)
 * @brief Sets the last error state.
 * @param p_error The error code to set.
 * @return The error code that was set.
 * 
 * @method set_cache_size(int p_size)
 * @brief Sets the size of the internal cache buffer.
 * @param p_size The desired cache size in bytes.
 * 
 * @method get_cache_size()
 * @brief Retrieves the current cache size.
 * @return The cache size in bytes.
 * 
 * @method cache_data_left()
 * @brief Calculates the amount of cached data remaining from the current file position.
 * @return Number of bytes available in cache, or the result of read_data_block if cache miss.
 * 
 * @method seek(size_t p_position)
 * @brief Moves the file position to an absolute position.
 * @param p_position The target position in the file.
 * 
 * @method seek_end(int64_t p_position)
 * @brief Seeks to a position relative to the end of the file.
 * @param p_position The offset from the end (negative for positions before end).
 * 
 * @method get_position()
 * @brief Gets the current file position.
 * @return The current file offset.
 * 
 * @method get_len()
 * @brief Gets the total file size.
 * @return The file size in bytes.
 * 
 * @method eof_reached()
 * @brief Checks if the end of file has been reached.
 * @return True if file offset exceeds file size, false otherwise.
 * 
 * @method get_8()
 * @brief Reads a single byte from the file.
 * @return The byte read, or 0 if file is not open.
 * @note Increments file position after reading.
 * 
 * @method get_buffer(uint8_t *p_dest, int p_length)
 * @brief Reads a buffer of data from the file into destination memory.
 * @param p_dest Pointer to destination buffer.
 * @param p_length Number of bytes to read.
 * @return Number of bytes actually read, or -1 if file is not open.
 * 
 * @method is_open()
 * @brief Checks if the file is currently open.
 * @return True if file is open, false otherwise.
 * 
 * @method get_error()
 * @brief Retrieves the last error that occurred.
 * @return The last error code.
 * 
 * @constructor FileAccessBuffered()
 * @brief Initializes a new FileAccessBuffered instance with default cache size.
 * 
 * @destructor ~FileAccessBuffered()
 * @brief Cleans up the FileAccessBuffered instance.
 */
#include "file_access_buffered.h"

#include "core/error_macros.h"

Error FileAccessBuffered::set_error(Error p_error) const {

	return (last_error = p_error);
}

void FileAccessBuffered::set_cache_size(int p_size) {

	cache_size = p_size;
}

int FileAccessBuffered::get_cache_size() {

	return cache_size;
}

int FileAccessBuffered::cache_data_left() const {

	if (file.offset >= file.size) {
		return 0;
	}

	if (cache.offset == -1 || file.offset < cache.offset || file.offset >= cache.offset + cache.buffer.size()) {

		return read_data_block(file.offset, cache_size);
	}

	return cache.buffer.size() - (file.offset - cache.offset);
}

void FileAccessBuffered::seek(size_t p_position) {

	file.offset = p_position;
}

void FileAccessBuffered::seek_end(int64_t p_position) {

	file.offset = file.size + p_position;
}

size_t FileAccessBuffered::get_position() const {

	return file.offset;
}

size_t FileAccessBuffered::get_len() const {

	return file.size;
}

bool FileAccessBuffered::eof_reached() const {

	return file.offset > file.size;
}

uint8_t FileAccessBuffered::get_8() const {

	ERR_FAIL_COND_V_MSG(!file.open, 0, "Can't get data, when file is not opened.");

	uint8_t byte = 0;
	if (cache_data_left() >= 1) {

		byte = cache.buffer[file.offset - cache.offset];
	}

	++file.offset;

	return byte;
}

int FileAccessBuffered::get_buffer(uint8_t *p_dest, int p_length) const {

	ERR_FAIL_COND_V_MSG(!file.open, -1, "Can't get buffer, when file is not opened.");

	if (p_length > cache_size) {

		int total_read = 0;

		if (!(cache.offset == -1 || file.offset < cache.offset || file.offset >= cache.offset + cache.buffer.size())) {

			int size = (cache.buffer.size() - (file.offset - cache.offset));
			size = size - (size % 4);
			//const uint8_t* read = cache.buffer.ptr();
			//memcpy(p_dest, read.ptr() + (file.offset - cache.offset), size);
			memcpy(p_dest, cache.buffer.ptr() + (file.offset - cache.offset), size);
			p_dest += size;
			p_length -= size;
			file.offset += size;
			total_read += size;
		}

		int err = read_data_block(file.offset, p_length, p_dest);
		if (err >= 0) {
			total_read += err;
			file.offset += err;
		}

		return total_read;
	}

	int to_read = p_length;
	int total_read = 0;
	while (to_read > 0) {

		int left = cache_data_left();
		if (left == 0) {
			file.offset += to_read;
			return total_read;
		}
		if (left < 0) {
			return left;
		}

		int r = MIN(left, to_read);
		//const uint8_t* read = cache.buffer.ptr();
		//memcpy(p_dest+total_read, &read.ptr()[file.offset - cache.offset], r);
		memcpy(p_dest + total_read, cache.buffer.ptr() + (file.offset - cache.offset), r);

		file.offset += r;
		total_read += r;
		to_read -= r;
	}

	return p_length;
}

bool FileAccessBuffered::is_open() const {

	return file.open;
}

Error FileAccessBuffered::get_error() const {

	return last_error;
}

FileAccessBuffered::FileAccessBuffered() {

	cache_size = DEFAULT_CACHE_SIZE;
}

FileAccessBuffered::~FileAccessBuffered() {
}

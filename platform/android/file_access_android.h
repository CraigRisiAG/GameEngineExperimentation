
/**
 * @class FileAccessAndroid
 * @brief Android-specific file access implementation using AAssetManager.
 * 
 * Provides file I/O operations for Android assets through the NDK AAssetManager.
 * Handles reading from the APK asset directory with support for seeking, 
 * position tracking, and EOF detection.
 * 
 * @note Requires AAssetManager to be set via the static asset_manager pointer
 *       before any file operations can be performed.
 */

/**
 * @brief Creates and returns a new FileAccessAndroid instance.
 * @return Pointer to a newly allocated FileAccessAndroid object cast as FileAccess.
 */

/**
 * @brief Opens a file from the Android assets directory.
 * @param p_path The path to the file within the asset directory.
 * @param p_mode_flags File open mode flags.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Closes the currently opened asset file.
 */

/**
 * @brief Checks if a file is currently open.
 * @return True if a file is open, false otherwise.
 */

/**
 * @brief Moves the file position to the specified offset.
 * @param p_position The absolute position to seek to.
 */

/**
 * @brief Moves the file position relative to the end of the file.
 * @param p_position Offset from the end (default 0 for end of file).
 */

/**
 * @brief Gets the current position in the file.
 * @return Current byte position in the file.
 */

/**
 * @brief Gets the total size of the file.
 * @return Size of the file in bytes.
 */

/**
 * @brief Checks if the end of file has been reached.
 * @return True if EOF has been reached, false otherwise.
 */

/**
 * @brief Reads a single byte from the file.
 * @return The byte read from the current position.
 */

/**
 * @brief Reads a buffer of data from the file.
 * @param p_dst Destination buffer for the data.
 * @param p_length Number of bytes to read.
 * @return Number of bytes actually read.
 */

/**
 * @brief Gets the last error that occurred during file operations.
 * @return Error code of the last operation.
 */

/**
 * @brief Flushes any pending write operations (no-op for asset files).
 */

/**
 * @brief Writes a single byte to the file.
 * @param p_dest The byte value to write.
 * @note Asset files are read-only, this operation may not be supported.
 */

/**
 * @brief Checks if a file exists in the asset directory.
 * @param p_path The path to check.
 * @return True if the file exists, false otherwise.
 */

/**
 * @brief Gets the last modification time of a file.
 * @param p_file The file path.
 * @return Modified time (returns 0 as assets don't support this).
 */

/**
 * @brief Gets Unix file permissions.
 * @param p_file The file path.
 * @return File permissions (returns 0 as assets don't support this).
 */

/**
 * @brief Sets Unix file permissions.
 * @param p_file The file path.
 * @param p_permissions The permissions to set.
 * @return Error code (returns FAILED as assets are read-only).
 */
#ifndef FILE_ACCESS_ANDROID_H
#define FILE_ACCESS_ANDROID_H

#include "core/os/file_access.h"
#include <android/asset_manager.h>
#include <android/log.h>
#include <stdio.h>
//#include <android_native_app_glue.h>

class FileAccessAndroid : public FileAccess {

	static FileAccess *create_android();
	mutable AAsset *a;
	mutable size_t len;
	mutable size_t pos;
	mutable bool eof;

public:
	static AAssetManager *asset_manager;

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

	virtual bool file_exists(const String &p_path); ///< return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file) { return 0; }
	virtual uint32_t _get_unix_permissions(const String &p_file) { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) { return FAILED; }

	//static void make_default();

	FileAccessAndroid();
	~FileAccessAndroid();
};

#endif // FILE_ACCESS_ANDROID_H

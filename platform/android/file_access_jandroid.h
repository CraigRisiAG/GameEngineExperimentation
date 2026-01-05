
/**
 * @class FileAccessJAndroid
 * @brief Android-specific file access implementation using JNI.
 * 
 * This class provides file I/O operations on Android by interfacing with Java
 * methods through JNI (Java Native Interface). It inherits from FileAccess and
 * implements platform-specific file operations for the Android platform.
 * 
 * @details
 * The class maintains static references to Java objects and method IDs required
 * for JNI calls. Each file instance is identified by an integer ID that maps to
 * the corresponding Java file handle.
 * 
 * Supported operations include:
 * - Opening and closing files
 * - Reading and writing data
 * - Seeking to specific positions
 * - Checking file existence and status
 * 
 * @note This implementation is specific to Android and relies on JNI bindings
 * to communicate with the Java layer for actual file system access.
 * 
 * @see FileAccess
 * @see java_godot_lib_jni.h
 */
#ifndef FILE_ACCESS_JANDROID_H
#define FILE_ACCESS_JANDROID_H

#include "core/os/file_access.h"
#include "java_godot_lib_jni.h"
class FileAccessJAndroid : public FileAccess {

	static jobject io;
	static jclass cls;

	static jmethodID _file_open;
	static jmethodID _file_get_size;
	static jmethodID _file_seek;
	static jmethodID _file_tell;
	static jmethodID _file_eof;
	static jmethodID _file_read;
	static jmethodID _file_close;

	int id;
	static FileAccess *create_jandroid();

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

	virtual bool file_exists(const String &p_path); ///< return true if a file exists

	static void setup(jobject p_io);

	virtual uint64_t _get_modified_time(const String &p_file) { return 0; }
	virtual uint32_t _get_unix_permissions(const String &p_file) { return 0; }
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions) { return FAILED; }

	FileAccessJAndroid();
	~FileAccessJAndroid();
};

#endif // FILE_ACCESS_JANDROID_H

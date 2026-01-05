
/**
 * @file file_access_jandroid.cpp
 * @brief Android file access implementation using JNI.
 * 
 * This file provides file I/O operations on Android through JNI calls to Java methods.
 * It implements the FileAccess interface for reading files from Android resources and storage.
 * 
 * @class FileAccessJAndroid
 * @brief Provides file access functionality on Android using Java/JNI bridge.
 * 
 * This class manages file operations by delegating to Java methods through JNI.
 * It maintains a file handle (id) and uses cached JNI method IDs for performance.
 * 
 * @static jobject io - Reference to the Java I/O object that handles file operations
 * @static jclass cls - Cached Java class reference
 * @static jmethodID _file_open - Method ID for opening files
 * @static jmethodID _file_get_size - Method ID for getting file size
 * @static jmethodID _file_seek - Method ID for seeking file position
 * @static jmethodID _file_read - Method ID for reading file data
 * @static jmethodID _file_tell - Method ID for getting current file position
 * @static jmethodID _file_eof - Method ID for checking end-of-file
 * @static jmethodID _file_close - Method ID for closing files
 * 
 * @method static FileAccess* create_jandroid() - Factory method to create a new FileAccessJAndroid instance
 * @method Error _open(const String &p_path, int p_mode_flags) - Opens a file from specified path
 * @method void close() - Closes the currently open file
 * @method bool is_open() const - Checks if a file is currently open
 * @method void seek(size_t p_position) - Seeks to absolute position in file
 * @method void seek_end(int64_t p_position) - Seeks relative to end of file
 * @method size_t get_position() const - Returns current file position
 * @method size_t get_len() const - Returns total file size
 * @method bool eof_reached() const - Checks if end-of-file has been reached
 * @method uint8_t get_8() const - Reads single byte from file
 * @method int get_buffer(uint8_t *p_dst, int p_length) const - Reads buffer of bytes from file
 * @method Error get_error() const - Returns current error status
 * @method void flush() - Flushes file buffer (no-op on Android)
 * @method void store_8(uint8_t p_dest) - Writes single byte (not implemented)
 * @method bool file_exists(const String &p_path) - Checks if file exists at given path
 * @method static void setup(jobject p_io) - Initializes JNI method IDs and references
 * @method FileAccessJAndroid() - Constructor, initializes file handle to 0
 * @method ~FileAccessJAndroid() - Destructor, closes file if open
 */
#include "file_access_jandroid.h"
#include "core/os/os.h"
#include "thread_jandroid.h"
#include <unistd.h>

jobject FileAccessJAndroid::io = NULL;
jclass FileAccessJAndroid::cls;
jmethodID FileAccessJAndroid::_file_open = 0;
jmethodID FileAccessJAndroid::_file_get_size = 0;
jmethodID FileAccessJAndroid::_file_seek = 0;
jmethodID FileAccessJAndroid::_file_read = 0;
jmethodID FileAccessJAndroid::_file_tell = 0;
jmethodID FileAccessJAndroid::_file_eof = 0;
jmethodID FileAccessJAndroid::_file_close = 0;

FileAccess *FileAccessJAndroid::create_jandroid() {

	return memnew(FileAccessJAndroid);
}

Error FileAccessJAndroid::_open(const String &p_path, int p_mode_flags) {

	if (is_open())
		close();

	String path = fix_path(p_path).simplify_path();
	if (path.begins_with("/"))
		path = path.substr(1, path.length());
	else if (path.begins_with("res://"))
		path = path.substr(6, path.length());

	JNIEnv *env = ThreadAndroid::get_env();

	jstring js = env->NewStringUTF(path.utf8().get_data());
	int res = env->CallIntMethod(io, _file_open, js, (p_mode_flags & WRITE) ? true : false);
	env->DeleteLocalRef(js);

	OS::get_singleton()->print("fopen: '%s' ret %i\n", path.utf8().get_data(), res);

	if (res <= 0)
		return ERR_FILE_CANT_OPEN;
	id = res;

	return OK;
}

void FileAccessJAndroid::close() {

	if (!is_open())
		return;

	JNIEnv *env = ThreadAndroid::get_env();

	env->CallVoidMethod(io, _file_close, id);
	id = 0;
}

bool FileAccessJAndroid::is_open() const {

	return id != 0;
}

void FileAccessJAndroid::seek(size_t p_position) {

	JNIEnv *env = ThreadAndroid::get_env();

	ERR_FAIL_COND_MSG(!is_open(), "File must be opened before use.");
	env->CallVoidMethod(io, _file_seek, id, p_position);
}

void FileAccessJAndroid::seek_end(int64_t p_position) {

	ERR_FAIL_COND_MSG(!is_open(), "File must be opened before use.");

	seek(get_len());
}

size_t FileAccessJAndroid::get_position() const {

	JNIEnv *env = ThreadAndroid::get_env();
	ERR_FAIL_COND_V_MSG(!is_open(), 0, "File must be opened before use.");
	return env->CallIntMethod(io, _file_tell, id);
}

size_t FileAccessJAndroid::get_len() const {

	JNIEnv *env = ThreadAndroid::get_env();
	ERR_FAIL_COND_V_MSG(!is_open(), 0, "File must be opened before use.");
	return env->CallIntMethod(io, _file_get_size, id);
}

bool FileAccessJAndroid::eof_reached() const {

	JNIEnv *env = ThreadAndroid::get_env();
	ERR_FAIL_COND_V_MSG(!is_open(), 0, "File must be opened before use.");
	return env->CallIntMethod(io, _file_eof, id);
}

uint8_t FileAccessJAndroid::get_8() const {

	ERR_FAIL_COND_V_MSG(!is_open(), 0, "File must be opened before use.");
	uint8_t byte;
	get_buffer(&byte, 1);
	return byte;
}
int FileAccessJAndroid::get_buffer(uint8_t *p_dst, int p_length) const {

	ERR_FAIL_COND_V_MSG(!is_open(), 0, "File must be opened before use.");
	if (p_length == 0)
		return 0;
	JNIEnv *env = ThreadAndroid::get_env();

	jbyteArray jca = (jbyteArray)env->CallObjectMethod(io, _file_read, id, p_length);

	int len = env->GetArrayLength(jca);
	env->GetByteArrayRegion(jca, 0, len, (jbyte *)p_dst);
	env->DeleteLocalRef((jobject)jca);

	return len;
}

Error FileAccessJAndroid::get_error() const {

	if (eof_reached())
		return ERR_FILE_EOF;
	return OK;
}

void FileAccessJAndroid::flush() {
}

void FileAccessJAndroid::store_8(uint8_t p_dest) {
}

bool FileAccessJAndroid::file_exists(const String &p_path) {

	JNIEnv *env = ThreadAndroid::get_env();

	String path = fix_path(p_path).simplify_path();
	if (path.begins_with("/"))
		path = path.substr(1, path.length());
	else if (path.begins_with("res://"))
		path = path.substr(6, path.length());

	jstring js = env->NewStringUTF(path.utf8().get_data());
	int res = env->CallIntMethod(io, _file_open, js, false);
	if (res <= 0) {
		env->DeleteLocalRef(js);
		return false;
	}
	env->CallVoidMethod(io, _file_close, res);
	env->DeleteLocalRef(js);
	return true;
}

void FileAccessJAndroid::setup(jobject p_io) {

	io = p_io;
	JNIEnv *env = ThreadAndroid::get_env();

	jclass c = env->GetObjectClass(io);
	cls = (jclass)env->NewGlobalRef(c);

	_file_open = env->GetMethodID(cls, "file_open", "(Ljava/lang/String;Z)I");
	_file_get_size = env->GetMethodID(cls, "file_get_size", "(I)I");
	_file_tell = env->GetMethodID(cls, "file_tell", "(I)I");
	_file_eof = env->GetMethodID(cls, "file_eof", "(I)Z");
	_file_seek = env->GetMethodID(cls, "file_seek", "(II)V");
	_file_read = env->GetMethodID(cls, "file_read", "(II)[B");
	_file_close = env->GetMethodID(cls, "file_close", "(I)V");
}

FileAccessJAndroid::FileAccessJAndroid() {

	id = 0;
}

FileAccessJAndroid::~FileAccessJAndroid() {

	if (is_open())
		close();
}

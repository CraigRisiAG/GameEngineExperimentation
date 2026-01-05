
/**
 * @file file_access_android.cpp
 * @brief Android platform-specific file access implementation using AAssetManager
 * 
 * This file provides file access functionality for Android applications by wrapping
 * the Android NDK's AAssetManager API. It enables reading files from the application's
 * asset directory in a way compatible with the engine's FileAccess interface.
 * 
 * @note Write operations are not supported on Android assets and will fail with ERR_UNAVAILABLE.
 * @note This implementation only supports read-only access to asset files.
 * 
 * @class FileAccessAndroid
 * @brief Implements file access operations for Android assets
 * 
 * Static Members:
 * - asset_manager: Pointer to the AAssetManager instance managing application assets
 * 
 * Key Methods:
 * - create_android(): Factory method to instantiate FileAccessAndroid objects
 * - _open(): Opens an asset file for reading, supporting both absolute and resource paths
 * - close(): Closes the currently open asset
 * - seek(): Seeks to an absolute position in the file
 * - seek_end(): Seeks relative to the end of the file
 * - get_8(): Reads a single byte from the file
 * - get_buffer(): Reads multiple bytes into a buffer
 * - file_exists(): Checks if an asset file exists
 * 
 * Path Handling:
 * - Supports absolute paths (strips leading '/')
 * - Supports resource paths (strips 'res://' prefix)
 * - Normalizes paths using simplify_path()
 * 
 * @see FileAccess base class for interface contract
 * @see AAssetManager Android NDK documentation
 */
#include "file_access_android.h"
#include "core/print_string.h"

AAssetManager *FileAccessAndroid::asset_manager = NULL;

/*void FileAccessAndroid::make_default() {

	create_func=create_android;
}*/

FileAccess *FileAccessAndroid::create_android() {

	return memnew(FileAccessAndroid);
}

Error FileAccessAndroid::_open(const String &p_path, int p_mode_flags) {

	String path = fix_path(p_path).simplify_path();
	if (path.begins_with("/"))
		path = path.substr(1, path.length());
	else if (path.begins_with("res://"))
		path = path.substr(6, path.length());

	ERR_FAIL_COND_V(p_mode_flags & FileAccess::WRITE, ERR_UNAVAILABLE); //can't write on android..
	a = AAssetManager_open(asset_manager, path.utf8().get_data(), AASSET_MODE_STREAMING);
	if (!a)
		return ERR_CANT_OPEN;
	//ERR_FAIL_COND_V(!a,ERR_FILE_NOT_FOUND);
	len = AAsset_getLength(a);
	pos = 0;
	eof = false;

	return OK;
}

void FileAccessAndroid::close() {

	if (!a)
		return;
	AAsset_close(a);
	a = NULL;
}

bool FileAccessAndroid::is_open() const {

	return a != NULL;
}

void FileAccessAndroid::seek(size_t p_position) {

	ERR_FAIL_COND(!a);
	AAsset_seek(a, p_position, SEEK_SET);
	pos = p_position;
	if (pos > len) {
		pos = len;
		eof = true;
	} else {
		eof = false;
	}
}

void FileAccessAndroid::seek_end(int64_t p_position) {

	ERR_FAIL_COND(!a);
	AAsset_seek(a, p_position, SEEK_END);
	pos = len + p_position;
}

size_t FileAccessAndroid::get_position() const {

	return pos;
}

size_t FileAccessAndroid::get_len() const {

	return len;
}

bool FileAccessAndroid::eof_reached() const {

	return eof;
}

uint8_t FileAccessAndroid::get_8() const {

	if (pos >= len) {
		eof = true;
		return 0;
	}

	uint8_t byte;
	AAsset_read(a, &byte, 1);
	pos++;
	return byte;
}

int FileAccessAndroid::get_buffer(uint8_t *p_dst, int p_length) const {

	off_t r = AAsset_read(a, p_dst, p_length);

	if (pos + p_length > len) {
		eof = true;
	}

	if (r >= 0) {

		pos += r;
		if (pos > len) {
			pos = len;
		}
	}
	return r;
}

Error FileAccessAndroid::get_error() const {

	return eof ? ERR_FILE_EOF : OK; //not sure what else it may happen
}

void FileAccessAndroid::flush() {

	ERR_FAIL();
}

void FileAccessAndroid::store_8(uint8_t p_dest) {

	ERR_FAIL();
}

bool FileAccessAndroid::file_exists(const String &p_path) {

	String path = fix_path(p_path).simplify_path();
	if (path.begins_with("/"))
		path = path.substr(1, path.length());
	else if (path.begins_with("res://"))
		path = path.substr(6, path.length());

	AAsset *at = AAssetManager_open(asset_manager, path.utf8().get_data(), AASSET_MODE_STREAMING);

	if (!at)
		return false;

	AAsset_close(at);
	return true;
}

FileAccessAndroid::FileAccessAndroid() {
	a = NULL;
	eof = false;
}

FileAccessAndroid::~FileAccessAndroid() {
	close();
}

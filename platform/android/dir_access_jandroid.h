
/**
 * @class DirAccessJAndroid
 * @brief Android-specific directory access implementation using JNI.
 * 
 * This class provides directory listing and manipulation functionality for Android platforms
 * by interfacing with Java code through JNI (Java Native Interface). It extends the base
 * DirAccess class to provide platform-specific directory operations.
 * 
 * @details
 * The class maintains static references to Java objects and methods for directory operations,
 * including opening, reading, and closing directories. Each instance tracks the current
 * directory state and the current file being enumerated.
 * 
 * Thread Safety: Static members are shared across all instances. Ensure proper synchronization
 * if used in multi-threaded contexts.
 * 
 * @note JNI setup must be performed via setup() before using directory operations.
 */
#ifndef DIR_ACCESS_JANDROID_H
#define DIR_ACCESS_JANDROID_H

#include "core/os/dir_access.h"
#include "java_godot_lib_jni.h"
#include <stdio.h>

class DirAccessJAndroid : public DirAccess {

	//AAssetDir* aad;

	static jobject io;
	static jclass cls;

	static jmethodID _dir_open;
	static jmethodID _dir_next;
	static jmethodID _dir_close;
	static jmethodID _dir_is_dir;

	int id;

	String current_dir;
	String current;

	static DirAccess *create_fs();

public:
	virtual Error list_dir_begin(); ///< This starts dir listing
	virtual String get_next();
	virtual bool current_is_dir() const;
	virtual bool current_is_hidden() const;
	virtual void list_dir_end(); ///<

	virtual int get_drive_count();
	virtual String get_drive(int p_drive);

	virtual Error change_dir(String p_dir); ///< can be relative or absolute, return false on success
	virtual String get_current_dir(bool p_include_drive = true); ///< return current dir location

	virtual bool file_exists(String p_file);
	virtual bool dir_exists(String p_dir);

	virtual Error make_dir(String p_dir);

	virtual Error rename(String p_from, String p_to);
	virtual Error remove(String p_name);

	virtual String get_filesystem_type() const;

	//virtual FileType get_file_type() const;
	size_t get_space_left();

	static void setup(jobject p_io);

	DirAccessJAndroid();
	~DirAccessJAndroid();
};

#endif // DIR_ACCESS_JANDROID_H

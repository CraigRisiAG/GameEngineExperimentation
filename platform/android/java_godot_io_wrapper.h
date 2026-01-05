
/// @file java_godot_io_wrapper.h
/// @brief JNI wrapper class for interfacing with GodotIO Java class
/// 
/// This header defines the GodotIOJavaWrapper class, which provides C++ bindings
/// to Java methods defined in org/godotengine/godot/GodotIO.java. It serves as
/// a bridge layer allowing C++ code to call Java-side functionality on Android.
/// 
/// The wrapper manages:
/// - URI opening operations
/// - File system directory access (user data, system directories)
/// - Device information retrieval (locale, model, unique ID, screen DPI)
/// - Virtual keyboard display and management
/// - Screen orientation control
/// - Video playback functionality
/// 
/// All Java method IDs are cached during construction for efficient repeated calls.
/// 
/// @see GodotIO.java

/// @class GodotIOJavaWrapper
/// @brief JNI wrapper for calling GodotIO Java methods from C++
/// 
/// Maintains JNI references to the GodotIO instance and caches method IDs
/// for zero-overhead calling of Java methods. Handles automatic cleanup of
/// JNI references in the destructor.
/// 
/// @note Instances should be created with a valid JNIEnv and GodotIO jobject.
/// Method calls will fail if the Java instance becomes invalid.
// note, swapped java and godot around in the file name so all the java
// wrappers are together

#ifndef JAVA_GODOT_IO_WRAPPER_H
#define JAVA_GODOT_IO_WRAPPER_H

#include <android/log.h>
#include <jni.h>

#include "string_android.h"

// Class that makes functions in java/src/org/godotengine/godot/GodotIO.java callable from C++
class GodotIOJavaWrapper {
private:
	jobject godot_io_instance;
	jclass cls;

	jmethodID _open_URI = 0;
	jmethodID _get_data_dir = 0;
	jmethodID _get_locale = 0;
	jmethodID _get_model = 0;
	jmethodID _get_screen_DPI = 0;
	jmethodID _get_unique_id = 0;
	jmethodID _show_keyboard = 0;
	jmethodID _hide_keyboard = 0;
	jmethodID _set_screen_orientation = 0;
	jmethodID _get_system_dir = 0;
	jmethodID _play_video = 0;
	jmethodID _is_video_playing = 0;
	jmethodID _pause_video = 0;
	jmethodID _stop_video = 0;

public:
	GodotIOJavaWrapper(JNIEnv *p_env, jobject p_godot_io_instance);
	~GodotIOJavaWrapper();

	jobject get_instance();

	Error open_uri(const String &p_uri);
	String get_user_data_dir();
	String get_locale();
	String get_model();
	int get_screen_dpi();
	String get_unique_id();
	bool has_vk();
	void show_vk(const String &p_existing, int p_max_input_length);
	void hide_vk();
	int get_vk_height();
	void set_vk_height(int p_height);
	void set_screen_orientation(int p_orient);
	String get_system_dir(int p_dir);
	void play_video(const String &p_path);
	bool is_video_playing();
	void pause_video();
	void stop_video();
};

#endif /* !JAVA_GODOT_IO_WRAPPER_H */

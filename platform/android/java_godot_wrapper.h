
/**
 * @class GodotJavaWrapper
 * @brief Wrapper class for calling Java methods from C++ in the Godot Android engine.
 * 
 * This class provides C++ bindings to Java methods defined in org/godotengine/godot/Godot.java,
 * enabling seamless communication between the native C++ layer and the Java/Android layer.
 * 
 * @note The class name was intentionally swapped (Java_Godot instead of Godot_Java) to
 *       organize all Java wrappers together in the file structure.
 * 
 * @section Usage
 * Instantiate with a valid JNI environment and Godot instance object.
 * All public methods automatically handle JNI method lookups and invocations.
 * 
 * @section Methods
 * - Video/Graphics: on_video_init(), get_gles_version_code()
 * - Activity Management: restart(), force_quit(), is_activity_resumed()
 * - Screen Control: set_keep_screen_on()
 * - User Interaction: alert(), vibrate()
 * - Clipboard Operations: get_clipboard(), set_clipboard(), has_get_clipboard(), has_set_clipboard()
 * - Permission Handling: request_permission(), request_permissions(), get_granted_permissions()
 * - Input: init_input_devices(), get_input_fallback_mapping()
 * - Surface Management: get_surface()
 * - Lifecycle: on_godot_main_loop_started()
 * - Utilities: get_activity(), get_class_loader(), get_member_object()
 */
// note, swapped java and godot around in the file name so all the java
// wrappers are together

#ifndef JAVA_GODOT_WRAPPER_H
#define JAVA_GODOT_WRAPPER_H

#include <android/log.h>
#include <jni.h>

#include "string_android.h"

// Class that makes functions in java/src/org/godotengine/godot/Godot.java callable from C++
class GodotJavaWrapper {
private:
	jobject godot_instance;
	jclass cls;

	jmethodID _on_video_init = 0;
	jmethodID _restart = 0;
	jmethodID _finish = 0;
	jmethodID _set_keep_screen_on = 0;
	jmethodID _alert = 0;
	jmethodID _get_GLES_version_code = 0;
	jmethodID _get_clipboard = 0;
	jmethodID _set_clipboard = 0;
	jmethodID _request_permission = 0;
	jmethodID _request_permissions = 0;
	jmethodID _get_granted_permissions = 0;
	jmethodID _init_input_devices = 0;
	jmethodID _get_surface = 0;
	jmethodID _is_activity_resumed = 0;
	jmethodID _vibrate = 0;
	jmethodID _get_input_fallback_mapping = 0;
	jmethodID _on_godot_main_loop_started = 0;

public:
	GodotJavaWrapper(JNIEnv *p_env, jobject p_godot_instance);
	~GodotJavaWrapper();

	jobject get_activity();
	jobject get_member_object(const char *p_name, const char *p_class, JNIEnv *p_env = NULL);

	jobject get_class_loader();

	void on_video_init(JNIEnv *p_env = NULL);
	void on_godot_main_loop_started(JNIEnv *p_env = NULL);
	void restart(JNIEnv *p_env = NULL);
	void force_quit(JNIEnv *p_env = NULL);
	void set_keep_screen_on(bool p_enabled);
	void alert(const String &p_message, const String &p_title);
	int get_gles_version_code();
	bool has_get_clipboard();
	String get_clipboard();
	bool has_set_clipboard();
	void set_clipboard(const String &p_text);
	bool request_permission(const String &p_name);
	bool request_permissions();
	Vector<String> get_granted_permissions() const;
	void init_input_devices();
	jobject get_surface();
	bool is_activity_resumed();
	void vibrate(int p_duration_ms);
	String get_input_fallback_mapping();
};

#endif /* !JAVA_GODOT_WRAPPER_H */

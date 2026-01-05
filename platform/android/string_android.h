
/**
 * @file string_android.h
 * @brief Android JNI string conversion utilities for Godot Engine.
 * 
 * This header provides functionality to convert JNI jstring objects to Godot String instances.
 * It abstracts the complexity of JNI string handling and automatic environment management.
 * 
 * @note Requires linkage with thread_jandroid.h for JNI environment access.
 */
#ifndef STRING_ANDROID_H
#define STRING_ANDROID_H
#include "core/ustring.h"
#include "thread_jandroid.h"
#include <jni.h>

/**
 * Converts JNI jstring to Godot String.
 * @param source Source JNI string. If null an empty string is returned.
 * @param env JNI environment instance. If null obtained by ThreadAndroid::get_env().
 * @return Godot string instance.
 */
static inline String jstring_to_string(jstring source, JNIEnv *env = NULL) {
	String result;
	if (source) {
		if (!env) {
			env = ThreadAndroid::get_env();
		}
		const char *const source_utf8 = env->GetStringUTFChars(source, NULL);
		if (source_utf8) {
			result.parse_utf8(source_utf8);
			env->ReleaseStringUTFChars(source, source_utf8);
		}
	}
	return result;
}

#endif // STRING_ANDROID_H

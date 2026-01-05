
/**
 * @file java_godot_lib_jni.h
 * @brief JNI interface declarations for Godot Engine on Android
 * 
 * This header file declares all native C++ functions that can be called from Java code
 * in the Godot Android implementation. These functions serve as callbacks from the Java
 * layer to the C++ game engine core.
 * 
 * @note These declarations correspond to the Java methods defined in
 *       java/src/org/godotengine/godot/GodotLib.java
 * 
 * The functions are organized into the following categories:
 * - **Lifecycle**: initialize, ondestroy, setup, resize, newcontext
 * - **Input Events**: step, back, touch, hover, doubletap, scroll, key
 * - **Joystick Input**: joybutton, joyaxis, joyhat, joyconnectionchanged
 * - **Sensor Data**: accelerometer, gravity, magnetometer, gyroscope
 * - **Audio**: audio
 * - **Focus Management**: focusin, focusout
 * - **Script Interaction**: getGlobal, callobject, calldeferred
 * - **System Events**: setVirtualKeyboardHeight, requestPermissionResult,
 *                      onRendererResumed, onRendererPaused
 * 
 * All functions follow the JNI naming convention:
 * Java_<package>_<class>_<method>(JNIEnv *env, jclass clazz, ...)
 */
#ifndef JAVA_GODOT_LIB_JNI_H
#define JAVA_GODOT_LIB_JNI_H

#include <android/log.h>
#include <jni.h>

// These functions can be called from within JAVA and are the means by which our JAVA implementation calls back into our C++ code.
// See java/src/org/godotengine/godot/GodotLib.java for the JAVA side of this (yes that's why we have the long names)
extern "C" {
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_initialize(JNIEnv *env, jclass clazz, jobject activity, jobject p_asset_manager, jboolean p_use_apk_expansion);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_ondestroy(JNIEnv *env, jclass clazz, jobject activity);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_setup(JNIEnv *env, jclass clazz, jobjectArray p_cmdline);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_resize(JNIEnv *env, jclass clazz, jint width, jint height);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_newcontext(JNIEnv *env, jclass clazz, jboolean p_32_bits);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_step(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_back(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_touch(JNIEnv *env, jclass clazz, jint ev, jint pointer, jint count, jintArray positions);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_hover(JNIEnv *env, jclass clazz, jint p_type, jint p_x, jint p_y);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_doubletap(JNIEnv *env, jclass clazz, jint p_x, jint p_y);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_scroll(JNIEnv *env, jclass clazz, jint p_x, jint p_y);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_key(JNIEnv *env, jclass clazz, jint p_keycode, jint p_scancode, jint p_unicode_char, jboolean p_pressed);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_joybutton(JNIEnv *env, jclass clazz, jint p_device, jint p_button, jboolean p_pressed);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_joyaxis(JNIEnv *env, jclass clazz, jint p_device, jint p_axis, jfloat p_value);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_joyhat(JNIEnv *env, jclass clazz, jint p_device, jint p_hat_x, jint p_hat_y);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_joyconnectionchanged(JNIEnv *env, jclass clazz, jint p_device, jboolean p_connected, jstring p_name);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_audio(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_accelerometer(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_gravity(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_magnetometer(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_gyroscope(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_focusin(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_focusout(JNIEnv *env, jclass clazz);
JNIEXPORT jstring JNICALL Java_org_godotengine_godot_GodotLib_getGlobal(JNIEnv *env, jclass clazz, jstring path);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_callobject(JNIEnv *env, jclass clazz, jint ID, jstring method, jobjectArray params);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_calldeferred(JNIEnv *env, jclass clazz, jint ID, jstring method, jobjectArray params);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_setVirtualKeyboardHeight(JNIEnv *env, jclass clazz, jint p_height);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_requestPermissionResult(JNIEnv *env, jclass clazz, jstring p_permission, jboolean p_result);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_onRendererResumed(JNIEnv *env, jclass clazz);
JNIEXPORT void JNICALL Java_org_godotengine_godot_GodotLib_onRendererPaused(JNIEnv *env, jclass clazz);
}

#endif /* !JAVA_GODOT_LIB_JNI_H */

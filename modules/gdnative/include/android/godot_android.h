// This file provides the interface for interacting with Android-specific
// functionality from the Godot engine through the GDNative interface. It
// includes functions to retrieve the JNI environment, the current activity, the
// surface, and to check if the activity is resumed.
#ifndef GODOT_ANDROID_GDN_H
#define GODOT_ANDROID_GDN_H

#include <gdnative/gdnative.h>

#ifdef __ANDROID__
#include <jni.h>
#else
#define JNIEnv void
#define jobject void *
#endif

#ifdef __cplusplus
extern "C" {
#endif

JNIEnv *GDAPI godot_android_get_env();
jobject GDAPI godot_android_get_activity();
jobject GDAPI godot_android_get_surface();
bool GDAPI godot_android_is_activity_resumed();

#ifdef __cplusplus
}
#endif

#endif /* !GODOT_ANDROID_GDN_H */

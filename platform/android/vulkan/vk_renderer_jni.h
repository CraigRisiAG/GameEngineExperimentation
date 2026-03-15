

/**
 * @file vk_renderer_jni.h
 * @brief JNI interface for the Vulkan renderer on Android.
 *
 * This header file defines the JNI (Java Native Interface) functions that bridge
 * the Java-side VkRenderer class (org.godotengine.godot.vulkan.VkRenderer) with
 * the native C++ Vulkan rendering implementation.
 *
 * @note All functions are exported with C linkage to prevent C++ name mangling.
 */

/**
 * @brief Called when the Vulkan surface is first created.
 *
 * Initializes the Vulkan renderer with the provided Android surface.
 *
 * @param env       Pointer to the JNI environment.
 * @param obj       Reference to the calling Java object.
 * @param j_surface The Android Surface object on which Vulkan will render.
 */

/**
 * @brief Called when the Vulkan surface changes (e.g., resize or recreation).
 *
 * Updates the Vulkan renderer to handle the new surface configuration.
 *
 * @param env       Pointer to the JNI environment.
 * @param object    Reference to the calling Java object.
 * @param j_surface The updated Android Surface object.
 * @param width     The new width of the surface in pixels.
 * @param height    The new height of the surface in pixels.
 */

/**
 * @brief Called when the application resumes from a paused state.
 *
 * Restores the Vulkan renderer to an active rendering state.
 *
 * @param env Pointer to the JNI environment.
 * @param obj Reference to the calling Java object.
 */

/**
 * @brief Called to render a single frame using Vulkan.
 *
 * Triggers the Vulkan renderer to draw the current frame to the surface.
 *
 * @param env Pointer to the JNI environment.
 * @param obj Reference to the calling Java object.
 */

/**
 * @brief Called when the application is paused.
 *
 * Suspends Vulkan rendering operations and preserves the current state.
 *
 * @param env Pointer to the JNI environment.
 * @param obj Reference to the calling Java object.
 */

/**
 * @brief Called when the Vulkan renderer is being destroyed.
 *
 * Performs cleanup of all Vulkan resources and terminates the renderer.
 *
 * @param env Pointer to the JNI environment.
 * @param obj Reference to the calling Java object.
 */
#ifndef VK_RENDERER_JNI_H
#define VK_RENDERER_JNI_H

#include <android/log.h>
#include <jni.h>

extern "C" {
JNIEXPORT void JNICALL Java_org_godotengine_godot_vulkan_VkRenderer_nativeOnVkSurfaceCreated(JNIEnv *env, jobject obj, jobject j_surface);
JNIEXPORT void JNICALL Java_org_godotengine_godot_vulkan_VkRenderer_nativeOnVkSurfaceChanged(JNIEnv *env, jobject object, jobject j_surface, jint width, jint height);
JNIEXPORT void JNICALL Java_org_godotengine_godot_vulkan_VkRenderer_nativeOnVkResume(JNIEnv *env, jobject obj);
JNIEXPORT void JNICALL Java_org_godotengine_godot_vulkan_VkRenderer_nativeOnVkDrawFrame(JNIEnv *env, jobject obj);
JNIEXPORT void JNICALL Java_org_godotengine_godot_vulkan_VkRenderer_nativeOnVkPause(JNIEnv *env, jobject obj);
JNIEXPORT void JNICALL Java_org_godotengine_godot_vulkan_VkRenderer_nativeOnVkDestroy(JNIEnv *env, jobject obj);
}

#endif // VK_RENDERER_JNI_H

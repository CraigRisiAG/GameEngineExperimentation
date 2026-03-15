

/**
 * @file api.cpp
 * @brief Android API implementation for Java class wrapping functionality.
 *
 * This file provides the implementation for registering and unregistering
 * the Android API, as well as stub implementations for non-Android platforms.
 * It allows Java classes to be accessed and wrapped within the engine's
 * class database system.
 */

/**
 * @brief Registers the Android API classes and singletons.
 *
 * Registers JavaClass and JavaClassWrapper with the ClassDB and adds
 * JavaClassWrapper as an engine singleton. On non-Android platforms,
 * a dummy JavaClassWrapper instance is created to maintain API compatibility.
 */

/**
 * @brief Unregisters the Android API and cleans up resources.
 *
 * On non-Android platforms, deletes the dummy JavaClassWrapper instance
 * that was created during registration.
 */

/**
 * @brief Binds the JavaClassWrapper methods to the ClassDB.
 *
 * Exposes the `wrap` method to the scripting API, allowing Java classes
 * to be wrapped by name.
 */

/**
 * @brief Stub implementation of JavaClass::call for non-Android platforms.
 *
 * @param p_method The name of the method to call (unused).
 * @param p_args Pointer to the array of arguments (unused).
 * @param p_argcount Number of arguments (unused).
 * @param r_error Reference to a CallError struct for error reporting (unused).
 * @return An empty Variant.
 */

/**
 * @brief Stub constructor for JavaClass on non-Android platforms.
 */

/**
 * @brief Stub implementation of JavaObject::call for non-Android platforms.
 *
 * @param p_method The name of the method to call (unused).
 * @param p_args Pointer to the array of arguments (unused).
 * @param p_argcount Number of arguments (unused).
 * @param r_error Reference to a CallError struct for error reporting (unused).
 * @return An empty Variant.
 */

/**
 * @brief Stub implementation of JavaClassWrapper::wrap for non-Android platforms.
 *
 * @param p_name The name of the Java class to wrap (unused).
 * @return An empty Ref<JavaClass>.
 */

/**
 * @brief Stub constructor for JavaClassWrapper on non-Android platforms.
 *
 * Initializes the singleton pointer to this instance.
 */
#include "api.h"

#include "core/engine.h"
#include "java_class_wrapper.h"

#if !defined(ANDROID_ENABLED)
static JavaClassWrapper *java_class_wrapper = NULL;
#endif

void register_android_api() {

#if !defined(ANDROID_ENABLED)
	java_class_wrapper = memnew(JavaClassWrapper); // Dummy
#endif

	ClassDB::register_class<JavaClass>();
	ClassDB::register_class<JavaClassWrapper>();
	Engine::get_singleton()->add_singleton(Engine::Singleton("JavaClassWrapper", JavaClassWrapper::get_singleton()));
}

void unregister_android_api() {

#if !defined(ANDROID_ENABLED)
	memdelete(java_class_wrapper);
#endif
}

void JavaClassWrapper::_bind_methods() {

	ClassDB::bind_method(D_METHOD("wrap", "name"), &JavaClassWrapper::wrap);
}

#if !defined(ANDROID_ENABLED)

Variant JavaClass::call(const StringName &, const Variant **, int, Callable::CallError &) {
	return Variant();
}

JavaClass::JavaClass() {
}

Variant JavaObject::call(const StringName &, const Variant **, int, Callable::CallError &) {
	return Variant();
}

JavaClassWrapper *JavaClassWrapper::singleton = NULL;

Ref<JavaClass> JavaClassWrapper::wrap(const String &) {
	return Ref<JavaClass>();
}

JavaClassWrapper::JavaClassWrapper() {
	singleton = this;
}

#endif

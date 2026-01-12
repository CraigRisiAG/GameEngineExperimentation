

/**
 * @file thread.cpp
 * @brief Thread abstraction layer implementation providing platform-independent thread operations.
 * 
 * This file implements the Thread class, which serves as a wrapper for platform-specific
 * thread creation and management. It uses function pointers that are set at runtime to
 * delegate actual thread operations to platform-specific implementations.
 */

/**
 * @class Thread
 * @brief Platform-independent thread abstraction.
 * 
 * The Thread class provides a static interface for thread operations including creation,
 * identification, and synchronization. Implementation details are delegated to platform-specific
 * function pointers that must be initialized before use.
 */

/**
 * @brief Gets the unique identifier of the calling thread.
 * @return Thread::ID The unique identifier of the current thread, or 0 if the thread ID function is not set.
 */

/**
 * @brief Creates a new thread with the specified callback and settings.
 * @param p_callback The function to execute in the new thread.
 * @param p_user User-defined data to pass to the callback.
 * @param p_settings Configuration settings for the new thread.
 * @return Thread* Pointer to the created thread object, or NULL if thread creation is unavailable.
 */

/**
 * @brief Blocks the calling thread until the specified thread completes execution.
 * @param p_thread Pointer to the thread to wait for. Must not be NULL if the wait function is set.
 */

/**
 * @brief Sets the name of the calling thread.
 * @param p_name The desired name for the thread.
 * @return Error ERR_OK on success, or ERR_UNAVAILABLE if the set_name function is not implemented.
 */

/**
 * @brief Default constructor.
 * @details Initializes an empty Thread object. Actual thread functionality depends on platform-specific implementations.
 */

/**
 * @brief Destructor.
 * @details Cleans up thread resources. Does not automatically wait for thread completion.
 */
#include "thread.h"

Thread *(*Thread::create_func)(ThreadCreateCallback, void *, const Settings &) = NULL;
Thread::ID (*Thread::get_thread_id_func)() = NULL;
void (*Thread::wait_to_finish_func)(Thread *) = NULL;
Error (*Thread::set_name_func)(const String &) = NULL;

Thread::ID Thread::_main_thread_id = 0;

Thread::ID Thread::get_caller_id() {

	if (get_thread_id_func)
		return get_thread_id_func();
	return 0;
}

Thread *Thread::create(ThreadCreateCallback p_callback, void *p_user, const Settings &p_settings) {

	if (create_func) {

		return create_func(p_callback, p_user, p_settings);
	}
	return NULL;
}

void Thread::wait_to_finish(Thread *p_thread) {

	if (wait_to_finish_func)
		wait_to_finish_func(p_thread);
}

Error Thread::set_name(const String &p_name) {

	if (set_name_func)
		return set_name_func(p_name);

	return ERR_UNAVAILABLE;
};

Thread::Thread() {
}

Thread::~Thread() {
}

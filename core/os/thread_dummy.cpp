

/**
 * @file thread_dummy.cpp
 * @brief Dummy implementation of threading and synchronization primitives.
 * 
 * This file provides placeholder implementations of Thread and RWLock classes
 * for single-threaded or non-threaded environments. These implementations
 * ignore threading callbacks and settings, making them suitable for platforms
 * or configurations that don't require true multithreading support.
 */

/**
 * @brief Creates a dummy thread instance.
 * 
 * @param p_callback The thread callback function (ignored in dummy implementation).
 * @param p_user User data to pass to the callback (ignored in dummy implementation).
 * @param p_settings Thread configuration settings (ignored in dummy implementation).
 * @return Thread* A new dummy thread instance allocated on the heap.
 * 
 * @note The returned thread does not execute the callback or support actual threading.
 *       This is a no-op implementation for single-threaded environments.
 */

/**
 * @brief Registers the dummy thread factory as the default thread creator.
 * 
 * This method sets the global Thread::create_func function pointer to use
 * the ThreadDummy::create method for all subsequent thread creation calls.
 */

/**
 * @brief Creates a dummy read-write lock instance.
 * 
 * @return RWLock* A new dummy RWLock instance allocated on the heap.
 * 
 * @note The returned lock does not provide actual synchronization.
 *       This is a no-op implementation for single-threaded environments.
 */

/**
 * @brief Registers the dummy RWLock factory as the default lock creator.
 * 
 * This method sets the global RWLock::create_func function pointer to use
 * the RWLockDummy::create method for all subsequent lock creation calls.
 */
#include "thread_dummy.h"

#include "core/os/memory.h"

Thread *ThreadDummy::create(ThreadCreateCallback p_callback, void *p_user, const Thread::Settings &p_settings) {
	return memnew(ThreadDummy);
};

void ThreadDummy::make_default() {
	Thread::create_func = &ThreadDummy::create;
};

RWLock *RWLockDummy::create() {
	return memnew(RWLockDummy);
};

void RWLockDummy::make_default() {
	RWLock::create_func = &RWLockDummy::create;
};

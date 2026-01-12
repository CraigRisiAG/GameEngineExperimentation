

/**
 * @file mutex.cpp
 * @brief Implementation of mutex synchronization primitives for thread-safe operations.
 * 
 * This file provides global mutex locking/unlocking functions and explicit template
 * instantiations for mutex implementations supporting both recursive and non-recursive
 * mutex types.
 */

/**
 * @brief Global mutex instance for coordinating access to shared resources.
 * 
 * Static instance used by _global_lock() and _global_unlock() functions.
 */

/**
 * @brief Acquires the global mutex lock.
 * 
 * Blocks the calling thread until the global mutex is acquired. Used for
 * synchronizing critical sections across the application.
 * 
 * @thread_safety Thread-safe. Blocks if mutex is already locked.
 * @note Must be paired with a corresponding _global_unlock() call.
 */

/**
 * @brief Releases the global mutex lock.
 * 
 * Unlocks the global mutex, allowing other threads waiting on _global_lock()
 * to proceed.
 * 
 * @thread_safety Thread-safe. Only should be called by the thread that acquired the lock.
 * @note Must follow a corresponding _global_lock() call.
 */
#include "mutex.h"

static Mutex _global_mutex;

void _global_lock() {
	_global_mutex.lock();
}

void _global_unlock() {
	_global_mutex.unlock();
}

#ifndef NO_THREADS

template class MutexImpl<std::recursive_mutex>;
template class MutexImpl<std::mutex>;
template class MutexLock<MutexImpl<std::recursive_mutex>>;
template class MutexLock<MutexImpl<std::mutex>>;

#endif

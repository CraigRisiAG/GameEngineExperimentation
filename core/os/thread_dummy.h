

/// @file thread_dummy.h
/// @brief Dummy implementations of threading and synchronization primitives.
///
/// This header provides no-op (dummy) implementations of Thread and RWLock classes
/// for single-threaded or non-threaded environments. These implementations can be
/// used as default threading backends when actual multi-threading support is not
/// required or available.

/// @class ThreadDummy
/// @brief A dummy thread implementation that performs no actual threading.
///
/// ThreadDummy provides a minimal Thread interface suitable for single-threaded
/// execution contexts. All threading operations are no-ops.
///
/// @see Thread

/// @brief Creates a new dummy thread instance.
/// @param p_callback The callback function to execute in the thread context.
/// @param p_user User-defined data passed to the callback.
/// @param p_settings Optional thread configuration settings.
/// @return A pointer to the created Thread instance, or nullptr on failure.
/// @note In dummy implementation, callback is not actually executed in a separate thread.

/// @brief Gets the identifier of the thread.
/// @return Always returns 0 for dummy implementation.

/// @brief Sets the dummy thread as the default thread implementation.
/// @note Must be called during engine initialization.

/// @class RWLockDummy
/// @brief A dummy read-write lock implementation with no actual synchronization.
///
/// RWLockDummy provides a minimal RWLock interface suitable for single-threaded
/// execution contexts where lock contention is not a concern. All lock operations
/// are no-ops.
///
/// @see RWLock

/// @brief Creates a new dummy read-write lock instance.
/// @return A pointer to the created RWLock instance.

/// @brief Acquires a read lock (no-op).
/// @note In dummy implementation, this has no effect.

/// @brief Releases a read lock (no-op).
/// @note In dummy implementation, this has no effect.

/// @brief Attempts to acquire a read lock (always succeeds).
/// @return Always returns OK for dummy implementation.

/// @brief Acquires a write lock (no-op).
/// @note In dummy implementation, this has no effect.

/// @brief Releases a write lock (no-op).
/// @note In dummy implementation, this has no effect.

/// @brief Attempts to acquire a write lock (always succeeds).
/// @return Always returns OK for dummy implementation.

/// @brief Sets the dummy read-write lock as the default RWLock implementation.
/// @note Must be called during engine initialization.
#ifndef THREAD_DUMMY_H
#define THREAD_DUMMY_H

#include "core/os/rw_lock.h"
#include "core/os/semaphore.h"
#include "core/os/thread.h"

class ThreadDummy : public Thread {

	static Thread *create(ThreadCreateCallback p_callback, void *p_user, const Settings &p_settings = Settings());

public:
	virtual ID get_id() const { return 0; };

	static void make_default();
};

class RWLockDummy : public RWLock {

	static RWLock *create();

public:
	virtual void read_lock() {}
	virtual void read_unlock() {}
	virtual Error read_try_lock() { return OK; }

	virtual void write_lock() {}
	virtual void write_unlock() {}
	virtual Error write_try_lock() { return OK; }

	static void make_default();
};

#endif // THREAD_DUMMY_H

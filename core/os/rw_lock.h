

/**
 * @file rw_lock.h
 * @brief Reader-Writer lock interface and RAII wrapper classes
 * 
 * Provides a platform-independent interface for reader-writer locks that allow
 * multiple concurrent readers or a single exclusive writer. Includes RAII wrapper
 * classes for automatic lock management.
 */

/**
 * @class RWLock
 * @brief Abstract base class for reader-writer lock implementations
 * 
 * Defines the interface for reader-writer synchronization primitives. Derived classes
 * must provide platform-specific implementations. The class uses a factory pattern
 * via the create() static method to instantiate appropriate lock implementations.
 */

/**
 * @class RWLockRead
 * @brief RAII wrapper for acquiring read locks
 * 
 * Automatically acquires a read lock on construction and releases it on destruction.
 * Allows multiple threads to hold read locks simultaneously. Safely handles null locks.
 * 
 * @note Uses const_cast to allow const references to be locked
 * 
 * @example
 * const RWLock *lock = RWLock::create();
 * {
 *     RWLockRead read_guard(lock);
 *     // Read-safe code here
 * } // Lock automatically released
 */

/**
 * @class RWLockWrite
 * @brief RAII wrapper for acquiring write locks
 * 
 * Automatically acquires a write lock on construction and releases it on destruction.
 * Ensures exclusive access - only one thread can hold a write lock at a time.
 * Safely handles null locks.
 * 
 * @example
 * RWLock *lock = RWLock::create();
 * {
 *     RWLockWrite write_guard(lock);
 *     // Write-safe code here
 * } // Lock automatically released
 */
#ifndef RW_LOCK_H
#define RW_LOCK_H

#include "core/error_list.h"

class RWLock {
protected:
	static RWLock *(*create_func)();

public:
	virtual void read_lock() = 0; ///< Lock the rwlock, block if locked by someone else
	virtual void read_unlock() = 0; ///< Unlock the rwlock, let other threads continue
	virtual Error read_try_lock() = 0; ///< Attempt to lock the rwlock, OK on success, ERROR means it can't lock.

	virtual void write_lock() = 0; ///< Lock the rwlock, block if locked by someone else
	virtual void write_unlock() = 0; ///< Unlock the rwlock, let other thwrites continue
	virtual Error write_try_lock() = 0; ///< Attempt to lock the rwlock, OK on success, ERROR means it can't lock.

	static RWLock *create(); ///< Create a rwlock

	virtual ~RWLock();
};

class RWLockRead {

	RWLock *lock;

public:
	RWLockRead(const RWLock *p_lock) {
		lock = const_cast<RWLock *>(p_lock);
		if (lock) lock->read_lock();
	}
	~RWLockRead() {
		if (lock) lock->read_unlock();
	}
};

class RWLockWrite {

	RWLock *lock;

public:
	RWLockWrite(RWLock *p_lock) {
		lock = p_lock;
		if (lock) lock->write_lock();
	}
	~RWLockWrite() {
		if (lock) lock->write_unlock();
	}
};

#endif // RW_LOCK_H

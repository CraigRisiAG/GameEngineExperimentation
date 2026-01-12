

/**
 * @file mutex.h
 * @brief Thread synchronization primitives for the game engine.
 * 
 * This header provides mutex implementations for thread-safe operations.
 * When threading is enabled (NO_THREADS not defined), it wraps standard
 * C++ mutex types. When threading is disabled, it provides no-op implementations.
 * 
 * @details
 * - MutexImpl: Template wrapper around standard mutex types providing lock(),
 *   unlock(), and try_lock() methods.
 * - MutexLock: RAII guard class that automatically locks a mutex on construction
 *   and unlocks it on destruction.
 * - Mutex: Recursive mutex alias for general use (allows same thread to lock multiple times).
 * - BinaryMutex: Non-recursive mutex alias requiring careful handling.
 * 
 * When NO_THREADS is defined, all mutex operations become no-ops, allowing
 * the engine to compile and run in single-threaded mode.
 */
#ifndef MUTEX_H
#define MUTEX_H

#include "core/error_list.h"
#include "core/typedefs.h"

#if !defined(NO_THREADS)

#include <mutex>

template <class StdMutexT>
class MutexImpl {
	mutable StdMutexT mutex;

public:
	_ALWAYS_INLINE_ void lock() const {
		mutex.lock();
	}

	_ALWAYS_INLINE_ void unlock() const {
		mutex.unlock();
	}

	_ALWAYS_INLINE_ Error try_lock() const {
		return mutex.try_lock() ? OK : ERR_BUSY;
	}
};

template <class MutexT>
class MutexLock {
	const MutexT &mutex;

public:
	_ALWAYS_INLINE_ explicit MutexLock(const MutexT &p_mutex) :
			mutex(p_mutex) {
		mutex.lock();
	}

	_ALWAYS_INLINE_ ~MutexLock() {
		mutex.unlock();
	}
};

using Mutex = MutexImpl<std::recursive_mutex>; // Recursive, for general use
using BinaryMutex = MutexImpl<std::mutex>; // Non-recursive, handle with care

extern template class MutexImpl<std::recursive_mutex>;
extern template class MutexImpl<std::mutex>;
extern template class MutexLock<MutexImpl<std::recursive_mutex>>;
extern template class MutexLock<MutexImpl<std::mutex>>;

#else

class FakeMutex {

	FakeMutex(){};
};

template <class MutexT>
class MutexImpl {
public:
	_ALWAYS_INLINE_ void lock() const {}
	_ALWAYS_INLINE_ void unlock() const {}
	_ALWAYS_INLINE_ Error try_lock() const { return OK; }
};

template <class MutexT>
class MutexLock {
public:
	explicit MutexLock(const MutexT &p_mutex) {}
};

using Mutex = MutexImpl<FakeMutex>;
using BinaryMutex = MutexImpl<FakeMutex>; // Non-recursive, handle with care

#endif // !NO_THREADS

#endif // MUTEX_H

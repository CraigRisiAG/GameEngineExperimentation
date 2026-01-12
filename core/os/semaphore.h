

/**
 * @class Semaphore
 * @brief A thread synchronization primitive that manages a counter for controlling access to shared resources.
 *
 * This class provides a semaphore implementation using standard C++ synchronization primitives.
 * When threading is enabled (NO_THREADS not defined), it uses std::mutex and std::condition_variable
 * for synchronization. When threading is disabled, it provides no-op implementations.
 *
 * The semaphore maintains an internal counter that is initialized to zero (locked state).
 * - post(): Increments the counter and notifies waiting threads.
 * - wait(): Blocks until the counter is greater than zero, then decrements it.
 * - try_wait(): Attempts to decrement the counter without blocking.
 *
 * @note All methods are marked as _ALWAYS_INLINE_ for performance optimization.
 * @note The internal synchronization primitives are mutable to allow const methods.
 * @note This implementation handles spurious wake-ups from condition variables.
 */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "core/error_list.h"
#include "core/typedefs.h"

#if !defined(NO_THREADS)

#include <condition_variable>
#include <mutex>

class Semaphore {
private:
	mutable std::mutex mutex_;
	mutable std::condition_variable condition_;
	mutable unsigned long count_ = 0; // Initialized as locked.

public:
	_ALWAYS_INLINE_ void post() const {
		std::lock_guard<decltype(mutex_)> lock(mutex_);
		++count_;
		condition_.notify_one();
	}

	_ALWAYS_INLINE_ void wait() const {
		std::unique_lock<decltype(mutex_)> lock(mutex_);
		while (!count_) // Handle spurious wake-ups.
			condition_.wait(lock);
		--count_;
	}

	_ALWAYS_INLINE_ bool try_wait() const {
		std::lock_guard<decltype(mutex_)> lock(mutex_);
		if (count_) {
			--count_;
			return true;
		}
		return false;
	}
};

#else

class Semaphore {
public:
	_ALWAYS_INLINE_ void post() const {}
	_ALWAYS_INLINE_ void wait() const {}
	_ALWAYS_INLINE_ bool try_wait() const { return true; }
};

#endif

#endif // SEMAPHORE_H

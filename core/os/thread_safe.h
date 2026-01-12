

/// @file thread_safe.h
/// @brief Thread safety utilities using mutex-based synchronization.
///
/// This header provides preprocessor macros for easily adding thread-safe
/// access to class members and methods. These macros encapsulate mutex
/// locking/unlocking operations to protect shared resources from concurrent
/// access.
///
/// @details
/// - `_THREAD_SAFE_CLASS_`: Declares a mutable mutex member variable for
///   thread-safe class instances.
/// - `_THREAD_SAFE_METHOD_`: Creates an automatic lock guard within a method
///   scope for method-level synchronization.
/// - `_THREAD_SAFE_LOCK_`: Manually locks the class mutex.
/// - `_THREAD_SAFE_UNLOCK_`: Manually unlocks the class mutex.
///
/// @warning
/// - Manual lock/unlock macros should be used with caution to avoid deadlocks.
/// - Prefer `_THREAD_SAFE_METHOD_` for automatic RAII-style lock management.
/// - Requires the `core/os/mutex.h` header for Mutex and MutexLock definitions.
///
/// @see core/os/mutex.h
#ifndef THREAD_SAFE_H
#define THREAD_SAFE_H

#include "core/os/mutex.h"

#define _THREAD_SAFE_CLASS_ mutable Mutex _thread_safe_;
#define _THREAD_SAFE_METHOD_ MutexLock _thread_safe_method_(_thread_safe_);
#define _THREAD_SAFE_LOCK_ _thread_safe_.lock();
#define _THREAD_SAFE_UNLOCK_ _thread_safe_.unlock();

#endif // THREAD_SAFE_H



/**
 * @file rw_lock.cpp
 * @brief Implementation of the RWLock (Read-Write Lock) class.
 * 
 * This file provides the core functionality for creating and managing read-write locks,
 * which allow multiple concurrent readers or a single exclusive writer.
 */

/**
 * @var RWLock::create_func
 * @brief Function pointer for platform-specific RWLock creation.
 * 
 * This static function pointer is set by the platform implementation to create
 * platform-specific RWLock instances. Must be initialized before calling create().
 */

/**
 * @fn RWLock* RWLock::create()
 * @brief Factory method to create a new RWLock instance.
 * 
 * Creates a new platform-specific read-write lock using the registered creation function.
 * 
 * @return A pointer to the newly created RWLock instance, or nullptr if create_func is not initialized.
 * @warning Requires create_func to be properly initialized. Will fail with an error macro if not set.
 */

/**
 * @fn RWLock::~RWLock()
 * @brief Virtual destructor for the RWLock class.
 * 
 * Allows proper cleanup of derived platform-specific implementations.
 */
#include "rw_lock.h"

#include "core/error_macros.h"

#include <stddef.h>

RWLock *(*RWLock::create_func)() = 0;

RWLock *RWLock::create() {

	ERR_FAIL_COND_V(!create_func, 0);

	return create_func();
}

RWLock::~RWLock() {
}

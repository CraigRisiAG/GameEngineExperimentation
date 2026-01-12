

/**
 * @class Thread
 * @brief Platform-independent thread management class.
 * 
 * Provides an abstraction layer for creating and managing threads across different platforms.
 * Uses function pointers to delegate platform-specific implementations.
 * 
 * @note This class uses a factory pattern with static function pointers for platform abstraction.
 */

/**
 * @enum Priority
 * @brief Thread execution priority levels.
 * 
 * @var PRIORITY_LOW - Low priority execution
 * @var PRIORITY_NORMAL - Normal priority execution (default)
 * @var PRIORITY_HIGH - High priority execution
 */

/**
 * @struct Settings
 * @brief Configuration settings for thread creation.
 * 
 * @member priority - The scheduling priority for the thread
 */

/**
 * @typedef ID
 * @brief Unique identifier for a thread, represented as a 64-bit unsigned integer.
 */

/**
 * @fn static ID get_main_id()
 * @brief Retrieves the thread ID of the main thread.
 * 
 * @return ID - The thread ID of the main application thread
 * @inline
 */

/**
 * @fn static ID get_caller_id()
 * @brief Retrieves the thread ID of the calling thread.
 * 
 * @return ID - The thread ID of the caller
 */

/**
 * @fn static void wait_to_finish(Thread *p_thread)
 * @brief Blocks until the specified thread completes execution and deallocates its resources.
 * 
 * @param p_thread - Pointer to the thread to wait for
 */

/**
 * @fn static Thread *create(ThreadCreateCallback p_callback, void *p_user, const Settings &p_settings = Settings())
 * @brief Creates a new thread and starts execution.
 * 
 * @param p_callback - Function to execute in the new thread
 * @param p_user - User data to pass to the callback function
 * @param p_settings - Configuration settings for the thread (optional)
 * @return Thread* - Pointer to the created thread
 */

/**
 * @fn static Error set_name(const String &p_name)
 * @brief Sets the name of the current thread.
 * 
 * @param p_name - The name to assign to the thread
 * @return Error - Status code indicating success or failure
 */

/**
 * @fn virtual ID get_id() const = 0
 * @brief Retrieves the unique identifier of this thread instance.
 * 
 * @return ID - The thread's unique identifier
 * @pure
 */

/**
 * @fn virtual ~Thread()
 * @brief Virtual destructor for proper cleanup of derived classes.
 */
#ifndef THREAD_H
#define THREAD_H

#include "core/typedefs.h"
#include "core/ustring.h"

typedef void (*ThreadCreateCallback)(void *p_userdata);

class Thread {
public:
	enum Priority {

		PRIORITY_LOW,
		PRIORITY_NORMAL,
		PRIORITY_HIGH
	};

	struct Settings {

		Priority priority;
		Settings() { priority = PRIORITY_NORMAL; }
	};

	typedef uint64_t ID;

protected:
	static Thread *(*create_func)(ThreadCreateCallback p_callback, void *, const Settings &);
	static ID (*get_thread_id_func)();
	static void (*wait_to_finish_func)(Thread *);
	static Error (*set_name_func)(const String &);

	friend class Main;

	static ID _main_thread_id;

	Thread();

public:
	virtual ID get_id() const = 0;

	static Error set_name(const String &p_name);
	_FORCE_INLINE_ static ID get_main_id() { return _main_thread_id; } ///< get the ID of the main thread
	static ID get_caller_id(); ///< get the ID of the caller function ID
	static void wait_to_finish(Thread *p_thread); ///< waits until thread is finished, and deallocates it.
	static Thread *create(ThreadCreateCallback p_callback, void *p_user, const Settings &p_settings = Settings()); ///< Static function to create a thread, will call p_callback

	virtual ~Thread();
};

#endif // THREAD_H

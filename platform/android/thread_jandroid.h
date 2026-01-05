
/**
 * @class ThreadAndroid
 * @brief Android-specific thread implementation using POSIX pthread with JNI support.
 * 
 * Provides thread management for Android platform, integrating with the Java Virtual Machine
 * through JNI (Java Native Interface). Extends the base Thread class to provide platform-specific
 * thread creation, management, and JVM environment handling.
 * 
 * @note This class manages pthread lifecycle and maintains JVM environment for JNI calls.
 * 
 * Static Members:
 *   - thread_id_key: pthread key for thread-local storage of thread IDs
 *   - next_thread_id: Counter for generating unique thread IDs
 *   - jvm_key: pthread key for thread-local JVM storage
 *   - java_vm: Reference to the Java Virtual Machine
 * 
 * Member Variables:
 *   - pthread: POSIX thread handle
 *   - pthread_attr: POSIX thread attributes
 *   - callback: User-defined thread callback function
 *   - user: User data passed to callback
 *   - id: Unique thread identifier
 * 
 * @see Thread
 */

/**
 * @brief Initializes the ThreadAndroid subsystem with JVM reference.
 * @param p_java_vm Pointer to the Java Virtual Machine instance
 */

/**
 * @brief Sets up JNI environment for the current thread.
 * @note Must be called once per thread that needs JNI access
 */

/**
 * @brief Retrieves the JNI environment for the current thread.
 * @return Pointer to JNIEnv for the calling thread
 */

/**
 * @brief Gets the unique identifier of the current thread.
 * @return Thread ID of the calling thread
 */

/**
 * @brief Destructor. Cleans up thread resources and detaches from JVM.
 */
#ifndef THREAD_POSIX_H
#define THREAD_POSIX_H

#include "core/os/thread.h"
#include <jni.h>
#include <pthread.h>
#include <sys/types.h>

class ThreadAndroid : public Thread {

	static pthread_key_t thread_id_key;
	static ID next_thread_id;

	pthread_t pthread;
	pthread_attr_t pthread_attr;
	ThreadCreateCallback callback;
	void *user;
	ID id;

	static Thread *create_thread_jandroid();

	static void *thread_callback(void *userdata);

	static Thread *create_func_jandroid(ThreadCreateCallback p_callback, void *, const Settings &);
	static ID get_thread_id_func_jandroid();
	static void wait_to_finish_func_jandroid(Thread *p_thread);

	static void _thread_destroyed(void *value);
	ThreadAndroid();

	static pthread_key_t jvm_key;
	static JavaVM *java_vm;

public:
	virtual ID get_id() const;

	static void make_default(JavaVM *p_java_vm);
	static void setup_thread();
	static JNIEnv *get_env();

	~ThreadAndroid();
};

#endif

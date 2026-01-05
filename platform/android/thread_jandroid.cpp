
/**
 * @file thread_jandroid.cpp
 * @brief Android thread implementation for the game engine.
 * 
 * This file implements threading functionality for Android platforms using POSIX threads (pthread)
 * and JNI integration with the Java Virtual Machine. It provides thread creation, management,
 * and proper cleanup with Java VM attachment/detachment.
 * 
 * Key Features:
 * - Thread creation and lifecycle management
 * - Unique thread ID generation and retrieval
 * - JNI environment setup and cleanup per thread
 * - Script server integration for thread-safe script execution
 * - Proper resource cleanup through pthread keys and destructors
 * 
 * Thread Safety:
 * - Uses atomic operations for thread ID generation
 * - Manages JNI environment per thread using thread-local storage (pthread keys)
 * - Ensures proper detachment from Java VM on thread destruction
 * 
 * Dependencies:
 * - POSIX threads (pthread)
 * - JNI for Java interoperability
 * - Core memory management utilities
 * - Script server for scripting support
 */
#include "thread_jandroid.h"

#include "core/os/memory.h"
#include "core/safe_refcount.h"
#include "core/script_language.h"

static void _thread_id_key_destr_callback(void *p_value) {
	memdelete(static_cast<Thread::ID *>(p_value));
}

static pthread_key_t _create_thread_id_key() {
	pthread_key_t key;
	pthread_key_create(&key, &_thread_id_key_destr_callback);
	return key;
}

pthread_key_t ThreadAndroid::thread_id_key = _create_thread_id_key();
Thread::ID ThreadAndroid::next_thread_id = 0;

Thread::ID ThreadAndroid::get_id() const {

	return id;
}

Thread *ThreadAndroid::create_thread_jandroid() {

	return memnew(ThreadAndroid);
}

void *ThreadAndroid::thread_callback(void *userdata) {

	ThreadAndroid *t = reinterpret_cast<ThreadAndroid *>(userdata);
	setup_thread();
	ScriptServer::thread_enter(); //scripts may need to attach a stack
	t->id = atomic_increment(&next_thread_id);
	pthread_setspecific(thread_id_key, (void *)memnew(ID(t->id)));
	t->callback(t->user);
	ScriptServer::thread_exit();
	return NULL;
}

Thread *ThreadAndroid::create_func_jandroid(ThreadCreateCallback p_callback, void *p_user, const Settings &) {

	ThreadAndroid *tr = memnew(ThreadAndroid);
	tr->callback = p_callback;
	tr->user = p_user;
	pthread_attr_init(&tr->pthread_attr);
	pthread_attr_setdetachstate(&tr->pthread_attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&tr->pthread, &tr->pthread_attr, thread_callback, tr);

	return tr;
}

Thread::ID ThreadAndroid::get_thread_id_func_jandroid() {

	void *value = pthread_getspecific(thread_id_key);

	if (value)
		return *static_cast<ID *>(value);

	ID new_id = atomic_increment(&next_thread_id);
	pthread_setspecific(thread_id_key, (void *)memnew(ID(new_id)));
	return new_id;
}

void ThreadAndroid::wait_to_finish_func_jandroid(Thread *p_thread) {

	ThreadAndroid *tp = static_cast<ThreadAndroid *>(p_thread);
	ERR_FAIL_COND(!tp);
	ERR_FAIL_COND(tp->pthread == 0);

	pthread_join(tp->pthread, NULL);
	tp->pthread = 0;
}

void ThreadAndroid::_thread_destroyed(void *value) {

	/* The thread is being destroyed, detach it from the Java VM and set the mThreadKey value to NULL as required */
	JNIEnv *env = (JNIEnv *)value;
	if (env != NULL) {
		java_vm->DetachCurrentThread();
		pthread_setspecific(jvm_key, NULL);
	}
}

pthread_key_t ThreadAndroid::jvm_key;
JavaVM *ThreadAndroid::java_vm = NULL;

void ThreadAndroid::setup_thread() {

	if (pthread_getspecific(jvm_key))
		return; //already setup
	JNIEnv *env;
	java_vm->AttachCurrentThread(&env, NULL);
	pthread_setspecific(jvm_key, (void *)env);
}

void ThreadAndroid::make_default(JavaVM *p_java_vm) {

	java_vm = p_java_vm;
	create_func = create_func_jandroid;
	get_thread_id_func = get_thread_id_func_jandroid;
	wait_to_finish_func = wait_to_finish_func_jandroid;
	pthread_key_create(&jvm_key, _thread_destroyed);
	setup_thread();
}

JNIEnv *ThreadAndroid::get_env() {

	if (!pthread_getspecific(jvm_key)) {
		setup_thread();
	}

	JNIEnv *env = NULL;
	java_vm->AttachCurrentThread(&env, NULL);
	return env;
}

ThreadAndroid::ThreadAndroid() {

	pthread = 0;
}

ThreadAndroid::~ThreadAndroid() {
}

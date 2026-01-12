

/**
 * @file threaded_array_processor.h
 * @brief Provides utilities for processing array elements in parallel across multiple threads.
 * 
 * This header defines a template-based system for distributing array processing work across
 * available CPU cores. It uses an atomic counter to distribute work dynamically to threads,
 * ensuring efficient load balancing without explicit work partitioning.
 * 
 * @details
 * - ThreadArrayProcessData: A data structure that encapsulates the processing context,
 *   including the target array size, current index, instance pointer, user data, and the
 *   member function to invoke on each element.
 * 
 * - process_array_thread: The thread worker function that atomically increments an index
 *   counter and processes array elements until all elements are consumed. This allows
 *   dynamic load distribution among threads.
 * 
 * - thread_process_array: The main entry point that initializes the processing pipeline.
 *   When threading is enabled (NO_THREADS not defined), it spawns one thread per available
 *   CPU core. The first element is processed on the calling thread, and remaining elements
 *   are distributed among worker threads. When threading is disabled, processing occurs
 *   sequentially on the calling thread.
 * 
 * @tparam C The class type containing the member function to be called.
 * @tparam M The member function pointer type.
 * @tparam U The user data type passed to each invocation.
 * 
 * @note This implementation requires atomic increment operations and thread management
 *       facilities from the OS abstraction layer.
 */
#ifndef THREADED_ARRAY_PROCESSOR_H
#define THREADED_ARRAY_PROCESSOR_H

#include "core/os/mutex.h"
#include "core/os/os.h"
#include "core/os/thread.h"
#include "core/os/thread_safe.h"
#include "core/safe_refcount.h"

template <class C, class U>
struct ThreadArrayProcessData {
	uint32_t elements;
	uint32_t index;
	C *instance;
	U userdata;
	void (C::*method)(uint32_t, U);

	void process(uint32_t p_index) {
		(instance->*method)(p_index, userdata);
	}
};

#ifndef NO_THREADS

template <class T>
void process_array_thread(void *ud) {

	T &data = *(T *)ud;
	while (true) {
		uint32_t index = atomic_increment(&data.index);
		if (index >= data.elements)
			break;
		data.process(index);
	}
}

template <class C, class M, class U>
void thread_process_array(uint32_t p_elements, C *p_instance, M p_method, U p_userdata) {

	ThreadArrayProcessData<C, U> data;
	data.method = p_method;
	data.instance = p_instance;
	data.userdata = p_userdata;
	data.index = 0;
	data.elements = p_elements;
	data.process(data.index); //process first, let threads increment for next

	Vector<Thread *> threads;

	threads.resize(OS::get_singleton()->get_processor_count());

	for (int i = 0; i < threads.size(); i++) {
		threads.write[i] = Thread::create(process_array_thread<ThreadArrayProcessData<C, U>>, &data);
	}

	for (int i = 0; i < threads.size(); i++) {
		Thread::wait_to_finish(threads[i]);
		memdelete(threads[i]);
	}
}

#else

template <class C, class M, class U>
void thread_process_array(uint32_t p_elements, C *p_instance, M p_method, U p_userdata) {

	ThreadArrayProcessData<C, U> data;
	data.method = p_method;
	data.instance = p_instance;
	data.userdata = p_userdata;
	data.index = 0;
	data.elements = p_elements;
	for (uint32_t i = 0; i < p_elements; i++) {
		data.process(i);
	}
}

#endif

#endif // THREADED_ARRAY_PROCESSOR_H

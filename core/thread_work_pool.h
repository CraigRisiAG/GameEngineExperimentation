

/**
 * @class ThreadWorkPool
 * @brief A thread pool for parallel work distribution across multiple threads.
 * 
 * ThreadWorkPool manages a pool of worker threads that execute tasks in parallel.
 * It uses a work-stealing pattern where threads atomically fetch indices to process
 * elements of a work item until all elements are consumed.
 * 
 * @details
 * The pool uses semaphores for thread synchronization:
 * - Threads wait on a "start" semaphore until work is assigned
 * - The main thread waits on "completed" semaphores for all threads to finish
 * 
 * @note Memory is allocated for work items using memnew/memdelete.
 * @note Uses relaxed memory ordering for atomic operations to minimize overhead.
 * 
 * @example
 * ThreadWorkPool pool;
 * pool.init(4); // Initialize with 4 threads
 * 
 * class MyClass {
 *     void process(uint32_t index, void* userdata) { }
 * };
 * 
 * MyClass obj;
 * pool.do_work(100, &obj, &MyClass::process, nullptr);
 * pool.finish();
 */
#ifndef THREAD_WORK_POOL_H
#define THREAD_WORK_POOL_H

#include "core/os/memory.h"
#include "core/os/semaphore.h"
#include <atomic>
#include <thread>
class ThreadWorkPool {

	std::atomic<uint32_t> index;

	struct BaseWork {
		std::atomic<uint32_t> *index;
		uint32_t max_elements;
		virtual void work() = 0;
		virtual ~BaseWork() = default;
	};

	template <class C, class M, class U>
	struct Work : public BaseWork {
		C *instance;
		M method;
		U userdata;
		virtual void work() {

			while (true) {
				uint32_t work_index = index->fetch_add(1, std::memory_order_relaxed);
				if (work_index >= max_elements) {
					break;
				}
				(instance->*method)(work_index, userdata);
			}
		}
	};

	struct ThreadData {
		std::thread *thread;
		Semaphore start;
		Semaphore completed;
		std::atomic<bool> exit;
		BaseWork *work;
	};

	ThreadData *threads = nullptr;
	uint32_t thread_count = 0;

	static void _thread_function(ThreadData *p_thread);

public:
	template <class C, class M, class U>
	void do_work(uint32_t p_elements, C *p_instance, M p_method, U p_userdata) {

		ERR_FAIL_COND(!threads); //never initialized

		index.store(0);

		Work<C, M, U> *w = memnew((Work<C, M, U>));
		w->instance = p_instance;
		w->userdata = p_userdata;
		w->method = p_method;
		w->index = &index;
		w->max_elements = p_elements;

		for (uint32_t i = 0; i < thread_count; i++) {
			threads[i].work = w;
			threads[i].start.post();
		}
		for (uint32_t i = 0; i < thread_count; i++) {
			threads[i].completed.wait();
			threads[i].work = nullptr;
		}

		memdelete(w);
	}

	void init(int p_thread_count = -1);
	void finish();
	~ThreadWorkPool();
};

#endif // THREAD_POOL_H




/**
 * @class ThreadWorkPool
 * @brief A thread pool implementation for distributing work across multiple threads.
 * 
 * This class manages a pool of worker threads that can execute tasks concurrently.
 * It provides initialization, task distribution, and graceful shutdown capabilities.
 */

/**
 * @brief Static thread function executed by each worker thread.
 * 
 * This function runs in a loop, waiting for work to be assigned, executing it,
 * and signaling completion. The thread exits when the exit flag is set.
 * 
 * @param p_thread Pointer to the ThreadData structure containing synchronization
 *                  primitives and work queue information for the thread.
 */

/**
 * @brief Initializes the thread pool with the specified number of threads.
 * 
 * Creates and starts the specified number of worker threads. If the thread count
 * is negative, defaults to the system's processor count.
 * 
 * @param p_thread_count The number of threads to create. If negative, uses the
 *                       system processor count.
 * 
 * @note Must not be called if the pool is already initialized.
 *       Fails with ERR_FAIL_COND if threads are already allocated.
 */

/**
 * @brief Shuts down the thread pool and releases all resources.
 * 
 * Signals all worker threads to exit, waits for them to complete, and deallocates
 * all thread resources. Safe to call even if the pool was not initialized.
 */

/**
 * @brief Destructor that ensures proper cleanup of the thread pool.
 * 
 * Calls finish() to gracefully shut down all threads and release resources.
 */
#include "thread_work_pool.h"
#include "core/os/os.h"

void ThreadWorkPool::_thread_function(ThreadData *p_thread) {

	while (true) {
		p_thread->start.wait();
		if (p_thread->exit.load()) {
			break;
		}
		p_thread->work->work();
		p_thread->completed.post();
	}
}

void ThreadWorkPool::init(int p_thread_count) {
	ERR_FAIL_COND(threads != nullptr);
	if (p_thread_count < 0) {
		p_thread_count = OS::get_singleton()->get_processor_count();
	}

	thread_count = p_thread_count;
	threads = memnew_arr(ThreadData, thread_count);

	for (uint32_t i = 0; i < thread_count; i++) {
		threads[i].exit.store(false);
		threads[i].thread = memnew(std::thread(ThreadWorkPool::_thread_function, &threads[i]));
	}
}

void ThreadWorkPool::finish() {

	if (threads == nullptr) {
		return;
	}

	for (uint32_t i = 0; i < thread_count; i++) {
		threads[i].exit.store(true);
		threads[i].start.post();
	}
	for (uint32_t i = 0; i < thread_count; i++) {
		threads[i].thread->join();
		memdelete(threads[i].thread);
	}

	memdelete_arr(threads);
	threads = nullptr;
}

ThreadWorkPool::~ThreadWorkPool() {

	finish();
}

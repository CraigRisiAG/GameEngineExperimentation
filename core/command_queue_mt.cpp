
/**
 * @file command_queue_mt.cpp
 * @brief Implementation of a thread-safe command queue for multi-threaded
 * command processing.
 *
 * This file implements CommandQueueMT, a multi-threaded command queue that
 * manages command execution with synchronization primitives and memory
 * allocation.
 */

/**
 * @brief Acquires the mutex lock for thread-safe access.
 */

/**
 * @brief Releases the mutex lock.
 */

/**
 * @brief Waits for a command queue flush operation.
 *
 * Pauses execution for 1 millisecond to allow pending flush operations to
 * complete.
 */

/**
 * @brief Allocates a synchronization semaphore from the semaphore pool.
 *
 * @return Pointer to an available SyncSemaphore structure.
 * @note Blocks until a semaphore becomes available. Uses polling with
 * wait_for_flush().
 */

/**
 * @brief Deallocates one command from the queue.
 *
 * Processes the next command in the deallocation queue, updating the
 * dealloc_ptr and handling wrap-around when reaching the end of the command
 * buffer.
 *
 * @return true if a command was successfully deallocated, false if queue is
 * empty or command is still in use.
 */

/**
 * @brief Constructs a CommandQueueMT instance.
 *
 * @param p_sync If true, initializes a Semaphore for synchronization; otherwise
 * NULL.
 *
 * Initializes the command queue with pointers set to zero and allocates command
 * memory. All synchronization semaphores are marked as available.
 */

/**
 * @brief Destructs a CommandQueueMT instance.
 *
 * Cleans up allocated resources including the semaphore (if initialized) and
 * command memory.
 */
#include "command_queue_mt.h"

#include "core/os/os.h"

void CommandQueueMT::lock() { mutex.lock(); }

void CommandQueueMT::unlock() { mutex.unlock(); }

void CommandQueueMT::wait_for_flush() {

  // wait one millisecond for a flush to happen
  OS::get_singleton()->delay_usec(1000);
}

CommandQueueMT::SyncSemaphore *CommandQueueMT::_alloc_sync_sem() {

  int idx = -1;

  while (true) {

    lock();
    for (int i = 0; i < SYNC_SEMAPHORES; i++) {

      if (!sync_sems[i].in_use) {
        sync_sems[i].in_use = true;
        idx = i;
        break;
      }
    }
    unlock();

    if (idx == -1) {
      wait_for_flush();
    } else {
      break;
    }
  }

  return &sync_sems[idx];
}

bool CommandQueueMT::dealloc_one() {
tryagain:
  if (dealloc_ptr == write_ptr) {
    // The queue is empty
    return false;
  }

  uint32_t size = *(uint32_t *)&command_mem[dealloc_ptr];

  if (size == 0) {
    // End of command buffer wrap down
    dealloc_ptr = 0;
    goto tryagain;
  }

  if (size & 1) {
    // Still used, nothing can be deallocated
    return false;
  }

  dealloc_ptr += (size >> 1) + 8;
  return true;
}

CommandQueueMT::CommandQueueMT(bool p_sync) {

  read_ptr = 0;
  write_ptr = 0;
  dealloc_ptr = 0;
  command_mem = (uint8_t *)memalloc(COMMAND_MEM_SIZE);

  for (int i = 0; i < SYNC_SEMAPHORES; i++) {

    sync_sems[i].in_use = false;
  }
  if (p_sync)
    sync = memnew(Semaphore);
  else
    sync = NULL;
}

CommandQueueMT::~CommandQueueMT() {

  if (sync)
    memdelete(sync);
  memfree(command_mem);
}

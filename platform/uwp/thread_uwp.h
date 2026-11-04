
#ifndef THREAD_UWP_H
#define THREAD_UWP_H

#ifdef UWP_ENABLED

#include "core/os/thread.h"

#include <thread>

class ThreadUWP : public Thread {

  std::thread thread;

  static Thread *create_func_uwp(ThreadCreateCallback p_callback, void *,
                                 const Settings &);
  static ID get_thread_id_func_uwp();
  static void wait_to_finish_func_uwp(Thread *p_thread);

  ThreadUWP();

public:
  virtual ID get_id() const;

  static void make_default();

  ~ThreadUWP();
};

#endif

#endif

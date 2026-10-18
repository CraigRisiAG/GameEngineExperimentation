
#ifndef THREAD_POSIX_H
#define THREAD_POSIX_H

#if (defined(UNIX_ENABLED) || defined(PTHREAD_ENABLED)) && !defined(NO_THREADS)

#include "core/os/thread.h"
#include <pthread.h>
#include <sys/types.h>

class ThreadPosix : public Thread {

  static pthread_key_t thread_id_key;
  static ID next_thread_id;

  pthread_t pthread;
  pthread_attr_t pthread_attr;
  ThreadCreateCallback callback;
  void *user;
  ID id;

  static Thread *create_thread_posix();

  static void *thread_callback(void *userdata);

  static Thread *create_func_posix(ThreadCreateCallback p_callback, void *,
                                   const Settings &);
  static ID get_thread_id_func_posix();
  static void wait_to_finish_func_posix(Thread *p_thread);

  static Error set_name_func_posix(const String &p_name);

  ThreadPosix();

public:
  virtual ID get_id() const;

  static void make_default();

  ~ThreadPosix();
};

#endif

#endif

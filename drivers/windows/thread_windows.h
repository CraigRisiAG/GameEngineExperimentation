
#ifndef THREAD_WINDOWS_H
#define THREAD_WINDOWS_H

#ifdef WINDOWS_ENABLED

#include "core/os/thread.h"
#include "core/script_language.h"

#include <windows.h>

class ThreadWindows : public Thread {

  ThreadCreateCallback callback;
  void *user;
  ID id;
  HANDLE handle;

  static Thread *create_thread_windows();

  static DWORD WINAPI thread_callback(LPVOID userdata);

  static Thread *create_func_windows(ThreadCreateCallback p_callback, void *,
                                     const Settings &);
  static ID get_thread_id_func_windows();
  static void wait_to_finish_func_windows(Thread *p_thread);

  ThreadWindows();

public:
  virtual ID get_id() const;

  static void make_default();

  ~ThreadWindows();
};

#endif

#endif

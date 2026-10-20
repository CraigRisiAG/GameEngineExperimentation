
#include "thread_windows.h"

#if defined(WINDOWS_ENABLED) && !defined(UWP_ENABLED)

#include "core/os/memory.h"

Thread::ID ThreadWindows::get_id() const { return id; }

Thread *ThreadWindows::create_thread_windows() { return memnew(ThreadWindows); }

DWORD ThreadWindows::thread_callback(LPVOID userdata) {

  ThreadWindows *t = reinterpret_cast<ThreadWindows *>(userdata);

  ScriptServer::thread_enter(); // scripts may need to attach a stack

  t->id = (ID)GetCurrentThreadId(); // must implement
  t->callback(t->user);
  SetEvent(t->handle);

  ScriptServer::thread_exit();

  return 0;
}

Thread *ThreadWindows::create_func_windows(ThreadCreateCallback p_callback,
                                           void *p_user, const Settings &) {

  ThreadWindows *tr = memnew(ThreadWindows);
  tr->callback = p_callback;
  tr->user = p_user;
  tr->handle = CreateEvent(NULL, TRUE, FALSE, NULL);

  QueueUserWorkItem(thread_callback, tr, WT_EXECUTELONGFUNCTION);

  return tr;
}
Thread::ID ThreadWindows::get_thread_id_func_windows() {

  return (ID)GetCurrentThreadId(); // must implement
}
void ThreadWindows::wait_to_finish_func_windows(Thread *p_thread) {

  ThreadWindows *tp = static_cast<ThreadWindows *>(p_thread);
  ERR_FAIL_COND(!tp);
  WaitForSingleObject(tp->handle, INFINITE);
  CloseHandle(tp->handle);
  //`memdelete(tp);
}

void ThreadWindows::make_default() {

  create_func = create_func_windows;
  get_thread_id_func = get_thread_id_func_windows;
  wait_to_finish_func = wait_to_finish_func_windows;
}

ThreadWindows::ThreadWindows() : handle(NULL) {}

ThreadWindows::~ThreadWindows() {}

#endif

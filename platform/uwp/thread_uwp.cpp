
#include "thread_uwp.h"

#include "core/os/memory.h"

Thread *ThreadUWP::create_func_uwp(ThreadCreateCallback p_callback,
                                   void *p_user, const Settings &) {

  ThreadUWP *thread = memnew(ThreadUWP);

  std::thread new_thread(p_callback, p_user);
  std::swap(thread->thread, new_thread);

  return thread;
};

Thread::ID ThreadUWP::get_thread_id_func_uwp() {

  return std::hash<std::thread::id>()(std::this_thread::get_id());
};

void ThreadUWP::wait_to_finish_func_uwp(Thread *p_thread) {

  ThreadUWP *tp = static_cast<ThreadUWP *>(p_thread);
  tp->thread.join();
};

Thread::ID ThreadUWP::get_id() const {

  return std::hash<std::thread::id>()(thread.get_id());
};

void ThreadUWP::make_default() {
  create_func = create_func_uwp;
  get_thread_id_func = get_thread_id_func_uwp;
  wait_to_finish_func = wait_to_finish_func_uwp;
};

ThreadUWP::ThreadUWP() {

};

ThreadUWP::~ThreadUWP() {

};


#include "mono_gc_handle.h"

#include "mono_gd/gd_mono.h"

void MonoGCHandleData::release() {
#ifdef DEBUG_ENABLED
  CRASH_COND(handle && GDMono::get_singleton() == NULL);
#endif

  if (handle && GDMono::get_singleton()->is_runtime_initialized()) {
    GDMonoUtils::free_gchandle(handle);
    handle = 0;
  }
}

MonoGCHandleData MonoGCHandleData::new_strong_handle(MonoObject *p_object) {
  return MonoGCHandleData(GDMonoUtils::new_strong_gchandle(p_object),
                          gdmono::GCHandleType::STRONG_HANDLE);
}

MonoGCHandleData
MonoGCHandleData::new_strong_handle_pinned(MonoObject *p_object) {
  return MonoGCHandleData(GDMonoUtils::new_strong_gchandle_pinned(p_object),
                          gdmono::GCHandleType::STRONG_HANDLE);
}

MonoGCHandleData MonoGCHandleData::new_weak_handle(MonoObject *p_object) {
  return MonoGCHandleData(GDMonoUtils::new_weak_gchandle(p_object),
                          gdmono::GCHandleType::WEAK_HANDLE);
}

Ref<MonoGCHandleRef> MonoGCHandleRef::create_strong(MonoObject *p_object) {

  return memnew(MonoGCHandleRef(MonoGCHandleData::new_strong_handle(p_object)));
}

Ref<MonoGCHandleRef> MonoGCHandleRef::create_weak(MonoObject *p_object) {

  return memnew(MonoGCHandleRef(MonoGCHandleData::new_weak_handle(p_object)));
}

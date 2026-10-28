
#ifndef CSHARP_GC_HANDLE_H
#define CSHARP_GC_HANDLE_H

#include <mono/jit/jit.h>

#include "core/reference.h"

namespace gdmono {

enum class GCHandleType : char { NIL, STRONG_HANDLE, WEAK_HANDLE };

}

// Manual release of the GC handle must be done when using this struct
struct MonoGCHandleData {
  uint32_t handle;
  gdmono::GCHandleType type;

  _FORCE_INLINE_ bool is_released() const { return !handle; }
  _FORCE_INLINE_ bool is_weak() const {
    return type == gdmono::GCHandleType::WEAK_HANDLE;
  }

  _FORCE_INLINE_ MonoObject *get_target() const {
    return handle ? mono_gchandle_get_target(handle) : NULL;
  }

  void release();

  MonoGCHandleData &operator=(const MonoGCHandleData &p_other) {
#ifdef DEBUG_ENABLED
    CRASH_COND(!is_released());
#endif
    handle = p_other.handle;
    type = p_other.type;
    return *this;
  }

  MonoGCHandleData(const MonoGCHandleData &) = default;

  MonoGCHandleData() : handle(0), type(gdmono::GCHandleType::NIL) {}

  MonoGCHandleData(uint32_t p_handle, gdmono::GCHandleType p_type)
      : handle(p_handle), type(p_type) {}

  static MonoGCHandleData new_strong_handle(MonoObject *p_object);
  static MonoGCHandleData new_strong_handle_pinned(MonoObject *p_object);
  static MonoGCHandleData new_weak_handle(MonoObject *p_object);
};

class MonoGCHandleRef : public Reference {

  GDCLASS(MonoGCHandleRef, Reference);

  MonoGCHandleData data;

public:
  static Ref<MonoGCHandleRef> create_strong(MonoObject *p_object);
  static Ref<MonoGCHandleRef> create_weak(MonoObject *p_object);

  _FORCE_INLINE_ bool is_released() const { return data.is_released(); }
  _FORCE_INLINE_ bool is_weak() const { return data.is_weak(); }

  _FORCE_INLINE_ MonoObject *get_target() const { return data.get_target(); }

  void release() { data.release(); }

  _FORCE_INLINE_ void set_handle(uint32_t p_handle,
                                 gdmono::GCHandleType p_handle_type) {
    data = MonoGCHandleData(p_handle, p_handle_type);
  }

  MonoGCHandleRef(const MonoGCHandleData &p_gc_handle_data)
      : data(p_gc_handle_data) {}
  ~MonoGCHandleRef() { release(); }
};

#endif // CSHARP_GC_HANDLE_H

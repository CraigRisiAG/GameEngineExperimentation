
#ifndef MANAGED_CALLABLE_H
#define MANAGED_CALLABLE_H

#include <mono/metadata/object.h>

#include "core/callable.h"
#include "core/os/mutex.h"
#include "core/self_list.h"

#include "mono_gc_handle.h"
#include "mono_gd/gd_mono_method.h"

class ManagedCallable : public CallableCustom {
  friend class CSharpLanguage;
  MonoGCHandleData delegate_handle;
  GDMonoMethod *delegate_invoke;

#ifdef GD_MONO_HOT_RELOAD
  SelfList<ManagedCallable> self_instance = this;
  static SelfList<ManagedCallable>::List instances;
  static Map<ManagedCallable *, Array> instances_pending_reload;
  static Mutex instances_mutex;
#endif

public:
  uint32_t hash() const override;
  String get_as_text() const override;
  CompareEqualFunc get_compare_equal_func() const override;
  CompareLessFunc get_compare_less_func() const override;
  ObjectID get_object() const override;
  void call(const Variant **p_arguments, int p_argcount,
            Variant &r_return_value,
            Callable::CallError &r_call_error) const override;

  _FORCE_INLINE_ MonoDelegate *get_delegate() {
    return (MonoDelegate *)delegate_handle.get_target();
  }

  void set_delegate(MonoDelegate *p_delegate);

  static bool compare_equal(const CallableCustom *p_a,
                            const CallableCustom *p_b);
  static bool compare_less(const CallableCustom *p_a,
                           const CallableCustom *p_b);

  static constexpr CompareEqualFunc compare_equal_func_ptr =
      &ManagedCallable::compare_equal;
  static constexpr CompareEqualFunc compare_less_func_ptr =
      &ManagedCallable::compare_less;

  ManagedCallable(MonoDelegate *p_delegate);
  ~ManagedCallable();
};

#endif // MANAGED_CALLABLE_H

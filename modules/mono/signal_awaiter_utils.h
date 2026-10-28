
#ifndef SIGNAL_AWAITER_UTILS_H
#define SIGNAL_AWAITER_UTILS_H

#include "core/reference.h"

#include "csharp_script.h"
#include "mono_gc_handle.h"

Error gd_mono_connect_signal_awaiter(Object *p_source,
                                     const StringName &p_signal,
                                     Object *p_target, MonoObject *p_awaiter);

class SignalAwaiterCallable : public CallableCustom {
  ObjectID target_id;
  MonoGCHandleData awaiter_handle;
  StringName signal;

public:
  static bool compare_equal(const CallableCustom *p_a,
                            const CallableCustom *p_b);
  static bool compare_less(const CallableCustom *p_a,
                           const CallableCustom *p_b);

  static constexpr CompareEqualFunc compare_equal_func_ptr =
      &SignalAwaiterCallable::compare_equal;
  static constexpr CompareEqualFunc compare_less_func_ptr =
      &SignalAwaiterCallable::compare_less;

  uint32_t hash() const override;

  String get_as_text() const override;

  CompareEqualFunc get_compare_equal_func() const override;
  CompareLessFunc get_compare_less_func() const override;

  ObjectID get_object() const override;

  _FORCE_INLINE_ StringName get_signal() const { return signal; }

  void call(const Variant **p_arguments, int p_argcount,
            Variant &r_return_value,
            Callable::CallError &r_call_error) const override;

  SignalAwaiterCallable(Object *p_target, MonoObject *p_awaiter,
                        const StringName &p_signal);
  ~SignalAwaiterCallable();
};

class EventSignalCallable : public CallableCustom {
  Object *owner;
  const CSharpScript::EventSignal *event_signal;

public:
  static bool compare_equal(const CallableCustom *p_a,
                            const CallableCustom *p_b);
  static bool compare_less(const CallableCustom *p_a,
                           const CallableCustom *p_b);

  static constexpr CompareEqualFunc compare_equal_func_ptr =
      &EventSignalCallable::compare_equal;
  static constexpr CompareEqualFunc compare_less_func_ptr =
      &EventSignalCallable::compare_less;

  uint32_t hash() const override;

  String get_as_text() const override;

  CompareEqualFunc get_compare_equal_func() const override;
  CompareLessFunc get_compare_less_func() const override;

  ObjectID get_object() const override;

  StringName get_signal() const;

  void call(const Variant **p_arguments, int p_argcount,
            Variant &r_return_value,
            Callable::CallError &r_call_error) const override;

  EventSignalCallable(Object *p_owner,
                      const CSharpScript::EventSignal *p_event_signal);
};

#endif // SIGNAL_AWAITER_UTILS_H

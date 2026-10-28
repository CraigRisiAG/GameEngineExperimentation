
#ifndef BASE_OBJECT_GLUE_H
#define BASE_OBJECT_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "core/class_db.h"
#include "core/object.h"

#include "../mono_gd/gd_mono_marshal.h"

Object *godot_icall_Object_Ctor(MonoObject *p_obj);

void godot_icall_Object_Disposed(MonoObject *p_obj, Object *p_ptr);

void godot_icall_Reference_Disposed(MonoObject *p_obj, Object *p_ptr,
                                    MonoBoolean p_is_finalizer);

void godot_icall_Object_ConnectEventSignals(Object *p_ptr);

MethodBind *godot_icall_Object_ClassDB_get_method(StringName *p_type,
                                                  MonoString *p_method);

MonoObject *godot_icall_Object_weakref(Object *p_ptr);

Error godot_icall_SignalAwaiter_connect(Object *p_source, StringName *p_signal,
                                        Object *p_target,
                                        MonoObject *p_awaiter);

// DynamicGodotObject

MonoArray *godot_icall_DynamicGodotObject_SetMemberList(Object *p_ptr);

MonoBoolean godot_icall_DynamicGodotObject_InvokeMember(Object *p_ptr,
                                                        MonoString *p_name,
                                                        MonoArray *p_args,
                                                        MonoObject **r_result);

MonoBoolean godot_icall_DynamicGodotObject_GetMember(Object *p_ptr,
                                                     MonoString *p_name,
                                                     MonoObject **r_result);

MonoBoolean godot_icall_DynamicGodotObject_SetMember(Object *p_ptr,
                                                     MonoString *p_name,
                                                     MonoObject *p_value);

MonoString *godot_icall_Object_ToString(Object *p_ptr);

// Register internal calls

void godot_register_object_icalls();

#endif // MONO_GLUE_ENABLED

#endif // BASE_OBJECT_GLUE_H

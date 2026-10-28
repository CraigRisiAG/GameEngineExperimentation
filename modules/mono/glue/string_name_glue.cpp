
#include "string_name_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "core/ustring.h"

StringName *godot_icall_StringName_Ctor(MonoString *p_path) {
  return memnew(StringName(GDMonoMarshal::mono_string_to_godot(p_path)));
}

void godot_icall_StringName_Dtor(StringName *p_ptr) {
  ERR_FAIL_NULL(p_ptr);
  memdelete(p_ptr);
}

MonoString *godot_icall_StringName_operator_String(StringName *p_np) {
  return GDMonoMarshal::mono_string_from_godot(p_np->operator String());
}

MonoBoolean godot_icall_StringName_is_empty(StringName *p_ptr) {
  return (MonoBoolean)(p_ptr == StringName());
}

void godot_register_string_name_icalls() {
  mono_add_internal_call("Godot.StringName::godot_icall_StringName_Ctor",
                         (void *)godot_icall_StringName_Ctor);
  mono_add_internal_call("Godot.StringName::godot_icall_StringName_Dtor",
                         (void *)godot_icall_StringName_Dtor);
  mono_add_internal_call(
      "Godot.StringName::godot_icall_StringName_operator_String",
      (void *)godot_icall_StringName_operator_String);
  mono_add_internal_call("Godot.StringName::godot_icall_StringName_is_empty",
                         (void *)godot_icall_StringName_is_empty);
}

#endif // MONO_GLUE_ENABLED

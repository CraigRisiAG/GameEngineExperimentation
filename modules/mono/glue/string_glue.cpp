
#include "string_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "core/ustring.h"
#include "core/variant.h"
#include "core/vector.h"

MonoArray *godot_icall_String_md5_buffer(MonoString *p_str) {
  Vector<uint8_t> ret = GDMonoMarshal::mono_string_to_godot(p_str).md5_buffer();
  // TODO Check possible Array/Vector<uint8_t> problem?
  return GDMonoMarshal::Array_to_mono_array(Variant(ret));
}

MonoString *godot_icall_String_md5_text(MonoString *p_str) {
  String ret = GDMonoMarshal::mono_string_to_godot(p_str).md5_text();
  return GDMonoMarshal::mono_string_from_godot(ret);
}

int godot_icall_String_rfind(MonoString *p_str, MonoString *p_what,
                             int p_from) {
  String what = GDMonoMarshal::mono_string_to_godot(p_what);
  return GDMonoMarshal::mono_string_to_godot(p_str).rfind(what, p_from);
}

int godot_icall_String_rfindn(MonoString *p_str, MonoString *p_what,
                              int p_from) {
  String what = GDMonoMarshal::mono_string_to_godot(p_what);
  return GDMonoMarshal::mono_string_to_godot(p_str).rfindn(what, p_from);
}

MonoArray *godot_icall_String_sha256_buffer(MonoString *p_str) {
  Vector<uint8_t> ret =
      GDMonoMarshal::mono_string_to_godot(p_str).sha256_buffer();
  return GDMonoMarshal::Array_to_mono_array(Variant(ret));
}

MonoString *godot_icall_String_sha256_text(MonoString *p_str) {
  String ret = GDMonoMarshal::mono_string_to_godot(p_str).sha256_text();
  return GDMonoMarshal::mono_string_from_godot(ret);
}

void godot_register_string_icalls() {
  mono_add_internal_call(
      "Godot.StringExtensions::godot_icall_String_md5_buffer",
      (void *)godot_icall_String_md5_buffer);
  mono_add_internal_call("Godot.StringExtensions::godot_icall_String_md5_text",
                         (void *)godot_icall_String_md5_text);
  mono_add_internal_call("Godot.StringExtensions::godot_icall_String_rfind",
                         (void *)godot_icall_String_rfind);
  mono_add_internal_call("Godot.StringExtensions::godot_icall_String_rfindn",
                         (void *)godot_icall_String_rfindn);
  mono_add_internal_call(
      "Godot.StringExtensions::godot_icall_String_sha256_buffer",
      (void *)godot_icall_String_sha256_buffer);
  mono_add_internal_call(
      "Godot.StringExtensions::godot_icall_String_sha256_text",
      (void *)godot_icall_String_sha256_text);
}

#endif // MONO_GLUE_ENABLED

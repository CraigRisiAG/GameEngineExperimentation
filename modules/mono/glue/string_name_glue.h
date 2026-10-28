
#ifndef STRING_NAME_GLUE_H
#define STRING_NAME_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "core/string_name.h"

#include "../mono_gd/gd_mono_marshal.h"

StringName *godot_icall_StringName_Ctor(MonoString *p_path);

void godot_icall_StringName_Dtor(StringName *p_ptr);

MonoString *godot_icall_StringName_operator_String(StringName *p_np);

MonoBoolean godot_icall_StringName_is_empty(StringName *p_ptr);

// Register internal calls

void godot_register_string_name_icalls();

#endif // MONO_GLUE_ENABLED

#endif // STRING_NAME_GLUE_H

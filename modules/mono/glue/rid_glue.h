
#ifndef RID_GLUE_H
#define RID_GLUE_H

#ifdef MONO_GLUE_ENABLED

#include "core/object.h"
#include "core/rid.h"

#include "../mono_gd/gd_mono_marshal.h"

RID *godot_icall_RID_Ctor(Object *p_from);

void godot_icall_RID_Dtor(RID *p_ptr);

uint32_t godot_icall_RID_get_id(RID *p_ptr);

// Register internal calls

void godot_register_rid_icalls();

#endif // MONO_GLUE_ENABLED

#endif // RID_GLUE_H

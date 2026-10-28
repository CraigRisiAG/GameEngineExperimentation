
#include "rid_glue.h"

#ifdef MONO_GLUE_ENABLED

#include "core/resource.h"

RID *godot_icall_RID_Ctor(Object *p_from) {
  Resource *res_from = Object::cast_to<Resource>(p_from);

  if (res_from)
    return memnew(RID(res_from->get_rid()));

  return memnew(RID);
}

void godot_icall_RID_Dtor(RID *p_ptr) {
  ERR_FAIL_NULL(p_ptr);
  memdelete(p_ptr);
}

uint32_t godot_icall_RID_get_id(RID *p_ptr) { return p_ptr->get_id(); }

void godot_register_rid_icalls() {
  mono_add_internal_call("Godot.RID::godot_icall_RID_Ctor",
                         (void *)godot_icall_RID_Ctor);
  mono_add_internal_call("Godot.RID::godot_icall_RID_Dtor",
                         (void *)godot_icall_RID_Dtor);
  mono_add_internal_call("Godot.RID::godot_icall_RID_get_id",
                         (void *)godot_icall_RID_get_id);
}

#endif // MONO_GLUE_ENABLED

// This file registers and unregisters the MobileVRInterface class with the
// Godot engine. It ensures that the MobileVRInterface is available to the
// ARVRServer and can be used for handling mobile VR input.
#include "register_types.h"

#include "mobile_vr_interface.h"

void register_mobile_vr_types() {
  ClassDB::register_class<MobileVRInterface>();

  Ref<MobileVRInterface> mobile_vr;
  mobile_vr.instance();
  ARVRServer::get_singleton()->add_interface(mobile_vr);
}

void unregister_mobile_vr_types() {}

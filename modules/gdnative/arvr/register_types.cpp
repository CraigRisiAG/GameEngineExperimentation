// This file implements the functions for registering and unregistering the
// GDNative ARVR module types within the Godot engine. It includes the
// register_arvr_types and unregister_arvr_types functions, which are used to
// integrate the GDNative ARVR functionality with the Godot editor.
#include "register_types.h"
#include "arvr_interface_gdnative.h"

void register_arvr_types() { ClassDB::register_class<ARVRInterfaceGDNative>(); }

void unregister_arvr_types() {}

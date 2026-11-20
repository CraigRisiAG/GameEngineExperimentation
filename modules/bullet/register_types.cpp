// This file implements the registration of the Bullet physics server types
// within the Godot engine. It defines the functions for registering and
// unregistering the Bullet physics server, as well as setting it as the default
// physics server for 3D physics.
#include "register_types.h"

#include "bullet_physics_server.h"
#include "core/class_db.h"
#include "core/project_settings.h"

/**
        @author AndreaCatania
*/

#ifndef _3D_DISABLED
PhysicsServer *_createBulletPhysicsCallback() {
  return memnew(BulletPhysicsServer);
}
#endif

void register_bullet_types() {
#ifndef _3D_DISABLED
  PhysicsServerManager::register_server("Bullet",
                                        &_createBulletPhysicsCallback);
  PhysicsServerManager::set_default_server("Bullet", 1);

  GLOBAL_DEF("physics/3d/active_soft_world", true);
  ProjectSettings::get_singleton()->set_custom_property_info(
      "physics/3d/active_soft_world",
      PropertyInfo(Variant::BOOL, "physics/3d/active_soft_world"));
#endif
}

void unregister_bullet_types() {}

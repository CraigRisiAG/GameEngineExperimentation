// This file declares the RIDBullet class, which represents a wrapper around a
// RID (Resource ID) for the Bullet physics server within the Godot engine. It
// provides methods for setting and getting the RID and the associated
// BulletPhysicsServer.
#ifndef RID_BULLET_H
#define RID_BULLET_H

#include "core/rid.h"

/**
        @author AndreaCatania
*/

class BulletPhysicsServer;

class RIDBullet {
  RID self;
  BulletPhysicsServer *physicsServer;

public:
  _FORCE_INLINE_ void set_self(const RID &p_self) { self = p_self; }
  _FORCE_INLINE_ RID get_self() const { return self; }

  _FORCE_INLINE_ void
  _set_physics_server(BulletPhysicsServer *p_physicsServer) {
    physicsServer = p_physicsServer;
  }
  _FORCE_INLINE_ BulletPhysicsServer *get_physics_server() const {
    return physicsServer;
  }
};
#endif

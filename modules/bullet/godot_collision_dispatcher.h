// This file declares the GodotCollisionDispatcher class, which provides a
// custom collision dispatcher for the Bullet physics engine within the Godot
// engine. It defines the methods for determining whether collisions and
// responses are needed between collision objects, with special handling for
// area-type objects.
#ifndef GODOT_COLLISION_DISPATCHER_H
#define GODOT_COLLISION_DISPATCHER_H

#include "core/int_types.h"

#include <btBulletDynamicsCommon.h>

/**
        @author AndreaCatania
*/

/// This class is required to implement custom collision behaviour in the
/// narrowphase
class GodotCollisionDispatcher : public btCollisionDispatcher {
private:
  static const int CASTED_TYPE_AREA;

public:
  GodotCollisionDispatcher(btCollisionConfiguration *collisionConfiguration);
  virtual bool needsCollision(const btCollisionObject *body0,
                              const btCollisionObject *body1);
  virtual bool needsResponse(const btCollisionObject *body0,
                             const btCollisionObject *body1);
};
#endif

// This file declares the JointBullet class, which provides a base class for
// joint constraints in the Bullet physics engine within the Godot engine. It
// defines the constructor and destructor for the JointBullet class, as well as
// a pure virtual method for getting the joint type.
#ifndef JOINT_BULLET_H
#define JOINT_BULLET_H

#include "constraint_bullet.h"
#include "servers/physics_server.h"

/**
        @author AndreaCatania
*/

class RigidBodyBullet;
class btTypedConstraint;

class JointBullet : public ConstraintBullet {

public:
  JointBullet();
  virtual ~JointBullet();

  virtual PhysicsServer::JointType get_type() const = 0;
};
#endif

// This file declares the ConeTwistJointBullet class, which represents a cone
// twist joint in the Bullet physics engine within the Godot engine. It defines
// the structure for creating and configuring the joint, setting and getting
// joint parameters, and integrating with the Bullet physics engine.
#ifndef CONE_TWIST_JOINT_BULLET_H
#define CONE_TWIST_JOINT_BULLET_H

#include "joint_bullet.h"

/**
        @author AndreaCatania
*/

class RigidBodyBullet;

class ConeTwistJointBullet : public JointBullet {
  class btConeTwistConstraint *coneConstraint;

public:
  ConeTwistJointBullet(RigidBodyBullet *rbA, RigidBodyBullet *rbB,
                       const Transform &rbAFrame, const Transform &rbBFrame);

  virtual PhysicsServer::JointType get_type() const {
    return PhysicsServer::JOINT_CONE_TWIST;
  }

  void set_param(PhysicsServer::ConeTwistJointParam p_param, real_t p_value);
  real_t get_param(PhysicsServer::ConeTwistJointParam p_param) const;
};
#endif

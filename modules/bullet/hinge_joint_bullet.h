// This file declares the HingeJointBullet class, which provides a hinge joint
// constraint for the Bullet physics engine within the Godot engine. It defines
// the constructors for creating hinge joints with either frame transforms or
// pivot and axis vectors, as well as methods for getting the hinge angle and
// setting and getting hinge joint parameters.
#ifndef HINGE_JOINT_BULLET_H
#define HINGE_JOINT_BULLET_H

#include "joint_bullet.h"

/**
        @author AndreaCatania
*/

class HingeJointBullet : public JointBullet {
  class btHingeConstraint *hingeConstraint;

public:
  HingeJointBullet(RigidBodyBullet *rbA, RigidBodyBullet *rbB,
                   const Transform &frameA, const Transform &frameB);
  HingeJointBullet(RigidBodyBullet *rbA, RigidBodyBullet *rbB,
                   const Vector3 &pivotInA, const Vector3 &pivotInB,
                   const Vector3 &axisInA, const Vector3 &axisInB);

  virtual PhysicsServer::JointType get_type() const {
    return PhysicsServer::JOINT_HINGE;
  }

  real_t get_hinge_angle();

  void set_param(PhysicsServer::HingeJointParam p_param, real_t p_value);
  real_t get_param(PhysicsServer::HingeJointParam p_param) const;

  void set_flag(PhysicsServer::HingeJointFlag p_flag, bool p_value);
  bool get_flag(PhysicsServer::HingeJointFlag p_flag) const;
};
#endif

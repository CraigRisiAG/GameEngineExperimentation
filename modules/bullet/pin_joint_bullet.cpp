
#include "pin_joint_bullet.h"

#include "bullet_types_converter.h"
#include "rigid_body_bullet.h"

#include <BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h>

/**
        @author AndreaCatania
*/

PinJointBullet::PinJointBullet(RigidBodyBullet *p_body_a,
                               const Vector3 &p_pos_a,
                               RigidBodyBullet *p_body_b,
                               const Vector3 &p_pos_b)
    : JointBullet() {
  if (p_body_b) {

    btVector3 btPivotA;
    btVector3 btPivotB;
    G_TO_B(p_pos_a * p_body_a->get_body_scale(), btPivotA);
    G_TO_B(p_pos_b * p_body_b->get_body_scale(), btPivotB);
    p2pConstraint = bulletnew(btPoint2PointConstraint(
        *p_body_a->get_bt_rigid_body(), *p_body_b->get_bt_rigid_body(),
        btPivotA, btPivotB));
  } else {
    btVector3 btPivotA;
    G_TO_B(p_pos_a, btPivotA);
    p2pConstraint = bulletnew(
        btPoint2PointConstraint(*p_body_a->get_bt_rigid_body(), btPivotA));
  }

  setup(p2pConstraint);
}

PinJointBullet::~PinJointBullet() {}

void PinJointBullet::set_param(PhysicsServer::PinJointParam p_param,
                               real_t p_value) {
  switch (p_param) {
  case PhysicsServer::PIN_JOINT_BIAS:
    p2pConstraint->m_setting.m_tau = p_value;
    break;
  case PhysicsServer::PIN_JOINT_DAMPING:
    p2pConstraint->m_setting.m_damping = p_value;
    break;
  case PhysicsServer::PIN_JOINT_IMPULSE_CLAMP:
    p2pConstraint->m_setting.m_impulseClamp = p_value;
    break;
  }
}

real_t PinJointBullet::get_param(PhysicsServer::PinJointParam p_param) const {
  switch (p_param) {
  case PhysicsServer::PIN_JOINT_BIAS:
    return p2pConstraint->m_setting.m_tau;
  case PhysicsServer::PIN_JOINT_DAMPING:
    return p2pConstraint->m_setting.m_damping;
  case PhysicsServer::PIN_JOINT_IMPULSE_CLAMP:
    return p2pConstraint->m_setting.m_impulseClamp;
  }
  // Compiler doesn't seem to notice that all code paths are fulfilled...
  return 0;
}

void PinJointBullet::setPivotInA(const Vector3 &p_pos) {
  btVector3 btVec;
  G_TO_B(p_pos, btVec);
  p2pConstraint->setPivotA(btVec);
}

void PinJointBullet::setPivotInB(const Vector3 &p_pos) {
  btVector3 btVec;
  G_TO_B(p_pos, btVec);
  p2pConstraint->setPivotB(btVec);
}

Vector3 PinJointBullet::getPivotInA() {
  btVector3 vec = p2pConstraint->getPivotInA();
  Vector3 gVec;
  B_TO_G(vec, gVec);
  return gVec;
}

Vector3 PinJointBullet::getPivotInB() {
  btVector3 vec = p2pConstraint->getPivotInB();
  Vector3 gVec;
  B_TO_G(vec, gVec);
  return gVec;
}

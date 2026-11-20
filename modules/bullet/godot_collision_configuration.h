// This file declares the GodotCollisionConfiguration and
// GodotSoftCollisionConfiguration classes, which provide custom collision
// configurations for the Bullet physics engine within the Godot engine. It
// defines the structure for creating and managing collision algorithm create
// functions, handling custom convex shape types, and integrating with the
// Bullet collision configuration system.
#ifndef GODOT_COLLISION_CONFIGURATION_H
#define GODOT_COLLISION_CONFIGURATION_H

#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>

/**
        @author AndreaCatania
*/

class btDiscreteDynamicsWorld;

class GodotCollisionConfiguration : public btDefaultCollisionConfiguration {
  btCollisionAlgorithmCreateFunc *m_rayWorldCF;
  btCollisionAlgorithmCreateFunc *m_swappedRayWorldCF;

public:
  GodotCollisionConfiguration(
      const btDiscreteDynamicsWorld *world,
      const btDefaultCollisionConstructionInfo &constructionInfo =
          btDefaultCollisionConstructionInfo());
  virtual ~GodotCollisionConfiguration();

  virtual btCollisionAlgorithmCreateFunc *
  getCollisionAlgorithmCreateFunc(int proxyType0, int proxyType1);
  virtual btCollisionAlgorithmCreateFunc *
  getClosestPointsAlgorithmCreateFunc(int proxyType0, int proxyType1);
};

class GodotSoftCollisionConfiguration
    : public btSoftBodyRigidBodyCollisionConfiguration {
  btCollisionAlgorithmCreateFunc *m_rayWorldCF;
  btCollisionAlgorithmCreateFunc *m_swappedRayWorldCF;

public:
  GodotSoftCollisionConfiguration(
      const btDiscreteDynamicsWorld *world,
      const btDefaultCollisionConstructionInfo &constructionInfo =
          btDefaultCollisionConstructionInfo());
  virtual ~GodotSoftCollisionConfiguration();

  virtual btCollisionAlgorithmCreateFunc *
  getCollisionAlgorithmCreateFunc(int proxyType0, int proxyType1);
  virtual btCollisionAlgorithmCreateFunc *
  getClosestPointsAlgorithmCreateFunc(int proxyType0, int proxyType1);
};
#endif

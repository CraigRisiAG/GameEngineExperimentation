// This file contains the declaration of the MultiMeshInstance class,
// which handles instances of MultiMesh objects in 3D. It manages the
// MultiMesh resource, updates the visual representation, and provides
// functionality for retrieving the axis-aligned bounding box (AABB)
// and faces for rendering purposes.
#ifndef MULTIMESH_INSTANCE_H
#define MULTIMESH_INSTANCE_H

#include "scene/3d/visual_instance.h"
#include "scene/resources/multimesh.h"

class MultiMeshInstance : public GeometryInstance {
  GDCLASS(MultiMeshInstance, GeometryInstance);

  Ref<MultiMesh> multimesh;

protected:
  static void _bind_methods();
  // bind helpers

public:
  virtual Vector<Face3> get_faces(uint32_t p_usage_flags) const;

  void set_multimesh(const Ref<MultiMesh> &p_multimesh);
  Ref<MultiMesh> get_multimesh() const;

  virtual AABB get_aabb() const;

  MultiMeshInstance();
  ~MultiMeshInstance();
};

#endif // MULTIMESH_INSTANCE_H

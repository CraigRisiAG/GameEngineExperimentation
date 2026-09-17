
#ifndef SPHERE_SHAPE_H
#define SPHERE_SHAPE_H

#include "scene/resources/shape.h"

class SphereShape : public Shape {

  GDCLASS(SphereShape, Shape);
  float radius;

protected:
  static void _bind_methods();

  virtual void _update_shape();

public:
  void set_radius(float p_radius);
  float get_radius() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const;

  SphereShape();
};

#endif // SPHERE_SHAPE_H

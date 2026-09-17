
#ifndef CYLINDER_SHAPE_H
#define CYLINDER_SHAPE_H

#include "scene/resources/shape.h"

class CylinderShape : public Shape {

  GDCLASS(CylinderShape, Shape);
  float radius;
  float height;

protected:
  static void _bind_methods();
  virtual void _update_shape();

public:
  void set_radius(float p_radius);
  float get_radius() const;
  void set_height(float p_height);
  float get_height() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const;

  CylinderShape();
};

#endif // CYLINDER_SHAPE_H


#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H

#include "scene/resources/shape.h"

class BoxShape : public Shape {

  GDCLASS(BoxShape, Shape);
  Vector3 extents;

protected:
  static void _bind_methods();

  virtual void _update_shape();

public:
  void set_extents(const Vector3 &p_extents);
  Vector3 get_extents() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const;

  BoxShape();
};

#endif // BOX_SHAPE_H

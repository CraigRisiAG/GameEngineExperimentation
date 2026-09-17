
#ifndef CONVEX_POLYGON_SHAPE_H
#define CONVEX_POLYGON_SHAPE_H

#include "scene/resources/shape.h"

class ConvexPolygonShape : public Shape {

  GDCLASS(ConvexPolygonShape, Shape);
  Vector<Vector3> points;

protected:
  static void _bind_methods();

  virtual void _update_shape();

public:
  void set_points(const Vector<Vector3> &p_points);
  Vector<Vector3> get_points() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const;

  ConvexPolygonShape();
};

#endif // CONVEX_POLYGON_SHAPE_H

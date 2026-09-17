
#ifndef RAY_SHAPE_H
#define RAY_SHAPE_H
#include "scene/resources/shape.h"

class RayShape : public Shape {

  GDCLASS(RayShape, Shape);
  float length;
  bool slips_on_slope;

protected:
  static void _bind_methods();
  virtual void _update_shape();

public:
  void set_length(float p_length);
  float get_length() const;

  void set_slips_on_slope(bool p_active);
  bool get_slips_on_slope() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const;

  RayShape();
};
#endif // RAY_SHAPE_H

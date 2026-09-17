
#ifndef WORLD_MARGIN_SHAPE_H
#define WORLD_MARGIN_SHAPE_H

#include "scene/resources/shape.h"

class WorldMarginShape : public Shape {

  GDCLASS(WorldMarginShape, Shape);
  Plane plane;

protected:
  static void _bind_methods();
  virtual void _update_shape();

public:
  void set_plane(Plane p_plane);
  Plane get_plane() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const {
    // Should be infinite?
    return 0;
  }

  WorldMarginShape();
};
#endif // WORLD_MARGIN_SHAPE_H

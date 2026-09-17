
#ifndef HEIGHT_MAP_SHAPE_H
#define HEIGHT_MAP_SHAPE_H

#include "scene/resources/shape.h"

class HeightMapShape : public Shape {
  GDCLASS(HeightMapShape, Shape);

  int map_width;
  int map_depth;
  PackedFloat32Array map_data;
  float min_height;
  float max_height;

protected:
  static void _bind_methods();
  virtual void _update_shape();

public:
  void set_map_width(int p_new);
  int get_map_width() const;
  void set_map_depth(int p_new);
  int get_map_depth() const;
  void set_map_data(PackedFloat32Array p_new);
  PackedFloat32Array get_map_data() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const;

  HeightMapShape();
};

#endif /* !HEIGHT_MAP_SHAPE_H */

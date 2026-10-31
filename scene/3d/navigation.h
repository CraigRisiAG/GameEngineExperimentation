
#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "scene/3d/navigation_region.h"
#include "scene/3d/spatial.h"

class Navigation : public Spatial {

  GDCLASS(Navigation, Spatial);

  RID map;

  Vector3 up;
  real_t cell_size;
  real_t edge_connection_margin;

protected:
  static void _bind_methods();
  void _notification(int p_what);

public:
  RID get_rid() const { return map; }

  void set_up_vector(const Vector3 &p_up);
  Vector3 get_up_vector() const;

  void set_cell_size(float p_cell_size);
  float get_cell_size() const { return cell_size; }

  void set_edge_connection_margin(float p_edge_connection_margin);
  float get_edge_connection_margin() const { return edge_connection_margin; }

  Vector<Vector3> get_simple_path(const Vector3 &p_start, const Vector3 &p_end,
                                  bool p_optimize = true) const;
  Vector3 get_closest_point_to_segment(const Vector3 &p_from,
                                       const Vector3 &p_to,
                                       bool p_use_collision = false) const;
  Vector3 get_closest_point(const Vector3 &p_point) const;
  Vector3 get_closest_point_normal(const Vector3 &p_point) const;
  RID get_closest_point_owner(const Vector3 &p_point) const;

  Navigation();
  ~Navigation();
};

#endif // NAVIGATION_H

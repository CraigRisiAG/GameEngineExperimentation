
#ifndef RAY_CAST_H
#define RAY_CAST_H

#include "scene/3d/spatial.h"

class RayCast : public Spatial {

  GDCLASS(RayCast, Spatial);

  bool enabled;
  bool collided;
  ObjectID against;
  int against_shape;
  Vector3 collision_point;
  Vector3 collision_normal;

  Vector3 cast_to;
  Set<RID> exclude;

  uint32_t collision_mask;
  bool exclude_parent_body;

  Node *debug_shape;
  Ref<Material> debug_material;

  void _create_debug_shape();
  void _update_debug_shape();
  void _clear_debug_shape();

  bool collide_with_areas;
  bool collide_with_bodies;

protected:
  void _notification(int p_what);
  void _update_raycast_state();
  static void _bind_methods();

public:
  void set_collide_with_areas(bool p_clip);
  bool is_collide_with_areas_enabled() const;

  void set_collide_with_bodies(bool p_clip);
  bool is_collide_with_bodies_enabled() const;

  void set_enabled(bool p_enabled);
  bool is_enabled() const;

  void set_cast_to(const Vector3 &p_point);
  Vector3 get_cast_to() const;

  void set_collision_mask(uint32_t p_mask);
  uint32_t get_collision_mask() const;

  void set_collision_mask_bit(int p_bit, bool p_value);
  bool get_collision_mask_bit(int p_bit) const;

  void set_exclude_parent_body(bool p_exclude_parent_body);
  bool get_exclude_parent_body() const;

  void force_raycast_update();
  bool is_colliding() const;
  Object *get_collider() const;
  int get_collider_shape() const;
  Vector3 get_collision_point() const;
  Vector3 get_collision_normal() const;

  void add_exception_rid(const RID &p_rid);
  void add_exception(const Object *p_object);
  void remove_exception_rid(const RID &p_rid);
  void remove_exception(const Object *p_object);
  void clear_exceptions();

  RayCast();
};

#endif // RAY_CAST_H

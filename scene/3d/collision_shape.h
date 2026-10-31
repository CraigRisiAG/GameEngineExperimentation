
#ifndef COLLISION_SHAPE_H
#define COLLISION_SHAPE_H

#include "scene/3d/spatial.h"
#include "scene/resources/shape.h"
class CollisionObject;
class CollisionShape : public Spatial {

  GDCLASS(CollisionShape, Spatial);
  OBJ_CATEGORY("3D Physics Nodes");

  Ref<Shape> shape;

  uint32_t owner_id;
  CollisionObject *parent;

  Node *debug_shape;
  bool debug_shape_dirty;

  void resource_changed(RES res);
  bool disabled;

protected:
  void _update_debug_shape();
  void _shape_changed();

  void _update_in_shape_owner(bool p_xform_only = false);

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void make_convex_from_brothers();

  void set_shape(const Ref<Shape> &p_shape);
  Ref<Shape> get_shape() const;

  void set_disabled(bool p_disabled);
  bool is_disabled() const;

  String get_configuration_warning() const;

  CollisionShape();
  ~CollisionShape();
};

#endif // BODY_VOLUME_H

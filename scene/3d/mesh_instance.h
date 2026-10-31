
#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H

#include "scene/3d/skeleton.h"
#include "scene/3d/visual_instance.h"
#include "scene/resources/mesh.h"
#include "scene/resources/skin.h"

class MeshInstance : public GeometryInstance {

  GDCLASS(MeshInstance, GeometryInstance);

protected:
  Ref<Mesh> mesh;
  Ref<Skin> skin;
  Ref<Skin> skin_internal;
  Ref<SkinReference> skin_ref;
  NodePath skeleton_path;

  struct BlendShapeTrack {

    int idx;
    float value;
    BlendShapeTrack() {
      idx = 0;
      value = 0;
    }
  };

  Map<StringName, BlendShapeTrack> blend_shape_tracks;
  Vector<Ref<Material>> materials;

  void _mesh_changed();
  void _resolve_skeleton_path();

protected:
  bool _set(const StringName &p_name, const Variant &p_value);
  bool _get(const StringName &p_name, Variant &r_ret) const;
  void _get_property_list(List<PropertyInfo> *p_list) const;

  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_mesh(const Ref<Mesh> &p_mesh);
  Ref<Mesh> get_mesh() const;

  void set_skin(const Ref<Skin> &p_skin);
  Ref<Skin> get_skin() const;

  void set_skeleton_path(const NodePath &p_skeleton);
  NodePath get_skeleton_path();

  int get_surface_material_count() const;
  void set_surface_material(int p_surface, const Ref<Material> &p_material);
  Ref<Material> get_surface_material(int p_surface) const;

  Node *create_trimesh_collision_node();
  void create_trimesh_collision();

  Node *create_convex_collision_node();
  void create_convex_collision();

  void create_debug_tangents();

  virtual AABB get_aabb() const;
  virtual Vector<Face3> get_faces(uint32_t p_usage_flags) const;

  MeshInstance();
  ~MeshInstance();
};

#endif

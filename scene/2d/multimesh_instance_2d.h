
#ifndef MULTIMESH_INSTANCE_2D_H
#define MULTIMESH_INSTANCE_2D_H

#include "scene/2d/node_2d.h"
#include "scene/resources/multimesh.h"

class MultiMeshInstance2D : public Node2D {
  GDCLASS(MultiMeshInstance2D, Node2D);

  Ref<MultiMesh> multimesh;

  Ref<Texture2D> texture;
  Ref<Texture2D> normal_map;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
#ifdef TOOLS_ENABLED
  virtual Rect2 _edit_get_rect() const;
#endif

  void set_multimesh(const Ref<MultiMesh> &p_multimesh);
  Ref<MultiMesh> get_multimesh() const;

  void set_texture(const Ref<Texture2D> &p_texture);
  Ref<Texture2D> get_texture() const;

  void set_normal_map(const Ref<Texture2D> &p_texture);
  Ref<Texture2D> get_normal_map() const;

  MultiMeshInstance2D();
  ~MultiMeshInstance2D();
};

#endif // MULTIMESH_INSTANCE_2D_H

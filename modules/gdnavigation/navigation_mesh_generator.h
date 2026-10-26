
#ifndef NAVIGATION_MESH_GENERATOR_H
#define NAVIGATION_MESH_GENERATOR_H

#ifndef _3D_DISABLED

#include "scene/3d/navigation_region.h"

#include <Recast.h>

#ifdef TOOLS_ENABLED
struct EditorProgress;
#endif

class NavigationMeshGenerator : public Object {
  GDCLASS(NavigationMeshGenerator, Object);

  static NavigationMeshGenerator *singleton;

protected:
  static void _bind_methods();

  static void _add_vertex(const Vector3 &p_vec3, Vector<float> &p_verticies);
  static void _add_mesh(const Ref<Mesh> &p_mesh, const Transform &p_xform,
                        Vector<float> &p_verticies, Vector<int> &p_indices);
  static void _add_faces(const PackedVector3Array &p_faces,
                         const Transform &p_xform, Vector<float> &p_verticies,
                         Vector<int> &p_indices);
  static void _parse_geometry(Transform p_accumulated_transform, Node *p_node,
                              Vector<float> &p_verticies,
                              Vector<int> &p_indices, int p_generate_from,
                              uint32_t p_collision_mask,
                              bool p_recurse_children);

  static void _convert_detail_mesh_to_native_navigation_mesh(
      const rcPolyMeshDetail *p_detail_mesh, Ref<NavigationMesh> p_nav_mesh);
  static void
  _build_recast_navigation_mesh(Ref<NavigationMesh> p_nav_mesh,
#ifdef TOOLS_ENABLED
                                EditorProgress *ep,
#endif
                                rcHeightfield *hf, rcCompactHeightfield *chf,
                                rcContourSet *cset, rcPolyMesh *poly_mesh,
                                rcPolyMeshDetail *detail_mesh,
                                Vector<float> &vertices, Vector<int> &indices);

public:
  static NavigationMeshGenerator *get_singleton();

  NavigationMeshGenerator();
  ~NavigationMeshGenerator();

  void bake(Ref<NavigationMesh> p_nav_mesh, Node *p_node);
  void clear(Ref<NavigationMesh> p_nav_mesh);
};

#endif

#endif // NAVIGATION_MESH_GENERATOR_H

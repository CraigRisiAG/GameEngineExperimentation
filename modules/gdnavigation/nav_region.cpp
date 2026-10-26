
#include "nav_region.h"

#include "nav_map.h"

/**
        @author AndreaCatania
*/

NavRegion::NavRegion() : map(NULL), polygons_dirty(true) {}

void NavRegion::set_map(NavMap *p_map) {
  map = p_map;
  polygons_dirty = true;
}

void NavRegion::set_transform(Transform p_transform) {
  transform = p_transform;
  polygons_dirty = true;
}

void NavRegion::set_mesh(Ref<NavigationMesh> p_mesh) {
  mesh = p_mesh;
  polygons_dirty = true;
}

bool NavRegion::sync() {
  bool something_changed = polygons_dirty /* || something_dirty? */;

  update_polygons();

  return something_changed;
}

void NavRegion::update_polygons() {
  if (!polygons_dirty) {
    return;
  }
  polygons.clear();
  polygons_dirty = false;

  if (map == NULL) {
    return;
  }

  if (mesh.is_null())
    return;

  Vector<Vector3> vertices = mesh->get_vertices();
  int len = vertices.size();
  if (len == 0)
    return;

  const Vector3 *vertices_r = vertices.ptr();

  polygons.resize(mesh->get_polygon_count());

  // Build
  for (size_t i(0); i < polygons.size(); i++) {

    gd::Polygon &p = polygons[i];
    p.owner = this;

    Vector<int> mesh_poly = mesh->get_polygon(i);
    const int *indices = mesh_poly.ptr();
    bool valid(true);
    p.points.resize(mesh_poly.size());
    p.edges.resize(mesh_poly.size());

    Vector3 center;
    float sum(0);

    for (int j(0); j < mesh_poly.size(); j++) {

      int idx = indices[j];
      if (idx < 0 || idx >= len) {
        valid = false;
        break;
      }

      Vector3 point_position = transform.xform(vertices_r[idx]);
      p.points[j].pos = point_position;
      p.points[j].key = map->get_point_key(point_position);

      center += point_position; // Composing the center of the polygon

      if (j >= 2) {
        Vector3 epa = transform.xform(vertices_r[indices[j - 2]]);
        Vector3 epb = transform.xform(vertices_r[indices[j - 1]]);

        sum += map->get_up().dot((epb - epa).cross(point_position - epa));
      }
    }

    if (!valid) {
      ERR_BREAK_MSG(!valid,
                    "The navigation mesh set in this region is not valid!");
    }

    p.clockwise = sum > 0;
    if (mesh_poly.size() != 0) {
      p.center = center / float(mesh_poly.size());
    }
  }
}

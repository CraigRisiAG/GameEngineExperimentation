
#ifndef NAV_REGION_H
#define NAV_REGION_H

#include "nav_rid.h"

#include "nav_utils.h"
#include "scene/3d/navigation.h"
#include <vector>

/**
        @author AndreaCatania
*/

class NavMap;
class NavRegion;

class NavRegion : public NavRid {
  NavMap *map;
  Transform transform;
  Ref<NavigationMesh> mesh;

  bool polygons_dirty;

  /// Cache
  std::vector<gd::Polygon> polygons;

public:
  NavRegion();

  void scratch_polygons() { polygons_dirty = true; }

  void set_map(NavMap *p_map);
  NavMap *get_map() const { return map; }

  void set_transform(Transform transform);
  const Transform &get_transform() const { return transform; }

  void set_mesh(Ref<NavigationMesh> p_mesh);
  const Ref<NavigationMesh> get_mesh() const { return mesh; }

  std::vector<gd::Polygon> const &get_polygons() const { return polygons; }

  bool sync();

private:
  void update_polygons();
};

#endif // NAV_REGION_H

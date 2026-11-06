// This file contains the declaration of the NavigationRegion class,
// which handles navigation regions in 3D. It manages the region's
// properties, updates the navigation mesh, interacts with the navigation
// server, and provides functionality for baking the navigation mesh and
// handling debug visualization.
#ifndef NAVIGATION_REGION_H
#define NAVIGATION_REGION_H

#include "scene/3d/spatial.h"
#include "scene/resources/mesh.h"
#include "scene/resources/navigation_mesh.h"

class Navigation;

class NavigationRegion : public Spatial {

  GDCLASS(NavigationRegion, Spatial);

  bool enabled;
  RID region;
  Ref<NavigationMesh> navmesh;

  Navigation *navigation;
  Node *debug_view;
  Thread *bake_thread;

protected:
  void _notification(int p_what);
  static void _bind_methods();
  void _changed_callback(Object *p_changed, const char *p_prop);

public:
  void set_enabled(bool p_enabled);
  bool is_enabled() const;

  void set_navigation_mesh(const Ref<NavigationMesh> &p_navmesh);
  Ref<NavigationMesh> get_navigation_mesh() const;

  /// Bakes the navigation mesh in a dedicated thread; once done, automatically
  /// sets the new navigation mesh and emits a signal
  void bake_navigation_mesh();
  void _bake_finished(Ref<NavigationMesh> p_nav_mesh);

  String get_configuration_warning() const;

  NavigationRegion();
  ~NavigationRegion();
};

#endif // NAVIGATION_REGION_H


#ifndef PROXIMITY_GROUP_H
#define PROXIMITY_GROUP_H

#include "spatial.h"

class ProximityGroup : public Spatial {

  GDCLASS(ProximityGroup, Spatial);
  OBJ_CATEGORY("3D");

public:
  enum DispatchMode {
    MODE_PROXY,
    MODE_SIGNAL,
  };

public:
  void clear_groups();
  void update_groups();

  void _notification(int p_what);

  DispatchMode dispatch_mode;

  Map<StringName, uint32_t> groups;
  String group_name;

  float cell_size;
  Vector3 grid_radius;
  uint32_t group_version;

  void add_groups(int *p_cell, String p_base, int p_depth);
  void _new_group(StringName p_name);

  void _proximity_group_broadcast(String p_name, Variant p_params);

  static void _bind_methods();

public:
  void set_group_name(const String &p_group_name);
  String get_group_name() const;

  void set_dispatch_mode(DispatchMode p_mode);
  DispatchMode get_dispatch_mode() const;

  void set_grid_radius(const Vector3 &p_radius);
  Vector3 get_grid_radius() const;

  void broadcast(String p_name, Variant p_params);

  ProximityGroup();
  ~ProximityGroup();
};

VARIANT_ENUM_CAST(ProximityGroup::DispatchMode);

#endif

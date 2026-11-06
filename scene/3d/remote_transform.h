// This file contains the declaration of the RemoteTransform class,
// which allows a Spatial node to automatically update the transform of
// another Spatial node (the remote node) in the scene. It provides
// functionality for setting the remote node, choosing whether to use global
// coordinates, and specifying which components of the transform (position,
// rotation, scale) should be updated. The class handles caching the remote node
// and updating its transform efficiently.
#ifndef REMOTETRANSFORM_H
#define REMOTETRANSFORM_H

#include "scene/3d/spatial.h"

class RemoteTransform : public Spatial {
  GDCLASS(RemoteTransform, Spatial);

  NodePath remote_node;

  ObjectID cache;

  bool use_global_coordinates;
  bool update_remote_position;
  bool update_remote_rotation;
  bool update_remote_scale;

  void _update_remote();
  void _update_cache();

protected:
  static void _bind_methods();
  void _notification(int p_what);

public:
  void set_remote_node(const NodePath &p_remote_node);
  NodePath get_remote_node() const;

  void set_use_global_coordinates(const bool p_enable);
  bool get_use_global_coordinates() const;

  void set_update_position(const bool p_update);
  bool get_update_position() const;

  void set_update_rotation(const bool p_update);
  bool get_update_rotation() const;

  void set_update_scale(const bool p_update);
  bool get_update_scale() const;

  void force_update_cache();

  virtual String get_configuration_warning() const;

  RemoteTransform();
};

#endif // REMOTETRANSFORM_H

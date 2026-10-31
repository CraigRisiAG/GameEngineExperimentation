
#ifndef SCENARIO_FX_H
#define SCENARIO_FX_H

#include "scene/3d/spatial.h"

class WorldEnvironment : public Node {

  GDCLASS(WorldEnvironment, Node);

  Ref<Environment> environment;
  Ref<CameraEffects> camera_effects;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_environment(const Ref<Environment> &p_environment);
  Ref<Environment> get_environment() const;

  void set_camera_effects(const Ref<CameraEffects> &p_camera_effects);
  Ref<CameraEffects> get_camera_effects() const;

  String get_configuration_warning() const;

  WorldEnvironment();
};

#endif

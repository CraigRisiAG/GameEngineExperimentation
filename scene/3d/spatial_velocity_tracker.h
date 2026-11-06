// This file contains the declaration of the SpatialVelocityTracker class,
// which tracks the linear velocity of a Spatial node in the scene.
// It provides functionality for updating the position of the node,
// calculating the tracked linear velocity based on the position history,
// and resetting the tracker to a new position. The class can track
// velocity either based on physics steps or idle frame ticks.
#ifndef SPATIAL_VELOCITY_TRACKER_H
#define SPATIAL_VELOCITY_TRACKER_H

#include "scene/3d/spatial.h"

class SpatialVelocityTracker : public Reference {
  GDCLASS(SpatialVelocityTracker, Reference);

  struct PositionHistory {
    uint64_t frame;
    Vector3 position;
  };

  bool physics_step;
  Vector<PositionHistory> position_history;
  int position_history_len;

protected:
  static void _bind_methods();

public:
  void reset(const Vector3 &p_new_pos);
  void set_track_physics_step(bool p_track_physics_step);
  bool is_tracking_physics_step() const;
  void update_position(const Vector3 &p_position);
  Vector3 get_tracked_linear_velocity() const;

  SpatialVelocityTracker();
};

#endif // SPATIAL_VELOCITY_TRACKER_H

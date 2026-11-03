
#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "core/object.h"

#define PERF_WARN_OFFLINE_FUNCTION
#define PERF_WARN_PROCESS_SYNC

class Performance : public Object {

  GDCLASS(Performance, Object);

  static Performance *singleton;
  static void _bind_methods();

  float _get_node_count() const;
  void _update_node_count_cache() const;

  float _process_time;
  float _physics_process_time;

  // Node count caching for performance optimization
  mutable uint32_t cached_node_count;
  mutable uint64_t cache_frame_id;

public:
  enum Monitor {

    TIME_FPS,
    TIME_PROCESS,
    TIME_PHYSICS_PROCESS,
    MEMORY_STATIC,
    MEMORY_STATIC_MAX,
    MEMORY_MESSAGE_BUFFER_MAX,
    OBJECT_COUNT,
    OBJECT_RESOURCE_COUNT,
    OBJECT_NODE_COUNT,
    OBJECT_ORPHAN_NODE_COUNT,
    RENDER_OBJECTS_IN_FRAME,
    RENDER_VERTICES_IN_FRAME,
    RENDER_MATERIAL_CHANGES_IN_FRAME,
    RENDER_SHADER_CHANGES_IN_FRAME,
    RENDER_SURFACE_CHANGES_IN_FRAME,
    RENDER_DRAW_CALLS_IN_FRAME,
    RENDER_VIDEO_MEM_USED,
    RENDER_TEXTURE_MEM_USED,
    RENDER_VERTEX_MEM_USED,
    RENDER_USAGE_VIDEO_MEM_TOTAL,
    PHYSICS_2D_ACTIVE_OBJECTS,
    PHYSICS_2D_COLLISION_PAIRS,
    PHYSICS_2D_ISLAND_COUNT,
    PHYSICS_3D_ACTIVE_OBJECTS,
    PHYSICS_3D_COLLISION_PAIRS,
    PHYSICS_3D_ISLAND_COUNT,
    // physics
    AUDIO_OUTPUT_LATENCY,
    MONITOR_MAX
  };

  enum MonitorType {
    MONITOR_TYPE_QUANTITY,
    MONITOR_TYPE_MEMORY,
    MONITOR_TYPE_TIME
  };

  float get_monitor(Monitor p_monitor) const;
  String get_monitor_name(Monitor p_monitor) const;

  MonitorType get_monitor_type(Monitor p_monitor) const;

  void set_process_time(float p_pt);
  void set_physics_process_time(float p_pt);

  static Performance *get_singleton() { return singleton; }

  Performance();
};

VARIANT_ENUM_CAST(Performance::Monitor);

#endif // PERFORMANCE_H

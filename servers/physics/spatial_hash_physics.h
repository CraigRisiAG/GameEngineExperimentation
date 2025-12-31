/**
 * @file spatial_hash_physics.h
 * @brief Integration of spatial hash into PhysicsServer3D
 *
 * This file provides the physics server integration for spatial hash
 * optimization, enabling O(k) queries instead of O(n²).
 */

#ifndef SPATIAL_HASH_PHYSICS_H
#define SPATIAL_HASH_PHYSICS_H

#include "core/math/aabb.h"
#include "core/object_id.h"
#include "core/rid.h"
#include "core/vector.h"
#include "spatial_hash.h"

/**
 * @class PhysicsSpaceOptimizer
 * @brief Manages spatial hashing for a physics space
 *
 * Each physics space (3D or 2D) gets its own spatial hash grid.
 * Objects are automatically tracked and queries are optimized.
 */
class PhysicsSpaceOptimizer {
public:
  struct Body {
    RID rid;
    AABB aabb;
    bool is_area;
    ObjectID object_id;
  };

private:
  SpatialHashGrid spatial_grid;
  HashMap<RID, Body> tracked_bodies;
  bool enabled = true;

public:
  explicit PhysicsSpaceOptimizer(float p_cell_size = 32.0f)
      : spatial_grid(p_cell_size) {}

  /**
   * Enable/disable spatial hashing
   * Useful for toggling optimization on/off for benchmarking
   */
  void set_enabled(bool p_enabled) {
    enabled = p_enabled;
    if (!enabled) {
      spatial_grid.clear();
    }
  }

  bool is_enabled() const { return enabled; }

  /**
   * Add a body to spatial tracking
   */
  void track_body(const RID &p_rid, const AABB &p_aabb, ObjectID p_object_id,
                  bool p_is_area = false) {
    if (!enabled)
      return;

    Body body;
    body.rid = p_rid;
    body.aabb = p_aabb;
    body.is_area = p_is_area;
    body.object_id = p_object_id;

    tracked_bodies[p_rid] = body;
    spatial_grid.insert(p_rid, p_aabb, p_is_area);
  }

  /**
   * Remove a body from spatial tracking
   */
  void untrack_body(const RID &p_rid) {
    if (!enabled)
      return;

    if (tracked_bodies.has(p_rid)) {
      const Body &body = tracked_bodies[p_rid];
      spatial_grid.remove(p_rid, body.aabb, body.is_area);
      tracked_bodies.erase(p_rid);
    }
  }

  /**
   * Update body position in spatial hash
   * Call this during physics space updates
   */
  void update_body(const RID &p_rid, const AABB &p_new_aabb) {
    if (!enabled)
      return;

    if (tracked_bodies.has(p_rid)) {
      Body &body = tracked_bodies[p_rid];
      spatial_grid.update(p_rid, body.aabb, p_new_aabb, body.is_area);
      body.aabb = p_new_aabb;
    }
  }

  /**
   * Query bodies in a region
   *
   * Returns all tracked bodies whose AABB intersects query region
   */
  int query_region(const AABB &p_region, Vector<RID> &p_bodies,
                   Vector<RID> &p_areas = nullptr) const {
    if (!enabled)
      return 0;

    Set<RID> exclude;
    return spatial_grid.query_aabb(p_region, exclude, &p_bodies,
                                   p_areas.size() > 0 ? &p_areas : nullptr);
  }

  /**
   * Query bodies intersecting shape
   */
  int query_shape(const AABB &p_shape_aabb, const Vector<RID> &p_exclude,
                  Vector<RID> &p_results) const {
    if (!enabled)
      return 0;

    Set<RID> exclude_set;
    for (const RID &rid : p_exclude) {
      exclude_set.insert(rid);
    }

    Vector<RID> temp_results;
    spatial_grid.query_aabb(p_shape_aabb, exclude_set, &temp_results);

    // Only return those actually matching (AABB is conservative)
    for (const RID &rid : temp_results) {
      p_results.push_back(rid);
    }

    return p_results.size();
  }

  /**
   * Get optimization statistics
   */
  struct OptimizationStats {
    bool enabled = false;
    int32_t tracked_bodies = 0;
    int32_t grid_cells = 0;
    float avg_bodies_per_cell = 0.0f;
    float grid_efficiency = 0.0f; // % of cells with bodies
  };

  OptimizationStats get_stats() const {
    OptimizationStats stats;
    stats.enabled = enabled;
    stats.tracked_bodies = tracked_bodies.size();

    SpatialHashGrid::Stats grid_stats = spatial_grid.get_stats();
    stats.grid_cells = grid_stats.occupied_cells;
    stats.avg_bodies_per_cell =
        grid_stats.avg_bodies_per_cell + grid_stats.avg_areas_per_cell;

    if (grid_stats.occupied_cells > 0) {
      stats.grid_efficiency = 100.0f * grid_stats.occupied_cells /
                              max(1, (int)grid_stats.total_cells);
    }

    return stats;
  }

  /**
   * Clear all tracked objects (called on physics space reset)
   */
  void clear() {
    tracked_bodies.clear();
    spatial_grid.clear();
  }
};

#endif // SPATIAL_HASH_PHYSICS_H

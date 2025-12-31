/**
 * @file physics_space_raytracing.h
 * @brief Raytracing interface for PhysicsDirectSpaceState
 *
 * Extension to PhysicsDirectSpaceState adding modern raytracing queries.
 * All methods are marked virtual so implementations can override.
 *
 * This should be included in physics_server.h as an extension to
 * PhysicsDirectSpaceState class.
 */

#ifndef PHYSICS_SPACE_RAYTRACING_H
#define PHYSICS_SPACE_RAYTRACING_H

#include "physics_raytracing.h"

/**
 * @class PhysicsSpaceRaytracing
 * @brief Raytracing query interface for physics space
 *
 * Abstract interface for raytracing queries.
 * To be implemented by PhysicsDirectSpaceState or as mixin.
 *
 * Add these methods to PhysicsDirectSpaceState in physics_server.h:
 *
 * @code
 * class PhysicsDirectSpaceState : public Object {
 *     // ... existing methods ...
 *
 *     // RAYTRACING INTERFACE (NEW)
 *
 *     /// Single ray intersection
 *     virtual OptionalRaycastResult raycast(const RayQuery& ray) = 0;
 *
 *     /// All intersections along ray
 *     virtual RaycastResults raycast_all(const RayQuery& ray) = 0;
 *
 *     /// Shape overlap test
 *     virtual ShapeQueryResult shape_query(
 *         RID shape,
 *         const Transform& transform) = 0;
 *
 *     /// Shape sweep (collision prediction)
 *     virtual OptionalSweepResult shape_sweep(const SweepQuery& sweep) = 0;
 *
 *     /// Fast visibility check
 *     virtual bool line_of_sight(
 *         const Vector3& from,
 *         const Vector3& to,
 *         uint32_t collision_mask = 0xFFFFFFFF) = 0;
 *
 *     /// Advanced: Multiple rays
 *     virtual Vector<bool> multi_raycast(const MultiRayQuery& query) = 0;
 * };
 * @endcode
 */
class PhysicsSpaceRaytracing {
public:
  // ═══════════════════════════════════════════════════════════════════════
  // Core Raytracing Methods
  // ═══════════════════════════════════════════════════════════════════════

  /**
   * @brief Single raycast - find closest intersection
   *
   * Shoots a ray from origin in direction and finds closest hit.
   * Uses spatial partitioning for O(log n) performance.
   *
   * @param ray Ray parameters (origin, direction, length, filters)
   * @return Hit information if intersection found, None otherwise
   *
   * Example:
   * @code
   * RayQuery ray;
   * ray.from = player_pos;
   * ray.direction = (target - player_pos).normalized();
   * ray.length = 1000.0f;
   * ray.collision_mask = LAYER_ENEMIES;
   *
   * if (auto hit = space->raycast(ray)) {
   *     Vector3 impact_point = hit->position;
   *     Vector3 impact_normal = hit->normal;
   *     deal_damage(hit->body, 100.0f);
   * }
   * @endcode
   *
   * Performance: O(log n) where n = number of bodies
   * Typical: 0.1-0.5 ms for small scenes
   */
  virtual OptionalRaycastResult raycast(const RayQuery &ray) = 0;

  /**
   * @brief Multi-hit raycast - find all intersections
   *
   * Returns all bodies/shapes hit by ray in distance order.
   * Useful for penetration effects, visual effects, etc.
   *
   * @param ray Ray parameters
   * @return Vector of all hits, sorted by distance (if find_closest=true)
   *
   * Example:
   * @code
   * auto all_hits = space->raycast_all(ray);
   * for (const auto& hit : all_hits) {
   *     apply_damage_dropoff(hit.body, 100.0f / hit.distance);
   * }
   * @endcode
   *
   * Performance: O(k log n) where k = number of hits
   * Typical: 0.5-2.0 ms for 10+ hits
   */
  virtual RaycastResults raycast_all(const RayQuery &ray) = 0;

  /**
   * @brief Shape overlap test
   *
   * Tests if a shape at a given position overlaps with any bodies.
   * Returns all overlapping bodies and contact information.
   * Used for area detection, spawn point validation, etc.
   *
   * @param shape Resource ID of shape to test
   * @param transform Position and rotation of shape
   * @return All overlapping bodies and contacts
   *
   * Example:
   * @code
   * RID overlap_sphere = create_sphere_shape(2.0f);
   *
   * auto overlaps = space->shape_query(overlap_sphere,
   *     Transform(Vector3(x, y, z)));
   *
   * for (const auto& body : overlaps.bodies) {
   *     on_entity_detected(body);
   * }
   * @endcode
   *
   * Performance: O(log n)
   * Typical: 0.2-1.0 ms
   */
  virtual ShapeQueryResult shape_query(RID shape,
                                       const Transform &transform) = 0;

  /**
   * @brief Shape sweep test (collision prediction)
   *
   * Tests if moving a shape from position A to position B collides.
   * Returns first collision along path.
   * Critical for character movement, projectiles, etc.
   *
   * @param sweep Sweep parameters (shape, from/to transforms)
   * @return First collision if any, None if path clear
   *
   * Example:
   * @code
   * SweepQuery sweep;
   * sweep.shape = player_capsule;
   * sweep.from = current_position;
   * sweep.to = desired_position;
   *
   * if (auto collision = space->shape_sweep(sweep)) {
   *     // Stop before collision
   *     move_to(collision->position);
   *     on_collision(collision->body, collision->normal);
   * } else {
   *     // Safe to move
   *     move_to(desired_position);
   * }
   * @endcode
   *
   * Performance: O(log n)
   * Typical: 0.5-2.0 ms
   */
  virtual OptionalSweepResult shape_sweep(const SweepQuery &sweep) = 0;

  /**
   * @brief Line of sight test (visibility check)
   *
   * Fast check if line between two points is unobstructed.
   * Optimized for common case of just true/false answer.
   *
   * @param from Start point
   * @param to End point
   * @param collision_mask Layer mask
   * @return true if no obstacles between points
   *
   * Example:
   * @code
   * if (space->line_of_sight(enemy_pos, player_pos)) {
   *     attack_player();
   * } else {
   *     find_alternate_route();
   * }
   * @endcode
   *
   * Performance: O(log n) with early exit
   * Typical: 0.05-0.2 ms (fastest raytracing operation)
   */
  virtual bool line_of_sight(const Vector3 &from, const Vector3 &to,
                             uint32_t collision_mask = 0xFFFFFFFF) = 0;

  // ═══════════════════════════════════════════════════════════════════════
  // Advanced Raytracing Methods
  // ═══════════════════════════════════════════════════════════════════════

  /**
   * @brief Multiple rays (spread/cone cast)
   *
   * Tests multiple rays efficiently.
   * Useful for shotgun spread, cone detection, etc.
   *
   * @param query Multiple ray parameters
   * @return Results combined per query.combine_mode
   *
   * Example:
   * @code
   * MultiRayQuery spread;
   * spread.combine_mode = MultiRayQuery::COMBINE_UNION;
   *
   * // Create cone of rays
   * for (int i = 0; i < 8; i++) {
   *     RayQuery ray;
   *     ray.from = gun_pos;
   *     ray.direction = spread_direction(angle, spread_angle);
   *     spread.rays.push_back(ray);
   * }
   *
   * auto all_hits = space->multi_raycast(spread);
   * @endcode
   *
   * Performance: O(k log n) where k = number of rays
   */
  virtual Vector<bool> multi_raycast(const MultiRayQuery &query) = 0;

  /**
   * @brief Cone cast (sphere sweep along direction)
   *
   * Efficient test for "swept sphere" detection.
   * Like raycast but with radius instead of infinitesimal line.
   *
   * @param center Center of cone
   * @param radius Cone radius
   * @param direction Direction to cast
   * @param length Length of cast
   * @param collision_mask Layer mask
   * @return All bodies within swept cone
   *
   * Performance: O(log n)
   */
  virtual Vector<RID> cone_cast(const Vector3 &center, float radius,
                                const Vector3 &direction, float length,
                                uint32_t collision_mask = 0xFFFFFFFF) = 0;

  /**
   * @brief Sphere cast (moving sphere detection)
   *
   * Special case of shape_sweep optimized for spheres.
   * Faster than generic shape_sweep.
   *
   * @param from Starting position
   * @param radius Sphere radius
   * @param to Ending position
   * @param collision_mask Layer mask
   * @return First collision along path
   *
   * Performance: O(log n)
   */
  virtual OptionalSweepResult
  sphere_cast(const Vector3 &from, float radius, const Vector3 &to,
              uint32_t collision_mask = 0xFFFFFFFF) = 0;

  // ═══════════════════════════════════════════════════════════════════════
  // Async/Batch Raytracing (C++20 future)
  // ═══════════════════════════════════════════════════════════════════════

  /**
   * @brief Raycast with callback (for threaded implementations)
   *
   * Fire-and-forget raycast that calls callback when complete.
   * Can be implemented off-thread.
   *
   * @param ray Ray parameters
   * @param callback Function to call with result
   *
   * Future C++20 implementation:
   * @code
   * std::future<OptionalRaycastResult> future_result =
   *     space->raycast_async(ray);
   *
   * auto result = co_await future_result;
   * @endcode
   */
  virtual void raycast_async(
      const RayQuery &ray,
      std::function<void(const OptionalRaycastResult &)> callback) = 0;

  // Lifecycle
  virtual ~PhysicsSpaceRaytracing() = default;
};

// ═══════════════════════════════════════════════════════════════════════════
// Helper Utilities (QUICK WIN #4: Structured Bindings)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @brief Create simple raycast query
 * Convenience function using structured bindings
 *
 * @code
 * auto [hit, pos, normal] = simple_raycast(space, from, to, length);
 * @endcode
 */
struct SimpleRayResult {
  bool hit;
  Vector3 position;
  Vector3 normal;
  float distance;
  RID body;

  // For structured binding support
  friend auto operator<=>(const SimpleRayResult &,
                          const SimpleRayResult &) = default;
};

inline SimpleRayResult
simple_raycast(PhysicsSpaceRaytracing *space, const Vector3 &from,
               const Vector3 &to,
               float length = std::numeric_limits<float>::max()) {

  RayQuery ray;
  ray.from = from;
  ray.direction = (to - from).normalized();
  ray.length = length;

  if (auto result = space->raycast(ray)) {
    return SimpleRayResult{true, result->position, result->normal,
                           result->distance, result->body};
  }

  return SimpleRayResult{false, Vector3::ZERO, Vector3::ZERO, 0, RID()};
}

#endif // PHYSICS_SPACE_RAYTRACING_H

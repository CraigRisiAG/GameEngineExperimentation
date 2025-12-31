/**
 * @file physics_raytracing.h
 * @brief Ray query types and interfaces for physics engine raytracing
 *
 * This header defines the core data structures for raytracing queries:
 * - RayQuery: Parameters for raycasting
 * - RaycastResult: Output of a raycast
 * - ShapeQuery: Parameters for shape overlap tests
 * - SweepQuery: Parameters for shape sweep (collision prediction)
 * - Specialized result types for each query type
 *
 * All types use modern C++17 patterns:
 * - std::optional for nullable returns
 * - Smart pointers for memory safety
 * - Clear, documented APIs
 *
 * Usage:
 * @code
 * // Simple raycast
 * RayQuery ray;
 * ray.from = Vector3(0, 1, 0);
 * ray.direction = Vector3(0, 0, 1);
 * ray.length = 100.0f;
 *
 * if (auto hit = space->raycast(ray)) {
 *     process_hit(hit->position, hit->normal);
 * }
 * @endcode
 */

#ifndef PHYSICS_RAYTRACING_H
#define PHYSICS_RAYTRACING_H

#include "core/math/transform.h"
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "core/modern_cpp.h"
#include "core/rid.h"
#include "core/vector.h"

// ═══════════════════════════════════════════════════════════════════════════
// QUICK WIN #5: Type-Safe Result Variants
// ═══════════════════════════════════════════════════════════════════════════

/**
 * @struct RaycastResult
 * @brief Output of a raycast query (single intersection)
 *
 * Contains complete information about a ray-geometry intersection:
 * - Exact hit position and surface normal
 * - Distance along the ray
 * - Identification of hit body/shape
 * - Optional UV coordinates and face index
 */
struct RaycastResult {
  /// True if ray hit something
  bool hit = false;

  /// Position of intersection point in world space
  Vector3 position = Vector3::ZERO;

  /// Surface normal at intersection point
  Vector3 normal = Vector3::ZERO;

  /// Resource ID of hit shape
  RID shape = RID();

  /// Resource ID of hit body
  RID body = RID();

  /// Distance from ray origin to hit point
  float distance = 0.0f;

  /// UV coordinates at hit point (if available from mesh)
  Vector2 uv = Vector2::ZERO;

  /// Face/triangle index (for mesh shapes)
  int face_index = -1;

  /// Material ID (if queried)
  RID material = RID();

  /// Linear interpolation parameter for hit (barycentric)
  Vector3 barycentric = Vector3::ZERO;
};

/**
 * @struct RayQuery
 * @brief Parameters for raycast query
 *
 * Defines a ray and filtering parameters.
 * All distances and directions should be in world space.
 */
struct RayQuery {
  /// Ray origin in world space
  Vector3 from = Vector3::ZERO;

  /// Ray direction (should be normalized)
  Vector3 direction = Vector3(0, 0, 1);

  /// Maximum ray length
  float length = 1000.0f;

  /// Collision layer mask (which layers to test)
  uint32_t collision_mask = 0xFFFFFFFF;

  /// Bodies to exclude from testing
  Vector<RID> exclude_bodies = {};

  /// Shapes to exclude from testing
  Vector<RID> exclude_shapes = {};

  /// Whether to test against sensor shapes
  bool test_sensors = false;

  /// Whether to test against rigid bodies
  bool test_rigid = true;

  /// Whether to test against static shapes
  bool test_static = true;

  /// Find closest intersection (vs first intersection)
  bool find_closest = true;

  /// Whether to sort results by distance
  bool sort_by_distance = true;

  /// Debug: visualize this ray
  bool debug_draw = false;
};

/**
 * @struct ShapeQueryResult
 * @brief Output of shape overlap test
 *
 * Contains all bodies and shapes that overlap with query shape.
 * May include contact information if collision data available.
 */
struct ShapeQueryResult {
  /// IDs of all overlapping bodies
  Vector<RID> bodies = {};

  /// IDs of all overlapping shapes
  Vector<RID> shapes = {};

  /// Contact points (point on shape A)
  Vector<Vector3> contact_points_a = {};

  /// Contact points (point on shape B)
  Vector<Vector3> contact_points_b = {};

  /// Contact normals (from A to B)
  Vector<Vector3> contact_normals = {};

  /// Penetration depth at each contact
  Vector<float> penetration_depths = {};

  /// Whether any contacts exist
  bool has_contacts = false;
};

/**
 * @struct SweepQuery
 * @brief Parameters for shape sweep test (collision prediction)
 *
 * Tests if moving a shape from position A to position B will collide.
 * Useful for character movement prediction.
 */
struct SweepQuery {
  /// Shape to sweep through space
  RID shape = RID();

  /// Starting position and rotation
  Transform from = Transform();

  /// Ending position and rotation
  Transform to = Transform();

  /// Collision layer mask
  uint32_t collision_mask = 0xFFFFFFFF;

  /// Bodies to exclude
  Vector<RID> exclude_bodies = {};

  /// Margin for swept shape (collision buffer)
  float margin = 0.04f;

  /// Whether to test against sensors
  bool test_sensors = false;

  /// Maximum number of hits to return
  int max_hits = 1;

  /// Debug visualization
  bool debug_draw = false;
};

/**
 * @struct SweepResult
 * @brief Output of shape sweep test
 *
 * Contains information about where shape collides during sweep.
 */
struct SweepResult {
  /// True if sweep hit something
  bool hit = false;

  /// Position where sweep stopped (collision point)
  Vector3 position = Vector3::ZERO;

  /// Surface normal at collision
  Vector3 normal = Vector3::ZERO;

  /// Distance swept before collision (0-1 of total motion)
  float distance = 0.0f;

  /// ID of body hit
  RID body = RID();

  /// ID of shape hit
  RID shape = RID();

  /// Contact point on this shape
  Vector3 contact_point = Vector3::ZERO;

  /// Contact point on other shape
  Vector3 contact_point_other = Vector3::ZERO;
};

/**
 * @struct LineOfSightQuery
 * @brief Parameters for visibility test
 *
 * Fast check if line between two points is unobstructed.
 */
struct LineOfSightQuery {
  /// Start point
  Vector3 from = Vector3::ZERO;

  /// End point
  Vector3 to = Vector3::ZERO;

  /// Collision layer mask
  uint32_t collision_mask = 0xFFFFFFFF;

  /// Whether to test against sensors (usually false)
  bool test_sensors = false;

  /// Bodies to ignore
  Vector<RID> exclude_bodies = {};

  /// Return hit information (vs just true/false)
  bool return_hit_info = false;
};

/**
 * @struct CastResult
 * @brief Unified result type (QUICK WIN #5: std::variant)
 *
 * Can hold result from any cast operation using type-safe variant.
 */
using CastResult = std::variant<RaycastResult, SweepResult, ShapeQueryResult>;

/**
 * @brief Parameters for advanced raycast with multiple rays
 * Useful for bullet spread, cone casts, etc.
 */
struct MultiRayQuery {
  /// List of rays to cast
  Vector<RayQuery> rays = {};

  /// How to combine results
  enum CombineMode {
    COMBINE_ANY,   // True if any ray hits
    COMBINE_ALL,   // True if all rays hit
    COMBINE_COUNT, // Return number of hits
    COMBINE_UNION, // Return all hits
  } combine_mode = COMBINE_ANY;

  /// Maximum combined distance
  float max_combined_distance = std::numeric_limits<float>::max();
};

// ═══════════════════════════════════════════════════════════════════════════
// QUICK WIN #2: Optional Type Safety
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Convenience types for optional raytracing results
 */
using OptionalRaycastResult = Optional<RaycastResult>;
using OptionalSweepResult = Optional<SweepResult>;
using RaycastResults = Vector<RaycastResult>;
using SweepResults = Vector<SweepResult>;

#endif // PHYSICS_RAYTRACING_H

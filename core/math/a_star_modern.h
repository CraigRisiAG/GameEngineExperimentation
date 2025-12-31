/**
 * @file a_star_modern.h
 * @brief Modern C++17 A* pathfinding implementation
 *
 * A* pathfinding algorithm using modern C++ patterns:
 * - Smart pointers for automatic memory management
 * - Optional types for safe nullable returns
 * - Range-based iteration
 * - Standard containers (std::unordered_map)
 * - Modern error handling
 *
 * Features:
 * - 3D (AStarModern) and 2D (AStar2DModern) pathfinding
 * - Point weight scaling for terrain cost
 * - Bidirectional and unidirectional connections
 * - Customizable heuristic functions
 * - Efficient neighbor lookups
 *
 * Usage Example:
 * @code
 * AStarModern pathfinder;
 * pathfinder.add_point(1, Vector3(0, 0, 0));
 * pathfinder.add_point(2, Vector3(1, 0, 0));
 * pathfinder.connect_points(1, 2);
 *
 * if (auto path = pathfinder.get_point_path(1, 2)) {
 *     for (const auto& point : *path) {
 *         // Process point
 *     }
 * }
 * @endcode
 */

#ifndef A_STAR_MODERN_H
#define A_STAR_MODERN_H

#include "core/modern_cpp.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <vector>

// Forward declarations
class AStarModern;
class AStar2DModern;

/**
 * @class AStarModern
 * @brief Modern C++ implementation of A* pathfinding for 3D space
 *
 * Safe, efficient pathfinding using smart pointers and Optional types.
 * All memory is automatically managed - no manual allocation/deallocation.
 */
class AStarModern {
public:
  // Type aliases for clarity
  using PointId = int;
  using Path3D = Vector<Vector3>;
  using PathIds = Vector<PointId>;

  /**
   * @brief Get the next available point ID
   * @return Unused point ID
   */
  PointId get_available_point_id() const;

  /**
   * @brief Add a point to the pathfinding graph
   * @param id Point identifier (must be >= 0)
   * @param pos Position in 3D space
   * @param weight_scale Cost multiplier for terrain (>= 1.0)
   * @throws std::invalid_argument if parameters invalid
   */
  void add_point(PointId id, const Vector3 &pos, float weight_scale = 1.0f);

  /**
   * @brief Get point position (safe, returns Optional)
   * @param id Point identifier
   * @return Position if point exists, None otherwise
   */
  Optional<Vector3> get_point_position(PointId id) const;

  /**
   * @brief Set point position
   * @param id Point identifier
   * @param pos New position
   * @return true if updated, false if point not found
   */
  bool set_point_position(PointId id, const Vector3 &pos);

  /**
   * @brief Get point weight scale (safe)
   * @param id Point identifier
   * @return Weight if point exists, None otherwise
   */
  Optional<float> get_point_weight_scale(PointId id) const;

  /**
   * @brief Set point weight scale
   * @param id Point identifier
   * @param weight_scale New weight (>= 1.0)
   * @throws std::invalid_argument if weight < 1.0
   */
  void set_point_weight_scale(PointId id, float weight_scale);

  /**
   * @brief Remove a point and all its connections
   * @param id Point identifier
   * @return true if removed, false if not found
   */
  bool remove_point(PointId id);

  /**
   * @brief Connect two points (create edge)
   * @param from_id Source point
   * @param to_id Destination point
   * @param bidirectional If true, creates connection in both directions
   * @throws std::invalid_argument if points don't exist
   */
  void connect_points(PointId from_id, PointId to_id,
                      bool bidirectional = true);

  /**
   * @brief Disconnect two points
   * @param from_id Source point
   * @param to_id Destination point
   * @param bidirectional If true, removes connection in both directions
   */
  void disconnect_points(PointId from_id, PointId to_id,
                         bool bidirectional = true);

  /**
   * @brief Check if two points are connected
   * @param from_id Source point
   * @param to_id Destination point
   * @return true if connected
   */
  bool are_points_connected(PointId from_id, PointId to_id) const;

  /**
   * @brief Find path between two points (returns positions)
   * @param from_id Start point
   * @param to_id End point
   * @return Path as Vector3 coordinates if found, None if unreachable
   */
  Optional<Path3D> get_point_path(PointId from_id, PointId to_id);

  /**
   * @brief Find path between two points (returns IDs)
   * @param from_id Start point
   * @param to_id End point
   * @return Path as point IDs if found, None if unreachable
   */
  Optional<PathIds> get_id_path(PointId from_id, PointId to_id);

  /**
   * @brief Get all point IDs in the graph
   * @return Vector of all point IDs
   */
  PathIds get_point_ids() const;

  /**
   * @brief Find closest point to a position
   * @param pos Query position
   * @param include_disabled Whether to include disabled points
   * @return Closest point ID if any exist, None otherwise
   */
  Optional<PointId> get_closest_point(const Vector3 &pos,
                                      bool include_disabled = false) const;

  /**
   * @brief Get closest position on any segment (edge)
   * @param pos Query position
   * @return Closest position on any edge
   */
  Vector3 get_closest_position_in_segment(const Vector3 &pos) const;

  /**
   * @brief Enable/disable a point for pathfinding
   * @param id Point identifier
   * @param disabled If true, point is excluded from pathfinding
   * @return true if point exists
   */
  bool set_point_disabled(PointId id, bool disabled = true);

  /**
   * @brief Check if a point is disabled
   * @param id Point identifier
   * @return true if disabled
   */
  bool is_point_disabled(PointId id) const;

  /**
   * @brief Get all neighbors of a point
   * @param id Point identifier
   * @return Vector of connected point IDs
   */
  PathIds get_point_connections(PointId id) const;

  /**
   * @brief Get number of points in graph
   * @return Point count
   */
  int get_point_count() const;

  /**
   * @brief Reserve space for points (optimization)
   * @param num_nodes Number of points to reserve
   * @throws std::invalid_argument if num_nodes <= 0
   */
  void reserve_space(int num_nodes);

  /**
   * @brief Remove all points and connections
   */
  void clear();

  /**
   * @brief Override to customize distance estimate (heuristic)
   * @param from_id Start point
   * @param to_id End point
   * @return Estimated cost (default: Euclidean distance)
   */
  virtual float estimate_cost(PointId from_id, PointId to_id) const;

  /**
   * @brief Override to customize actual cost calculation
   * @param from_id Start point
   * @param to_id End point
   * @return Actual cost between points
   */
  virtual float compute_cost(PointId from_id, PointId to_id) const;

  // Lifecycle
  AStarModern() = default;
  virtual ~AStarModern() = default;

  // Non-copyable due to internal state
  AStarModern(const AStarModern &) = delete;
  AStarModern &operator=(const AStarModern &) = delete;

  // Movable
  AStarModern(AStarModern &&) = default;
  AStarModern &operator=(AStarModern &&) = default;

protected:
  // Internal Point structure with modern C++
  struct Point {
    PointId id = -1;
    Vector3 pos = Vector3::ZERO;
    float weight_scale = 1.0f;
    bool enabled = true;

    // Pathfinding state (reset per search)
    SharedPtr<Point> prev_point;
    float g_score = 0.0f;     // Cost from start
    float f_score = 0.0f;     // Total estimated cost
    uint64_t open_pass = 0;   // Open list marker
    uint64_t closed_pass = 0; // Closed list marker

    // Neighbors stored with smart pointers
    std::unordered_map<PointId, SharedPtr<Point>> neighbors;
    std::unordered_map<PointId, SharedPtr<Point>> unlinked_neighbors;
  };

  // Segment definition for edge tracking
  struct Segment {
    enum Direction : uint8_t {
      NONE = 0,
      FORWARD = 1,
      BACKWARD = 2,
      BIDIRECTIONAL = FORWARD | BACKWARD
    };

    PointId from_id = -1;
    PointId to_id = -1;
    Direction direction = NONE;

    // Ordering for std::set
    bool operator<(const Segment &other) const {
      if (from_id != other.from_id)
        return from_id < other.from_id;
      return to_id < other.to_id;
    }
  };

  // Storage with smart pointers
  using PointPtr = SharedPtr<Point>;
  using PointMap = std::unordered_map<PointId, PointPtr>;

  PointMap points;
  std::set<Segment> segments;
  PointId last_free_id = 0;
  uint64_t search_pass = 0;

  // Internal helpers
  Optional<PointPtr> find_point(PointId id) const;
  Optional<Path3D> solve(PointId from_id, PointId to_id);

private:
  // A* implementation (private)
  Optional<Path3D> a_star_solve(PointId from_id, PointId to_id);
};

/**
 * @class AStar2DModern
 * @brief Modern C++ A* for 2D space
 *
 * Wrapper around AStarModern that converts 2D operations to 3D (with z=0).
 * Provides convenient 2D API while reusing robust 3D implementation.
 */
class AStar2DModern {
public:
  using PointId = int;
  using Path2D = Vector<Vector2>;
  using PathIds = Vector<PointId>;

  /**
   * @brief Add a point to the 2D pathfinding graph
   * @param id Point identifier
   * @param pos 2D position
   * @param weight_scale Cost multiplier (>= 1.0)
   */
  void add_point(PointId id, const Vector2 &pos, float weight_scale = 1.0f);

  /**
   * @brief Get point position
   * @param id Point identifier
   * @return 2D position if found
   */
  Optional<Vector2> get_point_position(PointId id) const;

  /**
   * @brief Set point position
   * @param id Point identifier
   * @param pos New 2D position
   * @return true if updated
   */
  bool set_point_position(PointId id, const Vector2 &pos);

  /**
   * @brief Connect two points
   * @param from_id Source point
   * @param to_id Destination point
   * @param bidirectional If true, bidirectional edge
   */
  void connect_points(PointId from_id, PointId to_id,
                      bool bidirectional = true);

  /**
   * @brief Disconnect two points
   * @param from_id Source point
   * @param to_id Destination point
   * @param bidirectional If true, bidirectional disconnect
   */
  void disconnect_points(PointId from_id, PointId to_id,
                         bool bidirectional = true);

  /**
   * @brief Find path in 2D space
   * @param from_id Start point
   * @param to_id End point
   * @return Path as Vector2 coordinates if found
   */
  Optional<Path2D> get_point_path(PointId from_id, PointId to_id);

  /**
   * @brief Find path as point IDs
   * @param from_id Start point
   * @param to_id End point
   * @return Path as point IDs if found
   */
  Optional<PathIds> get_id_path(PointId from_id, PointId to_id);

  /**
   * @brief Get all point IDs
   * @return Vector of point IDs
   */
  PathIds get_point_ids() const;

  /**
   * @brief Get point count
   * @return Number of points
   */
  int get_point_count() const;

  /**
   * @brief Clear all points
   */
  void clear();

  /**
   * @brief Override distance estimate
   * @param from_id Start point
   * @param to_id End point
   * @return Estimated cost
   */
  virtual float estimate_cost(PointId from_id, PointId to_id) const;

  /**
   * @brief Override actual cost calculation
   * @param from_id Start point
   * @param to_id End point
   * @return Actual cost
   */
  virtual float compute_cost(PointId from_id, PointId to_id) const;

  AStar2DModern() = default;
  virtual ~AStar2DModern() = default;

private:
  AStarModern impl;

  // Conversion helpers
  static Vector3 to_3d(const Vector2 &v);
  static Vector2 to_2d(const Vector3 &v);
};

#endif // A_STAR_MODERN_H

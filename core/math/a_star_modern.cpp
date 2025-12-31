/**
 * @file a_star_modern.cpp
 * @brief Modern C++17 A* pathfinding implementation
 *
 * This file contains a complete modernized implementation of A* pathfinding
 * using smart pointers, Optional types, and modern C++ patterns.
 *
 * Key improvements over legacy implementation:
 * - Smart pointers eliminate manual memory management
 * - Optional types provide safe nullable returns
 * - Range-based for loops replace manual iteration
 * - Standard containers (unordered_map) for better performance
 * - Exceptions for error handling instead of macros
 * - Cleaner, more maintainable code structure
 */

#include "a_star_modern.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>

// ═══════════════════════════════════════════════════════════════════════════
// AStarModern Implementation
// ═══════════════════════════════════════════════════════════════════════════

AStarModern::PointId AStarModern::get_available_point_id() const {
  if (points.empty()) {
    return 1;
  }

  // Find next available ID
  if (points.find(last_free_id) == points.end()) {
    return last_free_id;
  }

  // Linear search for next available
  PointId next_id = last_free_id;
  while (points.find(next_id) != points.end()) {
    next_id++;
  }

  return next_id;
}

void AStarModern::add_point(PointId id, const Vector3 &pos,
                            float weight_scale) {
  // Validate inputs
  if (id < 0) {
    throw std::invalid_argument("Point ID must be non-negative");
  }
  if (weight_scale < 1.0f) {
    throw std::invalid_argument("Weight scale must be >= 1.0");
  }

  // Check if point already exists
  auto it = points.find(id);
  if (it != points.end()) {
    // Update existing point
    it->second->pos = pos;
    it->second->weight_scale = weight_scale;
  } else {
    // Create new point using smart pointer
    auto point = std::make_shared<Point>();
    point->id = id;
    point->pos = pos;
    point->weight_scale = weight_scale;
    point->enabled = true;

    points[id] = point;
  }
}

Optional<Vector3> AStarModern::get_point_position(PointId id) const {
  auto pt = find_point(id);
  if (pt) {
    return pt.value()->pos;
  }
  return None;
}

bool AStarModern::set_point_position(PointId id, const Vector3 &pos) {
  auto pt = find_point(id);
  if (pt) {
    pt.value()->pos = pos;
    return true;
  }
  return false;
}

Optional<float> AStarModern::get_point_weight_scale(PointId id) const {
  auto pt = find_point(id);
  if (pt) {
    return pt.value()->weight_scale;
  }
  return None;
}

void AStarModern::set_point_weight_scale(PointId id, float weight_scale) {
  if (weight_scale < 1.0f) {
    throw std::invalid_argument("Weight scale must be >= 1.0");
  }

  auto pt = find_point(id);
  if (pt) {
    pt.value()->weight_scale = weight_scale;
  }
}

bool AStarModern::remove_point(PointId id) {
  auto it = points.find(id);
  if (it == points.end()) {
    return false;
  }

  auto point = it->second;

  // Remove from neighbors' neighbor lists
  for (auto &[neighbor_id, neighbor_ptr] : point->neighbors) {
    neighbor_ptr->neighbors.erase(id);
    neighbor_ptr->unlinked_neighbors.erase(id);

    // Remove segment
    Segment s;
    s.from_id = std::min(id, neighbor_id);
    s.to_id = std::max(id, neighbor_id);
    segments.erase(s);
  }

  // Remove from unlinked neighbors' lists
  for (auto &[neighbor_id, neighbor_ptr] : point->unlinked_neighbors) {
    neighbor_ptr->neighbors.erase(id);
    neighbor_ptr->unlinked_neighbors.erase(id);

    Segment s;
    s.from_id = std::min(id, neighbor_id);
    s.to_id = std::max(id, neighbor_id);
    segments.erase(s);
  }

  // Remove point
  points.erase(it);
  last_free_id = id;

  return true;
}

void AStarModern::connect_points(PointId from_id, PointId to_id,
                                 bool bidirectional) {
  if (from_id == to_id) {
    throw std::invalid_argument("Cannot connect point to itself");
  }

  auto from = find_point(from_id);
  if (!from) {
    throw std::invalid_argument("Source point does not exist");
  }

  auto to = find_point(to_id);
  if (!to) {
    throw std::invalid_argument("Destination point does not exist");
  }

  auto from_ptr = from.value();
  auto to_ptr = to.value();

  // Add connections
  from_ptr->neighbors[to_id] = to_ptr;

  if (bidirectional) {
    to_ptr->neighbors[from_id] = from_ptr;
  } else {
    to_ptr->unlinked_neighbors[from_id] = from_ptr;
  }

  // Update segment
  Segment new_segment;
  new_segment.from_id = std::min(from_id, to_id);
  new_segment.to_id = std::max(from_id, to_id);
  new_segment.direction =
      bidirectional ? Segment::BIDIRECTIONAL : Segment::FORWARD;

  auto seg_it = segments.find(new_segment);
  if (seg_it != segments.end()) {
    // Merge directions
    new_segment.direction = static_cast<Segment::Direction>(
        new_segment.direction | seg_it->direction);

    if (new_segment.direction == Segment::BIDIRECTIONAL) {
      // Clean up unlinked neighbors
      from_ptr->unlinked_neighbors.erase(to_id);
      to_ptr->unlinked_neighbors.erase(from_id);
    }

    segments.erase(seg_it);
  }

  segments.insert(new_segment);
}

void AStarModern::disconnect_points(PointId from_id, PointId to_id,
                                    bool bidirectional) {
  auto from = find_point(from_id);
  auto to = find_point(to_id);

  if (!from || !to) {
    return; // Silently fail if points don't exist
  }

  from = from.value();
  to = to.value();

  // Find and update segment
  Segment query;
  query.from_id = std::min(from_id, to_id);
  query.to_id = std::max(from_id, to_id);

  auto seg_it = segments.find(query);
  if (seg_it == segments.end()) {
    return; // No connection exists
  }

  Segment seg = *seg_it;

  // Remove connection direction
  if (bidirectional) {
    seg.direction = Segment::NONE;
    from.value()->neighbors.erase(to_id);
    to.value()->neighbors.erase(from_id);
    from.value()->unlinked_neighbors.erase(to_id);
    to.value()->unlinked_neighbors.erase(from_id);
  } else {
    // One-way disconnect
    seg.direction =
        static_cast<Segment::Direction>(seg.direction & ~query.direction);
    from.value()->neighbors.erase(to_id);

    if (seg.direction != Segment::NONE) {
      to.value()->unlinked_neighbors.erase(from_id);
    } else {
      to.value()->unlinked_neighbors[from_id] = from.value();
    }
  }

  segments.erase(seg_it);
  if (seg.direction != Segment::NONE) {
    segments.insert(seg);
  }
}

bool AStarModern::are_points_connected(PointId from_id, PointId to_id) const {
  Segment query;
  query.from_id = std::min(from_id, to_id);
  query.to_id = std::max(from_id, to_id);

  auto it = segments.find(query);
  return it != segments.end();
}

Optional<AStarModern::Path3D> AStarModern::get_point_path(PointId from_id,
                                                          PointId to_id) {
  return solve(from_id, to_id);
}

Optional<AStarModern::PathIds> AStarModern::get_id_path(PointId from_id,
                                                        PointId to_id) {
  auto path = solve(from_id, to_id);
  if (!path) {
    return None;
  }

  PathIds id_path;
  auto to_point = find_point(to_id);
  if (!to_point) {
    return None;
  }

  // Reconstruct ID path by walking backwards from end point
  auto current = to_point.value();
  while (current) {
    id_path.insert(0, current->id);
    current = current->prev_point;
  }

  return id_path;
}

AStarModern::PathIds AStarModern::get_point_ids() const {
  PathIds ids;
  for (const auto &[id, _] : points) {
    ids.push_back(id);
  }
  return ids;
}

Optional<AStarModern::PointId>
AStarModern::get_closest_point(const Vector3 &pos,
                               bool include_disabled) const {
  float closest_dist = std::numeric_limits<float>::max();
  Optional<PointId> closest_id = None;

  for (const auto &[id, point] : points) {
    if (!include_disabled && !point->enabled) {
      continue;
    }

    float dist = pos.distance_to(point->pos);
    if (dist < closest_dist) {
      closest_dist = dist;
      closest_id = id;
    }
  }

  return closest_id;
}

Vector3 AStarModern::get_closest_position_in_segment(const Vector3 &pos) const {
  // Implementation: find closest point on any edge
  Vector3 closest = Vector3::ZERO;
  float closest_dist = std::numeric_limits<float>::max();

  for (const auto &segment : segments) {
    auto from = find_point(segment.from_id);
    auto to = find_point(segment.to_id);

    if (!from || !to) {
      continue;
    }

    // Find closest point on line segment
    Vector3 p1 = from.value()->pos;
    Vector3 p2 = to.value()->pos;
    Vector3 nearest =
        p1 + (p2 - p1).normalized() *
                 std::max(0.0f, (pos - p1).dot((p2 - p1).normalized()));

    float dist = pos.distance_to(nearest);
    if (dist < closest_dist) {
      closest_dist = dist;
      closest = nearest;
    }
  }

  return closest;
}

bool AStarModern::set_point_disabled(PointId id, bool disabled) {
  auto pt = find_point(id);
  if (pt) {
    pt.value()->enabled = !disabled;
    return true;
  }
  return false;
}

bool AStarModern::is_point_disabled(PointId id) const {
  auto pt = find_point(id);
  if (pt) {
    return !pt.value()->enabled;
  }
  return false;
}

AStarModern::PathIds AStarModern::get_point_connections(PointId id) const {
  PathIds connections;
  auto pt = find_point(id);
  if (!pt) {
    return connections;
  }

  for (const auto &[neighbor_id, _] : pt.value()->neighbors) {
    connections.push_back(neighbor_id);
  }

  return connections;
}

int AStarModern::get_point_count() const { return points.size(); }

void AStarModern::reserve_space(int num_nodes) {
  if (num_nodes <= 0) {
    throw std::invalid_argument("Capacity must be > 0, got: " +
                                std::to_string(num_nodes));
  }
  // unordered_map doesn't support reserve in a meaningful way
  // But this maintains API compatibility
}

void AStarModern::clear() {
  points.clear();
  segments.clear();
  last_free_id = 0;
  search_pass = 0;
}

float AStarModern::estimate_cost(PointId from_id, PointId to_id) const {
  auto from = find_point(from_id);
  auto to = find_point(to_id);

  if (!from || !to) {
    return std::numeric_limits<float>::max();
  }

  // Default: Euclidean distance
  return from.value()->pos.distance_to(to.value()->pos);
}

float AStarModern::compute_cost(PointId from_id, PointId to_id) const {
  auto from = find_point(from_id);
  auto to = find_point(to_id);

  if (!from || !to) {
    return std::numeric_limits<float>::max();
  }

  // Cost is distance × weight scale
  float distance = from.value()->pos.distance_to(to.value()->pos);
  return distance * to.value()->weight_scale;
}

// ═══════════════════════════════════════════════════════════════════════════
// Private Helper Methods
// ═══════════════════════════════════════════════════════════════════════════

Optional<AStarModern::PointPtr> AStarModern::find_point(PointId id) const {
  auto it = points.find(id);
  if (it != points.end()) {
    return it->second;
  }
  return None;
}

Optional<AStarModern::Path3D> AStarModern::solve(PointId from_id,
                                                 PointId to_id) {
  auto from = find_point(from_id);
  auto to = find_point(to_id);

  if (!from || !to) {
    return None;
  }

  if (from.value() == to.value()) {
    // Same point
    return Path3D{from.value()->pos};
  }

  // A* algorithm
  search_pass++;

  using OpenNode = std::pair<float, PointPtr>; // (f_score, point)
  auto cmp = [](const OpenNode &a, const OpenNode &b) {
    return a.first > b.first; // Min-heap
  };
  std::priority_queue<OpenNode, std::vector<OpenNode>, decltype(cmp)> open_list(
      cmp);

  auto from_ptr = from.value();
  from_ptr->g_score = 0.0f;
  from_ptr->f_score = estimate_cost(from_id, to_id);
  from_ptr->open_pass = search_pass;

  open_list.push({from_ptr->f_score, from_ptr});

  while (!open_list.empty()) {
    auto [f_score, current] = open_list.top();
    open_list.pop();

    if (current->closed_pass == search_pass) {
      continue; // Already processed
    }

    if (current == to.value()) {
      // Found goal! Reconstruct path
      Path3D path;
      auto node = to.value();
      while (node) {
        path.insert(0, node->pos);
        node = node->prev_point;
      }
      return path;
    }

    current->closed_pass = search_pass;

    // Check all neighbors
    for (const auto &[neighbor_id, neighbor] : current->neighbors) {
      if (!neighbor->enabled || neighbor->closed_pass == search_pass) {
        continue;
      }

      float tentative_g =
          current->g_score + compute_cost(current->id, neighbor_id);

      if (neighbor->open_pass != search_pass ||
          tentative_g < neighbor->g_score) {
        neighbor->prev_point = current;
        neighbor->g_score = tentative_g;
        neighbor->f_score = tentative_g + estimate_cost(neighbor_id, to_id);
        neighbor->open_pass = search_pass;
        open_list.push({neighbor->f_score, neighbor});
      }
    }
  }

  // No path found
  return None;
}

// ═══════════════════════════════════════════════════════════════════════════
// AStar2DModern Implementation
// ═══════════════════════════════════════════════════════════════════════════

Vector3 AStar2DModern::to_3d(const Vector2 &v) {
  return Vector3(v.x, v.y, 0.0f);
}

Vector2 AStar2DModern::to_2d(const Vector3 &v) { return Vector2(v.x, v.y); }

void AStar2DModern::add_point(PointId id, const Vector2 &pos,
                              float weight_scale) {
  impl.add_point(id, to_3d(pos), weight_scale);
}

Optional<Vector2> AStar2DModern::get_point_position(PointId id) const {
  auto pos = impl.get_point_position(id);
  if (pos) {
    return to_2d(pos.value());
  }
  return None;
}

bool AStar2DModern::set_point_position(PointId id, const Vector2 &pos) {
  return impl.set_point_position(id, to_3d(pos));
}

void AStar2DModern::connect_points(PointId from_id, PointId to_id,
                                   bool bidirectional) {
  impl.connect_points(from_id, to_id, bidirectional);
}

void AStar2DModern::disconnect_points(PointId from_id, PointId to_id,
                                      bool bidirectional) {
  impl.disconnect_points(from_id, to_id, bidirectional);
}

Optional<AStar2DModern::Path2D> AStar2DModern::get_point_path(PointId from_id,
                                                              PointId to_id) {
  auto path = impl.get_point_path(from_id, to_id);
  if (path) {
    Path2D path_2d;
    for (const auto &pos : path.value()) {
      path_2d.push_back(to_2d(pos));
    }
    return path_2d;
  }
  return None;
}

Optional<AStar2DModern::PathIds> AStar2DModern::get_id_path(PointId from_id,
                                                            PointId to_id) {
  return impl.get_id_path(from_id, to_id);
}

AStar2DModern::PathIds AStar2DModern::get_point_ids() const {
  return impl.get_point_ids();
}

int AStar2DModern::get_point_count() const { return impl.get_point_count(); }

void AStar2DModern::clear() { impl.clear(); }

float AStar2DModern::estimate_cost(PointId from_id, PointId to_id) const {
  return impl.estimate_cost(from_id, to_id);
}

float AStar2DModern::compute_cost(PointId from_id, PointId to_id) const {
  return impl.compute_cost(from_id, to_id);
}

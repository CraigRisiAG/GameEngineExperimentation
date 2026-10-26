
#ifndef NAV_UTILS_H
#define NAV_UTILS_H

#include "core/math/vector3.h"
#include <vector>

/**
        @author AndreaCatania
*/

class NavRegion;

namespace gd {
struct Polygon;

union PointKey {

  struct {
    int64_t x : 21;
    int64_t y : 22;
    int64_t z : 21;
  };

  uint64_t key;
  bool operator<(const PointKey &p_key) const { return key < p_key.key; }
};

struct EdgeKey {

  PointKey a;
  PointKey b;

  bool operator<(const EdgeKey &p_key) const {
    return (a.key == p_key.a.key) ? (b.key < p_key.b.key)
                                  : (a.key < p_key.a.key);
  }

  EdgeKey(const PointKey &p_a = PointKey(), const PointKey &p_b = PointKey())
      : a(p_a), b(p_b) {
    if (a.key > b.key) {
      SWAP(a, b);
    }
  }
};

struct Point {
  Vector3 pos;
  PointKey key;
};

struct Edge {
  /// This edge ID
  int this_edge;

  /// Other Polygon
  Polygon *other_polygon;

  /// The other `Polygon` at this edge id has this `Polygon`.
  int other_edge;

  Edge() {
    this_edge = -1;
    other_polygon = NULL;
    other_edge = -1;
  }
};

struct Polygon {
  NavRegion *owner;

  /// The points of this `Polygon`
  std::vector<Point> points;

  /// Are the points clockwise ?
  bool clockwise;

  /// The edges of this `Polygon`
  std::vector<Edge> edges;

  /// The center of this `Polygon`
  Vector3 center;
};

struct Connection {

  Polygon *A;
  int A_edge;
  Polygon *B;
  int B_edge;

  Connection() {
    A = NULL;
    B = NULL;
    A_edge = -1;
    B_edge = -1;
  }
};

struct NavigationPoly {
  uint32_t self_id;
  /// This poly.
  const Polygon *poly;
  /// The previous navigation poly (id in the `navigation_poly` array).
  int prev_navigation_poly_id;
  /// The edge id in this `Poly` to reach the `prev_navigation_poly_id`.
  uint32_t back_navigation_edge;
  /// The entry location of this poly.
  Vector3 entry;
  /// The distance to the destination.
  float traveled_distance;

  NavigationPoly(const Polygon *p_poly)
      : self_id(0), poly(p_poly), prev_navigation_poly_id(-1),
        back_navigation_edge(0), traveled_distance(0.0) {}

  bool operator==(const NavigationPoly &other) const {
    return this->poly == other.poly;
  }

  bool operator!=(const NavigationPoly &other) const {
    return !operator==(other);
  }
};

struct FreeEdge {
  bool is_free;
  Polygon *poly;
  uint32_t edge_id;
  Vector3 edge_center;
  Vector3 edge_dir;
  float edge_len_squared;
};
} // namespace gd

#endif // NAV_UTILS_H

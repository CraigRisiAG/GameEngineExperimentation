
/**
 * @class Delaunay2D
 * @brief Implements 2D Delaunay triangulation over a set of points.
 *
 * Provides utility structures for triangles and edges, and static methods
 * to perform triangulation and supporting geometric tests.
 */

/**
 * @struct Triangle
 * @brief Represents a triangle by indices into a vertex list.
 *
 * Contains an array of three point indices and a flag indicating whether
 * the triangle has been marked as invalid during triangulation.
 *
 * @param points The indices of the triangle's vertices in the point array.
 * @param bad Indicates if the triangle should be discarded.
 */

/**
 * @struct Edge
 * @brief Represents an edge by indices into a vertex list.
 *
 * Contains an array of two point indices and a flag indicating whether
 * the edge has been marked as invalid during polygon edge processing.
 *
 * @param edge The indices of the edge's endpoints in the point array.
 * @param bad Indicates if the edge should be discarded.
 */

/**
 * @brief Tests whether a point lies inside the circumcircle of a triangle.
 *
 * Computes the circumcenter of the triangle defined by @p p_triangle and
 * checks if the point at index @p p_vertex is within or on the circumcircle.
 *
 * @param p_vertices The list of vertex positions.
 * @param p_triangle The triangle to test against.
 * @param p_vertex Index of the vertex to test.
 * @return true if the vertex lies inside or on the circumcircle; false
 * otherwise.
 */

/**
 * @brief Compares two edges for equivalence regardless of orientation.
 *
 * Determines if the endpoints of @p p_a and @p p_b correspond to the same
 * pair of points within an acceptable approximation.
 *
 * @param p_vertices The list of vertex positions.
 * @param p_a The first edge to compare.
 * @param p_b The second edge to compare.
 * @return true if both edges connect the same vertices; false otherwise.
 */

/**
 * @brief Generates a Delaunay triangulation from a set of 2D points.
 *
 * Builds a triangulation using the Bowyer-Watson algorithm: starts with a
 * super-triangle encompassing all points, iteratively inserts each point,
 * removes triangles whose circumcircles contain the point, and re-triangulates
 * the resulting polygonal hole. Discards any triangles that include
 * super-triangle vertices.
 *
 * @param p_points The input set of 2D points to triangulate.
 * @return A vector of Triangle instances representing the Delaunay
 * triangulation.
 */
#ifndef DELAUNAY_H
#define DELAUNAY_H

#include "core/math/rect2.h"

class Delaunay2D {
public:
  struct Triangle {

    int points[3];
    bool bad;
    Triangle() { bad = false; }
    Triangle(int p_a, int p_b, int p_c) {
      points[0] = p_a;
      points[1] = p_b;
      points[2] = p_c;
      bad = false;
    }
  };

  struct Edge {
    int edge[2];
    bool bad;
    Edge() { bad = false; }
    Edge(int p_a, int p_b) {
      bad = false;
      edge[0] = p_a;
      edge[1] = p_b;
    }
  };

  static bool circum_circle_contains(const Vector<Vector2> &p_vertices,
                                     const Triangle &p_triangle, int p_vertex) {

    Vector2 p1 = p_vertices[p_triangle.points[0]];
    Vector2 p2 = p_vertices[p_triangle.points[1]];
    Vector2 p3 = p_vertices[p_triangle.points[2]];

    real_t ab = p1.x * p1.x + p1.y * p1.y;
    real_t cd = p2.x * p2.x + p2.y * p2.y;
    real_t ef = p3.x * p3.x + p3.y * p3.y;

    Vector2 circum(
        (ab * (p3.y - p2.y) + cd * (p1.y - p3.y) + ef * (p2.y - p1.y)) /
            (p1.x * (p3.y - p2.y) + p2.x * (p1.y - p3.y) +
             p3.x * (p2.y - p1.y)),
        (ab * (p3.x - p2.x) + cd * (p1.x - p3.x) + ef * (p2.x - p1.x)) /
            (p1.y * (p3.x - p2.x) + p2.y * (p1.x - p3.x) +
             p3.y * (p2.x - p1.x)));

    circum *= 0.5;
    float r = p1.distance_squared_to(circum);
    float d = p_vertices[p_vertex].distance_squared_to(circum);
    return d <= r;
  }

  static bool edge_compare(const Vector<Vector2> &p_vertices, const Edge &p_a,
                           const Edge &p_b) {
    if (p_vertices[p_a.edge[0]].is_equal_approx(p_vertices[p_b.edge[0]]) &&
        p_vertices[p_a.edge[1]].is_equal_approx(p_vertices[p_b.edge[1]])) {
      return true;
    }

    if (p_vertices[p_a.edge[0]].is_equal_approx(p_vertices[p_b.edge[1]]) &&
        p_vertices[p_a.edge[1]].is_equal_approx(p_vertices[p_b.edge[0]])) {
      return true;
    }

    return false;
  }

  static Vector<Triangle> triangulate(const Vector<Vector2> &p_points) {

    Vector<Vector2> points = p_points;
    Vector<Triangle> triangles;

    Rect2 rect;
    for (int i = 0; i < p_points.size(); i++) {
      if (i == 0) {
        rect.position = p_points[i];
      } else {
        rect.expand_to(p_points[i]);
      }
    }

    float delta_max = MAX(rect.size.width, rect.size.height);
    Vector2 center = rect.position + rect.size * 0.5;

    points.push_back(Vector2(center.x - 20 * delta_max, center.y - delta_max));
    points.push_back(Vector2(center.x, center.y + 20 * delta_max));
    points.push_back(Vector2(center.x + 20 * delta_max, center.y - delta_max));

    triangles.push_back(Triangle(p_points.size() + 0, p_points.size() + 1,
                                 p_points.size() + 2));

    for (int i = 0; i < p_points.size(); i++) {
      // std::cout << "Traitement du point " << *p << std::endl;
      // std::cout << "_triangles contains " << _triangles.size() << " elements"
      // << std::endl;

      Vector<Edge> polygon;

      for (int j = 0; j < triangles.size(); j++) {
        if (circum_circle_contains(points, triangles[j], i)) {
          triangles.write[j].bad = true;
          polygon.push_back(
              Edge(triangles[j].points[0], triangles[j].points[1]));
          polygon.push_back(
              Edge(triangles[j].points[1], triangles[j].points[2]));
          polygon.push_back(
              Edge(triangles[j].points[2], triangles[j].points[0]));
        }
      }

      for (int j = 0; j < triangles.size(); j++) {
        if (triangles[j].bad) {
          triangles.remove(j);
          j--;
        }
      }

      for (int j = 0; j < polygon.size(); j++) {
        for (int k = j + 1; k < polygon.size(); k++) {
          if (edge_compare(points, polygon[j], polygon[k])) {
            polygon.write[j].bad = true;
            polygon.write[k].bad = true;
          }
        }
      }

      for (int j = 0; j < polygon.size(); j++) {

        if (polygon[j].bad) {
          continue;
        }
        triangles.push_back(
            Triangle(polygon[j].edge[0], polygon[j].edge[1], i));
      }
    }

    for (int i = 0; i < triangles.size(); i++) {
      bool invalid = false;
      for (int j = 0; j < 3; j++) {
        if (triangles[i].points[j] >= p_points.size()) {
          invalid = true;
          break;
        }
      }
      if (invalid) {
        triangles.remove(i);
        i--;
      }
    }

    return triangles;
  }
};

#endif // DELAUNAY_H

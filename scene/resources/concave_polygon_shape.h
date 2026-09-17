
#ifndef CONCAVE_POLYGON_SHAPE_H
#define CONCAVE_POLYGON_SHAPE_H

#include "scene/resources/shape.h"

class ConcavePolygonShape : public Shape {

  GDCLASS(ConcavePolygonShape, Shape);

  struct DrawEdge {

    Vector3 a;
    Vector3 b;
    bool operator<(const DrawEdge &p_edge) const {
      if (a == p_edge.a)
        return b < p_edge.b;
      else
        return a < p_edge.a;
    }

    DrawEdge(const Vector3 &p_a = Vector3(), const Vector3 &p_b = Vector3()) {
      a = p_a;
      b = p_b;
      if (a < b) {
        SWAP(a, b);
      }
    }
  };

protected:
  static void _bind_methods();

  virtual void _update_shape();

public:
  void set_faces(const Vector<Vector3> &p_faces);
  Vector<Vector3> get_faces() const;

  virtual Vector<Vector3> get_debug_mesh_lines();
  virtual real_t get_enclosing_radius() const;

  ConcavePolygonShape();
};

#endif // CONCAVE_POLYGON_SHAPE_H

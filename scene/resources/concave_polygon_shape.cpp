
#include "concave_polygon_shape.h"

#include "servers/physics_server.h"

Vector<Vector3> ConcavePolygonShape::get_debug_mesh_lines() {

  Set<DrawEdge> edges;

  Vector<Vector3> data = get_faces();
  int datalen = data.size();
  ERR_FAIL_COND_V((datalen % 3) != 0, Vector<Vector3>());

  const Vector3 *r = data.ptr();

  for (int i = 0; i < datalen; i += 3) {

    for (int j = 0; j < 3; j++) {

      DrawEdge de(r[i + j], r[i + ((j + 1) % 3)]);
      edges.insert(de);
    }
  }

  Vector<Vector3> points;
  points.resize(edges.size() * 2);
  int idx = 0;
  for (Set<DrawEdge>::Element *E = edges.front(); E; E = E->next()) {

    points.write[idx + 0] = E->get().a;
    points.write[idx + 1] = E->get().b;
    idx += 2;
  }

  return points;
}

real_t ConcavePolygonShape::get_enclosing_radius() const {
  Vector<Vector3> data = get_faces();
  const Vector3 *read = data.ptr();
  real_t r = 0;
  for (int i(0); i < data.size(); i++) {
    r = MAX(read[i].length_squared(), r);
  }
  return Math::sqrt(r);
}

void ConcavePolygonShape::_update_shape() { Shape::_update_shape(); }

void ConcavePolygonShape::set_faces(const Vector<Vector3> &p_faces) {

  PhysicsServer::get_singleton()->shape_set_data(get_shape(), p_faces);
  notify_change_to_owners();
}

Vector<Vector3> ConcavePolygonShape::get_faces() const {

  return PhysicsServer::get_singleton()->shape_get_data(get_shape());
}

void ConcavePolygonShape::_bind_methods() {

  ClassDB::bind_method(D_METHOD("set_faces", "faces"),
                       &ConcavePolygonShape::set_faces);
  ClassDB::bind_method(D_METHOD("get_faces"), &ConcavePolygonShape::get_faces);
  ADD_PROPERTY(PropertyInfo(Variant::PACKED_VECTOR3_ARRAY, "data",
                            PROPERTY_HINT_NONE, "",
                            PROPERTY_USAGE_NOEDITOR | PROPERTY_USAGE_INTERNAL),
               "set_faces", "get_faces");
}

ConcavePolygonShape::ConcavePolygonShape()
    : Shape(PhysicsServer::get_singleton()->shape_create(
          PhysicsServer::SHAPE_CONCAVE_POLYGON)) {

  // set_planes(Vector3(1,1,1));
}

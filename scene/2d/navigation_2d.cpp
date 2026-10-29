
#include "navigation_2d.h"
#include "servers/navigation_2d_server.h"

void Navigation2D::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_rid"), &Navigation2D::get_rid);

  ClassDB::bind_method(D_METHOD("get_simple_path", "start", "end", "optimize"),
                       &Navigation2D::get_simple_path, DEFVAL(true));
  ClassDB::bind_method(D_METHOD("get_closest_point", "to_point"),
                       &Navigation2D::get_closest_point);
  ClassDB::bind_method(D_METHOD("get_closest_point_owner", "to_point"),
                       &Navigation2D::get_closest_point_owner);

  ClassDB::bind_method(D_METHOD("set_cell_size", "cell_size"),
                       &Navigation2D::set_cell_size);
  ClassDB::bind_method(D_METHOD("get_cell_size"), &Navigation2D::get_cell_size);

  ClassDB::bind_method(D_METHOD("set_edge_connection_margin", "margin"),
                       &Navigation2D::set_edge_connection_margin);
  ClassDB::bind_method(D_METHOD("get_edge_connection_margin"),
                       &Navigation2D::get_edge_connection_margin);

  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cell_size"), "set_cell_size",
               "get_cell_size");
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "edge_connection_margin"),
               "set_edge_connection_margin", "get_edge_connection_margin");
}

void Navigation2D::_notification(int p_what) {
  switch (p_what) {
  case NOTIFICATION_READY: {
    Navigation2DServer::get_singleton()->map_set_active(map, true);
  } break;
  case NOTIFICATION_EXIT_TREE: {

    Navigation2DServer::get_singleton()->map_set_active(map, false);
  } break;
  }
}

void Navigation2D::set_cell_size(float p_cell_size) {
  cell_size = p_cell_size;
  Navigation2DServer::get_singleton()->map_set_cell_size(map, cell_size);
}

void Navigation2D::set_edge_connection_margin(float p_edge_connection_margin) {
  edge_connection_margin = p_edge_connection_margin;
  Navigation2DServer::get_singleton()->map_set_edge_connection_margin(
      map, edge_connection_margin);
}

Vector<Vector2> Navigation2D::get_simple_path(const Vector2 &p_start,
                                              const Vector2 &p_end,
                                              bool p_optimize) const {
  return Navigation2DServer::get_singleton()->map_get_path(map, p_start, p_end,
                                                           p_optimize);
}

Vector2 Navigation2D::get_closest_point(const Vector2 &p_point) const {
  return Navigation2DServer::get_singleton()->map_get_closest_point(map,
                                                                    p_point);
}

RID Navigation2D::get_closest_point_owner(const Vector2 &p_point) const {
  return Navigation2DServer::get_singleton()->map_get_closest_point_owner(
      map, p_point);
}

Navigation2D::Navigation2D() {

  map = Navigation2DServer::get_singleton()->map_create();
  set_cell_size(10); // Ten pixels
  set_edge_connection_margin(100);
}

Navigation2D::~Navigation2D() {
  Navigation2DServer::get_singleton()->free(map);
}

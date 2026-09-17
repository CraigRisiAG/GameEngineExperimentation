
#include "rectangle_shape_2d.h"

#include "servers/physics_2d_server.h"
#include "servers/visual_server.h"
void RectangleShape2D::_update_shape() {

  Physics2DServer::get_singleton()->shape_set_data(get_rid(), extents);
  emit_changed();
}

void RectangleShape2D::set_extents(const Vector2 &p_extents) {

  extents = p_extents;
  _update_shape();
}

Vector2 RectangleShape2D::get_extents() const { return extents; }

void RectangleShape2D::draw(const RID &p_to_rid, const Color &p_color) {

  VisualServer::get_singleton()->canvas_item_add_rect(
      p_to_rid, Rect2(-extents, extents * 2.0), p_color);
}

Rect2 RectangleShape2D::get_rect() const {

  return Rect2(-extents, extents * 2.0);
}

real_t RectangleShape2D::get_enclosing_radius() const {
  return extents.length();
}

void RectangleShape2D::_bind_methods() {

  ClassDB::bind_method(D_METHOD("set_extents", "extents"),
                       &RectangleShape2D::set_extents);
  ClassDB::bind_method(D_METHOD("get_extents"), &RectangleShape2D::get_extents);

  ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "extents"), "set_extents",
               "get_extents");
}

RectangleShape2D::RectangleShape2D()
    : Shape2D(Physics2DServer::get_singleton()->rectangle_shape_create()) {

  extents = Vector2(10, 10);
  _update_shape();
}

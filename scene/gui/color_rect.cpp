// This file contains the implementation of the ColorRect class, which is
// responsible for rendering and managing color rectangle controls within the
// GUI. It provides functionality for setting and getting the frame color, as
// well as handling drawing notifications to display the color rectangle.
#include "color_rect.h"

void ColorRect::set_frame_color(const Color &p_color) {

  color = p_color;
  update();
}

Color ColorRect::get_frame_color() const { return color; }

void ColorRect::_notification(int p_what) {

  if (p_what == NOTIFICATION_DRAW) {
    draw_rect(Rect2(Point2(), get_size()), color);
  }
}

void ColorRect::_bind_methods() {

  ClassDB::bind_method(D_METHOD("set_frame_color", "color"),
                       &ColorRect::set_frame_color);
  ClassDB::bind_method(D_METHOD("get_frame_color"),
                       &ColorRect::get_frame_color);

  ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_frame_color",
               "get_frame_color");
}

ColorRect::ColorRect() { color = Color(1, 1, 1); }

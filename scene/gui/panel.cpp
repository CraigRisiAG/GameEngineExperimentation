// This file contains the implementation of the Panel class, which is
// responsible for managing a panel within the GUI. It provides functionality
// for drawing the panel and handling mouse input.
#include "panel.h"
#include "core/print_string.h"

void Panel::_notification(int p_what) {

  if (p_what == NOTIFICATION_DRAW) {

    RID ci = get_canvas_item();
    Ref<StyleBox> style = get_stylebox("panel");
    style->draw(ci, Rect2(Point2(), get_size()));
  }
}

Panel::Panel() {
  // Has visible stylebox, so stop by default.
  set_mouse_filter(MOUSE_FILTER_STOP);
}

Panel::~Panel() {}

// This file contains the declaration of the ColorRect class, which is
// responsible for rendering and managing color rectangle controls within the
// GUI. It provides functionality for setting and getting the frame color, as
// well as handling drawing notifications to display the color rectangle.
#ifndef COLOR_RECT_H
#define COLOR_RECT_H

#include "scene/gui/control.h"

class ColorRect : public Control {
  GDCLASS(ColorRect, Control);

  Color color;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_frame_color(const Color &p_color);
  Color get_frame_color() const;

  ColorRect();
};

#endif // COLOR_RECT_H

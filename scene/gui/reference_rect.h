
#ifndef REFERENCE_RECT_H
#define REFERENCE_RECT_H

#include "scene/gui/control.h"

class ReferenceRect : public Control {

  GDCLASS(ReferenceRect, Control);
  Color border_color;
  bool editor_only;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  ReferenceRect();

  void set_border_color(const Color &p_color);
  Color get_border_color() const;

  void set_editor_only(const bool &p_enabled);
  bool get_editor_only() const;
};

#endif // REFERENCE_RECT_H

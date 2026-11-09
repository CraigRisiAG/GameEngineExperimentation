// This file contains the declarations of the LinkButton class, which is
// responsible for managing a clickable text link within the GUI.
// It provides functionality for handling text display, underline modes,
// focus indication, and visual feedback for different interaction states.
#ifndef LINKBUTTON_H
#define LINKBUTTON_H

#include "scene/gui/base_button.h"
#include "scene/resources/bit_map.h"

class LinkButton : public BaseButton {

  GDCLASS(LinkButton, BaseButton);

public:
  enum UnderlineMode {
    UNDERLINE_MODE_ALWAYS,
    UNDERLINE_MODE_ON_HOVER,
    UNDERLINE_MODE_NEVER
  };

private:
  String text;
  UnderlineMode underline_mode;

protected:
  virtual Size2 get_minimum_size() const;
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_text(const String &p_text);
  String get_text() const;

  void set_underline_mode(UnderlineMode p_underline_mode);
  UnderlineMode get_underline_mode() const;

  LinkButton();
};

VARIANT_ENUM_CAST(LinkButton::UnderlineMode);

#endif // LINKBUTTON_H

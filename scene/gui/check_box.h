// This file contains the declaration of the CheckBox class, which is
// responsible for rendering and managing checkbox controls within the GUI.
// It provides functionality for calculating minimum sizes, handling internal
// margins, drawing the checkbox in various states (checked, unchecked, radio),
// and managing icons and text within the checkbox.
#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "scene/gui/button.h"
/**
@author Mariano Suligoy <marianognu.esyrpg@gmail.com>
*/
class CheckBox : public Button {

  GDCLASS(CheckBox, Button);

protected:
  Size2 get_icon_size() const;
  Size2 get_minimum_size() const;
  void _notification(int p_what);

  bool is_radio();

public:
  CheckBox(const String &p_text = String());
  ~CheckBox();
};

#endif

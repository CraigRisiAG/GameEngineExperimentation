// This file contains the declaration of the CheckButton class, which is
// responsible for rendering and managing check button controls within the GUI.
// It provides functionality for calculating minimum sizes, handling internal
// margins, drawing the check button in various states (on, off, disabled),
// and managing icons and text within the check button.
#ifndef CHECK_BUTTON_H
#define CHECK_BUTTON_H

#include "scene/gui/button.h"
/**
@author Juan Linietsky <reduzio@gmail.com>
*/
class CheckButton : public Button {

  GDCLASS(CheckButton, Button);

protected:
  Size2 get_icon_size() const;
  virtual Size2 get_minimum_size() const;
  void _notification(int p_what);

public:
  CheckButton();
  ~CheckButton();
};

#endif

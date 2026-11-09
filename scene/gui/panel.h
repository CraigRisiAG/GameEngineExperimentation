// This file contains the declarations of the Panel class, which is
// responsible for managing a panel within the GUI. It provides functionality
// for drawing the panel and handling mouse input.
#ifndef PANEL_H
#define PANEL_H

#include "scene/gui/control.h"

class Panel : public Control {

  GDCLASS(Panel, Control);

protected:
  void _notification(int p_what);

public:
  Panel();
  ~Panel();
};

#endif

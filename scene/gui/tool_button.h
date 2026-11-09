// This file contains the declaration of the ToolButton class, which is
// responsible for managing tool button widgets within the GUI. It provides
// functionality for handling button states, flat appearance, and rendering the
// visual representation of the tool button.
#ifndef TOOL_BUTTON_H
#define TOOL_BUTTON_H

#include "scene/gui/button.h"

class ToolButton : public Button {
  GDCLASS(ToolButton, Button);

public:
  ToolButton();
};

#endif // TOOL_BUTTON_H

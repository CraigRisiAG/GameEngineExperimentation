/// A custom UI control that handles drag interactions for panes.
///
/// PaneDrag extends the Control class to provide drag functionality for UI
/// panes. It tracks mouse hover states and processes input events to enable
/// dragging behavior.
///
/// @note This class is designed for use within the Godot engine editor.

#ifndef PANE_DRAG_H
#define PANE_DRAG_H

#include "scene/gui/control.h"

class PaneDrag : public Control {

  GDCLASS(PaneDrag, Control);

  bool mouse_over;

protected:
  void _gui_input(const Ref<InputEvent> &p_input);
  void _notification(int p_what);
  virtual Size2 get_minimum_size() const;
  static void _bind_methods();

public:
  PaneDrag();
};

#endif // PANE_DRAG_H

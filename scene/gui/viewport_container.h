// This file contains the declaration of the ViewportContainer class, which is
// responsible for managing viewport container widgets within the GUI. It
// provides functionality for handling viewport stretching, input
// transformation, and rendering the visual representation of the contained
// viewports.
#ifndef VIEWPORTCONTAINER_H
#define VIEWPORTCONTAINER_H

#include "scene/gui/container.h"

class ViewportContainer : public Container {

  GDCLASS(ViewportContainer, Container);

  bool stretch;
  int shrink;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_stretch(bool p_enable);
  bool is_stretch_enabled() const;

  void _input(const Ref<InputEvent> &p_event);
  void _unhandled_input(const Ref<InputEvent> &p_event);
  void set_stretch_shrink(int p_shrink);
  int get_stretch_shrink() const;

  virtual Size2 get_minimum_size() const;

  ViewportContainer();
};

#endif // VIEWPORTCONTAINER_H

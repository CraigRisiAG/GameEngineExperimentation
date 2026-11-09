// This file contains the declaration of the ShortCut class, which is
// responsible for managing keyboard shortcuts within the GUI. It provides
// functionality for setting, retrieving, and validating shortcuts, as well
// as converting them to a human-readable text representation.
#ifndef SHORTCUT_H
#define SHORTCUT_H

#include "core/os/input_event.h"
#include "core/resource.h"

class ShortCut : public Resource {

  GDCLASS(ShortCut, Resource);

  Ref<InputEvent> shortcut;

protected:
  static void _bind_methods();

public:
  void set_shortcut(const Ref<InputEvent> &p_shortcut);
  Ref<InputEvent> get_shortcut() const;
  bool is_shortcut(const Ref<InputEvent> &p_event) const;
  bool is_valid() const;

  String get_as_text() const;

  ShortCut();
};

#endif // SHORTCUT_H

// This file contains the declaration of the Separator class, which is
// responsible for managing separator lines within the GUI. It provides
// functionality for handling the visual representation and layout of
// horizontal and vertical separators.
#ifndef SEPARATOR_H
#define SEPARATOR_H

#include "scene/gui/control.h"
class Separator : public Control {

  GDCLASS(Separator, Control);

protected:
  Orientation orientation;
  void _notification(int p_what);

public:
  virtual Size2 get_minimum_size() const;

  Separator();
  ~Separator();
};

class VSeparator : public Separator {

  GDCLASS(VSeparator, Separator);

public:
  VSeparator();
};

class HSeparator : public Separator {

  GDCLASS(HSeparator, Separator);

public:
  HSeparator();
};

#endif

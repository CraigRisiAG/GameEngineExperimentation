// This file contains the declarations of the MarginContainer class, which is
// responsible for managing a container with configurable margins within the
// GUI. It provides functionality for calculating the minimum size based on
// child controls and arranging child controls within the specified margins.
#ifndef MARGIN_CONTAINER_H
#define MARGIN_CONTAINER_H

#include "scene/gui/container.h"

class MarginContainer : public Container {
  GDCLASS(MarginContainer, Container);

protected:
  void _notification(int p_what);

public:
  virtual Size2 get_minimum_size() const;

  MarginContainer();
};

#endif // MARGIN_CONTAINER_H

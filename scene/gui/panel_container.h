// This file contains the declarations of the PanelContainer class, which is
// responsible for managing a container with a panel style within the GUI.
// It provides functionality for calculating the minimum size based on child
// controls, drawing the panel, and arranging child controls within the panel.
#ifndef PANEL_CONTAINER_H
#define PANEL_CONTAINER_H

#include "scene/gui/container.h"

class PanelContainer : public Container {

  GDCLASS(PanelContainer, Container);

protected:
  void _notification(int p_what);

public:
  virtual Size2 get_minimum_size() const;

  PanelContainer();
};

#endif // PANEL_CONTAINER_H

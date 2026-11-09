// This file contains the declarations of the GridContainer class, which is
// responsible for managing a grid layout of child controls within the GUI.
// It provides functionality for computing the minimum size of each cell,
// handling column and row expansion, and arranging child controls based on
// the specified number of columns and spacing constants.
#ifndef GRID_CONTAINER_H
#define GRID_CONTAINER_H

#include "scene/gui/container.h"

class GridContainer : public Container {

  GDCLASS(GridContainer, Container);

  int columns;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_columns(int p_columns);
  int get_columns() const;
  virtual Size2 get_minimum_size() const;

  GridContainer();
};

#endif // GRID_CONTAINER_H

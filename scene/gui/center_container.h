// This file contains the declaration of the CenterContainer class, which is
// responsible for centering child controls within the container. It provides
// functionality for calculating minimum sizes, handling top-left alignment,
// and sorting children to ensure proper positioning within the container.
#ifndef CENTER_CONTAINER_H
#define CENTER_CONTAINER_H

#include "scene/gui/container.h"

class CenterContainer : public Container {

  GDCLASS(CenterContainer, Container);

  bool use_top_left;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_use_top_left(bool p_enable);
  bool is_using_top_left() const;

  virtual Size2 get_minimum_size() const;

  CenterContainer();
};

#endif // CENTER_CONTAINER_H

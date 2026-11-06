// This file contains the declaration of the CanvasModulate class,
// which represents a node that can modulate the color of a 2D canvas.
// It provides functionality for setting and getting the modulation color,
// and ensures that only one visible CanvasModulate is active per canvas.
#ifndef CANVASMODULATE_H
#define CANVASMODULATE_H

#include "scene/2d/node_2d.h"

class CanvasModulate : public Node2D {

  GDCLASS(CanvasModulate, Node2D);

  Color color;

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_color(const Color &p_color);
  Color get_color() const;

  String get_configuration_warning() const;

  CanvasModulate();
  ~CanvasModulate();
};

#endif // CANVASMODULATE_H

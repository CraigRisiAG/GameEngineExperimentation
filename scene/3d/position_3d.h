// This file contains the declaration of the Position3D class,
// which represents a 3D position in the scene. It provides basic
// functionality for managing the position node within the 3D scene.
#ifndef POSITION_3D_H
#define POSITION_3D_H

#include "scene/3d/spatial.h"

class Position3D : public Spatial {

  GDCLASS(Position3D, Spatial);

public:
  Position3D();
};

#endif // POSITION_3D_H

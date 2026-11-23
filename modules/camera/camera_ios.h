// This file declares the CameraIOS class, which provides an interface
// for the camera on iOS devices within the Godot engine. It includes
// methods for updating camera feeds and interacting with the camera server.
#ifndef CAMERAIOS_H
#define CAMERAIOS_H

#include "servers/camera_server.h"

class CameraIOS : public CameraServer {
private:
public:
  CameraIOS();
  ~CameraIOS();

  void update_feeds();
};

#endif /* CAMERAIOS_H */

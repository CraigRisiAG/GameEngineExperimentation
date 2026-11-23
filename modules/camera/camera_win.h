// This file declares the CameraWindows class, which provides an interface
// for the camera on Windows devices within the Godot engine. It includes
// methods for managing active camera feeds and interacting with the camera
// server.
#ifndef CAMERAWIN_H
#define CAMERAWIN_H

#include "servers/camera/camera_feed.h"
#include "servers/camera_server.h"

class CameraWindows : public CameraServer {
private:
  void add_active_cameras();

public:
  CameraWindows();
  ~CameraWindows();
};

#endif /* CAMERAWIN_H */


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

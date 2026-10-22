
#ifndef CAMERAOSX_H
#define CAMERAOSX_H

///@TODO this is a near duplicate of CameraIOS, we should find a way to combine
///those to minimize code duplication!!!!
// If you fix something here, make sure you fix it there as wel!

#include "servers/camera_server.h"

class CameraOSX : public CameraServer {
public:
  CameraOSX();
  ~CameraOSX();

  void update_feeds();
};

#endif /* CAMERAOSX_H */

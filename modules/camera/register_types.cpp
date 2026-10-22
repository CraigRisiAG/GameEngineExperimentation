
#include "register_types.h"

#if defined(WINDOWS_ENABLED)
#include "camera_win.h"
#endif
#if defined(IPHONE_ENABLED)
#include "camera_ios.h"
#endif
#if defined(OSX_ENABLED)
#include "camera_osx.h"
#endif

void register_camera_types() {
#if defined(WINDOWS_ENABLED)
  CameraServer::make_default<CameraWindows>();
#endif
#if defined(IPHONE_ENABLED)
  CameraServer::make_default<CameraIOS>();
#endif
#if defined(OSX_ENABLED)
  CameraServer::make_default<CameraOSX>();
#endif
}

void unregister_camera_types() {}

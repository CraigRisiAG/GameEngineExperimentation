
#include "osx_utils.h"

#ifdef OSX_ENABLED

#include "core/print_string.h"

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>

bool osx_is_app_bundle_installed(const String &p_bundle_id) {

  CFURLRef app_url = NULL;
  CFStringRef bundle_id = CFStringCreateWithCString(NULL, p_bundle_id.utf8(),
                                                    kCFStringEncodingUTF8);
  OSStatus result = LSFindApplicationForInfo(kLSUnknownCreator, bundle_id, NULL,
                                             NULL, &app_url);
  CFRelease(bundle_id);

  if (app_url)
    CFRelease(app_url);

  switch (result) {
  case noErr:
    return true;
  case kLSApplicationNotFoundErr:
    break;
  default:
    break;
  }

  return false;
}

#endif


#include "core/ustring.h"

#ifndef OSX_UTILS_H
#define OSX_UTILS_H

#ifdef OSX_ENABLED

bool osx_is_app_bundle_installed(const String &p_bundle_id);

#endif

#endif // OSX_UTILS_H

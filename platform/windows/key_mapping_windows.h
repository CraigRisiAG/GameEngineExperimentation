
#ifndef KEY_MAPPING_WINDOWS_H
#define KEY_MAPPING_WINDOWS_H

#include "core/os/keyboard.h"

#include <windows.h>

#include <winuser.h>

class KeyMappingWindows {

  KeyMappingWindows() {};

public:
  static unsigned int get_keysym(unsigned int p_code);
  static unsigned int get_scansym(unsigned int p_code, bool p_extended);
};

#endif // KEY_MAPPING_WINDOWS_H

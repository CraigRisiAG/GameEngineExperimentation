
#ifndef MONO_REG_UTILS_H
#define MONO_REG_UTILS_H

#ifdef WINDOWS_ENABLED

#include "core/ustring.h"

struct MonoRegInfo {

  String version;
  String install_root_dir;
  String assembly_dir;
  String config_dir;
  String bin_dir;
};

namespace MonoRegUtils {

MonoRegInfo find_mono();
String find_msbuild_tools_path();
} // namespace MonoRegUtils

#endif // WINDOWS_ENABLED

#endif // MONO_REG_UTILS_H

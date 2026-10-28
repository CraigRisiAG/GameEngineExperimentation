
#ifndef GODOTSHARP_EXPORT_H
#define GODOTSHARP_EXPORT_H

#include "core/dictionary.h"
#include "core/error_list.h"
#include "core/ustring.h"

#include "../mono_gd/gd_mono_header.h"

namespace GodotSharpExport {

Error get_assembly_dependencies(GDMonoAssembly *p_assembly,
                                const Vector<String> &p_search_dirs,
                                Dictionary &r_dependencies);

Error get_exported_assembly_dependencies(
    const Dictionary &p_initial_dependencies, const String &p_build_config,
    const String &p_custom_lib_dir, Dictionary &r_dependencies);

} // namespace GodotSharpExport

#endif // GODOTSHARP_EXPORT_H


#ifndef CSHARP_PROJECT_H
#define CSHARP_PROJECT_H

#include "core/ustring.h"

namespace CSharpProject {

void add_item(const String &p_project_path, const String &p_item_type,
              const String &p_include);

} // namespace CSharpProject

#endif // CSHARP_PROJECT_H

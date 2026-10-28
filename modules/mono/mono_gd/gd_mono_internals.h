
#ifndef GD_MONO_INTERNALS_H
#define GD_MONO_INTERNALS_H

#include <mono/jit/jit.h>

#include "../utils/macros.h"

#include "core/object.h"

namespace GDMonoInternals {

void tie_managed_to_unmanaged(MonoObject *managed, Object *unmanaged);

/**
 * Do not call this function directly.
 * Use GDMonoUtils::debug_unhandled_exception(MonoException *) instead.
 */
void unhandled_exception(MonoException *p_exc);

} // namespace GDMonoInternals

#endif // GD_MONO_INTERNALS_H

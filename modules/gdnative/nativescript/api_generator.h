// This file declares the interface for generating a C API representation of the
// Godot classes, methods, properties, constants, signals, and enums. It is used
// to facilitate the creation of bindings for the Godot NativeScript API.
#ifndef API_GENERATOR_H
#define API_GENERATOR_H

#include "core/typedefs.h"
#include "core/ustring.h"

Error generate_c_api(const String &p_path);

#endif // API_GENERATOR_H

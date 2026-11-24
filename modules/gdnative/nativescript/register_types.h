// This file declares the functions for registering and unregistering the
// NativeScript types with the Godot engine. These functions are implemented in
// register_types.cpp and are used to integrate the NativeScript language and
// its associated resource loaders and savers into the Godot engine.
#ifndef NATIVESCRIPT_REGISTER_TYPES_H
#define NATIVESCRIPT_REGISTER_TYPES_H

void register_nativescript_types();
void unregister_nativescript_types();

#endif // NATIVESCRIPT_REGISTER_TYPES_H

// This file declares the functions for registering and unregistering
// GDNative-related types within the Godot engine. It provides the interface for
// initializing and cleaning up the GDNative module, ensuring that all necessary
// types and plugins are properly registered when the module is loaded and
// unregistered when it is unloaded.
#ifndef GDNATIVE_REGISTER_TYPES_H
#define GDNATIVE_REGISTER_TYPES_H

void register_gdnative_types();
void unregister_gdnative_types();

#endif // GDNATIVE_REGISTER_TYPES_H

// This file declares the functions for registering and unregistering the
// GDScript types within the Godot engine. It provides the interface for
// initializing and cleaning up the GDScript module, including resource loaders
// and savers.
#ifndef GDSCRIPT_REGISTER_TYPES_H
#define GDSCRIPT_REGISTER_TYPES_H

void register_gdscript_types();
void unregister_gdscript_types();

#endif // GDSCRIPT_REGISTER_TYPES_H

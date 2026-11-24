// This file declares the functions for registering and unregistering the
// PluginScript types and languages in Godot. It allows external code to define
// and interact with custom scripting languages via the GDNative API.
#ifndef PLUGINSCRIPT_REGISTER_TYPES_H
#define PLUGINSCRIPT_REGISTER_TYPES_H

void register_pluginscript_types();
void unregister_pluginscript_types();

#endif // PLUGINSCRIPT_REGISTER_TYPES_H

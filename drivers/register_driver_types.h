/**
 * Driver registration header for Godot Engine.
 * This file declares functions to register and unregister core and
 * platform-specific driver types, such as image loaders and resource savers.
 */
#ifndef REGISTER_DRIVER_TYPES_H
#define REGISTER_DRIVER_TYPES_H

void register_core_driver_types();
void unregister_core_driver_types();

void register_driver_types();
void unregister_driver_types();

#endif

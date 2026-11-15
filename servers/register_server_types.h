// This file declares the functions for registering and unregistering server
// types in the engine. It includes functions for preregistering server types,
// registering server types, unregistering server types, and registering server
// singletons.
#ifndef REGISTER_SERVER_TYPES_H
#define REGISTER_SERVER_TYPES_H

void preregister_server_types();
void register_server_types();
void unregister_server_types();

void register_server_singletons();

#endif // REGISTER_SERVER_TYPES_H

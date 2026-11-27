// This file declares the functions for registering and unregistering mbedtls
// module types in the Godot engine. It ensures that the necessary
// initialization and finalization functions for the mbedtls-based classes are
// called when the module is loaded and unloaded, respectively.
#ifndef MBEDTLS_REGISTER_TYPES_H
#define MBEDTLS_REGISTER_TYPES_H

void register_mbedtls_types();
void unregister_mbedtls_types();

#endif // MBEDTLS_REGISTER_TYPES_H

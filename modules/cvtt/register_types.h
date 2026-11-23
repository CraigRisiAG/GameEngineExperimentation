// This file declares the functions for registering and unregistering the CVTT
// module types within the Godot engine. It includes the register_cvtt_types and
// unregister_cvtt_types functions, which are used to integrate the CVTT module
// with the Godot editor.
#ifdef TOOLS_ENABLED

#ifndef CVTT_REGISTER_TYPES_H
#define CVTT_REGISTER_TYPES_H

void register_cvtt_types();
void unregister_cvtt_types();

#endif // CVTT_REGISTER_TYPES_H

#endif // TOOLS_ENABLED

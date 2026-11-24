// This file declares the functions for registering and unregistering the
// VideoDecoder types in Godot. It allows external code to define and interact
// with custom video stream formats via the GDNative API.
#ifndef VIDEODECODER_REGISTER_TYPES_H
#define VIDEODECODER_REGISTER_TYPES_H

void register_videodecoder_types();
void unregister_videodecoder_types();

#endif // VIDEODECODER_REGISTER_TYPES_H

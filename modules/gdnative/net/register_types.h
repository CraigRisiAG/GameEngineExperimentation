// This file declares the functions for registering and unregistering the
// GDNative networking types with the Godot engine. It allows the engine to
// recognize and use the GDNative implementations of MultiplayerPeer,
// PacketPeer, and StreamPeer.
#ifndef NET_REGISTER_TYPES_H
#define NET_REGISTER_TYPES_H

void register_net_types();
void unregister_net_types();

#endif // NET_REGISTER_TYPES_H

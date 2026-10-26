
#include "register_types.h"
#include "multiplayer_peer_gdnative.h"
#include "packet_peer_gdnative.h"
#include "stream_peer_gdnative.h"

void register_net_types() {
  ClassDB::register_class<MultiplayerPeerGDNative>();
  ClassDB::register_class<PacketPeerGDNative>();
  ClassDB::register_class<StreamPeerGDNative>();
}

void unregister_net_types() {}

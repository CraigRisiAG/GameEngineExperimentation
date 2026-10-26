
#include "packet_peer_gdnative.h"

PacketPeerGDNative::PacketPeerGDNative() { interface = NULL; }

PacketPeerGDNative::~PacketPeerGDNative() {}

void PacketPeerGDNative::set_native_packet_peer(
    const godot_net_packet_peer *p_impl) {
  interface = p_impl;
}

void PacketPeerGDNative::_bind_methods() {}

Error PacketPeerGDNative::get_packet(const uint8_t **r_buffer,
                                     int &r_buffer_size) {
  ERR_FAIL_COND_V(interface == NULL, ERR_UNCONFIGURED);
  return (Error)interface->get_packet(interface->data, r_buffer,
                                      &r_buffer_size);
}

Error PacketPeerGDNative::put_packet(const uint8_t *p_buffer,
                                     int p_buffer_size) {
  ERR_FAIL_COND_V(interface == NULL, ERR_UNCONFIGURED);
  return (Error)interface->put_packet(interface->data, p_buffer, p_buffer_size);
}

int PacketPeerGDNative::get_max_packet_size() const {
  ERR_FAIL_COND_V(interface == NULL, 0);
  return interface->get_max_packet_size(interface->data);
}

int PacketPeerGDNative::get_available_packet_count() const {
  ERR_FAIL_COND_V(interface == NULL, 0);
  return interface->get_available_packet_count(interface->data);
}

extern "C" {

void GDAPI godot_net_bind_packet_peer(godot_object *p_obj,
                                      const godot_net_packet_peer *p_impl) {

  ((PacketPeerGDNative *)p_obj)->set_native_packet_peer(p_impl);
}
}

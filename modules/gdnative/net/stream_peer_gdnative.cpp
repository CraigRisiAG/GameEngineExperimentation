
#include "stream_peer_gdnative.h"

StreamPeerGDNative::StreamPeerGDNative() { interface = NULL; }

StreamPeerGDNative::~StreamPeerGDNative() {}

void StreamPeerGDNative::set_native_stream_peer(
    const godot_net_stream_peer *p_interface) {
  interface = p_interface;
}

void StreamPeerGDNative::_bind_methods() {}

Error StreamPeerGDNative::put_data(const uint8_t *p_data, int p_bytes) {
  ERR_FAIL_COND_V(interface == NULL, ERR_UNCONFIGURED);
  return (Error)(interface->put_data(interface->data, p_data, p_bytes));
}

Error StreamPeerGDNative::put_partial_data(const uint8_t *p_data, int p_bytes,
                                           int &r_sent) {
  ERR_FAIL_COND_V(interface == NULL, ERR_UNCONFIGURED);
  return (Error)(interface->put_partial_data(interface->data, p_data, p_bytes,
                                             &r_sent));
}

Error StreamPeerGDNative::get_data(uint8_t *p_buffer, int p_bytes) {
  ERR_FAIL_COND_V(interface == NULL, ERR_UNCONFIGURED);
  return (Error)(interface->get_data(interface->data, p_buffer, p_bytes));
}

Error StreamPeerGDNative::get_partial_data(uint8_t *p_buffer, int p_bytes,
                                           int &r_received) {
  ERR_FAIL_COND_V(interface == NULL, ERR_UNCONFIGURED);
  return (Error)(interface->get_partial_data(interface->data, p_buffer, p_bytes,
                                             &r_received));
}

int StreamPeerGDNative::get_available_bytes() const {
  ERR_FAIL_COND_V(interface == NULL, 0);
  return interface->get_available_bytes(interface->data);
}

extern "C" {

void GDAPI godot_net_bind_stream_peer(
    godot_object *p_obj, const godot_net_stream_peer *p_interface) {
  ((StreamPeerGDNative *)p_obj)->set_native_stream_peer(p_interface);
}
}

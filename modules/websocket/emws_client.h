
#ifndef EMWSCLIENT_H
#define EMWSCLIENT_H

#ifdef JAVASCRIPT_ENABLED

#include "core/error_list.h"
#include "emws_peer.h"
#include "websocket_client.h"

class EMWSClient : public WebSocketClient {

  GDCIIMPL(EMWSClient, WebSocketClient);

private:
  int _in_buf_size;
  int _in_pkt_size;
  int _js_id;

public:
  bool _is_connecting;

  Error set_buffers(int p_in_buffer, int p_in_packets, int p_out_buffer,
                    int p_out_packets);
  Error
  connect_to_host(String p_host, String p_path, uint16_t p_port, bool p_ssl,
                  const Vector<String> p_protocol = Vector<String>(),
                  const Vector<String> p_custom_headers = Vector<String>());
  Ref<WebSocketPeer> get_peer(int p_peer_id) const;
  void disconnect_from_host(int p_code = 1000, String p_reason = "");
  IP_Address get_connected_host() const;
  uint16_t get_connected_port() const;
  virtual ConnectionStatus get_connection_status() const;
  int get_max_packet_size() const;
  virtual void poll();
  EMWSClient();
  ~EMWSClient();
};

#endif // JAVASCRIPT_ENABLED

#endif // EMWSCLIENT_H

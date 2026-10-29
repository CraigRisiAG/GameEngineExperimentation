
#ifndef WEBRTC_PEER_CONNECTION_JS_H
#define WEBRTC_PEER_CONNECTION_JS_H

#ifdef JAVASCRIPT_ENABLED

#include "webrtc_peer_connection.h"

class WebRTCPeerConnectionJS : public WebRTCPeerConnection {

private:
  int _js_id;
  ConnectionState _conn_state;

public:
  static WebRTCPeerConnection *_create() {
    return memnew(WebRTCPeerConnectionJS);
  }
  static void make_default() {
    WebRTCPeerConnection::_create = WebRTCPeerConnectionJS::_create;
  }

  void _on_connection_state_changed();
  virtual ConnectionState get_connection_state() const;

  virtual Error initialize(Dictionary configuration = Dictionary());
  virtual Ref<WebRTCDataChannel>
  create_data_channel(String p_channel_name,
                      Dictionary p_channel_config = Dictionary());
  virtual Error create_offer();
  virtual Error set_remote_description(String type, String sdp);
  virtual Error set_local_description(String type, String sdp);
  virtual Error add_ice_candidate(String sdpMidName, int sdpMlineIndexName,
                                  String sdpName);
  virtual Error poll();
  virtual void close();

  WebRTCPeerConnectionJS();
  ~WebRTCPeerConnectionJS();
};

#endif

#endif // WEBRTC_PEER_CONNECTION_JS_H

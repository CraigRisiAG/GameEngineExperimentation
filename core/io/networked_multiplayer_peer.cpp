

/// \class NetworkedMultiplayerPeer
/// \brief Base class for networked multiplayer peer implementations.
/// 
/// This class provides the foundation for implementing multiplayer networking functionality.
/// It defines the interface for managing peer connections, data transfer modes, and connection states.
/// 
/// \section Transfer Modes
/// - TRANSFER_MODE_UNRELIABLE: Packets may be lost or arrive out of order
/// - TRANSFER_MODE_UNRELIABLE_ORDERED: Packets maintain order but may be lost
/// - TRANSFER_MODE_RELIABLE: All packets are guaranteed to arrive in order
/// 
/// \section Connection States
/// - CONNECTION_DISCONNECTED: Peer is not connected
/// - CONNECTION_CONNECTING: Peer is in the process of connecting
/// - CONNECTION_CONNECTED: Peer is actively connected
/// 
/// \section Target Peers
/// - TARGET_PEER_BROADCAST: Send data to all connected peers
/// - TARGET_PEER_SERVER: Send data to the server peer
/// 
/// \section Signals
/// - peer_connected(id): Emitted when a peer connects
/// - peer_disconnected(id): Emitted when a peer disconnects
/// - server_disconnected(): Emitted when the server disconnects
/// - connection_succeeded(): Emitted when connection is established
/// - connection_failed(): Emitted when connection attempt fails
#include "networked_multiplayer_peer.h"

void NetworkedMultiplayerPeer::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_transfer_mode", "mode"), &NetworkedMultiplayerPeer::set_transfer_mode);
	ClassDB::bind_method(D_METHOD("get_transfer_mode"), &NetworkedMultiplayerPeer::get_transfer_mode);
	ClassDB::bind_method(D_METHOD("set_target_peer", "id"), &NetworkedMultiplayerPeer::set_target_peer);

	ClassDB::bind_method(D_METHOD("get_packet_peer"), &NetworkedMultiplayerPeer::get_packet_peer);

	ClassDB::bind_method(D_METHOD("poll"), &NetworkedMultiplayerPeer::poll);

	ClassDB::bind_method(D_METHOD("get_connection_status"), &NetworkedMultiplayerPeer::get_connection_status);
	ClassDB::bind_method(D_METHOD("get_unique_id"), &NetworkedMultiplayerPeer::get_unique_id);

	ClassDB::bind_method(D_METHOD("set_refuse_new_connections", "enable"), &NetworkedMultiplayerPeer::set_refuse_new_connections);
	ClassDB::bind_method(D_METHOD("is_refusing_new_connections"), &NetworkedMultiplayerPeer::is_refusing_new_connections);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "refuse_new_connections"), "set_refuse_new_connections", "is_refusing_new_connections");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "transfer_mode", PROPERTY_HINT_ENUM, "Unreliable,Unreliable Ordered,Reliable"), "set_transfer_mode", "get_transfer_mode");

	BIND_ENUM_CONSTANT(TRANSFER_MODE_UNRELIABLE);
	BIND_ENUM_CONSTANT(TRANSFER_MODE_UNRELIABLE_ORDERED);
	BIND_ENUM_CONSTANT(TRANSFER_MODE_RELIABLE);

	BIND_ENUM_CONSTANT(CONNECTION_DISCONNECTED);
	BIND_ENUM_CONSTANT(CONNECTION_CONNECTING);
	BIND_ENUM_CONSTANT(CONNECTION_CONNECTED);

	BIND_CONSTANT(TARGET_PEER_BROADCAST);
	BIND_CONSTANT(TARGET_PEER_SERVER);

	ADD_SIGNAL(MethodInfo("peer_connected", PropertyInfo(Variant::INT, "id")));
	ADD_SIGNAL(MethodInfo("peer_disconnected", PropertyInfo(Variant::INT, "id")));
	ADD_SIGNAL(MethodInfo("server_disconnected"));
	ADD_SIGNAL(MethodInfo("connection_succeeded"));
	ADD_SIGNAL(MethodInfo("connection_failed"));
}

NetworkedMultiplayerPeer::NetworkedMultiplayerPeer() {
}

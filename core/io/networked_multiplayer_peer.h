

/**
 * @class NetworkedMultiplayerPeer
 * @brief Abstract base class for networked multiplayer peer communication.
 * 
 * Provides an interface for handling networked multiplayer connections and packet transmission.
 * Extends PacketPeer to support transfer modes, target peers, and connection status management.
 * 
 * @note This is an abstract class and cannot be instantiated directly.
 */

/**
 * @enum TARGET_PEER_BROADCAST
 * @brief Constant indicating packets should be sent to all connected peers.
 */

/**
 * @enum TARGET_PEER_SERVER
 * @brief Constant indicating packets should be sent to the server.
 */

/**
 * @enum TransferMode
 * @brief Specifies the reliability mode for packet transmission.
 * 
 * @var TRANSFER_MODE_UNRELIABLE - Packets may be lost or arrive out of order.
 * @var TRANSFER_MODE_UNRELIABLE_ORDERED - Packets may be lost but maintain order.
 * @var TRANSFER_MODE_RELIABLE - All packets are guaranteed to arrive in order.
 */

/**
 * @enum ConnectionStatus
 * @brief Represents the current connection state of the peer.
 * 
 * @var CONNECTION_DISCONNECTED - Peer is not connected.
 * @var CONNECTION_CONNECTING - Peer is in the process of connecting.
 * @var CONNECTION_CONNECTED - Peer is successfully connected.
 */

/**
 * @fn virtual void set_transfer_mode(TransferMode p_mode)
 * @brief Sets the transfer mode for outgoing packets.
 * @param p_mode The desired transfer mode.
 */

/**
 * @fn virtual TransferMode get_transfer_mode() const
 * @brief Retrieves the current transfer mode.
 * @return The current transfer mode.
 */

/**
 * @fn virtual void set_target_peer(int p_peer_id)
 * @brief Sets the target peer for the next packet to be sent.
 * @param p_peer_id The peer ID or TARGET_PEER_BROADCAST/TARGET_PEER_SERVER constant.
 */

/**
 * @fn virtual int get_packet_peer() const
 * @brief Gets the ID of the peer from which the last received packet originated.
 * @return The peer ID of the packet source.
 */

/**
 * @fn virtual bool is_server() const
 * @brief Checks if this peer is the server.
 * @return True if this peer is the server, false otherwise.
 */

/**
 * @fn virtual void poll()
 * @brief Processes incoming network events and updates connection state.
 */

/**
 * @fn virtual int get_unique_id() const
 * @brief Retrieves the unique identifier for this peer.
 * @return The unique peer ID.
 */

/**
 * @fn virtual void set_refuse_new_connections(bool p_enable)
 * @brief Controls whether this peer accepts new incoming connections.
 * @param p_enable True to refuse new connections, false to accept them.
 */

/**
 * @fn virtual bool is_refusing_new_connections() const
 * @brief Checks if this peer is refusing new connections.
 * @return True if refusing new connections, false otherwise.
 */

/**
 * @fn virtual ConnectionStatus get_connection_status() const
 * @brief Retrieves the current connection status of this peer.
 * @return The current connection status.
 */
#ifndef NETWORKED_MULTIPLAYER_PEER_H
#define NETWORKED_MULTIPLAYER_PEER_H

#include "core/io/packet_peer.h"

class NetworkedMultiplayerPeer : public PacketPeer {

	GDCLASS(NetworkedMultiplayerPeer, PacketPeer);

protected:
	static void _bind_methods();

public:
	enum {
		TARGET_PEER_BROADCAST = 0,
		TARGET_PEER_SERVER = 1
	};
	enum TransferMode {
		TRANSFER_MODE_UNRELIABLE,
		TRANSFER_MODE_UNRELIABLE_ORDERED,
		TRANSFER_MODE_RELIABLE,
	};

	enum ConnectionStatus {
		CONNECTION_DISCONNECTED,
		CONNECTION_CONNECTING,
		CONNECTION_CONNECTED,
	};

	virtual void set_transfer_mode(TransferMode p_mode) = 0;
	virtual TransferMode get_transfer_mode() const = 0;
	virtual void set_target_peer(int p_peer_id) = 0;

	virtual int get_packet_peer() const = 0;

	virtual bool is_server() const = 0;

	virtual void poll() = 0;

	virtual int get_unique_id() const = 0;

	virtual void set_refuse_new_connections(bool p_enable) = 0;
	virtual bool is_refusing_new_connections() const = 0;

	virtual ConnectionStatus get_connection_status() const = 0;

	NetworkedMultiplayerPeer();
};

VARIANT_ENUM_CAST(NetworkedMultiplayerPeer::TransferMode)
VARIANT_ENUM_CAST(NetworkedMultiplayerPeer::ConnectionStatus)

#endif // NETWORKED_MULTIPLAYER_PEER_H

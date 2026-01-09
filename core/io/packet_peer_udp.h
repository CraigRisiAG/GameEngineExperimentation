

/**
 * @class PacketPeerUDP
 * @brief UDP packet communication peer for network operations.
 * 
 * Provides UDP socket functionality for sending and receiving packets over a network.
 * Supports both connected and connectionless modes, multicast groups, and broadcast messaging.
 * Inherits from PacketPeer to implement packet-based network communication.
 * 
 * @details
 * - Maintains internal ring buffer for packet queueing
 * - Supports blocking and non-blocking modes
 * - Can operate as a UDP server by accepting socket connections
 * - Handles multicast group join/leave operations
 * - Maximum packet size is 65536 bytes
 * 
 * @note Uses GDCLASS for Godot engine integration.
 */
#ifndef PACKET_PEER_UDP_H
#define PACKET_PEER_UDP_H

#include "core/io/ip.h"
#include "core/io/net_socket.h"
#include "core/io/packet_peer.h"

class PacketPeerUDP : public PacketPeer {
	GDCLASS(PacketPeerUDP, PacketPeer);

protected:
	enum {
		PACKET_BUFFER_SIZE = 65536
	};

	RingBuffer<uint8_t> rb;
	uint8_t recv_buffer[PACKET_BUFFER_SIZE];
	uint8_t packet_buffer[PACKET_BUFFER_SIZE];
	IP_Address packet_ip;
	int packet_port;
	int queue_count;

	IP_Address peer_addr;
	int peer_port;
	bool connected;
	bool blocking;
	bool broadcast;
	Ref<NetSocket> _sock;

	static void _bind_methods();

	String _get_packet_ip() const;

	Error _set_dest_address(const String &p_address, int p_port);
	Error _poll();

public:
	void set_blocking_mode(bool p_enable);

	Error listen(int p_port, const IP_Address &p_bind_address = IP_Address("*"), int p_recv_buffer_size = 65536);
	void close();
	Error wait();
	bool is_listening() const;

	Error connect_socket(Ref<NetSocket> p_sock); // Used by UDPServer
	Error connect_to_host(const IP_Address &p_host, int p_port);
	bool is_connected_to_host() const;

	IP_Address get_packet_address() const;
	int get_packet_port() const;
	void set_dest_address(const IP_Address &p_address, int p_port);

	Error put_packet(const uint8_t *p_buffer, int p_buffer_size);
	Error get_packet(const uint8_t **r_buffer, int &r_buffer_size);
	int get_available_packet_count() const;
	int get_max_packet_size() const;
	void set_broadcast_enabled(bool p_enabled);
	Error join_multicast_group(IP_Address p_multi_address, String p_if_name);
	Error leave_multicast_group(IP_Address p_multi_address, String p_if_name);

	PacketPeerUDP();
	~PacketPeerUDP();
};

#endif // PACKET_PEER_UDP_H

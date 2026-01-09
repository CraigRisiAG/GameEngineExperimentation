

/**
 * @class UDPServer
 * @brief A UDP server implementation for handling incoming UDP connections.
 * 
 * This class manages a UDP server socket that listens for incoming UDP packets
 * and allows accepting connections as PacketPeerUDP instances.
 * 
 * @note Inherits from Reference for automatic memory management.
 */

/// @brief Starts listening for UDP connections on the specified port and address.
/// @param p_port The port number to listen on (0-65535).
/// @param p_bind_address The IP address to bind to. Defaults to "*" (all interfaces).
/// @return Error code indicating success or failure of the operation.
Error listen(uint16_t p_port, const IP_Address &p_bind_address = IP_Address("*"));

/// @brief Checks if the server is currently listening for connections.
/// @return True if the server is actively listening, false otherwise.
bool is_listening() const;

/// @brief Checks if a new UDP connection is available to be accepted.
/// @return True if at least one connection is waiting to be accepted, false otherwise.
bool is_connection_available() const;

/// @brief Accepts and returns the next available UDP connection.
/// @return A Ref<PacketPeerUDP> representing the accepted connection, or null if none available.
/// @note Must call is_connection_available() first to ensure a connection is waiting.
Ref<PacketPeerUDP> take_connection();

/// @brief Stops the server and closes the listening socket.
void stop();
#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "core/io/net_socket.h"
#include "core/io/packet_peer_udp.h"

class UDPServer : public Reference {
	GDCLASS(UDPServer, Reference);

protected:
	static void _bind_methods();
	int bind_port;
	IP_Address bind_address;
	Ref<NetSocket> _sock;

public:
	Error listen(uint16_t p_port, const IP_Address &p_bind_address = IP_Address("*"));
	bool is_listening() const;
	bool is_connection_available() const;
	Ref<PacketPeerUDP> take_connection();

	void stop();

	UDPServer();
	~UDPServer();
};

#endif // UDP_SERVER_H

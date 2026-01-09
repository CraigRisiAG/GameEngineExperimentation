

/**
 * @class TCP_Server
 * @brief A TCP server implementation for accepting incoming client connections.
 * 
 * TCP_Server manages a listening socket and handles incoming TCP connections.
 * It allows applications to listen on a specified port and accept connections
 * from remote clients.
 * 
 * @details
 * The server maintains a queue of pending connections (up to MAX_PENDING_CONNECTIONS).
 * Clients can poll for available connections and accept them as StreamPeerTCP objects
 * for communication.
 */

/**
 * @brief Maximum number of pending connections that can be queued.
 */

/**
 * @fn Error listen(uint16_t p_port, const IP_Address &p_bind_address = IP_Address("*"))
 * @brief Starts listening for incoming connections on the specified port.
 * @param p_port The port number to listen on (0-65535).
 * @param p_bind_address The IP address to bind to. Defaults to "*" (all interfaces).
 * @return Error code indicating success or failure of the operation.
 */

/**
 * @fn bool is_listening() const
 * @brief Checks if the server is currently listening for connections.
 * @return true if the server is actively listening, false otherwise.
 */

/**
 * @fn bool is_connection_available() const
 * @brief Checks if there are pending connections waiting to be accepted.
 * @return true if at least one connection is available, false otherwise.
 */

/**
 * @fn Ref<StreamPeerTCP> take_connection()
 * @brief Accepts and returns the next pending connection.
 * @return A reference to a StreamPeerTCP object for the accepted connection,
 *         or null if no connections are available.
 */

/**
 * @fn void stop()
 * @brief Stops the server from listening for new connections.
 */

/**
 * @fn TCP_Server()
 * @brief Constructs a new TCP_Server instance.
 */

/**
 * @fn ~TCP_Server()
 * @brief Destructs the TCP_Server and releases associated resources.
 */
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "core/io/ip.h"
#include "core/io/net_socket.h"
#include "core/io/stream_peer.h"
#include "core/io/stream_peer_tcp.h"

class TCP_Server : public Reference {

	GDCLASS(TCP_Server, Reference);

protected:
	enum {
		MAX_PENDING_CONNECTIONS = 8
	};

	Ref<NetSocket> _sock;
	static void _bind_methods();

public:
	Error listen(uint16_t p_port, const IP_Address &p_bind_address = IP_Address("*"));
	bool is_listening() const;
	bool is_connection_available() const;
	Ref<StreamPeerTCP> take_connection();

	void stop(); // Stop listening

	TCP_Server();
	~TCP_Server();
};

#endif // TCP_SERVER_H

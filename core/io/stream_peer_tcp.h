

/// @class StreamPeerTCP
/// @brief TCP stream peer for network communication.
/// 
/// StreamPeerTCP provides TCP socket functionality for reading and writing data over a network connection.
/// It inherits from StreamPeer and manages the lifecycle of a TCP socket connection, including connection
/// establishment, status tracking, and data transmission.
/// 
/// @note This class is part of the core networking I/O subsystem.
/// 
/// @enum Status
/// @brief Connection status enumeration.
/// @var STATUS_NONE - No connection established.
/// @var STATUS_CONNECTING - Connection in progress.
/// @var STATUS_CONNECTED - Successfully connected.
/// @var STATUS_ERROR - Connection error occurred.

/// @brief Accepts an incoming socket connection.
/// @param p_sock Reference to the NetSocket to accept.
/// @param p_host The IP address of the peer.
/// @param p_port The port number of the peer.

/// @brief Initiates a TCP connection to a remote host.
/// @param p_host The IP address of the remote host.
/// @param p_port The port number to connect to.
/// @return Error code indicating success or failure.

/// @brief Checks if currently connected to a host.
/// @return true if connected, false otherwise.

/// @brief Retrieves the IP address of the connected host.
/// @return IP_Address of the peer host.

/// @brief Retrieves the port number of the connected host.
/// @return uint16_t port number.

/// @brief Disconnects from the currently connected host.

/// @brief Gets the number of available bytes to read.
/// @return Number of bytes available in the receive buffer.

/// @brief Retrieves the current connection status.
/// @return Status enumeration value.

/// @brief Enables or disables the TCP_NODELAY option.
/// @param p_enabled true to disable Nagle's algorithm, false to enable it.

/// @brief Polls the socket for readiness.
/// @param p_type The type of poll operation (read, write, or error).
/// @param timeout Timeout in milliseconds (0 = non-blocking).
/// @return Error code indicating the result of the poll operation.

/// @brief Writes data to the socket (blocking).
/// @param p_data Pointer to the data buffer.
/// @param p_bytes Number of bytes to send.
/// @return Error code.

/// @brief Writes data to the socket with partial send support.
/// @param p_data Pointer to the data buffer.
/// @param p_bytes Number of bytes to send.
/// @param r_sent Reference to store the actual number of bytes sent.
/// @return Error code.

/// @brief Reads data from the socket (blocking).
/// @param p_buffer Pointer to the buffer for received data.
/// @param p_bytes Number of bytes to read.
/// @return Error code.

/// @brief Reads data from the socket with partial receive support.
/// @param p_buffer Pointer to the buffer for received data.
/// @param p_bytes Number of bytes to read.
/// @param r_received Reference to store the actual number of bytes received.
/// @return Error code.
#ifndef STREAM_PEER_TCP_H
#define STREAM_PEER_TCP_H

#include "core/io/ip.h"
#include "core/io/ip_address.h"
#include "core/io/net_socket.h"
#include "core/io/stream_peer.h"

class StreamPeerTCP : public StreamPeer {

	GDCLASS(StreamPeerTCP, StreamPeer);
	OBJ_CATEGORY("Networking");

public:
	enum Status {

		STATUS_NONE,
		STATUS_CONNECTING,
		STATUS_CONNECTED,
		STATUS_ERROR,
	};

protected:
	Ref<NetSocket> _sock;
	uint64_t timeout;
	Status status;
	IP_Address peer_host;
	uint16_t peer_port;

	Error _connect(const String &p_address, int p_port);
	Error _poll_connection();
	Error write(const uint8_t *p_data, int p_bytes, int &r_sent, bool p_block);
	Error read(uint8_t *p_buffer, int p_bytes, int &r_received, bool p_block);

	static void _bind_methods();

public:
	void accept_socket(Ref<NetSocket> p_sock, IP_Address p_host, uint16_t p_port);

	Error connect_to_host(const IP_Address &p_host, uint16_t p_port);
	bool is_connected_to_host() const;
	IP_Address get_connected_host() const;
	uint16_t get_connected_port() const;
	void disconnect_from_host();

	int get_available_bytes() const;
	Status get_status();

	void set_no_delay(bool p_enabled);

	// Poll functions (wait or check for writable, readable)
	Error poll(NetSocket::PollType p_type, int timeout = 0);

	// Read/Write from StreamPeer
	Error put_data(const uint8_t *p_data, int p_bytes);
	Error put_partial_data(const uint8_t *p_data, int p_bytes, int &r_sent);
	Error get_data(uint8_t *p_buffer, int p_bytes);
	Error get_partial_data(uint8_t *p_buffer, int p_bytes, int &r_received);

	StreamPeerTCP();
	~StreamPeerTCP();
};

VARIANT_ENUM_CAST(StreamPeerTCP::Status);

#endif // STREAM_PEER_TCP_H

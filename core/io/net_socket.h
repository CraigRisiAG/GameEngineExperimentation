

/**
 * @class NetSocket
 * @brief Abstract base class for network socket operations.
 * 
 * NetSocket provides a platform-independent interface for TCP and UDP socket communication.
 * It supports both IPv4 and IPv6 protocols and includes advanced features like multicast,
 * non-blocking I/O, and various socket options configuration.
 * 
 * @note This is an abstract class. Use create() to instantiate platform-specific implementations.
 */

/**
 * @enum PollType
 * @brief Specifies the type of I/O event to monitor.
 * @var POLL_TYPE_IN Monitor for incoming data availability
 * @var POLL_TYPE_OUT Monitor for write readiness
 * @var POLL_TYPE_IN_OUT Monitor for both input and output events
 */

/**
 * @enum Type
 * @brief Socket protocol type.
 * @var TYPE_NONE No socket type specified
 * @var TYPE_TCP Transmission Control Protocol (connection-oriented)
 * @var TYPE_UDP User Datagram Protocol (connectionless)
 */

/**
 * @fn static NetSocket *create()
 * @brief Factory method to create a platform-specific socket instance.
 * @return Pointer to a new NetSocket instance, or nullptr on failure.
 */

/**
 * @fn virtual Error open(Type p_type, IP::Type &ip_type) = 0
 * @brief Opens a socket with the specified type and IP version.
 * @param p_type The protocol type (TCP or UDP)
 * @param ip_type The IP version (IPv4 or IPv6)
 * @return Error code indicating success or failure
 */

/**
 * @fn virtual void close() = 0
 * @brief Closes the socket and releases associated resources.
 */

/**
 * @fn virtual Error bind(IP_Address p_addr, uint16_t p_port) = 0
 * @brief Binds the socket to a specific address and port.
 * @param p_addr The IP address to bind to
 * @param p_port The port number to bind to
 * @return Error code indicating success or failure
 */

/**
 * @fn virtual Error listen(int p_max_pending) = 0
 * @brief Puts the socket in listening mode for incoming connections.
 * @param p_max_pending Maximum number of pending connections to queue
 * @return Error code indicating success or failure
 * @note Only applicable for TCP sockets
 */

/**
 * @fn virtual Error connect_to_host(IP_Address p_addr, uint16_t p_port) = 0
 * @brief Establishes a connection to a remote host.
 * @param p_addr The IP address of the remote host
 * @param p_port The port number of the remote host
 * @return Error code indicating success or failure
 */

/**
 * @fn virtual Error poll(PollType p_type, int timeout) const = 0
 * @brief Polls the socket for I/O readiness with a timeout.
 * @param p_type The type of event to monitor
 * @param timeout Timeout in milliseconds; use 0 for non-blocking check
 * @return Error code indicating socket readiness or timeout
 */

/**
 * @fn virtual Error recv(uint8_t *p_buffer, int p_len, int &r_read) = 0
 * @brief Receives data from a connected socket.
 * @param p_buffer Buffer to store received data
 * @param p_len Maximum number of bytes to receive
 * @param r_read Output parameter: number of bytes actually received
 * @return Error code indicating success or failure
 */

/**
 * @fn virtual Error recvfrom(uint8_t *p_buffer, int p_len, int &r_read, IP_Address &r_ip, uint16_t &r_port, bool p_peek) = 0
 * @brief Receives data from any remote address (connectionless).
 * @param p_buffer Buffer to store received data
 * @param p_len Maximum number of bytes to receive
 * @param r_read Output parameter: number of bytes actually received
 * @param r_ip Output parameter: source IP address
 * @param r_port Output parameter: source port number
 * @param p_peek If true, peek at data without removing it from buffer (default: false)
 * @return Error code indicating success or failure
 * @note Primarily used for UDP sockets
 */

/**
 * @fn virtual Error send(const uint8_t *p_buffer, int p_len, int &r_sent) = 0
 * @brief Sends data through a connected socket.
 * @param p_buffer Data to send
 * @param p_len Number of bytes to send
 * @param r_sent Output parameter: number of bytes actually sent
 * @return Error code indicating success or failure
 */

/**
 * @fn virtual Error sendto(const uint8_t *p_buffer, int p_len, int &r_sent, IP_Address p_ip, uint16_t p_port) = 0
 * @brief Sends data to a specific remote address (connectionless).
 * @param p_buffer Data to send
 * @param p_len Number of bytes to send
 * @param r_sent Output parameter: number of bytes actually sent
 * @param p_ip Destination IP address
 * @param p_port Destination port number
 * @return Error code indicating success or failure
 * @note Primarily used for UDP sockets
 */

/**
 * @fn virtual Ref<NetSocket> accept(IP_Address &r_ip, uint16_t &r_port) = 0
 * @brief Accepts an incoming connection and creates a new socket for it.
 * @param r_ip Output parameter: IP address of the connecting client
 * @param r_port Output parameter: port number of the connecting client
 * @return A new NetSocket for the accepted connection, or nullptr if no connection available
 * @note Only applicable for TCP sockets in listening mode
 */

/**
 * @fn virtual bool is_open() const = 0
 * @brief Checks if the socket is currently open.
 * @return true if socket is open, false otherwise
 */

/**
 * @fn virtual int get_available_bytes() const = 0
 * @brief Returns the number of bytes available to read without blocking.
 * @return Number of bytes available to read
 */

/**
 * @fn virtual Error set_broadcasting_enabled(bool p_enabled) = 0
 * @brief Enables or disables broadcasting for UDP sockets.
 * @param p_enabled true to enable, false to disable
 * @return Error code; returns OK if the socket option was set successfully
 * @note Only applicable for UDP sockets
 */

/**
 * @fn virtual void set_blocking_enabled(bool p_enabled) = 0
 * @brief Sets the socket to blocking or non-blocking mode.
 * @param p_enabled true for blocking mode, false for non-blocking mode
 */

/**
 * @fn virtual void set_ipv6_only_enabled(bool p_enabled) = 0
 * @brief Restricts an IPv6 socket to IPv6 communication only.
 * @param p_enabled true to restrict to IPv6 only, false to allow IPv4-mapped IPv6 addresses
 * @note Only applicable for IPv6 sockets
 */

/**
 * @fn virtual void set_tcp_no_delay_enabled(bool p_enabled) = 0
 * @brief Enables or disables the TCP_NODELAY option (Nagle's algorithm).
 * @param p_enabled true to disable Nagle's algorithm (send immediately), false to enable it
 * @note Only applicable for TCP sockets
 */

/**
 * @fn virtual void set_reuse_address_enabled(bool p_enabled) = 0
 * @brief Enables or disables address reuse (SO_REUSEADDR).
 * @param p_enabled true to enable reuse, false to disable
 * @note Useful when restarting a server to quickly rebind to the same port
 */

/**
 * @fn virtual Error join_multicast_group(const IP_Address &p_multi_address, String p_if_name) = 0
 * @brief Joins a multicast group on a specific network interface.
 * @param p_multi_address The multicast group address to join
 * @param p_if_name The network interface name (empty string for default interface)
 * @return Error code indicating success or failure
 * @note Only applicable for UDP sockets
 */

/**
 * @fn virtual Error leave_multicast_group(const IP_Address &p_multi_address, String p_if_name) = 0
 * @brief Leaves a previously joined multicast group.
 * @param p_multi_address The multicast group address to leave
 * @param p_if_name The network interface name (empty string for default interface)
 * @return Error code indicating success or failure
 * @note Only applicable for UDP sockets
 */
#ifndef NET_SOCKET_H
#define NET_SOCKET_H

#include "core/io/ip.h"
#include "core/reference.h"

class NetSocket : public Reference {

protected:
	static NetSocket *(*_create)();

public:
	static NetSocket *create();

	enum PollType {
		POLL_TYPE_IN,
		POLL_TYPE_OUT,
		POLL_TYPE_IN_OUT
	};

	enum Type {
		TYPE_NONE,
		TYPE_TCP,
		TYPE_UDP,
	};

	virtual Error open(Type p_type, IP::Type &ip_type) = 0;
	virtual void close() = 0;
	virtual Error bind(IP_Address p_addr, uint16_t p_port) = 0;
	virtual Error listen(int p_max_pending) = 0;
	virtual Error connect_to_host(IP_Address p_addr, uint16_t p_port) = 0;
	virtual Error poll(PollType p_type, int timeout) const = 0;
	virtual Error recv(uint8_t *p_buffer, int p_len, int &r_read) = 0;
	virtual Error recvfrom(uint8_t *p_buffer, int p_len, int &r_read, IP_Address &r_ip, uint16_t &r_port, bool p_peek = false) = 0;
	virtual Error send(const uint8_t *p_buffer, int p_len, int &r_sent) = 0;
	virtual Error sendto(const uint8_t *p_buffer, int p_len, int &r_sent, IP_Address p_ip, uint16_t p_port) = 0;
	virtual Ref<NetSocket> accept(IP_Address &r_ip, uint16_t &r_port) = 0;

	virtual bool is_open() const = 0;
	virtual int get_available_bytes() const = 0;

	virtual Error set_broadcasting_enabled(bool p_enabled) = 0; // Returns OK if the socket option has been set successfully.
	virtual void set_blocking_enabled(bool p_enabled) = 0;
	virtual void set_ipv6_only_enabled(bool p_enabled) = 0;
	virtual void set_tcp_no_delay_enabled(bool p_enabled) = 0;
	virtual void set_reuse_address_enabled(bool p_enabled) = 0;
	virtual Error join_multicast_group(const IP_Address &p_multi_address, String p_if_name) = 0;
	virtual Error leave_multicast_group(const IP_Address &p_multi_address, String p_if_name) = 0;
};

#endif // NET_SOCKET_H

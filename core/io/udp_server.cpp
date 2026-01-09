

/// @class UDPServer
/// @brief A UDP server implementation for handling incoming UDP connections.
/// 
/// UDPServer provides functionality to listen on a UDP port and accept incoming
/// UDP connections. It manages a single NetSocket for receiving connections and
/// creates new PacketPeerUDP instances for each accepted connection.
/// 
/// @note This class is designed for single-threaded, non-blocking UDP operations.

/// @brief Binds Godot script methods to the C++ class methods.
/// 
/// Exposes the following methods to GDScript:
/// - listen(port: int, bind_address: String = "*") -> Error
/// - is_connection_available() -> bool
/// - is_listening() -> bool
/// - take_connection() -> PacketPeerUDP
/// - stop() -> void
void UDPServer::_bind_methods();

/// @brief Starts listening for incoming UDP connections on the specified port and address.
/// 
/// @param p_port The UDP port number to listen on (1-65535).
/// @param p_bind_address The IP address to bind to. Use "*" or an empty address to bind to all interfaces.
/// @return Error code: OK if successful, ERR_UNAVAILABLE if socket is invalid, 
///         ERR_ALREADY_IN_USE if already listening, ERR_INVALID_PARAMETER if address is invalid,
///         ERR_CANT_CREATE if socket creation fails.
Error UDPServer::listen(uint16_t p_port, const IP_Address &p_bind_address);

/// @brief Checks if the server is currently listening for connections.
/// 
/// @return true if the socket is open and listening, false otherwise.
bool UDPServer::is_listening() const;

/// @brief Checks if an incoming UDP connection is available to be accepted.
/// 
/// Uses non-blocking socket polling to check for incoming data without waiting.
/// 
/// @return true if a connection is available, false otherwise.
bool UDPServer::is_connection_available() const;

/// @brief Accepts and returns the next available UDP connection.
/// 
/// Creates a new PacketPeerUDP instance connected to the incoming socket,
/// then creates a new listening socket to replace the current one.
/// 
/// @return A new PacketPeerUDP reference if a connection is available, 
///         or a null reference if no connection is pending.
Ref<PacketPeerUDP> UDPServer::take_connection();

/// @brief Stops the UDP server and closes the listening socket.
/// 
/// Cleans up resources and resets the bind port and address.
void UDPServer::stop();

/// @brief Constructs a UDPServer instance with a new NetSocket.
UDPServer::UDPServer();

/// @brief Destructs the UDPServer instance, closing any open connections.
UDPServer::~UDPServer();
#include "udp_server.h"

void UDPServer::_bind_methods() {

	ClassDB::bind_method(D_METHOD("listen", "port", "bind_address"), &UDPServer::listen, DEFVAL("*"));
	ClassDB::bind_method(D_METHOD("is_connection_available"), &UDPServer::is_connection_available);
	ClassDB::bind_method(D_METHOD("is_listening"), &UDPServer::is_listening);
	ClassDB::bind_method(D_METHOD("take_connection"), &UDPServer::take_connection);
	ClassDB::bind_method(D_METHOD("stop"), &UDPServer::stop);
}

Error UDPServer::listen(uint16_t p_port, const IP_Address &p_bind_address) {

	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);
	ERR_FAIL_COND_V(_sock->is_open(), ERR_ALREADY_IN_USE);
	ERR_FAIL_COND_V(!p_bind_address.is_valid() && !p_bind_address.is_wildcard(), ERR_INVALID_PARAMETER);

	Error err;
	IP::Type ip_type = IP::TYPE_ANY;

	if (p_bind_address.is_valid())
		ip_type = p_bind_address.is_ipv4() ? IP::TYPE_IPV4 : IP::TYPE_IPV6;

	err = _sock->open(NetSocket::TYPE_UDP, ip_type);

	if (err != OK)
		return ERR_CANT_CREATE;

	_sock->set_blocking_enabled(false);
	_sock->set_reuse_address_enabled(true);
	err = _sock->bind(p_bind_address, p_port);

	if (err != OK) {
		stop();
		return err;
	}
	bind_address = p_bind_address;
	bind_port = p_port;
	return OK;
}

bool UDPServer::is_listening() const {
	ERR_FAIL_COND_V(!_sock.is_valid(), false);

	return _sock->is_open();
}

bool UDPServer::is_connection_available() const {

	ERR_FAIL_COND_V(!_sock.is_valid(), false);

	if (!_sock->is_open())
		return false;

	Error err = _sock->poll(NetSocket::POLL_TYPE_IN, 0);
	return (err == OK);
}

Ref<PacketPeerUDP> UDPServer::take_connection() {

	Ref<PacketPeerUDP> conn;
	if (!is_connection_available()) {
		return conn;
	}

	conn = Ref<PacketPeerUDP>(memnew(PacketPeerUDP));
	conn->connect_socket(_sock);
	_sock = Ref<NetSocket>(NetSocket::create());
	listen(bind_port, bind_address);
	return conn;
}

void UDPServer::stop() {

	if (_sock.is_valid()) {
		_sock->close();
	}
	bind_port = 0;
	bind_address = IP_Address();
}

UDPServer::UDPServer() :
		_sock(Ref<NetSocket>(NetSocket::create())) {
}

UDPServer::~UDPServer() {

	stop();
}

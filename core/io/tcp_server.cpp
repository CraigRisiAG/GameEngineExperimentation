

/// @class TCP_Server
/// @brief A TCP server implementation for handling incoming socket connections.
///
/// TCP_Server provides functionality to listen for incoming TCP connections on a specified port
/// and bind address. It manages a non-blocking socket and allows accepting individual client
/// connections as StreamPeerTCP objects.
///
/// @method listen(uint16_t p_port, const IP_Address &p_bind_address) -> Error
/// @brief Start listening for incoming TCP connections.
/// @param p_port The port number to listen on.
/// @param p_bind_address The IP address to bind to (use "*" for all interfaces).
/// @return OK on success, ERR_UNAVAILABLE if socket is invalid, ERR_ALREADY_IN_USE if already listening,
///         ERR_INVALID_PARAMETER if bind address is invalid, ERR_CANT_CREATE if socket creation fails.
///
/// @method is_listening() const -> bool
/// @brief Check if the server is currently listening for connections.
/// @return true if the server socket is open and listening, false otherwise.
///
/// @method is_connection_available() const -> bool
/// @brief Check if there is an incoming connection waiting to be accepted.
/// @return true if a connection is available, false otherwise.
///
/// @method take_connection() -> Ref<StreamPeerTCP>
/// @brief Accept and retrieve the next pending incoming connection.
/// @return A StreamPeerTCP object representing the accepted connection, or null if no connection is available.
///
/// @method stop() -> void
/// @brief Stop listening and close the server socket.
#include "tcp_server.h"

void TCP_Server::_bind_methods() {

	ClassDB::bind_method(D_METHOD("listen", "port", "bind_address"), &TCP_Server::listen, DEFVAL("*"));
	ClassDB::bind_method(D_METHOD("is_connection_available"), &TCP_Server::is_connection_available);
	ClassDB::bind_method(D_METHOD("is_listening"), &TCP_Server::is_listening);
	ClassDB::bind_method(D_METHOD("take_connection"), &TCP_Server::take_connection);
	ClassDB::bind_method(D_METHOD("stop"), &TCP_Server::stop);
}

Error TCP_Server::listen(uint16_t p_port, const IP_Address &p_bind_address) {

	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);
	ERR_FAIL_COND_V(_sock->is_open(), ERR_ALREADY_IN_USE);
	ERR_FAIL_COND_V(!p_bind_address.is_valid() && !p_bind_address.is_wildcard(), ERR_INVALID_PARAMETER);

	Error err;
	IP::Type ip_type = IP::TYPE_ANY;

	// If the bind address is valid use its type as the socket type
	if (p_bind_address.is_valid())
		ip_type = p_bind_address.is_ipv4() ? IP::TYPE_IPV4 : IP::TYPE_IPV6;

	err = _sock->open(NetSocket::TYPE_TCP, ip_type);

	ERR_FAIL_COND_V(err != OK, ERR_CANT_CREATE);

	_sock->set_blocking_enabled(false);
	_sock->set_reuse_address_enabled(true);

	err = _sock->bind(p_bind_address, p_port);

	if (err != OK) {

		_sock->close();
		return ERR_ALREADY_IN_USE;
	}

	err = _sock->listen(MAX_PENDING_CONNECTIONS);

	if (err != OK) {
		_sock->close();
		return FAILED;
	}
	return OK;
}

bool TCP_Server::is_listening() const {
	ERR_FAIL_COND_V(!_sock.is_valid(), false);

	return _sock->is_open();
}

bool TCP_Server::is_connection_available() const {

	ERR_FAIL_COND_V(!_sock.is_valid(), false);

	if (!_sock->is_open())
		return false;

	Error err = _sock->poll(NetSocket::POLL_TYPE_IN, 0);
	return (err == OK);
}

Ref<StreamPeerTCP> TCP_Server::take_connection() {

	Ref<StreamPeerTCP> conn;
	if (!is_connection_available()) {
		return conn;
	}

	Ref<NetSocket> ns;
	IP_Address ip;
	uint16_t port = 0;
	ns = _sock->accept(ip, port);
	if (!ns.is_valid())
		return conn;

	conn = Ref<StreamPeerTCP>(memnew(StreamPeerTCP));
	conn->accept_socket(ns, ip, port);
	return conn;
}

void TCP_Server::stop() {

	if (_sock.is_valid()) {
		_sock->close();
	}
}

TCP_Server::TCP_Server() :
		_sock(Ref<NetSocket>(NetSocket::create())) {
}

TCP_Server::~TCP_Server() {

	stop();
}

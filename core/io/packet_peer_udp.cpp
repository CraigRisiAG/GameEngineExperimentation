

/**
 * @class PacketPeerUDP
 * @brief UDP packet peer implementation for network communication.
 * 
 * Provides functionality for sending and receiving UDP packets, including support for
 * multicast groups, broadcast, and both connected and connectionless modes.
 * 
 * @details
 * This class wraps a NetSocket for UDP operations and manages:
 * - Listening on a specified port and address
 * - Connecting to remote hosts
 * - Sending and receiving packets with address/port information
 * - Multicast group management (join/leave)
 * - Broadcast mode control
 * - Non-blocking I/O with internal packet queueing
 * 
 * @note
 * - Uses an internal ring buffer (rb) to queue received packets
 * - Supports both IPv4 and IPv6 addresses
 * - Can operate in connected or connectionless mode
 * - Non-blocking socket operations by default
 * 
 * @see NetSocket, IP_Address, PacketPeer
 */

/// @brief Enables or disables blocking mode for socket operations.
/// @param p_enable True to enable blocking mode, false for non-blocking.
void set_blocking_mode(bool p_enable);

/// @brief Sets broadcast mode for the UDP socket.
/// @param p_enabled True to enable broadcasting, false to disable.
void set_broadcast_enabled(bool p_enabled);

/// @brief Joins a multicast group on the specified interface.
/// @param p_multi_address The multicast address to join.
/// @param p_if_name The network interface name (empty string for default).
/// @return OK on success, error code otherwise.
Error join_multicast_group(IP_Address p_multi_address, String p_if_name);

/// @brief Leaves a previously joined multicast group.
/// @param p_multi_address The multicast address to leave.
/// @param p_if_name The network interface name.
/// @return OK on success, error code otherwise.
Error leave_multicast_group(IP_Address p_multi_address, String p_if_name);

/// @brief Starts listening for incoming UDP packets on a specified port.
/// @param p_port The port number to listen on.
/// @param p_bind_address The address to bind to ("*" for wildcard).
/// @param p_recv_buffer_size The size of the receive buffer (default 65536 bytes).
/// @return OK on success, error code otherwise.
Error listen(int p_port, const IP_Address &p_bind_address, int p_recv_buffer_size);

/// @brief Connects to a remote UDP host.
/// @param p_host The IP address of the remote host.
/// @param p_port The port number of the remote host.
/// @return OK on success, error code otherwise.
Error connect_to_host(const IP_Address &p_host, int p_port);

/// @brief Connects using an existing socket and receives initial packet.
/// @param p_sock The NetSocket reference to use.
/// @return OK on success, error code otherwise.
Error connect_socket(Ref<NetSocket> p_sock);

/// @brief Checks if the socket is connected to a host.
/// @return True if connected, false otherwise.
bool is_connected_to_host() const;

/// @brief Closes the UDP socket and clears buffers.
void close();

/// @brief Waits for incoming data on the socket.
/// @return OK on success, error code otherwise.
Error wait();

/// @brief Retrieves the next available packet from the queue.
/// @param r_buffer Output pointer to the packet data.
/// @param r_buffer_size Output variable containing packet size in bytes.
/// @return OK on success, error code otherwise.
Error get_packet(const uint8_t **r_buffer, int &r_buffer_size);

/// @brief Sends a packet to the configured destination address.
/// @param p_buffer Pointer to the packet data to send.
/// @param p_buffer_size Size of the packet in bytes.
/// @return OK on success, error code otherwise.
Error put_packet(const uint8_t *p_buffer, int p_buffer_size);

/// @brief Gets the number of available packets in the queue.
/// @return Number of queued packets, or -1 on error.
int get_available_packet_count() const;

/// @brief Gets the maximum packet size supported.
/// @return Maximum packet size in bytes.
int get_max_packet_size() const;

/// @brief Gets the IP address of the last received packet.
/// @return IP address of the packet source.
IP_Address get_packet_address() const;

/// @brief Gets the port number of the last received packet.
/// @return Port number of the packet source.
int get_packet_port() const;

/// @brief Checks if the socket is actively listening.
/// @return True if listening, false otherwise.
bool is_listening() const;

/// @brief Sets the destination address for sending packets (connectionless mode).
/// @param p_address The destination IP address.
/// @param p_port The destination port number.
/// @note Cannot be used for connected sockets.
void set_dest_address(const IP_Address &p_address, int p_port);
#include "packet_peer_udp.h"

#include "core/io/ip.h"

void PacketPeerUDP::set_blocking_mode(bool p_enable) {

	blocking = p_enable;
}

void PacketPeerUDP::set_broadcast_enabled(bool p_enabled) {
	broadcast = p_enabled;
	if (_sock.is_valid() && _sock->is_open())
		_sock->set_broadcasting_enabled(p_enabled);
}

Error PacketPeerUDP::join_multicast_group(IP_Address p_multi_address, String p_if_name) {

	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);
	ERR_FAIL_COND_V(!p_multi_address.is_valid(), ERR_INVALID_PARAMETER);

	if (!_sock->is_open()) {
		IP::Type ip_type = p_multi_address.is_ipv4() ? IP::TYPE_IPV4 : IP::TYPE_IPV6;
		Error err = _sock->open(NetSocket::TYPE_UDP, ip_type);
		ERR_FAIL_COND_V(err != OK, err);
		_sock->set_blocking_enabled(false);
		_sock->set_broadcasting_enabled(broadcast);
	}
	return _sock->join_multicast_group(p_multi_address, p_if_name);
}

Error PacketPeerUDP::leave_multicast_group(IP_Address p_multi_address, String p_if_name) {

	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);
	ERR_FAIL_COND_V(!_sock->is_open(), ERR_UNCONFIGURED);
	return _sock->leave_multicast_group(p_multi_address, p_if_name);
}

String PacketPeerUDP::_get_packet_ip() const {

	return get_packet_address();
}

Error PacketPeerUDP::_set_dest_address(const String &p_address, int p_port) {

	IP_Address ip;
	if (p_address.is_valid_ip_address()) {
		ip = p_address;
	} else {
		ip = IP::get_singleton()->resolve_hostname(p_address);
		if (!ip.is_valid())
			return ERR_CANT_RESOLVE;
	}

	set_dest_address(ip, p_port);
	return OK;
}

int PacketPeerUDP::get_available_packet_count() const {

	// TODO we should deprecate this, and expose poll instead!
	Error err = const_cast<PacketPeerUDP *>(this)->_poll();
	if (err != OK)
		return -1;

	return queue_count;
}

Error PacketPeerUDP::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {

	Error err = _poll();
	if (err != OK)
		return err;
	if (queue_count == 0)
		return ERR_UNAVAILABLE;

	uint32_t size = 0;
	uint8_t ipv6[16];
	rb.read(ipv6, 16, true);
	packet_ip.set_ipv6(ipv6);
	rb.read((uint8_t *)&packet_port, 4, true);
	rb.read((uint8_t *)&size, 4, true);
	rb.read(packet_buffer, size, true);
	--queue_count;
	*r_buffer = packet_buffer;
	r_buffer_size = size;
	return OK;
}

Error PacketPeerUDP::put_packet(const uint8_t *p_buffer, int p_buffer_size) {

	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);
	ERR_FAIL_COND_V(!peer_addr.is_valid(), ERR_UNCONFIGURED);

	Error err;
	int sent = -1;

	if (!_sock->is_open()) {
		IP::Type ip_type = peer_addr.is_ipv4() ? IP::TYPE_IPV4 : IP::TYPE_IPV6;
		err = _sock->open(NetSocket::TYPE_UDP, ip_type);
		ERR_FAIL_COND_V(err != OK, err);
		_sock->set_blocking_enabled(false);
		_sock->set_broadcasting_enabled(broadcast);
	}

	do {
		if (connected) {
			err = _sock->send(p_buffer, p_buffer_size, sent);
		} else {
			err = _sock->sendto(p_buffer, p_buffer_size, sent, peer_addr, peer_port);
		}
		if (err != OK) {
			if (err != ERR_BUSY)
				return FAILED;
			else if (!blocking)
				return ERR_BUSY;
			// Keep trying to send full packet
			continue;
		}
		return OK;

	} while (sent != p_buffer_size);

	return OK;
}

int PacketPeerUDP::get_max_packet_size() const {

	return 512; // uhm maybe not
}

Error PacketPeerUDP::listen(int p_port, const IP_Address &p_bind_address, int p_recv_buffer_size) {

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
	_sock->set_broadcasting_enabled(broadcast);
	err = _sock->bind(p_bind_address, p_port);

	if (err != OK) {
		_sock->close();
		return err;
	}
	rb.resize(nearest_shift(p_recv_buffer_size));
	return OK;
}

Error PacketPeerUDP::connect_socket(Ref<NetSocket> p_sock) {
	Error err;
	int read = 0;
	uint16_t r_port;
	IP_Address r_ip;

	err = p_sock->recvfrom(recv_buffer, sizeof(recv_buffer), read, r_ip, r_port, true);
	ERR_FAIL_COND_V(err != OK, err);
	err = p_sock->connect_to_host(r_ip, r_port);
	ERR_FAIL_COND_V(err != OK, err);
	_sock = p_sock;
	peer_addr = r_ip;
	peer_port = r_port;
	packet_ip = peer_addr;
	packet_port = peer_port;
	connected = true;
	return OK;
}

Error PacketPeerUDP::connect_to_host(const IP_Address &p_host, int p_port) {
	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);
	ERR_FAIL_COND_V(!p_host.is_valid(), ERR_INVALID_PARAMETER);

	Error err;

	if (!_sock->is_open()) {
		IP::Type ip_type = p_host.is_ipv4() ? IP::TYPE_IPV4 : IP::TYPE_IPV6;
		err = _sock->open(NetSocket::TYPE_UDP, ip_type);
		ERR_FAIL_COND_V(err != OK, ERR_CANT_OPEN);
		_sock->set_blocking_enabled(false);
	}

	err = _sock->connect_to_host(p_host, p_port);

	// I see no reason why we should get ERR_BUSY (wouldblock/eagain) here.
	// This is UDP, so connect is only used to tell the OS to which socket
	// it shuold deliver packets when multiple are bound on the same address/port.
	if (err != OK) {
		close();
		ERR_FAIL_V_MSG(FAILED, "Unable to connect");
	}

	connected = true;

	peer_addr = p_host;
	peer_port = p_port;

	// Flush any packet we might still have in queue.
	rb.clear();
	return OK;
}

bool PacketPeerUDP::is_connected_to_host() const {
	return connected;
}

void PacketPeerUDP::close() {

	if (_sock.is_valid())
		_sock->close();
	rb.resize(16);
	queue_count = 0;
	connected = false;
}

Error PacketPeerUDP::wait() {

	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);
	return _sock->poll(NetSocket::POLL_TYPE_IN, -1);
}

Error PacketPeerUDP::_poll() {

	ERR_FAIL_COND_V(!_sock.is_valid(), ERR_UNAVAILABLE);

	if (!_sock->is_open()) {
		return FAILED;
	}

	Error err;
	int read;
	IP_Address ip;
	uint16_t port;

	while (true) {
		if (connected) {
			err = _sock->recv(recv_buffer, sizeof(recv_buffer), read);
			ip = peer_addr;
			port = peer_port;
		} else {
			err = _sock->recvfrom(recv_buffer, sizeof(recv_buffer), read, ip, port);
		}

		if (err != OK) {
			if (err == ERR_BUSY)
				break;
			return FAILED;
		}

		if (rb.space_left() < read + 24) {
#ifdef TOOLS_ENABLED
			WARN_PRINT("Buffer full, dropping packets!");
#endif
			continue;
		}

		uint32_t port32 = port;
		rb.write(ip.get_ipv6(), 16);
		rb.write((uint8_t *)&port32, 4);
		rb.write((uint8_t *)&read, 4);
		rb.write(recv_buffer, read);
		++queue_count;
	}

	return OK;
}
bool PacketPeerUDP::is_listening() const {

	return _sock.is_valid() && _sock->is_open();
}

IP_Address PacketPeerUDP::get_packet_address() const {

	return packet_ip;
}

int PacketPeerUDP::get_packet_port() const {

	return packet_port;
}

void PacketPeerUDP::set_dest_address(const IP_Address &p_address, int p_port) {

	ERR_FAIL_COND_MSG(connected, "Destination address cannot be set for connected sockets");
	peer_addr = p_address;
	peer_port = p_port;
}

void PacketPeerUDP::_bind_methods() {

	ClassDB::bind_method(D_METHOD("listen", "port", "bind_address", "recv_buf_size"), &PacketPeerUDP::listen, DEFVAL("*"), DEFVAL(65536));
	ClassDB::bind_method(D_METHOD("close"), &PacketPeerUDP::close);
	ClassDB::bind_method(D_METHOD("wait"), &PacketPeerUDP::wait);
	ClassDB::bind_method(D_METHOD("is_listening"), &PacketPeerUDP::is_listening);
	ClassDB::bind_method(D_METHOD("connect_to_host", "host", "port"), &PacketPeerUDP::connect_to_host);
	ClassDB::bind_method(D_METHOD("is_connected_to_host"), &PacketPeerUDP::is_connected_to_host);
	ClassDB::bind_method(D_METHOD("get_packet_ip"), &PacketPeerUDP::_get_packet_ip);
	ClassDB::bind_method(D_METHOD("get_packet_port"), &PacketPeerUDP::get_packet_port);
	ClassDB::bind_method(D_METHOD("set_dest_address", "host", "port"), &PacketPeerUDP::_set_dest_address);
	ClassDB::bind_method(D_METHOD("set_broadcast_enabled", "enabled"), &PacketPeerUDP::set_broadcast_enabled);
	ClassDB::bind_method(D_METHOD("join_multicast_group", "multicast_address", "interface_name"), &PacketPeerUDP::join_multicast_group);
	ClassDB::bind_method(D_METHOD("leave_multicast_group", "multicast_address", "interface_name"), &PacketPeerUDP::leave_multicast_group);
}

PacketPeerUDP::PacketPeerUDP() :
		packet_port(0),
		queue_count(0),
		peer_port(0),
		connected(false),
		blocking(true),
		broadcast(false),
		_sock(Ref<NetSocket>(NetSocket::create())) {
	rb.resize(16);
}

PacketPeerUDP::~PacketPeerUDP() {

	close();
}

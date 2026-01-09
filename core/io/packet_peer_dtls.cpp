

/// @class PacketPeerDTLS
/// @brief Handles DTLS (Datagram Transport Layer Security) connections for packet-based communication.
///
/// PacketPeerDTLS provides secure UDP-based communication using the DTLS protocol.
/// It allows establishing encrypted connections to peer packet sockets with optional
/// certificate validation.
///
/// @note This class uses a factory pattern for instantiation via the static create() method.
///       Availability depends on the underlying platform implementation.
///
/// @example
/// @code
/// var dtls_peer = PacketPeerDTLS.new()
/// dtls_peer.connect_to_peer(packet_peer, true, "example.com")
/// dtls_peer.poll()
/// var status = dtls_peer.get_status()
/// @endcode

/// @fn static PacketPeerDTLS* PacketPeerDTLS::create()
/// @brief Creates a new instance of PacketPeerDTLS.
/// @return A pointer to a new PacketPeerDTLS instance, or nullptr if creation fails.

/// @fn static bool PacketPeerDTLS::is_available()
/// @brief Checks if DTLS support is available on the current platform.
/// @return true if DTLS is available, false otherwise.

/// @fn void PacketPeerDTLS::_bind_methods()
/// @brief Binds C++ methods to the scripting API.
/// @details Exposes poll(), connect_to_peer(), get_status(), disconnect_from_peer() 
///          methods and connection status enum constants to GDScript.

/// @fn PacketPeerDTLS::PacketPeerDTLS()
/// @brief Constructs a new PacketPeerDTLS instance.
#include "packet_peer_dtls.h"
#include "core/os/file_access.h"
#include "core/project_settings.h"

PacketPeerDTLS *(*PacketPeerDTLS::_create)() = NULL;
bool PacketPeerDTLS::available = false;

PacketPeerDTLS *PacketPeerDTLS::create() {

	return _create();
}

bool PacketPeerDTLS::is_available() {
	return available;
}

void PacketPeerDTLS::_bind_methods() {

	ClassDB::bind_method(D_METHOD("poll"), &PacketPeerDTLS::poll);
	ClassDB::bind_method(D_METHOD("connect_to_peer", "packet_peer", "validate_certs", "for_hostname", "valid_certificate"), &PacketPeerDTLS::connect_to_peer, DEFVAL(true), DEFVAL(String()), DEFVAL(Ref<X509Certificate>()));
	ClassDB::bind_method(D_METHOD("get_status"), &PacketPeerDTLS::get_status);
	ClassDB::bind_method(D_METHOD("disconnect_from_peer"), &PacketPeerDTLS::disconnect_from_peer);

	BIND_ENUM_CONSTANT(STATUS_DISCONNECTED);
	BIND_ENUM_CONSTANT(STATUS_HANDSHAKING);
	BIND_ENUM_CONSTANT(STATUS_CONNECTED);
	BIND_ENUM_CONSTANT(STATUS_ERROR);
	BIND_ENUM_CONSTANT(STATUS_ERROR_HOSTNAME_MISMATCH);
}

PacketPeerDTLS::PacketPeerDTLS() {
}

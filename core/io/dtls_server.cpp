

/// @class DTLSServer
/// @brief Manages DTLS (Datagram Transport Layer Security) server functionality.
/// 
/// Provides a factory pattern for creating DTLS server instances and binding
/// methods for setup and connection handling. This class serves as an interface
/// for secure UDP-based communication using DTLS protocol.
///
/// @note Availability of DTLS server functionality depends on the platform
/// and whether the implementation has been registered via _create.
///
/// @see X509Certificate, take_connection, setup

/// @fn static DTLSServer *DTLSServer::create()
/// @brief Creates a new instance of the DTLS server.
/// @return A pointer to a new DTLSServer instance, or NULL if not available.
/// @note This uses a factory function pointer pattern. _create must be
/// initialized before calling this method.

/// @fn static bool DTLSServer::is_available()
/// @brief Checks if DTLS server functionality is available.
/// @return true if DTLS server is available on this platform, false otherwise.

/// @fn void DTLSServer::setup(Ref<PrivateKey> key, Ref<X509Certificate> certificate, Ref<X509Certificate> chain = Ref<X509Certificate>())
/// @brief Configures the DTLS server with cryptographic credentials.
/// @param key The private key for the server.
/// @param certificate The X.509 certificate for the server.
/// @param chain Optional certificate chain for additional certificates.

/// @fn PacketPeerDTLS *DTLSServer::take_connection(Ref<PacketPeerUDP> udp_peer)
/// @brief Upgrades a UDP peer connection to a secure DTLS connection.
/// @param udp_peer The UDP peer to upgrade to DTLS.
/// @return A new PacketPeerDTLS instance wrapping the UDP connection, or NULL on failure.

/// @fn DTLSServer::DTLSServer()
/// @brief Constructs a new DTLSServer instance.
#include "dtls_server.h"
#include "core/os/file_access.h"
#include "core/project_settings.h"

DTLSServer *(*DTLSServer::_create)() = NULL;
bool DTLSServer::available = false;

DTLSServer *DTLSServer::create() {

	return _create();
}

bool DTLSServer::is_available() {
	return available;
}

void DTLSServer::_bind_methods() {

	ClassDB::bind_method(D_METHOD("setup", "key", "certificate", "chain"), &DTLSServer::setup, DEFVAL(Ref<X509Certificate>()));
	ClassDB::bind_method(D_METHOD("take_connection", "udp_peer"), &DTLSServer::take_connection);
}

DTLSServer::DTLSServer() {
}

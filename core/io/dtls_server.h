

/**
 * @class DTLSServer
 * @brief Manages DTLS (Datagram Transport Layer Security) server connections.
 * 
 * DTLSServer is a reference-counted class that handles the setup and management
 * of secure UDP connections using DTLS protocol. It provides methods to initialize
 * the server with cryptographic credentials and to accept incoming DTLS connections.
 * 
 * @note This class is a virtual interface that must be implemented by a platform-specific backend.
 * 
 * @see PacketPeerDTLS
 * @see CryptoKey
 * @see X509Certificate
 */

/**
 * @fn static bool DTLSServer::is_available()
 * @brief Checks if DTLS server support is available on the current platform.
 * 
 * @return true if DTLS server is supported, false otherwise.
 */

/**
 * @fn static DTLSServer* DTLSServer::create()
 * @brief Creates a new instance of the platform-specific DTLSServer implementation.
 * 
 * @return A new DTLSServer instance, or nullptr if creation fails.
 */

/**
 * @fn virtual Error DTLSServer::setup(Ref<CryptoKey> p_key, Ref<X509Certificate> p_cert, Ref<X509Certificate> p_ca_chain)
 * @brief Initializes the DTLS server with cryptographic credentials.
 * 
 * @param p_key The private key used for the server.
 * @param p_cert The X.509 certificate for the server.
 * @param p_ca_chain Optional CA certificate chain for validation.
 * 
 * @return Error code indicating success (OK) or failure.
 */

/**
 * @fn virtual void DTLSServer::stop()
 * @brief Stops the DTLS server and releases associated resources.
 */

/**
 * @fn virtual Ref<PacketPeerDTLS> DTLSServer::take_connection(Ref<PacketPeerUDP> p_peer)
 * @brief Accepts and wraps an incoming UDP peer connection in a DTLS wrapper.
 * 
 * @param p_peer The UDP peer connection to wrap with DTLS.
 * 
 * @return A new PacketPeerDTLS instance wrapping the UDP peer, or nullptr on failure.
 */
#ifndef DTLS_SERVER_H
#define DTLS_SERVER_H

#include "core/io/net_socket.h"
#include "core/io/packet_peer_dtls.h"

class DTLSServer : public Reference {
	GDCLASS(DTLSServer, Reference);

protected:
	static DTLSServer *(*_create)();
	static void _bind_methods();

	static bool available;

public:
	static bool is_available();
	static DTLSServer *create();

	virtual Error setup(Ref<CryptoKey> p_key, Ref<X509Certificate> p_cert, Ref<X509Certificate> p_ca_chain = Ref<X509Certificate>()) = 0;
	virtual void stop() = 0;
	virtual Ref<PacketPeerDTLS> take_connection(Ref<PacketPeerUDP> p_peer) = 0;

	DTLSServer();
};

#endif // DTLS_SERVER_H

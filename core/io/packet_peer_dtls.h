

/// @class PacketPeerDTLS
/// @brief DTLS (Datagram Transport Layer Security) packet peer implementation.
///
/// PacketPeerDTLS provides a secure communication layer over UDP using DTLS encryption.
/// It wraps a PacketPeerUDP instance and handles the DTLS handshake and encrypted
/// packet transmission/reception.
///
/// @note This is an abstract base class. Use create() to instantiate the platform-specific implementation.
///
/// @see PacketPeer, PacketPeerUDP, X509Certificate

/// @enum Status
/// @brief Connection status of the DTLS peer.
/// @var STATUS_DISCONNECTED - No connection established.
/// @var STATUS_HANDSHAKING - DTLS handshake in progress.
/// @var STATUS_CONNECTED - Connection established and encrypted.
/// @var STATUS_ERROR - An error occurred during connection.
/// @var STATUS_ERROR_HOSTNAME_MISMATCH - Certificate hostname validation failed.

/// @brief Poll for incoming packets and process connection state.
/// @remarks Must be called regularly to maintain the connection and receive data.
virtual void poll() = 0;

/// @brief Establish a DTLS connection to a remote peer.
/// @param p_base The underlying UDP peer to use for communication.
/// @param p_validate_certs Whether to validate server certificates (default: true).
/// @param p_for_hostname The hostname to validate against the server certificate.
/// @param p_ca_certs Custom CA certificate for validation (optional).
/// @return OK on success, or an error code on failure.
virtual Error connect_to_peer(Ref<PacketPeerUDP> p_base, bool p_validate_certs = true, const String &p_for_hostname = String(), Ref<X509Certificate> p_ca_certs = Ref<X509Certificate>()) = 0;

/// @brief Terminate the DTLS connection.
virtual void disconnect_from_peer() = 0;

/// @brief Get the current connection status.
/// @return The current Status enum value.
virtual Status get_status() const = 0;

/// @brief Create a platform-specific DTLS peer instance.
/// @return A new PacketPeerDTLS instance, or nullptr if DTLS is not available.
static PacketPeerDTLS *create();

/// @brief Check if DTLS is available on this platform.
/// @return true if DTLS support is compiled in, false otherwise.
static bool is_available();
#ifndef PACKET_PEER_DTLS_H
#define PACKET_PEER_DTLS_H

#include "core/crypto/crypto.h"
#include "core/io/packet_peer_udp.h"

class PacketPeerDTLS : public PacketPeer {
	GDCLASS(PacketPeerDTLS, PacketPeer);

protected:
	static PacketPeerDTLS *(*_create)();
	static void _bind_methods();

	static bool available;

public:
	enum Status {
		STATUS_DISCONNECTED,
		STATUS_HANDSHAKING,
		STATUS_CONNECTED,
		STATUS_ERROR,
		STATUS_ERROR_HOSTNAME_MISMATCH
	};

	virtual void poll() = 0;
	virtual Error connect_to_peer(Ref<PacketPeerUDP> p_base, bool p_validate_certs = true, const String &p_for_hostname = String(), Ref<X509Certificate> p_ca_certs = Ref<X509Certificate>()) = 0;
	virtual void disconnect_from_peer() = 0;
	virtual Status get_status() const = 0;

	static PacketPeerDTLS *create();
	static bool is_available();

	PacketPeerDTLS();
};

VARIANT_ENUM_CAST(PacketPeerDTLS::Status);

#endif // PACKET_PEER_DTLS_H

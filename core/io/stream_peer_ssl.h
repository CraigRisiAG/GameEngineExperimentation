

/// @class StreamPeerSSL
/// @brief SSL/TLS stream peer for secure communication over network streams.
/// 
/// StreamPeerSSL provides an abstraction for establishing secure SSL/TLS connections
/// over existing StreamPeer implementations. It handles the SSL/TLS handshake process,
/// certificate validation, and encryption/decryption of data.
/// 
/// @note This class is abstract and must be implemented by platform-specific subclasses.

/// @enum Status
/// @brief Connection status states for SSL/TLS stream.
/// - STATUS_DISCONNECTED: No active connection
/// - STATUS_HANDSHAKING: Currently performing SSL/TLS handshake
/// - STATUS_CONNECTED: Successfully established secure connection
/// - STATUS_ERROR: Connection error occurred
/// - STATUS_ERROR_HOSTNAME_MISMATCH: Certificate hostname validation failed

/// @fn void set_blocking_handshake_enabled(bool p_enabled)
/// @brief Enable or disable blocking mode during SSL/TLS handshake.
/// @param p_enabled True to block until handshake completes, false for non-blocking mode

/// @fn bool is_blocking_handshake_enabled() const
/// @brief Check if blocking handshake mode is currently enabled.
/// @return True if blocking handshake is enabled, false otherwise

/// @fn virtual void poll() = 0
/// @brief Poll the SSL/TLS connection for events and state changes.
/// @note Must be called regularly to process non-blocking handshakes and data.

/// @fn virtual Error accept_stream(Ref<StreamPeer> p_base, Ref<CryptoKey> p_key, Ref<X509Certificate> p_cert, Ref<X509Certificate> p_ca_chain = Ref<X509Certificate>()) = 0
/// @brief Accept an incoming SSL/TLS connection on the given stream.
/// @param p_base The underlying stream to secure
/// @param p_key Private key for the server certificate
/// @param p_cert Server certificate
/// @param p_ca_chain Optional CA certificate chain for client validation
/// @return Error code if operation failed

/// @fn virtual Error connect_to_stream(Ref<StreamPeer> p_base, bool p_validate_certs = false, const String &p_for_hostname = String(), Ref<X509Certificate> p_valid_cert = Ref<X509Certificate>()) = 0
/// @brief Initiate an SSL/TLS connection to a remote peer.
/// @param p_base The underlying stream to secure
/// @param p_validate_certs Whether to validate server certificate
/// @param p_for_hostname Hostname to validate against certificate
/// @param p_valid_cert Optional certificate to validate against
/// @return Error code if connection initiation failed

/// @fn virtual Status get_status() const = 0
/// @brief Get the current SSL/TLS connection status.
/// @return Current connection status

/// @fn virtual void disconnect_from_stream() = 0
/// @brief Disconnect and close the SSL/TLS connection.

/// @fn static StreamPeerSSL* create()
/// @brief Create a new platform-specific SSL/TLS stream peer instance.
/// @return New StreamPeerSSL instance or nullptr if SSL is not available

/// @fn static bool is_available()
/// @brief Check if SSL/TLS support is available on this platform.
/// @return True if SSL support is compiled and available
#ifndef STREAM_PEER_SSL_H
#define STREAM_PEER_SSL_H

#include "core/crypto/crypto.h"
#include "core/io/stream_peer.h"

class StreamPeerSSL : public StreamPeer {
	GDCLASS(StreamPeerSSL, StreamPeer);

protected:
	static StreamPeerSSL *(*_create)();
	static void _bind_methods();

	static bool available;

	bool blocking_handshake;

public:
	enum Status {
		STATUS_DISCONNECTED,
		STATUS_HANDSHAKING,
		STATUS_CONNECTED,
		STATUS_ERROR,
		STATUS_ERROR_HOSTNAME_MISMATCH
	};

	void set_blocking_handshake_enabled(bool p_enabled);
	bool is_blocking_handshake_enabled() const;

	virtual void poll() = 0;
	virtual Error accept_stream(Ref<StreamPeer> p_base, Ref<CryptoKey> p_key, Ref<X509Certificate> p_cert, Ref<X509Certificate> p_ca_chain = Ref<X509Certificate>()) = 0;
	virtual Error connect_to_stream(Ref<StreamPeer> p_base, bool p_validate_certs = false, const String &p_for_hostname = String(), Ref<X509Certificate> p_valid_cert = Ref<X509Certificate>()) = 0;
	virtual Status get_status() const = 0;

	virtual void disconnect_from_stream() = 0;

	static StreamPeerSSL *create();

	static bool is_available();

	StreamPeerSSL();
};

VARIANT_ENUM_CAST(StreamPeerSSL::Status);

#endif // STREAM_PEER_SSL_H



/**
 * @class StreamPeerSSL
 * @brief Manages SSL/TLS connections for stream-based peer communication.
 * 
 * StreamPeerSSL provides functionality to establish secure SSL/TLS connections
 * over stream peers. It supports both client-side connections and server-side
 * acceptance of encrypted streams with certificate validation and blocking
 * handshake control.
 * 
 * @note This class uses a factory pattern (_create) for platform-specific
 * implementations.
 */

/**
 * @fn static StreamPeerSSL* create()
 * @brief Factory method to create a platform-specific StreamPeerSSL instance.
 * @return Pointer to a new StreamPeerSSL instance, or NULL if creation is not available.
 */

/**
 * @fn static bool is_available()
 * @brief Checks if SSL/TLS support is available on the current platform.
 * @return true if SSL/TLS is available, false otherwise.
 */

/**
 * @fn void set_blocking_handshake_enabled(bool p_enabled)
 * @brief Enables or disables blocking mode for the SSL handshake process.
 * @param p_enabled true to enable blocking handshake, false for non-blocking.
 */

/**
 * @fn bool is_blocking_handshake_enabled() const
 * @brief Retrieves the current blocking handshake setting.
 * @return true if blocking handshake is enabled, false otherwise.
 */

/**
 * @fn void _bind_methods()
 * @brief Binds all exposed methods and properties to the script interface.
 * Exposes methods for polling, stream connection/acceptance, status checking,
 * and handshake configuration to the scripting language.
 */

/**
 * @fn StreamPeerSSL()
 * @brief Default constructor that initializes the SSL peer with blocking
 * handshake enabled by default.
 */
#include "stream_peer_ssl.h"

#include "core/engine.h"

StreamPeerSSL *(*StreamPeerSSL::_create)() = NULL;

StreamPeerSSL *StreamPeerSSL::create() {

	if (_create)
		return _create();
	return NULL;
}

bool StreamPeerSSL::available = false;

bool StreamPeerSSL::is_available() {
	return available;
}

void StreamPeerSSL::set_blocking_handshake_enabled(bool p_enabled) {
	blocking_handshake = p_enabled;
}

bool StreamPeerSSL::is_blocking_handshake_enabled() const {
	return blocking_handshake;
}

void StreamPeerSSL::_bind_methods() {

	ClassDB::bind_method(D_METHOD("poll"), &StreamPeerSSL::poll);
	ClassDB::bind_method(D_METHOD("accept_stream", "stream", "private_key", "certificate", "chain"), &StreamPeerSSL::accept_stream, DEFVAL(Ref<X509Certificate>()));
	ClassDB::bind_method(D_METHOD("connect_to_stream", "stream", "validate_certs", "for_hostname", "valid_certificate"), &StreamPeerSSL::connect_to_stream, DEFVAL(false), DEFVAL(String()), DEFVAL(Ref<X509Certificate>()));
	ClassDB::bind_method(D_METHOD("get_status"), &StreamPeerSSL::get_status);
	ClassDB::bind_method(D_METHOD("disconnect_from_stream"), &StreamPeerSSL::disconnect_from_stream);
	ClassDB::bind_method(D_METHOD("set_blocking_handshake_enabled", "enabled"), &StreamPeerSSL::set_blocking_handshake_enabled);
	ClassDB::bind_method(D_METHOD("is_blocking_handshake_enabled"), &StreamPeerSSL::is_blocking_handshake_enabled);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "blocking_handshake"), "set_blocking_handshake_enabled", "is_blocking_handshake_enabled");

	BIND_ENUM_CONSTANT(STATUS_DISCONNECTED);
	BIND_ENUM_CONSTANT(STATUS_HANDSHAKING);
	BIND_ENUM_CONSTANT(STATUS_CONNECTED);
	BIND_ENUM_CONSTANT(STATUS_ERROR);
	BIND_ENUM_CONSTANT(STATUS_ERROR_HOSTNAME_MISMATCH);
}

StreamPeerSSL::StreamPeerSSL() {
	blocking_handshake = true;
}

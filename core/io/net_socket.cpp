

/// @brief Creates a new network socket instance.
/// @details This is a factory method that uses a platform-specific creation function
/// pointer to instantiate a NetSocket object. The actual implementation is set via
/// the _create function pointer, which should be initialized by platform-specific code.
/// @return A pointer to a newly created NetSocket object, or NULL if the platform
/// is not supported or if the creation function has not been set.
/// @note Prints an error message if _create is not set before calling this method.
static NetSocket *create();
#include "net_socket.h"

NetSocket *(*NetSocket::_create)() = NULL;

NetSocket *NetSocket::create() {

	if (_create)
		return _create();

	ERR_PRINT("Unable to create network socket, platform not supported");
	return NULL;
}

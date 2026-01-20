


/**
 * @class EditorDebuggerServer
 * @brief Abstract base class for managing editor debugger server connections.
 * 
 * EditorDebuggerServer provides an interface for creating and managing connections
 * to remote debuggers. Implementations of this class handle the server-side logic
 * for accepting and processing debugger connections.
 */

/**
 * @fn static EditorDebuggerServer *create_default()
 * @brief Factory method to create a default debugger server implementation.
 * @return A pointer to the created EditorDebuggerServer instance.
 */

/**
 * @fn virtual void poll()
 * @brief Process pending debugger server operations.
 * 
 * Should be called regularly to check for new connections and process
 * any pending communication from connected debuggers.
 */

/**
 * @fn virtual Error start()
 * @brief Start the debugger server.
 * @return Error code indicating success or failure of the operation.
 */

/**
 * @fn virtual void stop()
 * @brief Stop the debugger server and close any active connections.
 */

/**
 * @fn virtual bool is_active() const
 * @brief Check if the debugger server is currently running.
 * @return true if the server is active, false otherwise.
 */

/**
 * @fn virtual bool is_connection_available() const
 * @brief Check if a new debugger connection is available.
 * @return true if a connection is waiting to be accepted, false otherwise.
 */

/**
 * @fn virtual Ref<RemoteDebuggerPeer> take_connection()
 * @brief Accept and retrieve the next available debugger connection.
 * @return A reference to the RemoteDebuggerPeer representing the connection,
 *         or null if no connection is available.
 */
#ifndef EDITOR_DEBUGGER_CONNECTION_H
#define EDITOR_DEBUGGER_CONNECTION_H

#include "core/debugger/remote_debugger_peer.h"
#include "core/reference.h"

class EditorDebuggerServer : public Reference {

public:
	static EditorDebuggerServer *create_default();
	virtual void poll() = 0;
	virtual Error start() = 0;
	virtual void stop() = 0;
	virtual bool is_active() const = 0;
	virtual bool is_connection_available() const = 0;
	virtual Ref<RemoteDebuggerPeer> take_connection() = 0;
};

#endif // EDITOR_DEBUGGER_CONNECTION_H




/**
 * @class EditorDebuggerServerTCP
 * @brief TCP-based implementation of the editor debugger server.
 * 
 * Manages TCP server connections for the editor debugger, handling incoming
 * debug connections from remote debugger peers. Inherits from EditorDebuggerServer
 * and provides TCP-specific server functionality.
 * 
 * @details
 * - Listens for incoming TCP connections on a configurable remote port
 * - Validates port availability and logs connection errors
 * - Manages connection lifecycle (start, stop, poll)
 * - Creates RemoteDebuggerPeerTCP instances for accepted connections
 */

/**
 * @fn EditorDebuggerServerTCP::EditorDebuggerServerTCP()
 * @brief Constructs the TCP debugger server and initializes the internal TCP_Server instance.
 */

/**
 * @fn Error EditorDebuggerServerTCP::start()
 * @brief Starts listening for incoming debugger connections on the configured remote port.
 * @return OK on successful start, error code if port binding fails.
 * @details If the server fails to listen, an error message is logged to the editor log.
 */

/**
 * @fn void EditorDebuggerServerTCP::stop()
 * @brief Stops the TCP server from listening for new connections.
 */

/**
 * @fn bool EditorDebuggerServerTCP::is_active() const
 * @brief Checks if the server is currently active and listening.
 * @return true if the server is listening, false otherwise.
 */

/**
 * @fn bool EditorDebuggerServerTCP::is_connection_available() const
 * @brief Checks if the server is listening and has an available incoming connection.
 * @return true if a connection is available, false otherwise.
 */

/**
 * @fn Ref<RemoteDebuggerPeer> EditorDebuggerServerTCP::take_connection()
 * @brief Accepts and wraps the next available connection in a RemoteDebuggerPeerTCP.
 * @return A reference to the new RemoteDebuggerPeerTCP, or null if no connection is available.
 * @details Asserts that a connection is available before attempting to take it.
 */

/**
 * @fn EditorDebuggerServer* EditorDebuggerServer::create_default()
 * @brief Factory method that creates the default TCP-based debugger server implementation.
 * @return A new instance of EditorDebuggerServerTCP.
 */
#include "editor_debugger_server.h"

#include "core/io/marshalls.h"
#include "core/io/tcp_server.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "editor/editor_log.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"

class EditorDebuggerServerTCP : public EditorDebuggerServer {

private:
	Ref<TCP_Server> server;

public:
	virtual void poll() {}
	virtual Error start();
	virtual void stop();
	virtual bool is_active() const;
	virtual bool is_connection_available() const;
	virtual Ref<RemoteDebuggerPeer> take_connection();

	EditorDebuggerServerTCP();
};

EditorDebuggerServerTCP::EditorDebuggerServerTCP() {
	server.instance();
}

Error EditorDebuggerServerTCP::start() {
	int remote_port = (int)EditorSettings::get_singleton()->get("network/debug/remote_port");
	const Error err = server->listen(remote_port);
	if (err != OK) {
		EditorNode::get_log()->add_message(String("Error listening on port ") + itos(remote_port), EditorLog::MSG_TYPE_ERROR);
		return err;
	}
	return err;
}

void EditorDebuggerServerTCP::stop() {
	server->stop();
}

bool EditorDebuggerServerTCP::is_active() const {
	return server->is_listening();
}

bool EditorDebuggerServerTCP::is_connection_available() const {
	return server->is_listening() && server->is_connection_available();
}

Ref<RemoteDebuggerPeer> EditorDebuggerServerTCP::take_connection() {
	ERR_FAIL_COND_V(!is_connection_available(), Ref<RemoteDebuggerPeer>());
	return memnew(RemoteDebuggerPeerTCP(server->take_connection()));
}

EditorDebuggerServer *EditorDebuggerServer::create_default() {
	return memnew(EditorDebuggerServerTCP);
}

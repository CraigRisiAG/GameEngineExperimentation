

/**
 * @class RemoteDebuggerPeer
 * @brief Abstract base class for remote debugger peer connections.
 * 
 * Provides an interface for communicating with a remote debugger over a network connection.
 * Handles message queuing and transmission of debugging information.
 */

/**
 * @var int RemoteDebuggerPeer::max_queued_messages
 * @brief Maximum number of messages that can be queued before dropping new ones. Default is 4096.
 */

/**
 * @fn static Ref<RemoteDebuggerPeer> RemoteDebuggerPeer::create_from_uri(const String p_uri)
 * @brief Factory method to create a debugger peer from a URI string.
 * @param p_uri The connection URI (e.g., "tcp://localhost:6007")
 * @return A reference to the created RemoteDebuggerPeer instance.
 */

/**
 * @fn virtual bool RemoteDebuggerPeer::is_peer_connected()
 * @brief Checks if the peer is currently connected.
 * @return true if connected, false otherwise.
 */

/**
 * @fn virtual bool RemoteDebuggerPeer::has_message()
 * @brief Checks if there are incoming messages waiting to be processed.
 * @return true if messages are available, false otherwise.
 */

/**
 * @fn virtual Error RemoteDebuggerPeer::put_message(const Array &p_arr)
 * @brief Queues a message to be sent to the remote debugger.
 * @param p_arr The message array to send.
 * @return Error code indicating success or failure.
 */

/**
 * @fn virtual Array RemoteDebuggerPeer::get_message()
 * @brief Retrieves the next incoming message from the queue.
 * @return The message array, or an empty array if none available.
 */

/**
 * @fn virtual void RemoteDebuggerPeer::close()
 * @brief Closes the peer connection and cleans up resources.
 */

/**
 * @fn virtual void RemoteDebuggerPeer::poll()
 * @brief Processes pending network operations and message handling.
 */

/**
 * @fn virtual int RemoteDebuggerPeer::get_max_message_size() const
 * @brief Retrieves the maximum size of a single message that can be transmitted.
 * @return Maximum message size in bytes.
 */

/**
 * @class RemoteDebuggerPeerTCP
 * @brief TCP implementation of the RemoteDebuggerPeer for network-based debugging.
 * 
 * Manages TCP socket communication with a remote debugger, using a background thread
 * for non-blocking message transmission and reception.
 */

/**
 * @var Ref<StreamPeerTCP> RemoteDebuggerPeerTCP::tcp_client
 * @brief The underlying TCP stream peer for socket operations.
 */

/**
 * @var Mutex RemoteDebuggerPeerTCP::mutex
 * @brief Synchronization primitive for thread-safe queue access.
 */

/**
 * @var Thread* RemoteDebuggerPeerTCP::thread
 * @brief Background thread handling asynchronous network I/O.
 */

/**
 * @var List<Array> RemoteDebuggerPeerTCP::in_queue
 * @brief Queue of incoming messages from the remote debugger.
 */

/**
 * @var List<Array> RemoteDebuggerPeerTCP::out_queue
 * @brief Queue of outgoing messages to be sent to the remote debugger.
 */

/**
 * @fn Error RemoteDebuggerPeerTCP::connect_to_host(const String &p_host, uint16_t p_port)
 * @brief Establishes a TCP connection to the specified host and port.
 * @param p_host The hostname or IP address to connect to.
 * @param p_port The port number to connect to.
 * @return OK on success, or an error code on failure.
 */

/**
 * @fn RemoteDebuggerPeerTCP::RemoteDebuggerPeerTCP(Ref<StreamPeerTCP> p_stream)
 * @brief Constructs a TCP debugger peer with optional pre-configured stream.
 * @param p_stream Optional existing StreamPeerTCP instance to use for communication.
 */

/**
 * @fn RemoteDebuggerPeerTCP::~RemoteDebuggerPeerTCP()
 * @brief Destructor that closes the connection and cleans up the background thread.
 */
#ifndef REMOTE_DEBUGGER_PEER_H
#define REMOTE_DEBUGGER_PEER_H

#include "core/io/stream_peer_tcp.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/reference.h"
#include "core/ustring.h"

class RemoteDebuggerPeer : public Reference {
protected:
	int max_queued_messages = 4096;

public:
	static Ref<RemoteDebuggerPeer> create_from_uri(const String p_uri);
	virtual bool is_peer_connected() = 0;
	virtual bool has_message() = 0;
	virtual Error put_message(const Array &p_arr) = 0;
	virtual Array get_message() = 0;
	virtual void close() = 0;
	virtual void poll() = 0;
	virtual int get_max_message_size() const = 0;

	RemoteDebuggerPeer();
};

class RemoteDebuggerPeerTCP : public RemoteDebuggerPeer {
private:
	Ref<StreamPeerTCP> tcp_client;
	Mutex mutex;
	Thread *thread = NULL;
	List<Array> in_queue;
	List<Array> out_queue;
	int out_left = 0;
	int out_pos = 0;
	Vector<uint8_t> out_buf;
	int in_left = 0;
	int in_pos = 0;
	Vector<uint8_t> in_buf;
	bool connected = false;
	bool running = false;

	static void _thread_func(void *p_ud);

	void _poll();
	void _write_out();
	void _read_in();

public:
	Error connect_to_host(const String &p_host, uint16_t p_port);

	void poll();
	bool is_peer_connected();
	bool has_message();
	Array get_message();
	Error put_message(const Array &p_arr);
	int get_max_message_size() const;
	void close();

	RemoteDebuggerPeerTCP(Ref<StreamPeerTCP> p_stream = Ref<StreamPeerTCP>());
	~RemoteDebuggerPeerTCP();
};

#endif // REMOTE_DEBUGGER_PEER_H

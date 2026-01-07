

/**
 * @class RemoteDebugger
 * @brief Remote debugger implementation for engine debugging over network connections.
 * 
 * RemoteDebugger extends EngineDebugger to provide remote debugging capabilities,
 * allowing communication with a debugger client over a network peer connection.
 * It manages profiling data (network, servers, visual, performance), error/warning
 * throttling, and thread-safe message marshalling.
 * 
 * Key responsibilities:
 * - Maintain network connection with remote debugger peer
 * - Collect and throttle error/warning messages and output
 * - Manage multiple profiler instances (network, servers, visual, performance)
 * - Thread-safe message queuing and flushing
 * - Handle print and error callbacks from the engine
 * - Capture profiling and debug data on request
 * 
 * Thread Safety:
 * - Mutex protects message handlers and send_message operations
 * - Flushing state and thread ID tracked to ensure single flush operation
 * 
 * @see EngineDebugger
 * @see RemoteDebuggerPeer
 * @see DebuggerMarshalls
 */
#ifndef REMOTE_DEBUGGER_H
#define REMOTE_DEBUGGER_H

#include "core/array.h"
#include "core/debugger/debugger_marshalls.h"
#include "core/debugger/engine_debugger.h"
#include "core/debugger/remote_debugger_peer.h"
#include "core/object.h"
#include "core/string_name.h"
#include "core/ustring.h"

class RemoteDebugger : public EngineDebugger {

private:
	typedef DebuggerMarshalls::OutputError ErrorMessage;

	struct NetworkProfiler;
	struct ServersProfiler;
	struct ScriptsProfiler;
	struct VisualProfiler;
	struct PerformanceProfiler;

	NetworkProfiler *network_profiler = NULL;
	ServersProfiler *servers_profiler = NULL;
	VisualProfiler *visual_profiler = NULL;
	PerformanceProfiler *performance_profiler = NULL;

	Ref<RemoteDebuggerPeer> peer;

	List<String> output_strings;
	List<ErrorMessage> errors;

	int n_messages_dropped = 0;
	int max_errors_per_second = 0;
	int max_chars_per_second = 0;
	int max_warnings_per_second = 0;
	int n_errors_dropped = 0;
	int n_warnings_dropped = 0;
	int char_count = 0;
	int err_count = 0;
	int warn_count = 0;
	int last_reset = 0;
	bool reload_all_scripts = false;

	// Make handlers and send_message thread safe.
	Mutex mutex;
	bool flushing = false;
	Thread::ID flush_thread = 0;

	PrintHandlerList phl;
	static void _print_handler(void *p_this, const String &p_string, bool p_error);
	ErrorHandlerList eh;
	static void _err_handler(void *p_this, const char *p_func, const char *p_file, int p_line, const char *p_err, const char *p_descr, ErrorHandlerType p_type);

	ErrorMessage _create_overflow_error(const String &p_what, const String &p_descr);
	Error _put_msg(String p_message, Array p_data);

	bool is_peer_connected() { return peer->is_peer_connected(); }
	void flush_output();

	void _send_resource_usage();
	void _send_stack_vars(List<String> &p_names, List<Variant> &p_vals, int p_type);

	Error _profiler_capture(const String &p_cmd, const Array &p_data, bool &r_captured);
	Error _core_capture(const String &p_cmd, const Array &p_data, bool &r_captured);

	template <typename T>
	void _bind_profiler(const String &p_name, T *p_prof);
	Error _try_capture(const String &p_name, const Array &p_data, bool &r_captured);

public:
	static RemoteDebugger *create_for_uri(const String &p_uri);

	// Overrides
	void poll_events(bool p_is_idle);
	void send_message(const String &p_message, const Array &p_args);
	void send_error(const String &p_func, const String &p_file, int p_line, const String &p_err, const String &p_descr, ErrorHandlerType p_type);
	void debug(bool p_can_continue = true, bool p_is_error_breakpoint = false);

	RemoteDebugger(Ref<RemoteDebuggerPeer> p_peer);
	~RemoteDebugger();
};

#endif // REMOTE_DEBUGGER_H

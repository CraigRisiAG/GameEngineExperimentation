

/**
 * @file debugger_marshalls.h
 * @brief Marshalling structures for debugger data serialization and deserialization.
 * 
 * This header defines a collection of data structures used by the debugger to package,
 * serialize, and deserialize various profiling and diagnostic information including:
 * - Resource memory usage statistics
 * - Network profiler data for multiplayer nodes
 * - Script profiler function signatures and performance metrics
 * - Server profiler frame data and function timing information
 * - Script stack traces and call stacks
 * - Runtime errors with full context information
 * - Visual profiler frame statistics
 * 
 * Each structure provides serialize() and deserialize() methods to convert data to/from
 * Array format for transmission or storage.
 */
#ifndef DEBUGGER_MARSHARLLS_H
#define DEBUGGER_MARSHARLLS_H

#include "core/script_language.h"
#include "servers/visual_server.h"

struct DebuggerMarshalls {

	// Memory usage
	struct ResourceInfo {
		String path;
		String format;
		String type;
		RID id;
		int vram;
		bool operator<(const ResourceInfo &p_img) const { return vram == p_img.vram ? id < p_img.id : vram > p_img.vram; }
		ResourceInfo() {
			vram = 0;
		}
	};

	struct ResourceUsage {
		List<ResourceInfo> infos;

		Array serialize();
		bool deserialize(const Array &p_arr);
	};

	// Network profiler
	struct MultiplayerNodeInfo {
		ObjectID node;
		String node_path;
		int incoming_rpc = 0;
		int incoming_rset = 0;
		int outgoing_rpc = 0;
		int outgoing_rset = 0;
	};

	struct NetworkProfilerFrame {
		Vector<MultiplayerNodeInfo> infos;

		Array serialize();
		bool deserialize(const Array &p_arr);
	};

	// Script Profiler
	class ScriptFunctionSignature {
	public:
		StringName name;
		int id = -1;

		Array serialize();
		bool deserialize(const Array &p_arr);
	};

	struct ScriptFunctionInfo {
		StringName name;
		int sig_id = -1;
		int call_count = 0;
		float self_time = 0;
		float total_time = 0;
	};

	// Servers profiler
	struct ServerFunctionInfo {
		StringName name;
		float time = 0;
	};

	struct ServerInfo {
		StringName name;
		List<ServerFunctionInfo> functions;
	};

	struct ServersProfilerFrame {
		int frame_number = 0;
		float frame_time = 0;
		float idle_time = 0;
		float physics_time = 0;
		float physics_frame_time = 0;
		float script_time = 0;
		List<ServerInfo> servers;
		Vector<ScriptFunctionInfo> script_functions;

		Array serialize();
		bool deserialize(const Array &p_arr);
	};

	struct ScriptStackVariable {
		String name;
		Variant value;
		int type;
		ScriptStackVariable() {
			type = -1;
		}

		Array serialize(int max_size = 1 << 20); // 1 MiB default.
		bool deserialize(const Array &p_arr);
	};

	struct ScriptStackDump {
		List<ScriptLanguage::StackInfo> frames;
		ScriptStackDump() {}

		Array serialize();
		bool deserialize(const Array &p_arr);
	};

	struct OutputError {
		int hr;
		int min;
		int sec;
		int msec;
		String source_file;
		String source_func;
		int source_line;
		String error;
		String error_descr;
		bool warning;
		Vector<ScriptLanguage::StackInfo> callstack;

		OutputError() {
			hr = -1;
			min = -1;
			sec = -1;
			msec = -1;
			source_line = -1;
			warning = false;
		}

		Array serialize();
		bool deserialize(const Array &p_arr);
	};

	// Visual Profiler
	struct VisualProfilerFrame {
		uint64_t frame_number;
		Vector<VS::FrameProfileArea> areas;

		Array serialize();
		bool deserialize(const Array &p_arr);
	};
};

#endif // DEBUGGER_MARSHARLLS_H

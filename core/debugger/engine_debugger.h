

/**
 * @class EngineDebugger
 * @brief Main debugger interface for the game engine.
 * 
 * EngineDebugger is a singleton class that manages debugging functionality including profiling,
 * message capturing, and script debugging. It provides a bridge between the engine and debugging tools.
 * 
 * @details
 * The class manages two main debugging mechanisms:
 * - Profilers: Monitor and report performance metrics (frame time, idle time, physics time)
 * - Captures: Handle custom debug messages and commands
 * 
 * @note This is a singleton pattern implementation. Use get_singleton() to access the instance.
 * 
 * @see Profiler, Capture, ScriptDebugger
 */

/**
 * @typedef ProfilingToggle
 * @brief Callback function for enabling/disabling profilers.
 * @param p_user User data pointer
 * @param p_enable Enable or disable the profiler
 * @param p_opts Array of options for the profiler
 */

/**
 * @typedef ProfilingTick
 * @brief Callback function for profiler frame updates.
 * @param p_user User data pointer
 * @param p_frame_time Time spent in the current frame
 * @param p_idle_time Time spent idling
 * @param p_physics_time Time spent in physics calculations
 * @param p_physics_frame_time Physics frame duration
 */

/**
 * @typedef ProfilingAdd
 * @brief Callback function for adding profiling data.
 * @param p_user User data pointer
 * @param p_arr Array of profiling data to add
 */

/**
 * @typedef CaptureFunc
 * @brief Callback function for capturing debug messages.
 * @param p_user User data pointer
 * @param p_msg Debug message string
 * @param p_args Array of message arguments
 * @param r_captured Output parameter indicating if message was captured
 * @return Error code if any error occurred
 */

/**
 * @class Profiler
 * @brief Container for profiler callbacks and data.
 * 
 * Stores function pointers and user data for a profiler instance.
 * Allows the debugger to toggle profiling and receive performance data.
 */

/**
 * @class Capture
 * @brief Container for message capture callbacks and data.
 * 
 * Stores function pointer and user data for handling custom debug messages.
 */
#ifndef ENGINE_DEBUGGER_H
#define ENGINE_DEBUGGER_H

#include "core/array.h"
#include "core/map.h"
#include "core/string_name.h"
#include "core/ustring.h"
#include "core/variant.h"
#include "core/vector.h"

class ScriptDebugger;

class EngineDebugger {
public:
	typedef void (*ProfilingToggle)(void *p_user, bool p_enable, const Array &p_opts);
	typedef void (*ProfilingTick)(void *p_user, float p_frame_time, float p_idle_time, float p_physics_time, float p_physics_frame_time);
	typedef void (*ProfilingAdd)(void *p_user, const Array &p_arr);
	typedef Error (*CaptureFunc)(void *p_user, const String &p_msg, const Array &p_args, bool &r_captured);

	class Profiler {
		friend class EngineDebugger;

		ProfilingToggle toggle = NULL;
		ProfilingAdd add = NULL;
		ProfilingTick tick = NULL;
		void *data = NULL;
		bool active = false;

	public:
		Profiler() {}
		Profiler(void *p_data, ProfilingToggle p_toggle, ProfilingAdd p_add, ProfilingTick p_tick) {
			data = p_data;
			toggle = p_toggle;
			add = p_add;
			tick = p_tick;
		}
	};

	class Capture {
		friend class EngineDebugger;

		CaptureFunc capture = NULL;
		void *data = NULL;

	public:
		Capture() {}
		Capture(void *p_data, CaptureFunc p_capture) {
			data = p_data;
			capture = p_capture;
		}
	};

private:
	float frame_time = 0.0;
	float idle_time = 0.0;
	float physics_time = 0.0;
	float physics_frame_time = 0.0;

	uint32_t poll_every = 0;

protected:
	static EngineDebugger *singleton;
	static ScriptDebugger *script_debugger;

	static Map<StringName, Profiler> profilers;
	static Map<StringName, Capture> captures;

public:
	_FORCE_INLINE_ static EngineDebugger *get_singleton() { return singleton; }
	_FORCE_INLINE_ static bool is_active() { return singleton != NULL && script_debugger != NULL; }

	_FORCE_INLINE_ static ScriptDebugger *get_script_debugger() { return script_debugger; };

	static void initialize(const String &p_uri, bool p_skip_breakpoints, Vector<String> p_breakpoints);
	static void deinitialize();
	static void register_profiler(const StringName &p_name, const Profiler &p_profiler);
	static void unregister_profiler(const StringName &p_name);
	static bool is_profiling(const StringName &p_name);
	static bool has_profiler(const StringName &p_name);
	static void profiler_add_frame_data(const StringName &p_name, const Array &p_data);

	static void register_message_capture(const StringName &p_name, Capture p_func);
	static void unregister_message_capture(const StringName &p_name);
	static bool has_capture(const StringName &p_name);

	void iteration(uint64_t p_frame_ticks, uint64_t p_idle_ticks, uint64_t p_physics_ticks, float p_physics_frame_time);
	void profiler_enable(const StringName &p_name, bool p_enabled, const Array &p_opts = Array());
	Error capture_parse(const StringName &p_name, const String &p_msg, const Array &p_args, bool &r_captured);

	void line_poll();

	virtual void poll_events(bool p_is_idle) {}
	virtual void send_message(const String &p_msg, const Array &p_data) = 0;
	virtual void send_error(const String &p_func, const String &p_file, int p_line, const String &p_err, const String &p_descr, ErrorHandlerType p_type) = 0;
	virtual void debug(bool p_can_continue = true, bool p_is_error_breakpoint = false) = 0;

	virtual ~EngineDebugger();
};

#endif // ENGINE_DEBUGGER_H

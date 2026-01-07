

/// @class LocalDebugger
/// @brief Local debugger implementation for engine debugging and script profiling.
/// 
/// LocalDebugger extends EngineDebugger to provide local debugging capabilities including
/// breakpoint management, variable inspection, and script profiling functionality.
/// 
/// @details
/// - Manages script profiling through an internal ScriptsProfiler
/// - Handles breakpoint parsing and execution
/// - Provides variable inspection and printing utilities
/// - Supports inter-process debugging communication via messages and errors

/// @brief Parses a string line into a breakpoint pair.
/// @param p_line The input line to parse as a breakpoint
/// @return A Pair containing the breakpoint location (String) and line number (int)
/// @note Used internally to convert user input to breakpoint data structures

/// @brief Prints variables with their corresponding values.
/// @param names List of variable names to display
/// @param values List of variable values corresponding to each name
/// @param variable_prefix Prefix string to prepend to variable names in output
/// @details Formats and outputs variable information for debugging inspection

/// @brief Executes debug break with continuation and error state options.
/// @param p_can_continue Whether execution can resume from this breakpoint
/// @param p_is_error_breakpoint Whether this breakpoint was triggered by an error condition

/// @brief Sends a debug message to connected debugger clients.
/// @param p_message The message string to transmit
/// @param p_args Array of arguments to include with the message

/// @brief Reports an error to the debugger with full context information.
/// @param p_func Function name where error occurred
/// @param p_file Source file path where error occurred
/// @param p_line Line number in source file where error occurred
/// @param p_err Error code or type identifier
/// @param p_descr Human-readable error description
/// @param p_type Category of error (ErrorHandlerType)

/// @brief Constructs a new LocalDebugger instance.

/// @brief Destructs the LocalDebugger instance, cleaning up profiler resources.
#ifndef LOCAL_DEBUGGER_H
#define LOCAL_DEBUGGER_H

#include "core/debugger/engine_debugger.h"
#include "core/list.h"
#include "core/script_language.h"

class LocalDebugger : public EngineDebugger {

private:
	struct ScriptsProfiler;

	ScriptsProfiler *scripts_profiler = NULL;

	String target_function;
	Map<String, String> options;

	Pair<String, int> to_breakpoint(const String &p_line);
	void print_variables(const List<String> &names, const List<Variant> &values, const String &variable_prefix);

public:
	void debug(bool p_can_continue, bool p_is_error_breakpoint);
	void send_message(const String &p_message, const Array &p_args);
	void send_error(const String &p_func, const String &p_file, int p_line, const String &p_err, const String &p_descr, ErrorHandlerType p_type);

	LocalDebugger();
	~LocalDebugger();
};

#endif // LOCAL_DEBUGGER_H

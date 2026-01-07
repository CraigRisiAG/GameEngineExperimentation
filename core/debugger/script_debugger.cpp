
/**
 * @class ScriptDebugger
 * @brief Manages script debugging functionality including breakpoints and debug sessions.
 * 
 * Provides an interface for managing breakpoints, controlling script execution flow,
 * and handling debug information. Acts as a bridge between script languages and the
 * engine debugger.
 */

/**
 * @brief Sets the number of lines remaining to execute before pausing.
 * @param p_left The number of lines to execute.
 */

/**
 * @brief Gets the number of lines remaining to execute.
 * @return The number of lines left to execute.
 */

/**
 * @brief Sets the current stack depth.
 * @param p_depth The depth value to set.
 */

/**
 * @brief Gets the current stack depth.
 * @return The current stack depth.
 */

/**
 * @brief Inserts a breakpoint at the specified line in the given source.
 * @param p_line The line number where the breakpoint should be placed.
 * @param p_source The source file identifier.
 */

/**
 * @brief Removes a breakpoint from the specified line in the given source.
 * @param p_line The line number of the breakpoint to remove.
 * @param p_source The source file identifier.
 */

/**
 * @brief Checks if a breakpoint exists at the specified line and source.
 * @param p_line The line number to check.
 * @param p_source The source file identifier.
 * @return True if a breakpoint exists at the location, false otherwise.
 */

/**
 * @brief Checks if any breakpoint exists at the specified line.
 * @param p_line The line number to check.
 * @return True if any breakpoint exists at the line, false otherwise.
 */

/**
 * @brief Finds the source file based on the provided source identifier.
 * @param p_source The source identifier.
 * @return The resolved source file path.
 */

/**
 * @brief Clears all existing breakpoints.
 */

/**
 * @brief Sets whether breakpoints should be skipped during execution.
 * @param p_skip_breakpoints True to skip breakpoints, false to respect them.
 */

/**
 * @brief Checks if breakpoints are currently being skipped.
 * @return True if breakpoints are being skipped, false otherwise.
 */

/**
 * @brief Initiates a debug session for the specified script language.
 * @param p_lang The script language to debug.
 * @param p_can_continue Whether execution can continue after the debug break.
 * @param p_is_error_breakpoint Whether this break was triggered by an error.
 */

/**
 * @brief Sends error information to the engine debugger with stack information.
 * @param p_func The function name where the error occurred.
 * @param p_file The file where the error occurred.
 * @param p_line The line number where the error occurred.
 * @param p_err The error message.
 * @param p_descr The error description.
 * @param p_type The type of error handler.
 * @param p_stack_info The stack information associated with the error.
 */

/**
 * @brief Retrieves the stack information from the most recent error.
 * @return A vector containing the error stack information.
 */

/**
 * @brief Gets the script language that is currently in a debug break.
 * @return Pointer to the script language in debug mode, or nullptr if none.
 */
#include "script_debugger.h"

#include "core/debugger/engine_debugger.h"

void ScriptDebugger::set_lines_left(int p_left) {

	lines_left = p_left;
}

int ScriptDebugger::get_lines_left() const {

	return lines_left;
}

void ScriptDebugger::set_depth(int p_depth) {

	depth = p_depth;
}

int ScriptDebugger::get_depth() const {

	return depth;
}

void ScriptDebugger::insert_breakpoint(int p_line, const StringName &p_source) {

	if (!breakpoints.has(p_line))
		breakpoints[p_line] = Set<StringName>();
	breakpoints[p_line].insert(p_source);
}

void ScriptDebugger::remove_breakpoint(int p_line, const StringName &p_source) {

	if (!breakpoints.has(p_line))
		return;

	breakpoints[p_line].erase(p_source);
	if (breakpoints[p_line].size() == 0)
		breakpoints.erase(p_line);
}
bool ScriptDebugger::is_breakpoint(int p_line, const StringName &p_source) const {

	if (!breakpoints.has(p_line))
		return false;
	return breakpoints[p_line].has(p_source);
}
bool ScriptDebugger::is_breakpoint_line(int p_line) const {

	return breakpoints.has(p_line);
}

String ScriptDebugger::breakpoint_find_source(const String &p_source) const {

	return p_source;
}

void ScriptDebugger::clear_breakpoints() {

	breakpoints.clear();
}

void ScriptDebugger::set_skip_breakpoints(bool p_skip_breakpoints) {

	skip_breakpoints = p_skip_breakpoints;
}

bool ScriptDebugger::is_skipping_breakpoints() {

	return skip_breakpoints;
}

void ScriptDebugger::debug(ScriptLanguage *p_lang, bool p_can_continue, bool p_is_error_breakpoint) {
	ScriptLanguage *prev = break_lang;
	break_lang = p_lang;
	EngineDebugger::get_singleton()->debug(p_can_continue, p_is_error_breakpoint);
	break_lang = prev;
}

void ScriptDebugger::send_error(const String &p_func, const String &p_file, int p_line, const String &p_err, const String &p_descr, ErrorHandlerType p_type, const Vector<StackInfo> &p_stack_info) {
	// Store stack info, this is ugly, but allows us to separate EngineDebugger and ScriptDebugger. There might be a better way.
	error_stack_info.append_array(p_stack_info);
	EngineDebugger::get_singleton()->send_error(p_func, p_file, p_line, p_err, p_descr, p_type);
	error_stack_info.resize(0);
}

Vector<ScriptLanguage::StackInfo> ScriptDebugger::get_error_stack_info() const {
	return error_stack_info;
}

ScriptLanguage *ScriptDebugger::get_break_language() const {

	return break_lang;
}

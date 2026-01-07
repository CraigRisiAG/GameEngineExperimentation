

/**
 * @class ScriptDebugger
 * @brief Manages script debugging functionality including breakpoints, execution flow control, and error handling.
 * 
 * ScriptDebugger provides a comprehensive debugging interface for script languages. It handles breakpoint
 * management, execution stepping, language-specific debugging operations, and error stack tracking.
 * 
 * @details
 * The debugger maintains a collection of breakpoints organized by line number and source file, tracks
 * execution depth and remaining lines before stopping, and manages the currently active debugging language.
 * It supports operations such as stepping through code, skipping breakpoints, and capturing error information
 * with full stack traces.
 * 
 * @note This class works in conjunction with ScriptLanguage implementations to provide language-specific
 * debugging capabilities.
 * 
 * @see ScriptLanguage
 */
#ifndef SCRIPT_DEBUGGER_H
#define SCRIPT_DEBUGGER_H

#include "core/map.h"
#include "core/script_language.h"
#include "core/set.h"
#include "core/string_name.h"
#include "core/vector.h"

class ScriptDebugger {

	typedef ScriptLanguage::StackInfo StackInfo;

	int lines_left = -1;
	int depth = -1;
	bool skip_breakpoints = false;

	Map<int, Set<StringName>> breakpoints;

	ScriptLanguage *break_lang = NULL;
	Vector<StackInfo> error_stack_info;

public:
	void set_lines_left(int p_left);
	int get_lines_left() const;

	void set_depth(int p_depth);
	int get_depth() const;

	String breakpoint_find_source(const String &p_source) const;
	void set_break_language(ScriptLanguage *p_lang) { break_lang = p_lang; }
	ScriptLanguage *get_break_language() { return break_lang; }
	void set_skip_breakpoints(bool p_skip_breakpoints);
	bool is_skipping_breakpoints();
	void insert_breakpoint(int p_line, const StringName &p_source);
	void remove_breakpoint(int p_line, const StringName &p_source);
	bool is_breakpoint(int p_line, const StringName &p_source) const;
	bool is_breakpoint_line(int p_line) const;
	void clear_breakpoints();
	const Map<int, Set<StringName>> &get_breakpoints() const { return breakpoints; }

	void debug(ScriptLanguage *p_lang, bool p_can_continue = true, bool p_is_error_breakpoint = false);
	ScriptLanguage *get_break_language() const;

	void send_error(const String &p_func, const String &p_file, int p_line, const String &p_err, const String &p_descr, ErrorHandlerType p_type, const Vector<StackInfo> &p_stack_info);
	Vector<StackInfo> get_error_stack_info() const;
	ScriptDebugger() {}
};

#endif // SCRIPT_DEBUGGER_H

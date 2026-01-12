

/// @file print_string.h
/// @brief Print handler system for managing string output with custom handlers.
///
/// This header provides a flexible printing system that allows registering custom
/// print handlers to intercept and process string output. It supports different
/// print levels (normal, error, verbose) with individual enable/disable flags.
///
/// @details
/// - Custom print handlers can be added/removed dynamically via a linked list
/// - Each handler receives the string, user-defined data, and error flag
/// - Global print function pointer can be overridden for default behavior
/// - Supports conditional printing based on enable flags
#ifndef PRINT_STRING_H
#define PRINT_STRING_H

#include "core/ustring.h"

extern void (*_print_func)(String);

typedef void (*PrintHandlerFunc)(void *, const String &p_string, bool p_error);

struct PrintHandlerList {

	PrintHandlerFunc printfunc;
	void *userdata;

	PrintHandlerList *next;

	PrintHandlerList() {
		printfunc = 0;
		next = 0;
		userdata = 0;
	}
};

void add_print_handler(PrintHandlerList *p_handler);
void remove_print_handler(PrintHandlerList *p_handler);

extern bool _print_line_enabled;
extern bool _print_error_enabled;
extern void print_line(String p_string);
extern void print_error(String p_string);
extern void print_verbose(String p_string);

#endif // PRINT_STRING_H

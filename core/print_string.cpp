

/**
 * @file print_string.cpp
 * @brief Print string handler implementation for managing console output and custom print handlers.
 * 
 * This module provides functionality for printing strings to the console and distributing
 * output to registered custom print handlers. It maintains a linked list of handler callbacks
 * that are invoked whenever print_line() or print_error() is called.
 */

/**
 * @brief Registers a new print handler to receive output notifications.
 * @param p_handler Pointer to the PrintHandlerList node to add to the handler chain.
 * 
 * Adds the provided handler to the front of the print handler list. The handler will
 * receive callbacks for both regular prints and error messages. Thread-safe operation
 * is ensured through global locking.
 */

/**
 * @brief Unregisters a print handler from the handler chain.
 * @param p_handler Pointer to the PrintHandlerList node to remove.
 * 
 * Removes the specified handler from the print handler linked list. If the handler
 * is not found in the list, an error condition is triggered. Thread-safe operation
 * is ensured through global locking.
 */

/**
 * @brief Prints a line of text to the console and all registered handlers.
 * @param p_string The string to print.
 * 
 * Outputs the string to the OS console with a newline, then distributes the message
 * to all registered print handlers with the error flag set to false. Can be disabled
 * via the _print_line_enabled flag. Thread-safe operation is ensured through global locking.
 */

/**
 * @brief Prints an error message to the console and all registered handlers.
 * @param p_string The error string to print.
 * 
 * Outputs the string to the OS error stream with a newline, then distributes the message
 * to all registered print handlers with the error flag set to true. Can be disabled
 * via the _print_error_enabled flag. Thread-safe operation is ensured through global locking.
 */

/**
 * @brief Conditionally prints a verbose message if verbose mode is enabled.
 * @param p_string The message to print verbosely.
 * 
 * Checks if the OS is in verbose stdout mode. If enabled, delegates to print_line()
 * to output the message. Useful for debug or detailed logging information.
 */
#include "print_string.h"

#include "core/os/os.h"

#include <stdio.h>

static PrintHandlerList *print_handler_list = NULL;
bool _print_line_enabled = true;
bool _print_error_enabled = true;

void add_print_handler(PrintHandlerList *p_handler) {

	_global_lock();
	p_handler->next = print_handler_list;
	print_handler_list = p_handler;
	_global_unlock();
}

void remove_print_handler(PrintHandlerList *p_handler) {

	_global_lock();

	PrintHandlerList *prev = NULL;
	PrintHandlerList *l = print_handler_list;

	while (l) {

		if (l == p_handler) {

			if (prev)
				prev->next = l->next;
			else
				print_handler_list = l->next;
			break;
		}
		prev = l;
		l = l->next;
	}
	//OS::get_singleton()->print("print handler list is %p\n",print_handler_list);

	_global_unlock();
	ERR_FAIL_COND(l == NULL);
}

void print_line(String p_string) {

	if (!_print_line_enabled)
		return;

	OS::get_singleton()->print("%s\n", p_string.utf8().get_data());

	_global_lock();
	PrintHandlerList *l = print_handler_list;
	while (l) {

		l->printfunc(l->userdata, p_string, false);
		l = l->next;
	}

	_global_unlock();
}

void print_error(String p_string) {

	if (!_print_error_enabled)
		return;

	OS::get_singleton()->printerr("%s\n", p_string.utf8().get_data());

	_global_lock();
	PrintHandlerList *l = print_handler_list;
	while (l) {

		l->printfunc(l->userdata, p_string, true);
		l = l->next;
	}

	_global_unlock();
}

void print_verbose(String p_string) {

	if (OS::get_singleton()->is_stdout_verbose()) {
		print_line(p_string);
	}
}

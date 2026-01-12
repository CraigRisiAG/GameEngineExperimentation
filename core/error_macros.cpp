


/**
 * @file error_macros.cpp
 * @brief Error handling and reporting system implementation.
 * 
 * This module provides a centralized error handling mechanism with support for
 * custom error handlers, logging, and detailed error reporting with file location
 * and line number information.
 */

/**
 * @brief Registers a custom error handler to the global error handler list.
 * 
 * Adds the provided error handler to the linked list of handlers that will be
 * invoked when errors are printed. Uses global locking to ensure thread safety.
 * 
 * @param p_handler Pointer to the ErrorHandlerList structure to add.
 * 
 * @note The handler is added to the front of the list.
 */

/**
 * @brief Removes a custom error handler from the global error handler list.
 * 
 * Searches for and removes the specified error handler from the linked list.
 * Uses global locking to ensure thread safety during removal.
 * 
 * @param p_handler Pointer to the ErrorHandlerList structure to remove.
 * 
 * @note Safely handles removal of any handler in the list, including the head.
 */

/**
 * @brief Prints an error message with automatic error type inference.
 * 
 * Overloaded version that accepts a C-style error string and delegates to the
 * full implementation with an empty message parameter.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_error Error description (C-style string).
 * @param p_type Type of error (ErrorHandlerType enum).
 */

/**
 * @brief Prints an error message from a String object.
 * 
 * Overloaded version that accepts a String object for the error description.
 * Converts to UTF-8 and delegates to the C-string implementation.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_error Error description (String object).
 * @param p_type Type of error (ErrorHandlerType enum).
 */

/**
 * @brief Core error reporting function with full error and message details.
 * 
 * Handles error reporting by notifying the OS, logging to the global logger,
 * and invoking all registered custom error handlers in sequence. Uses global
 * locking to ensure thread-safe handler invocation.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_error Error description (C-style string).
 * @param p_message Additional error message (C-style string).
 * @param p_type Type of error (ErrorHandlerType enum).
 */

/**
 * @brief Prints an error with String error and C-string message.
 * 
 * Converts String error to UTF-8 and delegates to the core implementation.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_error Error description (String object).
 * @param p_message Additional error message (C-style string).
 * @param p_type Type of error (ErrorHandlerType enum).
 */

/**
 * @brief Prints an error with C-string error and String message.
 * 
 * Converts String message to UTF-8 and delegates to the core implementation.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_error Error description (C-style string).
 * @param p_message Additional error message (String object).
 * @param p_type Type of error (ErrorHandlerType enum).
 */

/**
 * @brief Prints an error with both String error and String message.
 * 
 * Converts both String parameters to UTF-8 and delegates to the core implementation.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_error Error description (String object).
 * @param p_message Additional error message (String object).
 * @param p_type Type of error (ErrorHandlerType enum).
 */

/**
 * @brief Reports an out-of-bounds index error with detailed context.
 * 
 * Generates a formatted error message describing an index that exceeded bounds,
 * and reports it via the standard error reporting mechanism. Optionally marks
 * the error as fatal.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_index The invalid index value.
 * @param p_size The valid size/boundary.
 * @param p_index_str String representation of the index variable name.
 * @param p_size_str String representation of the size variable name.
 * @param p_message Additional error context (C-style string).
 * @param fatal Whether this is a fatal error.
 */

/**
 * @brief Reports an out-of-bounds index error with String message.
 * 
 * Overloaded version that accepts a String object for the message parameter.
 * Converts to UTF-8 and delegates to the C-string implementation.
 * 
 * @param p_function Name of the function where the error occurred.
 * @param p_file Path to the source file where the error occurred.
 * @param p_line Line number where the error occurred.
 * @param p_index The invalid index value.
 * @param p_size The valid size/boundary.
 * @param p_index_str String representation of the index variable name.
 * @param p_size_str String representation of the size variable name.
 * @param p_message Additional error context (String object).
 * @param fatal Whether this is a fatal error.
 */
#include "error_macros.h"

#include "core/io/logger.h"
#include "core/ustring.h"
#include "os/os.h"

static ErrorHandlerList *error_handler_list = NULL;

void add_error_handler(ErrorHandlerList *p_handler) {

	_global_lock();
	p_handler->next = error_handler_list;
	error_handler_list = p_handler;
	_global_unlock();
}

void remove_error_handler(ErrorHandlerList *p_handler) {

	_global_lock();

	ErrorHandlerList *prev = NULL;
	ErrorHandlerList *l = error_handler_list;

	while (l) {

		if (l == p_handler) {

			if (prev)
				prev->next = l->next;
			else
				error_handler_list = l->next;
			break;
		}
		prev = l;
		l = l->next;
	}

	_global_unlock();
}

void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, ErrorHandlerType p_type) {
	_err_print_error(p_function, p_file, p_line, p_error, "", p_type);
}

void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, ErrorHandlerType p_type) {
	_err_print_error(p_function, p_file, p_line, p_error.utf8().get_data(), "", p_type);
}

void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, const char *p_message, ErrorHandlerType p_type) {

	OS::get_singleton()->print_error(p_function, p_file, p_line, p_error, p_message, (Logger::ErrorType)p_type);

	_global_lock();
	ErrorHandlerList *l = error_handler_list;
	while (l) {

		l->errfunc(l->userdata, p_function, p_file, p_line, p_error, p_message, p_type);
		l = l->next;
	}

	_global_unlock();
}

void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, const char *p_message, ErrorHandlerType p_type) {
	_err_print_error(p_function, p_file, p_line, p_error.utf8().get_data(), p_message, p_type);
}

void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, const String &p_message, ErrorHandlerType p_type) {
	_err_print_error(p_function, p_file, p_line, p_error, p_message.utf8().get_data(), p_type);
}

void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, const String &p_message, ErrorHandlerType p_type) {
	_err_print_error(p_function, p_file, p_line, p_error.utf8().get_data(), p_message.utf8().get_data(), p_type);
}

void _err_print_index_error(const char *p_function, const char *p_file, int p_line, int64_t p_index, int64_t p_size, const char *p_index_str, const char *p_size_str, const char *p_message, bool fatal) {

	String fstr(fatal ? "FATAL: " : "");
	String err(fstr + "Index " + p_index_str + " = " + itos(p_index) + " is out of bounds (" + p_size_str + " = " + itos(p_size) + ").");
	_err_print_error(p_function, p_file, p_line, err.utf8().get_data(), p_message);
}

void _err_print_index_error(const char *p_function, const char *p_file, int p_line, int64_t p_index, int64_t p_size, const char *p_index_str, const char *p_size_str, const String &p_message, bool fatal) {
	_err_print_index_error(p_function, p_file, p_line, p_index, p_size, p_index_str, p_size_str, p_message.utf8().get_data(), fatal);
}

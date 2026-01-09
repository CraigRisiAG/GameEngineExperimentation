

/**
 * @file logger.h
 * @brief Logging system for the game engine with support for multiple output targets.
 * 
 * This header defines a flexible logging architecture that allows messages to be directed
 * to different outputs (stdout/stderr, files, or multiple destinations simultaneously).
 * 
 * @class Logger
 * @brief Abstract base class for all logging implementations.
 * 
 * Provides the interface for logging messages with support for formatted output and error tracking.
 * 
 * @enum ErrorType
 * @brief Categorizes the type of error or message being logged.
 * @var ERR_ERROR Standard error message
 * @var ERR_WARNING Warning message
 * @var ERR_SCRIPT Script-related error
 * @var ERR_SHADER Shader compilation or runtime error
 * 
 * @method bool should_log(bool p_err)
 * @brief Determines if a message should be logged based on type and logging settings.
 * @param p_err Whether this is an error message
 * @return true if the message should be logged, false otherwise
 * 
 * @method virtual void logv(const char *p_format, va_list p_list, bool p_err)
 * @brief Core logging function using va_list for formatted output.
 * @param p_format Printf-style format string
 * @param p_list Variable argument list
 * @param p_err Whether this is an error stream message
 * 
 * @method virtual void log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, ErrorType p_type)
 * @brief Logs detailed error information with context.
 * @param p_function Name of the function where error occurred
 * @param p_file Source file path
 * @param p_line Line number in source file
 * @param p_code The problematic code or condition
 * @param p_rationale Explanation of why this is an error
 * @param p_type Category of error (default: ERR_ERROR)
 * 
 * @method void logf(const char *p_format, ...)
 * @brief Convenience function for formatted logging to standard output.
 * @param p_format Printf-style format string
 * @param ... Variable arguments
 * 
 * @method void logf_error(const char *p_format, ...)
 * @brief Convenience function for formatted logging to error output.
 * @param p_format Printf-style format string
 * @param ... Variable arguments
 * 
 * @class StdLogger
 * @brief Writes log messages to standard output (stdout) or standard error (stderr).
 * 
 * Simple logger implementation that directs output to console streams.
 * 
 * @class RotatedFileLogger
 * @brief Writes log messages to a file with automatic rotation and backup management.
 * 
 * When the log file reaches a size threshold or on new session, it's rotated
 * (renamed with timestamp) and a new log file is created. Old backups beyond
 * the maximum count are automatically deleted.
 * 
 * @param p_base_path Base file path for the log file (without extension)
 * @param p_max_files Maximum number of backup files to retain (default: 10)
 * 
 * @class CompositeLogger
 * @brief Aggregates multiple loggers to write messages to multiple destinations simultaneously.
 * 
 * Useful for logging to both file and console, or any combination of output targets.
 * 
 * @param p_loggers Vector of logger instances to manage
 * 
 * @method void add_logger(Logger *p_logger)
 * @brief Dynamically adds a logger to the composite set.
 * @param p_logger Pointer to the logger instance to add
 */
#ifndef LOGGER_H
#define LOGGER_H

#include "core/os/file_access.h"
#include "core/ustring.h"
#include "core/vector.h"

#include <stdarg.h>

class Logger {
protected:
	bool should_log(bool p_err);

public:
	enum ErrorType {
		ERR_ERROR,
		ERR_WARNING,
		ERR_SCRIPT,
		ERR_SHADER
	};

	virtual void logv(const char *p_format, va_list p_list, bool p_err) _PRINTF_FORMAT_ATTRIBUTE_2_0 = 0;
	virtual void log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, ErrorType p_type = ERR_ERROR);

	void logf(const char *p_format, ...) _PRINTF_FORMAT_ATTRIBUTE_2_3;
	void logf_error(const char *p_format, ...) _PRINTF_FORMAT_ATTRIBUTE_2_3;

	virtual ~Logger();
};

/**
 * Writes messages to stdout/stderr.
 */
class StdLogger : public Logger {

public:
	virtual void logv(const char *p_format, va_list p_list, bool p_err) _PRINTF_FORMAT_ATTRIBUTE_2_0;
	virtual ~StdLogger();
};

/**
 * Writes messages to the specified file. If the file already exists, creates a copy (backup)
 * of it with timestamp appended to the file name. Maximum number of backups is configurable.
 * When maximum is reached, the oldest backups are erased. With the maximum being equal to 1,
 * it acts as a simple file logger.
 */
class RotatedFileLogger : public Logger {
	String base_path;
	int max_files;

	FileAccess *file;

	void rotate_file_without_closing();
	void close_file();
	void clear_old_backups();
	void rotate_file();

public:
	RotatedFileLogger(const String &p_base_path, int p_max_files = 10);

	virtual void logv(const char *p_format, va_list p_list, bool p_err) _PRINTF_FORMAT_ATTRIBUTE_2_0;

	virtual ~RotatedFileLogger();
};

class CompositeLogger : public Logger {
	Vector<Logger *> loggers;

public:
	CompositeLogger(Vector<Logger *> p_loggers);

	virtual void logv(const char *p_format, va_list p_list, bool p_err) _PRINTF_FORMAT_ATTRIBUTE_2_0;
	virtual void log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, ErrorType p_type = ERR_ERROR);

	void add_logger(Logger *p_logger);

	virtual ~CompositeLogger();
};

#endif // LOGGER_H

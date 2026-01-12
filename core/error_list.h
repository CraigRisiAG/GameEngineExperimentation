

/**
 * @file error_list.h
 * @brief Comprehensive error enumeration for the game engine.
 * 
 * Defines a standardized set of error codes used throughout the engine for consistent
 * error reporting and handling. This generic error list provides a language for returning
 * detailed error information.
 * 
 * @note When checking error results, use `result != OK` or `!result` comparisons.
 *       Do NOT compare directly against FAILED, as error codes may become more detailed
 *       in future versions.
 * 
 * @enum Error
 * @brief Standard error codes for the game engine.
 * 
 * @var Error::OK
 * Operation completed successfully. Value: 0.
 * 
 * @var Error::FAILED
 * Generic failure. Use only when a more specific error code is unavailable.
 * 
 * @var Error::ERR_UNAVAILABLE
 * Requested feature is unsupported or unavailable on the current platform/configuration.
 * 
 * @var Error::ERR_UNCONFIGURED
 * Object has not been properly initialized or configured before use.
 * 
 * @var Error::ERR_UNAUTHORIZED
 * User lacks necessary credentials or permissions for the requested operation.
 * 
 * @var Error::ERR_PARAMETER_RANGE_ERROR
 * Supplied parameter is outside acceptable range. Value: 5.
 * 
 * @var Error::ERR_OUT_OF_MEMORY
 * Insufficient memory to complete operation.
 * 
 * @var Error::ERR_FILE_NOT_FOUND
 * Requested file does not exist.
 * 
 * @var Error::ERR_FILE_BAD_DRIVE
 * File operation failed due to invalid or inaccessible drive.
 * 
 * @var Error::ERR_FILE_BAD_PATH
 * Specified file path is invalid or malformed.
 * 
 * @var Error::ERR_FILE_NO_PERMISSION
 * Insufficient permissions to access file. Value: 10.
 * 
 * @var Error::ERR_FILE_ALREADY_IN_USE
 * File is locked or already in use by another process.
 * 
 * @var Error::ERR_FILE_CANT_OPEN
 * Failed to open file.
 * 
 * @var Error::ERR_FILE_CANT_WRITE
 * Failed to write to file.
 * 
 * @var Error::ERR_FILE_CANT_READ
 * Failed to read from file.
 * 
 * @var Error::ERR_FILE_UNRECOGNIZED
 * File format is not recognized. Value: 15.
 * 
 * @var Error::ERR_FILE_CORRUPT
 * File data is corrupted or invalid.
 * 
 * @var Error::ERR_FILE_MISSING_DEPENDENCIES
 * File references missing dependencies.
 * 
 * @var Error::ERR_FILE_EOF
 * Unexpected end-of-file encountered.
 * 
 * @var Error::ERR_CANT_OPEN
 * Cannot open resource, socket, or file.
 * 
 * @var Error::ERR_CANT_CREATE
 * Failed to create resource. Value: 20.
 * 
 * @var Error::ERR_QUERY_FAILED
 * Database or resource query operation failed.
 * 
 * @var Error::ERR_ALREADY_IN_USE
 * Resource is currently in use.
 * 
 * @var Error::ERR_LOCKED
 * Resource is locked and unavailable.
 * 
 * @var Error::ERR_TIMEOUT
 * Operation exceeded maximum allowed time.
 * 
 * @var Error::ERR_CANT_CONNECT
 * Failed to establish connection. Value: 25.
 * 
 * @var Error::ERR_CANT_RESOLVE
 * Failed to resolve network address or resource identifier.
 * 
 * @var Error::ERR_CONNECTION_ERROR
 * Connection failed or was interrupted.
 * 
 * @var Error::ERR_CANT_ACQUIRE_RESOURCE
 * Unable to acquire necessary resource.
 * 
 * @var Error::ERR_CANT_FORK
 * Process fork operation failed.
 * 
 * @var Error::ERR_INVALID_DATA
 * Provided data is invalid or malformed. Value: 30.
 * 
 * @var Error::ERR_INVALID_PARAMETER
 * Supplied parameter is invalid.
 * 
 * @var Error::ERR_ALREADY_EXISTS
 * Item already exists when attempting to add.
 * 
 * @var Error::ERR_DOES_NOT_EXIST
 * Item does not exist when attempting retrieval or deletion.
 * 
 * @var Error::ERR_DATABASE_CANT_READ
 * Database read operation failed.
 * 
 * @var Error::ERR_DATABASE_CANT_WRITE
 * Database write operation failed. Value: 35.
 * 
 * @var Error::ERR_COMPILATION_FAILED
 * Compilation process failed.
 * 
 * @var Error::ERR_METHOD_NOT_FOUND
 * Requested method does not exist.
 * 
 * @var Error::ERR_LINK_FAILED
 * Linking operation failed.
 * 
 * @var Error::ERR_SCRIPT_FAILED
 * Script execution failed.
 * 
 * @var Error::ERR_CYCLIC_LINK
 * Circular dependency detected. Value: 40.
 * 
 * @var Error::ERR_INVALID_DECLARATION
 * Declaration is invalid or malformed.
 * 
 * @var Error::ERR_DUPLICATE_SYMBOL
 * Duplicate symbol definition detected.
 * 
 * @var Error::ERR_PARSE_ERROR
 * Parsing operation failed.
 * 
 * @var Error::ERR_BUSY
 * Resource is busy; operation cannot be performed now.
 * 
 * @var Error::ERR_SKIP
 * Operation should be skipped. Value: 45.
 * 
 * @var Error::ERR_HELP
 * User requested help information.
 * 
 * @var Error::ERR_BUG
 * Critical bug detected - validation check or assertion failed unexpectedly.
 * 
 * @var Error::ERR_PRINTER_ON_FIRE
 * Hardware failure or catastrophic error (parallel port printer malfunction).
 */
#ifndef ERROR_LIST_H
#define ERROR_LIST_H

/** Error List. Please never compare an error against FAILED
 * Either do result != OK , or !result. This way, Error fail
 * values can be more detailed in the future.
 *
 * This is a generic error list, mainly for organizing a language of returning errors.
 */

enum Error {
	OK, // (0)
	FAILED, ///< Generic fail error
	ERR_UNAVAILABLE, ///< What is requested is unsupported/unavailable
	ERR_UNCONFIGURED, ///< The object being used hasn't been properly set up yet
	ERR_UNAUTHORIZED, ///< Missing credentials for requested resource
	ERR_PARAMETER_RANGE_ERROR, ///< Parameter given out of range (5)
	ERR_OUT_OF_MEMORY, ///< Out of memory
	ERR_FILE_NOT_FOUND,
	ERR_FILE_BAD_DRIVE,
	ERR_FILE_BAD_PATH,
	ERR_FILE_NO_PERMISSION, // (10)
	ERR_FILE_ALREADY_IN_USE,
	ERR_FILE_CANT_OPEN,
	ERR_FILE_CANT_WRITE,
	ERR_FILE_CANT_READ,
	ERR_FILE_UNRECOGNIZED, // (15)
	ERR_FILE_CORRUPT,
	ERR_FILE_MISSING_DEPENDENCIES,
	ERR_FILE_EOF,
	ERR_CANT_OPEN, ///< Can't open a resource/socket/file
	ERR_CANT_CREATE, // (20)
	ERR_QUERY_FAILED,
	ERR_ALREADY_IN_USE,
	ERR_LOCKED, ///< resource is locked
	ERR_TIMEOUT,
	ERR_CANT_CONNECT, // (25)
	ERR_CANT_RESOLVE,
	ERR_CONNECTION_ERROR,
	ERR_CANT_ACQUIRE_RESOURCE,
	ERR_CANT_FORK,
	ERR_INVALID_DATA, ///< Data passed is invalid (30)
	ERR_INVALID_PARAMETER, ///< Parameter passed is invalid
	ERR_ALREADY_EXISTS, ///< When adding, item already exists
	ERR_DOES_NOT_EXIST, ///< When retrieving/erasing, if item does not exist
	ERR_DATABASE_CANT_READ, ///< database is full
	ERR_DATABASE_CANT_WRITE, ///< database is full (35)
	ERR_COMPILATION_FAILED,
	ERR_METHOD_NOT_FOUND,
	ERR_LINK_FAILED,
	ERR_SCRIPT_FAILED,
	ERR_CYCLIC_LINK, // (40)
	ERR_INVALID_DECLARATION,
	ERR_DUPLICATE_SYMBOL,
	ERR_PARSE_ERROR,
	ERR_BUSY,
	ERR_SKIP, // (45)
	ERR_HELP, ///< user requested help!!
	ERR_BUG, ///< a bug in the software certainly happened, due to a double check failing or unexpected behavior.
	ERR_PRINTER_ON_FIRE, /// the parallel port printer is engulfed in flames
};

#endif // ERROR_LIST_H

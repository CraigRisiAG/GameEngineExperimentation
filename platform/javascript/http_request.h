
/**
 * @file http_request.h
 * @brief HTTP request interface for JavaScript/Web platform
 * 
 * This header defines the C API for XMLHttpRequest (XHR) functionality on the JavaScript platform.
 * It provides functions to create, configure, send HTTP requests, and retrieve responses.
 */

/**
 * @enum godot_xhr_ready_state_t
 * @brief Enumeration of XMLHttpRequest ready states
 * 
 * Represents the current state of an XMLHttpRequest object throughout its lifecycle.
 * 
 * @var XHR_READY_STATE_UNSENT
 *      Request has been created but not yet opened
 * @var XHR_READY_STATE_OPENED
 *      open() has been called
 * @var XHR_READY_STATE_HEADERS_RECEIVED
 *      send() has been called and response headers are available
 * @var XHR_READY_STATE_LOADING
 *      Response body is being received
 * @var XHR_READY_STATE_DONE
 *      Request operation is complete
 */

/**
 * @brief Create a new XMLHttpRequest instance
 * @return ID of the newly created XHR object
 */

/**
 * @brief Reset an XMLHttpRequest to its initial state
 * @param p_xhr_id ID of the XHR object to reset
 */

/**
 * @brief Free resources associated with an XMLHttpRequest
 * @param p_xhr_id ID of the XHR object to free
 * @return true if successfully freed, false otherwise
 */

/**
 * @brief Initialize an HTTP request
 * @param p_xhr_id ID of the XHR object
 * @param p_method HTTP method (e.g., "GET", "POST")
 * @param p_url Target URL
 * @param p_user Optional username for authentication
 * @param p_password Optional password for authentication
 * @return Status code of the open operation
 */

/**
 * @brief Set a request header
 * @param p_xhr_id ID of the XHR object
 * @param p_header Header name
 * @param p_value Header value
 */

/**
 * @brief Send HTTP request with no body
 * @param p_xhr_id ID of the XHR object
 */

/**
 * @brief Send HTTP request with string body
 * @param p_xhr_id ID of the XHR object
 * @param p_data String data to send
 */

/**
 * @brief Send HTTP request with binary data
 * @param p_xhr_id ID of the XHR object
 * @param p_data Pointer to binary data
 * @param p_len Length of the data in bytes
 */

/**
 * @brief Abort an in-progress request
 * @param p_xhr_id ID of the XHR object
 */

/**
 * @brief Get the HTTP response status code
 * @param p_xhr_id ID of the XHR object
 * @return HTTP status code (HTTPClient::ResponseCode)
 */

/**
 * @brief Get the current ready state of the request
 * @param p_xhr_id ID of the XHR object
 * @return Current godot_xhr_ready_state_t value
 */

/**
 * @brief Get the total length of response headers
 * @param p_xhr_id ID of the XHR object
 * @return Length of response headers in bytes
 */

/**
 * @brief Retrieve response headers
 * @param p_xhr_id ID of the XHR object
 * @param r_dst Destination buffer for headers
 * @param p_len Maximum length to read
 */

/**
 * @brief Get the total length of the response body
 * @param p_xhr_id ID of the XHR object
 * @return Length of response body in bytes
 */

/**
 * @brief Retrieve response body
 * @param p_xhr_id ID of the XHR object
 * @param r_dst Destination buffer for response data
 * @param p_len Maximum length to read
 */
#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"

typedef enum {
	XHR_READY_STATE_UNSENT = 0,
	XHR_READY_STATE_OPENED = 1,
	XHR_READY_STATE_HEADERS_RECEIVED = 2,
	XHR_READY_STATE_LOADING = 3,
	XHR_READY_STATE_DONE = 4,
} godot_xhr_ready_state_t;

extern int godot_xhr_new();
extern void godot_xhr_reset(int p_xhr_id);
extern bool godot_xhr_free(int p_xhr_id);

extern int godot_xhr_open(int p_xhr_id, const char *p_method, const char *p_url, const char *p_user = NULL, const char *p_password = NULL);

extern void godot_xhr_set_request_header(int p_xhr_id, const char *p_header, const char *p_value);

extern void godot_xhr_send_null(int p_xhr_id);
extern void godot_xhr_send_string(int p_xhr_id, const char *p_data);
extern void godot_xhr_send_data(int p_xhr_id, const void *p_data, int p_len);
extern void godot_xhr_abort(int p_xhr_id);

/* this is an HTTPClient::ResponseCode, not ::Status */
extern int godot_xhr_get_status(int p_xhr_id);
extern godot_xhr_ready_state_t godot_xhr_get_ready_state(int p_xhr_id);

extern int godot_xhr_get_response_headers_length(int p_xhr_id);
extern void godot_xhr_get_response_headers(int p_xhr_id, char *r_dst, int p_len);

extern int godot_xhr_get_response_length(int p_xhr_id);
extern void godot_xhr_get_response(int p_xhr_id, void *r_dst, int p_len);

#ifdef __cplusplus
}
#endif

#endif /* HTTP_REQUEST_H */

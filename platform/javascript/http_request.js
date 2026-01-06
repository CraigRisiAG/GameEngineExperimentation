
/**
 * Godot HTTP Request Module
 * Provides a wrapper around XMLHttpRequest for Godot engine's HTTP functionality
 * 
 * @namespace GodotHTTPRequest
 */

/**
 * Internal storage for XMLHttpRequest objects
 * @type {Array<XMLHttpRequest|null>}
 */

/**
 * Finds an unused request ID slot or creates a new one
 * @returns {number} Unused request ID
 */

/**
 * Configures an XMLHttpRequest with Godot-specific settings
 * @param {XMLHttpRequest} xhr - The XMLHttpRequest object to configure
 */

/**
 * Creates a new XMLHttpRequest and returns its ID
 * @returns {number} The ID of the new request
 */

/**
 * Resets an existing XMLHttpRequest with a new instance
 * @param {number} xhrId - The ID of the request to reset
 */

/**
 * Aborts and frees an XMLHttpRequest
 * @param {number} xhrId - The ID of the request to free
 */

/**
 * Opens an XMLHttpRequest with the specified parameters
 * @param {number} xhrId - The ID of the request
 * @param {number} method - Pointer to the HTTP method string
 * @param {number} url - Pointer to the URL string
 * @param {number} user - Pointer to the username string (0 if null)
 * @param {number} password - Pointer to the password string (0 if null)
 */

/**
 * Sets a request header on the XMLHttpRequest
 * @param {number} xhrId - The ID of the request
 * @param {number} header - Pointer to the header name string
 * @param {number} value - Pointer to the header value string
 */

/**
 * Sends the XMLHttpRequest with no body
 * @param {number} xhrId - The ID of the request
 */

/**
 * Sends the XMLHttpRequest with a string body
 * @param {number} xhrId - The ID of the request
 * @param {number} strPtr - Pointer to the request body string
 */

/**
 * Sends the XMLHttpRequest with binary data
 * @param {number} xhrId - The ID of the request
 * @param {number} ptr - Pointer to the data buffer
 * @param {number} len - Length of the data buffer in bytes
 */

/**
 * Aborts an XMLHttpRequest
 * @param {number} xhrId - The ID of the request
 */

/**
 * Gets the HTTP status code of the response
 * @param {number} xhrId - The ID of the request
 * @returns {number} The HTTP status code
 */

/**
 * Gets the ready state of the XMLHttpRequest
 * @param {number} xhrId - The ID of the request
 * @returns {number} The ready state (0-4)
 */

/**
 * Gets the byte length of the response headers string
 * @param {number} xhrId - The ID of the request
 * @returns {number} The length in bytes of the response headers
 */

/**
 * Copies response headers to the destination buffer
 * @param {number} xhrId - The ID of the request
 * @param {number} dst - Pointer to the destination buffer
 * @param {number} len - Maximum length to copy
 */

/**
 * Gets the byte length of the response body
 * @param {number} xhrId - The ID of the request
 * @returns {number} The length in bytes of the response body
 */

/**
 * Copies the response body to the destination buffer
 * @param {number} xhrId - The ID of the request
 * @param {number} dst - Pointer to the destination buffer
 * @param {number} len - Maximum length to copy
 */
var GodotHTTPRequest = {

	$GodotHTTPRequest: {

		requests: [],

		getUnusedRequestId: function() {
			var idMax = GodotHTTPRequest.requests.length;
			for (var potentialId = 0; potentialId < idMax; ++potentialId) {
				if (GodotHTTPRequest.requests[potentialId] instanceof XMLHttpRequest) {
					continue;
				}
				return potentialId;
			}
			GodotHTTPRequest.requests.push(null)
			return idMax;
		},

		setupRequest: function(xhr) {
			xhr.responseType = 'arraybuffer';
		},
	},

	godot_xhr_new: function() {
		var newId = GodotHTTPRequest.getUnusedRequestId();
		GodotHTTPRequest.requests[newId] = new XMLHttpRequest;
		GodotHTTPRequest.setupRequest(GodotHTTPRequest.requests[newId]);
		return newId;
	},

	godot_xhr_reset: function(xhrId) {
		GodotHTTPRequest.requests[xhrId] = new XMLHttpRequest;
		GodotHTTPRequest.setupRequest(GodotHTTPRequest.requests[xhrId]);
	},

	godot_xhr_free: function(xhrId) {
		GodotHTTPRequest.requests[xhrId].abort();
		GodotHTTPRequest.requests[xhrId] = null;
	},

	godot_xhr_open: function(xhrId, method, url, user, password) {
		user = user > 0 ? UTF8ToString(user) : null;
		password = password > 0 ? UTF8ToString(password) : null;
		GodotHTTPRequest.requests[xhrId].open(UTF8ToString(method), UTF8ToString(url), true, user, password);
	},

	godot_xhr_set_request_header: function(xhrId, header, value) {
		GodotHTTPRequest.requests[xhrId].setRequestHeader(UTF8ToString(header), UTF8ToString(value));
	},

	godot_xhr_send_null: function(xhrId) {
		GodotHTTPRequest.requests[xhrId].send();
	},

	godot_xhr_send_string: function(xhrId, strPtr) {
		if (!strPtr) {
			err("Failed to send string per XHR: null pointer");
			return;
		}
		GodotHTTPRequest.requests[xhrId].send(UTF8ToString(strPtr));
	},

	godot_xhr_send_data: function(xhrId, ptr, len) {
		if (!ptr) {
			err("Failed to send data per XHR: null pointer");
			return;
		}
		if (len < 0) {
			err("Failed to send data per XHR: buffer length less than 0");
			return;
		}
		GodotHTTPRequest.requests[xhrId].send(HEAPU8.subarray(ptr, ptr + len));
	},

	godot_xhr_abort: function(xhrId) {
		GodotHTTPRequest.requests[xhrId].abort();
	},

	godot_xhr_get_status: function(xhrId) {
		return GodotHTTPRequest.requests[xhrId].status;
	},

	godot_xhr_get_ready_state: function(xhrId) {
		return GodotHTTPRequest.requests[xhrId].readyState;
	},

	godot_xhr_get_response_headers_length: function(xhrId) {
		var headers = GodotHTTPRequest.requests[xhrId].getAllResponseHeaders();
		return headers === null ? 0 : lengthBytesUTF8(headers);
	},

	godot_xhr_get_response_headers: function(xhrId, dst, len) {
		var str = GodotHTTPRequest.requests[xhrId].getAllResponseHeaders();
		if (str === null)
			return;
		var buf = new Uint8Array(len + 1);
		stringToUTF8Array(str, buf, 0, buf.length);
		buf = buf.subarray(0, -1);
		HEAPU8.set(buf, dst);
	},

	godot_xhr_get_response_length: function(xhrId) {
		var body = GodotHTTPRequest.requests[xhrId].response;
		return body === null ? 0 : body.byteLength;
	},

	godot_xhr_get_response: function(xhrId, dst, len) {
		var buf = GodotHTTPRequest.requests[xhrId].response;
		if (buf === null)
			return;
		buf = new Uint8Array(buf).subarray(0, len);
		HEAPU8.set(buf, dst);
	},
};

autoAddDeps(GodotHTTPRequest, "$GodotHTTPRequest");
mergeInto(LibraryManager.library, GodotHTTPRequest);

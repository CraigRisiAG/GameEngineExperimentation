********************************************************************/

// HTTPClient's additional private members in the javascript platform

Error prepare_request(Method p_method, const String &p_url, const Vector<String> &p_headers);

int xhr_id;
int read_limit;
int response_read_offset;
Status status;

String host;
int port;
bool use_tls;
String username;
String password;

int polled_response_code;
String polled_response_header;
PackedByteArray polled_response;

#ifdef DEBUG_ENABLED
bool has_polled;
uint64_t last_polling_frame;
#endif

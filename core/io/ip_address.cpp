

/**
 * @file ip_address.cpp
 * @brief Implementation of IP address parsing and conversion functionality.
 * 
 * This file contains the implementation for handling both IPv4 and IPv6 addresses.
 * IPv4 addresses are internally mapped to IPv6 format for unified storage and processing.
 * 
 * Key functionalities:
 * - String to IP address parsing (supports IPv4, IPv6, and wildcard formats)
 * - IP address to string conversion
 * - IPv4 and IPv6 getter/setter methods
 * - Hexadecimal and decimal parsing utilities
 * - Internal representation using 16 uint8_t fields (128 bits)
 */

/**
 * @brief Converts IP_Address object to String representation.
 * @return String representation of the IP address (IPv4 dotted notation or IPv6 colon notation).
 *         Returns "*" for wildcard, empty string for invalid addresses.
 */

/**
 * @brief Parses a hexadecimal segment from an IPv6 address string.
 * @param p_string The source IPv6 address string.
 * @param p_start The starting position in the string.
 * @param p_dst Pointer to destination 2-byte buffer for parsed value.
 */

/**
 * @brief Parses an IPv6 address string and populates internal field16 array.
 * @param p_string The IPv6 address string to parse (e.g., "2001:db8::1").
 * @note Handles compressed notation (::) and mixed IPv4/IPv6 addresses.
 */

/**
 * @brief Parses an IPv4 address string into byte array.
 * @param p_string The IPv4 address string to parse.
 * @param p_start Starting position in the string.
 * @param p_ret Pointer to destination 4-byte buffer for parsed IPv4 octets.
 * @note Validates that exactly 4 octets are present.
 */

/**
 * @brief Resets IP address to empty/invalid state.
 */

/**
 * @brief Checks if the current IP address is IPv4 (mapped in IPv6 format).
 * @return True if address is IPv4, false if IPv6.
 */

/**
 * @brief Retrieves pointer to IPv4 address bytes.
 * @return Pointer to 4-byte IPv4 address buffer.
 * @note Returns mapped IPv4 portion even if stored as IPv6.
 */

/**
 * @brief Sets the IP address from IPv4 bytes.
 * @param p_ip Pointer to 4-byte IPv4 address buffer.
 */

/**
 * @brief Retrieves pointer to IPv6 address bytes.
 * @return Pointer to 16-byte IPv6 address buffer.
 */

/**
 * @brief Sets the IP address from IPv6 bytes.
 * @param p_buf Pointer to 16-byte IPv6 address buffer.
 */

/**
 * @brief Constructs IP_Address from string representation.
 * @param p_string IP address string (supports "*" for wildcard, IPv4 dotted notation, IPv6 colon notation).
 * @note Automatically detects format and parses accordingly.
 */

/**
 * @brief Constructs IP_Address from four 32-bit components.
 * @param p_a First 32-bit component.
 * @param p_b Second 32-bit component.
 * @param p_c Third 32-bit component.
 * @param p_d Fourth 32-bit component.
 * @param is_v6 True for IPv6 format, false for IPv4 (default).
 */

/**
 * @brief Helper function to convert 32-bit integer to network byte order (big-endian).
 * @param p_dst Pointer to 4-byte destination buffer.
 * @param p_n 32-bit value to convert.
 */
#include "ip_address.h"
/*
IP_Address::operator Variant() const {

	return operator String();
}*/

#include <stdio.h>
#include <string.h>

IP_Address::operator String() const {

	if (wildcard)
		return "*";

	if (!valid)
		return "";

	if (is_ipv4())
		// IPv4 address mapped to IPv6
		return itos(field8[12]) + "." + itos(field8[13]) + "." + itos(field8[14]) + "." + itos(field8[15]);
	String ret;
	for (int i = 0; i < 8; i++) {
		if (i > 0)
			ret = ret + ":";
		uint16_t num = (field8[i * 2] << 8) + field8[i * 2 + 1];
		ret = ret + String::num_int64(num, 16);
	};

	return ret;
}

static void _parse_hex(const String &p_string, int p_start, uint8_t *p_dst) {

	uint16_t ret = 0;
	for (int i = p_start; i < p_start + 4; i++) {

		if (i >= p_string.length()) {
			break;
		};

		int n = 0;
		CharType c = p_string[i];
		if (c >= '0' && c <= '9') {

			n = c - '0';
		} else if (c >= 'a' && c <= 'f') {
			n = 10 + (c - 'a');
		} else if (c >= 'A' && c <= 'F') {
			n = 10 + (c - 'A');
		} else if (c == ':') {
			break;
		} else {
			ERR_FAIL_MSG("Invalid character in IPv6 address: " + p_string + ".");
		};
		ret = ret << 4;
		ret += n;
	};

	p_dst[0] = ret >> 8;
	p_dst[1] = ret & 0xff;
};

void IP_Address::_parse_ipv6(const String &p_string) {

	static const int parts_total = 8;
	int parts[parts_total] = { 0 };
	int parts_count = 0;
	bool part_found = false;
	bool part_skip = false;
	bool part_ipv4 = false;
	int parts_idx = 0;

	for (int i = 0; i < p_string.length(); i++) {

		CharType c = p_string[i];
		if (c == ':') {

			if (i == 0) {
				continue; // next must be a ":"
			};
			if (!part_found) {
				part_skip = true;
				parts[parts_idx++] = -1;
			};
			part_found = false;
		} else if (c == '.') {

			part_ipv4 = true;

		} else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
			if (!part_found) {
				parts[parts_idx++] = i;
				part_found = true;
				++parts_count;
			};
		} else {
			ERR_FAIL_MSG("Invalid character in IPv6 address: " + p_string + ".");
		};
	};

	int parts_extra = 0;
	if (part_skip) {
		parts_extra = parts_total - parts_count;
	};

	int idx = 0;
	for (int i = 0; i < parts_idx; i++) {

		if (parts[i] == -1) {

			for (int j = 0; j < parts_extra; j++) {
				field16[idx++] = 0;
			};
			continue;
		};

		if (part_ipv4 && i == parts_idx - 1) {
			_parse_ipv4(p_string, parts[i], (uint8_t *)&field16[idx]); // should be the last one
		} else {
			_parse_hex(p_string, parts[i], (uint8_t *)&(field16[idx++]));
		};
	};
};

void IP_Address::_parse_ipv4(const String &p_string, int p_start, uint8_t *p_ret) {

	String ip;
	if (p_start != 0) {
		ip = p_string.substr(p_start, p_string.length() - p_start);
	} else {
		ip = p_string;
	};

	int slices = ip.get_slice_count(".");
	ERR_FAIL_COND_MSG(slices != 4, "Invalid IP address string: " + ip + ".");
	for (int i = 0; i < 4; i++) {
		p_ret[i] = ip.get_slicec('.', i).to_int();
	}
};

void IP_Address::clear() {

	memset(&field8[0], 0, sizeof(field8));
	valid = false;
	wildcard = false;
};

bool IP_Address::is_ipv4() const {
	return (field32[0] == 0 && field32[1] == 0 && field16[4] == 0 && field16[5] == 0xffff);
}

const uint8_t *IP_Address::get_ipv4() const {
	ERR_FAIL_COND_V_MSG(!is_ipv4(), &(field8[12]), "IPv4 requested, but current IP is IPv6."); // Not the correct IPv4 (it's an IPv6), but we don't want to return a null pointer risking an engine crash.
	return &(field8[12]);
}

void IP_Address::set_ipv4(const uint8_t *p_ip) {
	clear();
	valid = true;
	field16[5] = 0xffff;
	field32[3] = *((const uint32_t *)p_ip);
}

const uint8_t *IP_Address::get_ipv6() const {
	return field8;
}

void IP_Address::set_ipv6(const uint8_t *p_buf) {
	clear();
	valid = true;
	for (int i = 0; i < 16; i++)
		field8[i] = p_buf[i];
}

IP_Address::IP_Address(const String &p_string) {

	clear();

	if (p_string == "*") {
		// Wildcard (not a valid IP)
		wildcard = true;

	} else if (p_string.find(":") >= 0) {
		// IPv6
		_parse_ipv6(p_string);
		valid = true;

	} else if (p_string.get_slice_count(".") == 4) {
		// IPv4 (mapped to IPv6 internally)
		field16[5] = 0xffff;
		_parse_ipv4(p_string, 0, &field8[12]);
		valid = true;

	} else {
		ERR_PRINT("Invalid IP address.");
	}
}

_FORCE_INLINE_ static void _32_to_buf(uint8_t *p_dst, uint32_t p_n) {

	p_dst[0] = (p_n >> 24) & 0xff;
	p_dst[1] = (p_n >> 16) & 0xff;
	p_dst[2] = (p_n >> 8) & 0xff;
	p_dst[3] = (p_n >> 0) & 0xff;
};

IP_Address::IP_Address(uint32_t p_a, uint32_t p_b, uint32_t p_c, uint32_t p_d, bool is_v6) {

	clear();
	valid = true;
	if (!is_v6) {
		// Mapped to IPv6
		field16[5] = 0xffff;
		field8[12] = p_a;
		field8[13] = p_b;
		field8[14] = p_c;
		field8[15] = p_d;
	} else {

		_32_to_buf(&field8[0], p_a);
		_32_to_buf(&field8[4], p_b);
		_32_to_buf(&field8[8], p_c);
		_32_to_buf(&field8[12], p_d);
	}
}



/**
 * @struct IP_Address
 * @brief Represents an IPv4 or IPv6 address with validation and parsing capabilities.
 * 
 * This structure provides a unified interface for handling both IPv4 and IPv6 addresses.
 * It stores the address data in a union to support multiple representations (8-bit, 16-bit,
 * and 32-bit fields) and includes validation and wildcard state tracking.
 * 
 * @member field8 Array of 16 8-bit unsigned integers for IPv6 address storage
 * @member field16 Array of 8 16-bit unsigned integers for IPv6 address storage
 * @member field32 Array of 4 32-bit unsigned integers for address storage
 * @member valid Flag indicating whether the address is valid
 * @member wildcard Flag indicating whether this is a wildcard address
 * 
 * @method _parse_ipv6 Parses an IPv6 address from a string
 * @param p_string The string representation of the IPv6 address
 * 
 * @method _parse_ipv4 Parses an IPv4 address from a string
 * @param p_string The string to parse
 * @param p_start The starting position in the string
 * @param p_ret Pointer to store the parsed IPv4 address bytes
 * 
 * @method operator== Compares two IP addresses for equality
 * @return True if both addresses are valid and equal, false otherwise
 * 
 * @method operator!= Compares two IP addresses for inequality
 * @return True if addresses differ or validity states differ, false otherwise
 * 
 * @method clear Clears the address and resets validation state
 * 
 * @method is_wildcard Returns whether this address is a wildcard
 * @return True if the address is a wildcard pattern
 * 
 * @method is_valid Returns the validity state of this address
 * @return True if the address was successfully parsed and is valid
 * 
 * @method is_ipv4 Determines if this address is IPv4 format
 * @return True if the address is IPv4, false if IPv6
 * 
 * @method get_ipv4 Retrieves the IPv4 address bytes
 * @return Pointer to 4 bytes representing the IPv4 address
 * 
 * @method set_ipv4 Sets the IPv4 address from raw bytes
 * @param p_ip Pointer to 4 bytes containing the IPv4 address
 * 
 * @method get_ipv6 Retrieves the IPv6 address bytes
 * @return Pointer to 16 bytes representing the IPv6 address
 * 
 * @method set_ipv6 Sets the IPv6 address from raw bytes
 * @param p_buf Pointer to 16 bytes containing the IPv6 address
 * 
 * @method operator String Converts the IP address to its string representation
 * @return String representation of the IP address
 * 
 * @constructor IP_Address(const String &p_string) Constructs an IP address from a string
 * @param p_string String representation of the IP address (IPv4 or IPv6)
 * 
 * @constructor IP_Address(uint32_t p_a, uint32_t p_b, uint32_t p_c, uint32_t p_d, bool is_v6)
 * Constructs an IP address from four 32-bit components
 * @param p_a First component
 * @param p_b Second component
 * @param p_c Third component
 * @param p_d Fourth component
 * @param is_v6 If true, interprets input as IPv6; if false, as IPv4 (default: false)
 * 
 * @constructor IP_Address() Default constructor that clears the address
 */
#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H

#include "core/ustring.h"

struct IP_Address {

private:
	union {
		uint8_t field8[16];
		uint16_t field16[8];
		uint32_t field32[4];
	};

	bool valid;
	bool wildcard;

protected:
	void _parse_ipv6(const String &p_string);
	void _parse_ipv4(const String &p_string, int p_start, uint8_t *p_ret);

public:
	//operator Variant() const;
	bool operator==(const IP_Address &p_ip) const {
		if (p_ip.valid != valid) return false;
		if (!valid) return false;
		for (int i = 0; i < 4; i++)
			if (field32[i] != p_ip.field32[i])
				return false;
		return true;
	}
	bool operator!=(const IP_Address &p_ip) const {
		if (p_ip.valid != valid) return true;
		if (!valid) return true;
		for (int i = 0; i < 4; i++)
			if (field32[i] != p_ip.field32[i])
				return true;
		return false;
	}

	void clear();
	bool is_wildcard() const { return wildcard; }
	bool is_valid() const { return valid; }
	bool is_ipv4() const;
	const uint8_t *get_ipv4() const;
	void set_ipv4(const uint8_t *p_ip);

	const uint8_t *get_ipv6() const;
	void set_ipv6(const uint8_t *p_buf);

	operator String() const;
	IP_Address(const String &p_string);
	IP_Address(uint32_t p_a, uint32_t p_b, uint32_t p_c, uint32_t p_d, bool is_v6 = false);
	IP_Address() { clear(); }
};

#endif // IP_ADDRESS_H

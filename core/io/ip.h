

/// @class IP
/// @brief Provides IP address resolution and network interface information.
///
/// The IP class is a singleton that handles both synchronous and asynchronous
/// hostname resolution, as well as querying local network interfaces and addresses.
/// It serves as an abstract base class with platform-specific implementations.
///
/// @note This is a Godot Engine networking utility class.
///
/// @enum ResolverStatus
/// @brief Status of an asynchronous hostname resolution request.
/// - RESOLVER_STATUS_NONE: No resolution in progress
/// - RESOLVER_STATUS_WAITING: Resolution is pending
/// - RESOLVER_STATUS_DONE: Resolution completed successfully
/// - RESOLVER_STATUS_ERROR: Resolution failed
///
/// @enum Type
/// @brief IP address type filter for resolution.
/// - TYPE_NONE: No type specified
/// - TYPE_IPV4: IPv4 addresses only
/// - TYPE_IPV6: IPv6 addresses only
/// - TYPE_ANY: Both IPv4 and IPv6 addresses
///
/// @struct Interface_Info
/// @brief Information about a network interface.
/// @var name: System interface name
/// @var name_friendly: Human-readable interface name
/// @var index: Interface index identifier
/// @var ip_addresses: List of IP addresses bound to this interface
///
/// @method resolve_hostname
/// @brief Synchronously resolve a hostname to an IP address.
/// @param p_hostname The hostname to resolve
/// @param p_type IP address type filter (default: TYPE_ANY)
/// @return IP_Address of the resolved hostname
///
/// @method resolve_hostname_queue_item
/// @brief Queue an asynchronous hostname resolution request.
/// @param p_hostname The hostname to resolve
/// @param p_type IP address type filter (default: TYPE_ANY)
/// @return ResolverID identifier for tracking the async request
///
/// @method get_resolve_item_status
/// @brief Get the status of a queued resolution request.
/// @param p_id The ResolverID returned from resolve_hostname_queue_item
/// @return ResolverStatus indicating the current state
///
/// @method get_resolve_item_address
/// @brief Get the resolved IP address from a completed request.
/// @param p_id The ResolverID returned from resolve_hostname_queue_item
/// @return IP_Address of the resolved hostname
///
/// @method get_local_addresses
/// @brief Retrieve all local IP addresses.
/// @param r_addresses Output list to populate with local IP addresses
///
/// @method get_local_interfaces
/// @brief Retrieve detailed information about all network interfaces.
/// @param r_interfaces Output map to populate with interface information
///
/// @method erase_resolve_item
/// @brief Cancel and remove a queued or completed resolution request.
/// @param p_id The ResolverID to erase
///
/// @method clear_cache
/// @brief Clear hostname resolution cache.
/// @param p_hostname Specific hostname to clear, or empty string to clear all
#ifndef IP_H
#define IP_H

#include "core/io/ip_address.h"
#include "core/os/os.h"

struct _IP_ResolverPrivate;

class IP : public Object {
	GDCLASS(IP, Object);
	OBJ_CATEGORY("Networking");

public:
	enum ResolverStatus {

		RESOLVER_STATUS_NONE,
		RESOLVER_STATUS_WAITING,
		RESOLVER_STATUS_DONE,
		RESOLVER_STATUS_ERROR,
	};

	enum Type {

		TYPE_NONE = 0,
		TYPE_IPV4 = 1,
		TYPE_IPV6 = 2,
		TYPE_ANY = 3,
	};

	enum {
		RESOLVER_MAX_QUERIES = 32,
		RESOLVER_INVALID_ID = -1
	};

	typedef int ResolverID;

private:
	_IP_ResolverPrivate *resolver;

protected:
	static IP *singleton;
	static void _bind_methods();

	virtual IP_Address _resolve_hostname(const String &p_hostname, Type p_type = TYPE_ANY) = 0;
	Array _get_local_addresses() const;
	Array _get_local_interfaces() const;

	static IP *(*_create)();

public:
	struct Interface_Info {
		String name;
		String name_friendly;
		String index;
		List<IP_Address> ip_addresses;
	};

	IP_Address resolve_hostname(const String &p_hostname, Type p_type = TYPE_ANY);
	// async resolver hostname
	ResolverID resolve_hostname_queue_item(const String &p_hostname, Type p_type = TYPE_ANY);
	ResolverStatus get_resolve_item_status(ResolverID p_id) const;
	IP_Address get_resolve_item_address(ResolverID p_id) const;
	virtual void get_local_addresses(List<IP_Address> *r_addresses) const;
	virtual void get_local_interfaces(Map<String, Interface_Info> *r_interfaces) const = 0;
	void erase_resolve_item(ResolverID p_id);

	void clear_cache(const String &p_hostname = "");

	static IP *get_singleton();

	static IP *create();

	IP();
	~IP();
};

VARIANT_ENUM_CAST(IP::Type);

#endif // IP_H

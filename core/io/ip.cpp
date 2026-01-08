

/**
 * @file ip.cpp
 * @brief IP address resolution and local network interface management implementation
 * 
 * This file implements the IP class which provides hostname resolution services,
 * including both synchronous and asynchronous (queue-based) resolution, caching,
 * and local network interface enumeration.
 * 
 * @class IP
 * @brief Singleton class for IP address operations and hostname resolution
 * 
 * The IP class manages:
 * - Synchronous hostname resolution with caching
 * - Asynchronous hostname resolution via a dedicated resolver thread
 * - Local IP address and network interface enumeration
 * - Thread-safe access to resolver queue and cache via mutex protection
 * 
 * @struct _IP_ResolverPrivate
 * @brief Private implementation details for the hostname resolver
 * 
 * Manages a queue of resolution requests processed by a background thread,
 * with a HashMap cache for storing previously resolved addresses.
 * 
 * @struct QueueItem
 * @brief Represents a single hostname resolution request in the resolver queue
 * 
 * @member status - Current resolution status (WAITING, DONE, ERROR, NONE)
 * @member response - The resolved IP address
 * @member hostname - The hostname being resolved
 * @member type - IP address type filter (IPV4, IPV6, ANY, NONE)
 * 
 * @method resolve_hostname() - Synchronously resolves a hostname with caching
 * @method resolve_hostname_queue_item() - Asynchronously queues a hostname for resolution
 * @method get_resolve_item_status() - Checks the status of a queued resolution
 * @method get_resolve_item_address() - Retrieves the result of a completed resolution
 * @method erase_resolve_item() - Clears a resolution result from the queue
 * @method get_local_addresses() - Gets all local IP addresses
 * @method get_local_interfaces() - Gets detailed local network interface information
 * @method clear_cache() - Clears the hostname resolution cache
 * @method _bind_methods() - Binds public methods to the scripting API
 */
#include "ip.h"

#include "core/hash_map.h"
#include "core/os/semaphore.h"
#include "core/os/thread.h"

VARIANT_ENUM_CAST(IP::ResolverStatus);

/************* RESOLVER ******************/

struct _IP_ResolverPrivate {

	struct QueueItem {

		volatile IP::ResolverStatus status;
		IP_Address response;
		String hostname;
		IP::Type type;

		void clear() {
			status = IP::RESOLVER_STATUS_NONE;
			response = IP_Address();
			type = IP::TYPE_NONE;
			hostname = "";
		};

		QueueItem() {
			clear();
		};
	};

	QueueItem queue[IP::RESOLVER_MAX_QUERIES];

	IP::ResolverID find_empty_id() const {

		for (int i = 0; i < IP::RESOLVER_MAX_QUERIES; i++) {
			if (queue[i].status == IP::RESOLVER_STATUS_NONE)
				return i;
		}
		return IP::RESOLVER_INVALID_ID;
	}

	Mutex mutex;
	Semaphore sem;

	Thread *thread;
	//Semaphore* semaphore;
	bool thread_abort;

	void resolve_queues() {

		for (int i = 0; i < IP::RESOLVER_MAX_QUERIES; i++) {

			if (queue[i].status != IP::RESOLVER_STATUS_WAITING)
				continue;
			queue[i].response = IP::get_singleton()->resolve_hostname(queue[i].hostname, queue[i].type);

			if (!queue[i].response.is_valid())
				queue[i].status = IP::RESOLVER_STATUS_ERROR;
			else
				queue[i].status = IP::RESOLVER_STATUS_DONE;
		}
	}

	static void _thread_function(void *self) {

		_IP_ResolverPrivate *ipr = (_IP_ResolverPrivate *)self;

		while (!ipr->thread_abort) {

			ipr->sem.wait();

			MutexLock lock(ipr->mutex);
			ipr->resolve_queues();
		}
	}

	HashMap<String, IP_Address> cache;

	static String get_cache_key(String p_hostname, IP::Type p_type) {
		return itos(p_type) + p_hostname;
	}
};

IP_Address IP::resolve_hostname(const String &p_hostname, IP::Type p_type) {

	MutexLock lock(resolver->mutex);

	String key = _IP_ResolverPrivate::get_cache_key(p_hostname, p_type);
	if (resolver->cache.has(key) && resolver->cache[key].is_valid()) {
		IP_Address res = resolver->cache[key];
		return res;
	}

	IP_Address res = _resolve_hostname(p_hostname, p_type);
	resolver->cache[key] = res;
	return res;
}

IP::ResolverID IP::resolve_hostname_queue_item(const String &p_hostname, IP::Type p_type) {

	MutexLock lock(resolver->mutex);

	ResolverID id = resolver->find_empty_id();

	if (id == RESOLVER_INVALID_ID) {
		WARN_PRINT("Out of resolver queries");
		return id;
	}

	String key = _IP_ResolverPrivate::get_cache_key(p_hostname, p_type);
	resolver->queue[id].hostname = p_hostname;
	resolver->queue[id].type = p_type;
	if (resolver->cache.has(key) && resolver->cache[key].is_valid()) {
		resolver->queue[id].response = resolver->cache[key];
		resolver->queue[id].status = IP::RESOLVER_STATUS_DONE;
	} else {
		resolver->queue[id].response = IP_Address();
		resolver->queue[id].status = IP::RESOLVER_STATUS_WAITING;
		if (resolver->thread)
			resolver->sem.post();
		else
			resolver->resolve_queues();
	}

	return id;
}

IP::ResolverStatus IP::get_resolve_item_status(ResolverID p_id) const {

	ERR_FAIL_INDEX_V(p_id, IP::RESOLVER_MAX_QUERIES, IP::RESOLVER_STATUS_NONE);

	MutexLock lock(resolver->mutex);

	if (resolver->queue[p_id].status == IP::RESOLVER_STATUS_NONE) {
		ERR_PRINT("Condition status == IP::RESOLVER_STATUS_NONE");
		resolver->mutex.unlock();
		return IP::RESOLVER_STATUS_NONE;
	}
	return resolver->queue[p_id].status;
}

IP_Address IP::get_resolve_item_address(ResolverID p_id) const {

	ERR_FAIL_INDEX_V(p_id, IP::RESOLVER_MAX_QUERIES, IP_Address());

	MutexLock lock(resolver->mutex);

	if (resolver->queue[p_id].status != IP::RESOLVER_STATUS_DONE) {
		ERR_PRINT("Resolve of '" + resolver->queue[p_id].hostname + "'' didn't complete yet.");
		resolver->mutex.unlock();
		return IP_Address();
	}

	return resolver->queue[p_id].response;
}

void IP::erase_resolve_item(ResolverID p_id) {

	ERR_FAIL_INDEX(p_id, IP::RESOLVER_MAX_QUERIES);

	MutexLock lock(resolver->mutex);

	resolver->queue[p_id].status = IP::RESOLVER_STATUS_NONE;
}

void IP::clear_cache(const String &p_hostname) {

	MutexLock lock(resolver->mutex);

	if (p_hostname.empty()) {
		resolver->cache.clear();
	} else {
		resolver->cache.erase(_IP_ResolverPrivate::get_cache_key(p_hostname, IP::TYPE_NONE));
		resolver->cache.erase(_IP_ResolverPrivate::get_cache_key(p_hostname, IP::TYPE_IPV4));
		resolver->cache.erase(_IP_ResolverPrivate::get_cache_key(p_hostname, IP::TYPE_IPV6));
		resolver->cache.erase(_IP_ResolverPrivate::get_cache_key(p_hostname, IP::TYPE_ANY));
	}
}

Array IP::_get_local_addresses() const {

	Array addresses;
	List<IP_Address> ip_addresses;
	get_local_addresses(&ip_addresses);
	for (List<IP_Address>::Element *E = ip_addresses.front(); E; E = E->next()) {
		addresses.push_back(E->get());
	}

	return addresses;
}

Array IP::_get_local_interfaces() const {

	Array results;
	Map<String, Interface_Info> interfaces;
	get_local_interfaces(&interfaces);
	for (Map<String, Interface_Info>::Element *E = interfaces.front(); E; E = E->next()) {
		Interface_Info &c = E->get();
		Dictionary rc;
		rc["name"] = c.name;
		rc["friendly"] = c.name_friendly;
		rc["index"] = c.index;

		Array ips;
		for (const List<IP_Address>::Element *F = c.ip_addresses.front(); F; F = F->next()) {
			ips.push_front(F->get());
		}
		rc["addresses"] = ips;

		results.push_front(rc);
	}

	return results;
}

void IP::get_local_addresses(List<IP_Address> *r_addresses) const {

	Map<String, Interface_Info> interfaces;
	get_local_interfaces(&interfaces);
	for (Map<String, Interface_Info>::Element *E = interfaces.front(); E; E = E->next()) {
		for (const List<IP_Address>::Element *F = E->get().ip_addresses.front(); F; F = F->next()) {
			r_addresses->push_front(F->get());
		}
	}
}

void IP::_bind_methods() {

	ClassDB::bind_method(D_METHOD("resolve_hostname", "host", "ip_type"), &IP::resolve_hostname, DEFVAL(IP::TYPE_ANY));
	ClassDB::bind_method(D_METHOD("resolve_hostname_queue_item", "host", "ip_type"), &IP::resolve_hostname_queue_item, DEFVAL(IP::TYPE_ANY));
	ClassDB::bind_method(D_METHOD("get_resolve_item_status", "id"), &IP::get_resolve_item_status);
	ClassDB::bind_method(D_METHOD("get_resolve_item_address", "id"), &IP::get_resolve_item_address);
	ClassDB::bind_method(D_METHOD("erase_resolve_item", "id"), &IP::erase_resolve_item);
	ClassDB::bind_method(D_METHOD("get_local_addresses"), &IP::_get_local_addresses);
	ClassDB::bind_method(D_METHOD("get_local_interfaces"), &IP::_get_local_interfaces);
	ClassDB::bind_method(D_METHOD("clear_cache", "hostname"), &IP::clear_cache, DEFVAL(""));

	BIND_ENUM_CONSTANT(RESOLVER_STATUS_NONE);
	BIND_ENUM_CONSTANT(RESOLVER_STATUS_WAITING);
	BIND_ENUM_CONSTANT(RESOLVER_STATUS_DONE);
	BIND_ENUM_CONSTANT(RESOLVER_STATUS_ERROR);

	BIND_CONSTANT(RESOLVER_MAX_QUERIES);
	BIND_CONSTANT(RESOLVER_INVALID_ID);

	BIND_ENUM_CONSTANT(TYPE_NONE);
	BIND_ENUM_CONSTANT(TYPE_IPV4);
	BIND_ENUM_CONSTANT(TYPE_IPV6);
	BIND_ENUM_CONSTANT(TYPE_ANY);
}

IP *IP::singleton = NULL;

IP *IP::get_singleton() {

	return singleton;
}

IP *(*IP::_create)() = NULL;

IP *IP::create() {

	ERR_FAIL_COND_V_MSG(singleton, NULL, "IP singleton already exist.");
	ERR_FAIL_COND_V(!_create, NULL);
	return _create();
}

IP::IP() {

	singleton = this;
	resolver = memnew(_IP_ResolverPrivate);

#ifndef NO_THREADS

	resolver->thread_abort = false;

	resolver->thread = Thread::create(_IP_ResolverPrivate::_thread_function, resolver);
#else
	resolver->thread = NULL;
#endif
}

IP::~IP() {

#ifndef NO_THREADS
	if (resolver->thread) {
		resolver->thread_abort = true;
		resolver->sem.post();
		Thread::wait_to_finish(resolver->thread);
		memdelete(resolver->thread);
	}

#endif

	memdelete(resolver);
}


/**
 * @class ICloud
 * @brief iCloud integration for iOS platform
 * 
 * Provides an interface for interacting with Apple's iCloud service on iOS devices.
 * This class allows storing and retrieving key-value pairs in iCloud, synchronizing
 * data across devices, and handling asynchronous events.
 * 
 * This class is only available when ICLOUD_ENABLED is defined.
 * 
 * @note This is a singleton class. Use get_singleton() to access the instance.
 */

/**
 * @brief Removes a key from iCloud storage
 * 
 * @param p_param Variant containing the key to remove
 * @return Error code indicating success or failure
 */

/**
 * @brief Sets one or more key-value pairs in iCloud
 * 
 * @param p_param Variant containing the key-value pairs to set
 * @return Variant with the result of the operation
 */

/**
 * @brief Retrieves a single value from iCloud by key
 * 
 * @param p_param Variant containing the key to retrieve
 * @return Variant containing the value associated with the key
 */

/**
 * @brief Synchronizes all key-value pairs with iCloud servers
 * 
 * Triggers an upload/download of changes with the remote iCloud service.
 * 
 * @return Error code indicating success or failure
 */

/**
 * @brief Retrieves all key-value pairs currently stored in iCloud
 * 
 * @return Variant containing all key-value pairs
 */

/**
 * @brief Gets the number of pending iCloud events
 * 
 * @return Number of events waiting to be processed
 */

/**
 * @brief Retrieves and removes the next pending event from the queue
 * 
 * @return Variant containing the event data, or empty if no events pending
 */

/**
 * @brief Returns the singleton instance of the ICloud class
 * 
 * @return Pointer to the ICloud singleton instance
 */
#ifdef ICLOUD_ENABLED

#ifndef ICLOUD_H
#define ICLOUD_H

#include "core/object.h"

class ICloud : public Object {

	GDCLASS(ICloud, Object);

	static ICloud *instance;
	static void _bind_methods();

	List<Variant> pending_events;

public:
	Error remove_key(Variant p_param);
	Variant set_key_values(Variant p_param);
	Variant get_key_value(Variant p_param);
	Error synchronize_key_values();
	Variant get_all_key_values();

	int get_pending_event_count();
	Variant pop_pending_event();

	static ICloud *get_singleton();

	ICloud();
	~ICloud();
};

#endif

#endif

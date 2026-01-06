
/// @class InAppStore
/// @brief Manages in-app purchases and store transactions for iPhone platform.
///
/// This class provides an interface to interact with Apple's StoreKit framework,
/// enabling product information requests, purchase transactions, and purchase restoration.
/// It maintains a queue of pending events that can be processed asynchronously.
///
/// @note This class is only available when STOREKIT_ENABLED is defined.

/// @brief Requests product information from the App Store.
/// @param p_params Variant containing product identifiers and request parameters.
/// @return Error code indicating success or failure of the request.
/// @see Error

/// @brief Restores previously purchased products for the user.
/// @return Error code indicating success or failure of the restoration.
/// @see Error

/// @brief Initiates a purchase transaction for a product.
/// @param p_params Variant containing product identifier and purchase details.
/// @return Error code indicating success or failure of the purchase initiation.
/// @see Error

/// @brief Gets the number of pending events in the queue.
/// @return The count of pending events.

/// @brief Retrieves and removes the next pending event from the queue.
/// @return A Variant containing the event data, or empty if queue is empty.

/// @brief Marks a transaction as finished and removes it from processing.
/// @param product_id The product identifier of the transaction to finish.

/// @brief Sets whether transactions should be automatically finished.
/// @param b True to enable auto-finish, false to disable.

/// @brief Posts an event to the pending events queue (internal use).
/// @param p_event The event Variant to queue.

/// @brief Records a purchase internally (internal use).
/// @param product_id The product identifier that was purchased.

/// @brief Gets the singleton instance of InAppStore.
/// @return Pointer to the InAppStore singleton instance.
#ifdef STOREKIT_ENABLED

#ifndef IN_APP_STORE_H
#define IN_APP_STORE_H

#include "core/object.h"

class InAppStore : public Object {

	GDCLASS(InAppStore, Object);

	static InAppStore *instance;
	static void _bind_methods();

	List<Variant> pending_events;

public:
	Error request_product_info(Variant p_params);
	Error restore_purchases();
	Error purchase(Variant p_params);

	int get_pending_event_count();
	Variant pop_pending_event();
	void finish_transaction(String product_id);
	void set_auto_finish_transaction(bool b);

	void _post_event(Variant p_event);
	void _record_purchase(String product_id);

	static InAppStore *get_singleton();

	InAppStore();
	~InAppStore();
};

#endif

#endif

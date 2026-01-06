
/**
 * @class iOS
 * @brief Platform-specific iOS functionality wrapper.
 * 
 * Provides access to iOS-specific features and utilities for the game engine.
 * This class serves as an interface to native iOS functionality through the Object base class.
 */

/**
 * @brief Displays an alert dialog on iOS.
 * 
 * @param p_alert The message text to display in the alert dialog.
 * @param p_title The title text to display at the top of the alert dialog.
 * 
 * @note This is a static method and can be called without an instance.
 */

/**
 * @brief Retrieves the device model name.
 * 
 * @return A String containing the iOS device model identifier (e.g., "iPhone12,1").
 */

/**
 * @brief Constructs the URL for rating the app on the App Store.
 * 
 * @param p_app_id The application ID on the App Store.
 * @return A String containing the complete URL to the app's App Store page.
 */

/**
 * @brief Constructs an iOS platform utility instance.
 * 
 * Initializes the iOS platform wrapper. Typically used internally by the engine.
 */
#ifndef IOS_H
#define IOS_H

#include "core/object.h"

class iOS : public Object {

	GDCLASS(iOS, Object);

	static void _bind_methods();

public:
	static void alert(const char *p_alert, const char *p_title);

	String get_model() const;
	String get_rate_url(int p_app_id) const;

	iOS();
};

#endif

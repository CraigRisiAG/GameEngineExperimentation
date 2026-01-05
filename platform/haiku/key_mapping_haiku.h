
/**
 * @class KeyMappingHaiku
 * @brief Provides key mapping utilities for the Haiku operating system platform.
 * 
 * This class handles the conversion of raw character codes and key values
 * from the Haiku operating system into cross-platform keysym representations.
 * It serves as a platform-specific abstraction layer for keyboard input handling.
 */

/**
 * @brief Converts a raw character and key code to a platform-independent keysym.
 * 
 * Maps Haiku-specific key input (raw character and key code) to a standard
 * keysym value that can be used uniformly across the application.
 * 
 * @param raw_char The raw character code from the Haiku keyboard event
 * @param key The key code from the Haiku keyboard event
 * 
 * @return An unsigned integer representing the cross-platform keysym value
 */

/**
 * @brief Converts a Haiku modifier key code to its corresponding keysym.
 * 
 * Translates modifier keys (such as Shift, Control, Alt) from Haiku's
 * key representation into standard keysym values.
 * 
 * @param key The Haiku modifier key code
 * 
 * @return An unsigned integer representing the modifier keysym value
 */
#ifndef KEY_MAPPING_HAIKU_H
#define KEY_MAPPING_HAIKU_H

class KeyMappingHaiku {
	KeyMappingHaiku(){};

public:
	static unsigned int get_keysym(int32 raw_char, int32 key);
	static unsigned int get_modifier_keysym(int32 key);
};

#endif


/**
 * @class CrashHandler
 * @brief Handles crash detection and reporting for macOS platform.
 * 
 * This class manages the crash handler functionality for the game engine on macOS.
 * It allows enabling/disabling crash handling and initialization of the crash handler system.
 */
class CrashHandler {

	/**
	 * @brief Flag indicating whether crash handling is disabled.
	 */
	bool disabled;

public:
	/**
	 * @brief Initializes the crash handler.
	 * 
	 * Sets up the necessary crash handling mechanisms for the macOS platform.
	 */
	void initialize();

	/**
	 * @brief Disables crash handling.
	 * 
	 * Prevents the crash handler from processing crashes.
	 */
	void disable();

	/**
	 * @brief Checks if crash handling is disabled.
	 * 
	 * @return true if crash handling is disabled, false otherwise.
	 */
	bool is_disabled() const { return disabled; };

	/**
	 * @brief Constructs a CrashHandler instance.
	 */
	CrashHandler();

	/**
	 * @brief Destroys the CrashHandler instance.
	 */
	~CrashHandler();
};

#endif // CRASH_HANDLER_OSX_H

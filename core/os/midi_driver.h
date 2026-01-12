

/**
 * @class MIDIDriver
 * @brief Multi-platform abstraction layer for MIDI input/output operations.
 * 
 * MIDIDriver provides a unified interface for accessing MIDI devices across different platforms.
 * It follows the singleton pattern to ensure only one instance manages MIDI communication.
 * 
 * @note This is an abstract base class and should not be instantiated directly.
 *       Derived classes must implement platform-specific MIDI functionality.
 */

/**
 * @brief Gets the singleton instance of MIDIDriver.
 * @return Pointer to the singleton MIDIDriver instance.
 */

/**
 * @brief Sets this instance as the singleton.
 */

/**
 * @brief Opens the MIDI driver and initializes connections to MIDI devices.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Closes the MIDI driver and releases all MIDI device connections.
 */

/**
 * @brief Retrieves an array of all connected MIDI input device names.
 * @return PackedStringArray containing names of connected MIDI input devices.
 */

/**
 * @brief Processes an incoming MIDI message packet.
 * @param timestamp The timestamp in microseconds when the MIDI message was received.
 * @param data Pointer to the MIDI message data buffer.
 * @param length The length of the MIDI message data in bytes.
 */

/**
 * @brief Constructs a MIDIDriver instance.
 */

/**
 * @brief Destructs the MIDIDriver instance.
 */
#ifndef MIDI_DRIVER_H
#define MIDI_DRIVER_H

#include "core/typedefs.h"
#include "core/variant.h"

/**
 * Multi-Platform abstraction for accessing to MIDI.
 */

class MIDIDriver {

	static MIDIDriver *singleton;
	static uint8_t last_received_message;

public:
	static MIDIDriver *get_singleton();
	void set_singleton();

	virtual Error open() = 0;
	virtual void close() = 0;

	virtual PackedStringArray get_connected_inputs();

	static void receive_input_packet(uint64_t timestamp, uint8_t *data, uint32_t length);

	MIDIDriver();
	virtual ~MIDIDriver() {}
};

#endif // MIDI_DRIVER_H

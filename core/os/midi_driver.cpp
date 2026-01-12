

/// @class MIDIDriver
/// @brief Handles MIDI input device communication and event processing.
/// 
/// This class manages MIDI (Musical Instrument Digital Interface) input from connected devices.
/// It processes raw MIDI packets, interprets MIDI messages, and converts them into input events
/// for the engine's input system.
/// 
/// Features:
/// - Singleton pattern for global access
/// - Support for MIDI running status
/// - Handling of various MIDI message types (Note On/Off, Control Change, Pitch Bend, etc.)
/// - Automatic conversion of zero-velocity Note On to Note Off events
/// - Integration with the engine's input event system
/// 
/// @note This driver uses a singleton pattern. Use MIDIDriver::get_singleton() to access the instance.

/// @brief Gets the singleton instance of the MIDI driver.
/// @return Pointer to the MIDIDriver singleton instance, or NULL if not initialized.

/// @brief Sets this instance as the singleton.
/// @details Called automatically during construction.

/// @brief Processes an incoming MIDI input packet.
/// @param timestamp The timestamp when the MIDI data was received (in microseconds).
/// @param data Pointer to the MIDI message data bytes.
/// @param length The number of bytes in the MIDI message.
/// 
/// @details Parses raw MIDI data and creates InputEventMIDI events. Handles:
/// - System common messages (0xF0 and above)
/// - Channel voice messages with running status support
/// - Multiple MIDI message types with appropriate parameter extraction
/// 
/// The parsed event is passed to the input system for further processing.

/// @brief Retrieves a list of connected MIDI input device names.
/// @return A PackedStringArray containing the names of available MIDI input devices.

/// @brief Constructs a new MIDIDriver instance and sets it as the singleton.
#include "midi_driver.h"

#include "core/os/os.h"
#include "main/input_default.h"

uint8_t MIDIDriver::last_received_message = 0x00;
MIDIDriver *MIDIDriver::singleton = NULL;
MIDIDriver *MIDIDriver::get_singleton() {

	return singleton;
}

void MIDIDriver::set_singleton() {

	singleton = this;
}

void MIDIDriver::receive_input_packet(uint64_t timestamp, uint8_t *data, uint32_t length) {

	Ref<InputEventMIDI> event;
	event.instance();
	uint32_t param_position = 1;

	if (length >= 1) {
		if (data[0] >= 0xF0) {
			// channel does not apply to system common messages
			event->set_channel(0);
			event->set_message(data[0]);
			last_received_message = data[0];
		} else if ((data[0] & 0x80) == 0x00) {
			// running status
			event->set_channel(last_received_message & 0xF);
			event->set_message(last_received_message >> 4);
			param_position = 0;
		} else {
			event->set_channel(data[0] & 0xF);
			event->set_message(data[0] >> 4);
			param_position = 1;
			last_received_message = data[0];
		}
	}

	switch (event->get_message()) {
		case MIDI_MESSAGE_AFTERTOUCH:
			if (length >= 2 + param_position) {
				event->set_pitch(data[param_position]);
				event->set_pressure(data[param_position + 1]);
			}
			break;

		case MIDI_MESSAGE_CONTROL_CHANGE:
			if (length >= 2 + param_position) {
				event->set_controller_number(data[param_position]);
				event->set_controller_value(data[param_position + 1]);
			}
			break;

		case MIDI_MESSAGE_NOTE_ON:
		case MIDI_MESSAGE_NOTE_OFF:
			if (length >= 2 + param_position) {
				event->set_pitch(data[param_position]);
				event->set_velocity(data[param_position + 1]);

				if (event->get_message() == MIDI_MESSAGE_NOTE_ON && event->get_velocity() == 0) {
					// https://www.midi.org/forum/228-writing-midi-software-send-note-off,-or-zero-velocity-note-on
					event->set_message(MIDI_MESSAGE_NOTE_OFF);
				}
			}
			break;

		case MIDI_MESSAGE_PITCH_BEND:
			if (length >= 2 + param_position) {
				event->set_pitch((data[param_position + 1] << 7) | data[param_position]);
			}
			break;

		case MIDI_MESSAGE_PROGRAM_CHANGE:
			if (length >= 1 + param_position) {
				event->set_instrument(data[param_position]);
			}
			break;

		case MIDI_MESSAGE_CHANNEL_PRESSURE:
			if (length >= 1 + param_position) {
				event->set_pressure(data[param_position]);
			}
			break;
	}

	InputDefault *id = Object::cast_to<InputDefault>(Input::get_singleton());
	id->parse_input_event(event);
}

PackedStringArray MIDIDriver::get_connected_inputs() {

	PackedStringArray list;
	return list;
}

MIDIDriver::MIDIDriver() {

	set_singleton();
}

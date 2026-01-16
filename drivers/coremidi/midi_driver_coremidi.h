

/**
 * @class MIDIDriverCoreMidi
 * @brief CoreMIDI driver implementation for MIDI input handling on macOS.
 * 
 * This class provides MIDI input functionality on macOS by interfacing with the CoreMIDI framework.
 * It manages MIDI client creation, port configuration, and input source connections.
 * 
 * @note This class is only compiled when COREMIDI_ENABLED is defined.
 * 
 * @see MIDIDriver
 */

/**
 * @var MIDIClientRef MIDIDriverCoreMidi::client
 * @brief Reference to the CoreMIDI client instance.
 */

/**
 * @var MIDIPortRef MIDIDriverCoreMidi::port_in
 * @brief Reference to the MIDI input port.
 */

/**
 * @var Vector<MIDIEndpointRef> MIDIDriverCoreMidi::connected_sources
 * @brief Collection of connected MIDI input endpoints.
 */

/**
 * @fn void MIDIDriverCoreMidi::read(const MIDIPacketList *packet_list, void *read_proc_ref_con, void *src_conn_ref_con)
 * @brief Static callback function for processing incoming MIDI packets.
 * 
 * @param packet_list Pointer to the list of MIDI packets received.
 * @param read_proc_ref_con Reference context passed during port creation.
 * @param src_conn_ref_con Source connection reference context.
 */

/**
 * @fn Error MIDIDriverCoreMidi::open()
 * @brief Opens the MIDI driver and initializes CoreMIDI connections.
 * 
 * @return Error code indicating success or failure.
 */

/**
 * @fn void MIDIDriverCoreMidi::close()
 * @brief Closes the MIDI driver and cleans up CoreMIDI resources.
 */

/**
 * @fn PackedStringArray MIDIDriverCoreMidi::get_connected_inputs()
 * @brief Retrieves the names of all currently connected MIDI input sources.
 * 
 * @return Array of strings containing the names of connected MIDI inputs.
 */

/**
 * @fn MIDIDriverCoreMidi::MIDIDriverCoreMidi()
 * @brief Constructor for the CoreMIDI driver.
 */

/**
 * @fn MIDIDriverCoreMidi::~MIDIDriverCoreMidi()
 * @brief Virtual destructor for proper cleanup of CoreMIDI resources.
 */
#ifdef COREMIDI_ENABLED

#ifndef MIDI_DRIVER_COREMIDI_H
#define MIDI_DRIVER_COREMIDI_H

#include "core/os/midi_driver.h"
#include "core/vector.h"

#include <CoreMIDI/CoreMIDI.h>
#include <stdio.h>

class MIDIDriverCoreMidi : public MIDIDriver {

	MIDIClientRef client;
	MIDIPortRef port_in;

	Vector<MIDIEndpointRef> connected_sources;

	static void read(const MIDIPacketList *packet_list, void *read_proc_ref_con, void *src_conn_ref_con);

public:
	virtual Error open();
	virtual void close();

	PackedStringArray get_connected_inputs();

	MIDIDriverCoreMidi();
	virtual ~MIDIDriverCoreMidi();
};

#endif // MIDI_DRIVER_COREMIDI_H
#endif // COREMIDI_ENABLED

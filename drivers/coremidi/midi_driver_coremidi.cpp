

/**
 * @file midi_driver_coremidi.cpp
 * @brief CoreMIDI driver implementation for MIDI input handling on macOS.
 * 
 * This file implements a MIDI driver using Apple's CoreMIDI framework to handle
 * MIDI input from connected sources. It manages the creation, connection, and
 * cleanup of MIDI ports and endpoints.
 */

/**
 * @brief Callback function for reading MIDI packets from input sources.
 * 
 * This static callback is invoked by the CoreMIDI framework when MIDI data
 * is received from a connected source. It processes each packet in the packet
 * list and forwards the MIDI data to the input handler.
 * 
 * @param packet_list Pointer to the MIDIPacketList containing received packets.
 * @param read_proc_ref_con User-defined reference data (unused).
 * @param src_conn_ref_con Pointer to the MIDIDriverCoreMidi instance (this).
 */

/**
 * @brief Initializes and opens the CoreMIDI driver.
 * 
 * Creates a MIDI client, establishes an input port, and automatically connects
 * to all available MIDI sources. Must be called before any MIDI input can be
 * received.
 * 
 * @return Error code: OK on success, ERR_CANT_OPEN if client or port creation fails.
 */

/**
 * @brief Closes and cleans up the CoreMIDI driver.
 * 
 * Disconnects from all MIDI sources, disposes of the input port and MIDI client.
 * Should be called when the driver is no longer needed. Safe to call multiple times.
 */

/**
 * @brief Retrieves the display names of all connected MIDI input sources.
 * 
 * @return PackedStringArray containing the names of connected MIDI input devices.
 */

/**
 * @brief Constructs a new MIDIDriverCoreMidi instance.
 * 
 * Initializes the MIDI client handle to null.
 */

/**
 * @brief Destructs the MIDIDriverCoreMidi instance.
 * 
 * Ensures proper cleanup by calling close() if it hasn't been called explicitly.
 */
#ifdef COREMIDI_ENABLED

#include "midi_driver_coremidi.h"

#include "core/print_string.h"

#include <CoreAudio/HostTime.h>
#include <CoreServices/CoreServices.h>

void MIDIDriverCoreMidi::read(const MIDIPacketList *packet_list, void *read_proc_ref_con, void *src_conn_ref_con) {
	MIDIPacket *packet = const_cast<MIDIPacket *>(packet_list->packet);
	for (UInt32 i = 0; i < packet_list->numPackets; i++) {
		receive_input_packet(packet->timeStamp, packet->data, packet->length);
		packet = MIDIPacketNext(packet);
	}
}

Error MIDIDriverCoreMidi::open() {

	CFStringRef name = CFStringCreateWithCString(NULL, "Godot", kCFStringEncodingASCII);
	OSStatus result = MIDIClientCreate(name, NULL, NULL, &client);
	CFRelease(name);
	if (result != noErr) {
		ERR_PRINT("MIDIClientCreate failed, code: " + itos(result));
		return ERR_CANT_OPEN;
	}

	result = MIDIInputPortCreate(client, CFSTR("Godot Input"), MIDIDriverCoreMidi::read, (void *)this, &port_in);
	if (result != noErr) {
		ERR_PRINT("MIDIInputPortCreate failed, code: " + itos(result));
		return ERR_CANT_OPEN;
	}

	int sources = MIDIGetNumberOfSources();
	for (int i = 0; i < sources; i++) {

		MIDIEndpointRef source = MIDIGetSource(i);
		if (source) {
			MIDIPortConnectSource(port_in, source, (void *)this);
			connected_sources.insert(i, source);
		}
	}

	return OK;
}

void MIDIDriverCoreMidi::close() {

	for (int i = 0; i < connected_sources.size(); i++) {
		MIDIEndpointRef source = connected_sources[i];
		MIDIPortDisconnectSource(port_in, source);
	}
	connected_sources.clear();

	if (port_in != 0) {
		MIDIPortDispose(port_in);
		port_in = 0;
	}

	if (client != 0) {
		MIDIClientDispose(client);
		client = 0;
	}
}

PackedStringArray MIDIDriverCoreMidi::get_connected_inputs() {

	PackedStringArray list;

	for (int i = 0; i < connected_sources.size(); i++) {
		MIDIEndpointRef source = connected_sources[i];
		CFStringRef ref = NULL;
		char name[256];

		MIDIObjectGetStringProperty(source, kMIDIPropertyDisplayName, &ref);
		CFStringGetCString(ref, name, sizeof(name), kCFStringEncodingUTF8);
		CFRelease(ref);

		list.push_back(name);
	}

	return list;
}

MIDIDriverCoreMidi::MIDIDriverCoreMidi() :
		client(0) {
}

MIDIDriverCoreMidi::~MIDIDriverCoreMidi() {
	close();
}

#endif // COREMIDI_ENABLED

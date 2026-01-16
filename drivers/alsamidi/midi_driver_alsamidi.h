

/**
 * @class MIDIDriverALSAMidi
 * @brief ALSA MIDI driver implementation for Linux systems.
 * 
 * This class provides MIDI input functionality using the Advanced Linux Sound Architecture (ALSA).
 * It manages raw MIDI input connections and processes incoming MIDI events in a separate thread.
 * 
 * @details
 * The driver maintains a list of connected MIDI input devices and uses a dedicated thread
 * to handle incoming MIDI messages. Thread-safe operations are ensured through mutex locking.
 * 
 * @note This class is only compiled when ALSAMIDI_ENABLED is defined.
 * 
 * @see MIDIDriver
 */
#ifdef ALSAMIDI_ENABLED

#ifndef MIDI_DRIVER_ALSAMIDI_H
#define MIDI_DRIVER_ALSAMIDI_H

#include "core/os/midi_driver.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/vector.h"

#include <alsa/asoundlib.h>
#include <stdio.h>

class MIDIDriverALSAMidi : public MIDIDriver {

	Thread *thread;
	Mutex mutex;

	Vector<snd_rawmidi_t *> connected_inputs;

	bool exit_thread;

	static void thread_func(void *p_udata);

	void lock() const;
	void unlock() const;

public:
	virtual Error open();
	virtual void close();

	virtual PackedStringArray get_connected_inputs();

	MIDIDriverALSAMidi();
	virtual ~MIDIDriverALSAMidi();
};

#endif // MIDI_DRIVER_ALSAMIDI_H
#endif // ALSAMIDI_ENABLED

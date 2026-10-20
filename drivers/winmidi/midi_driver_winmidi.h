
#ifdef WINMIDI_ENABLED

#ifndef MIDI_DRIVER_WINMIDI_H
#define MIDI_DRIVER_WINMIDI_H

#include "core/os/midi_driver.h"
#include "core/vector.h"

#include <stdio.h>
#include <windows.h>

#include <mmsystem.h>

class MIDIDriverWinMidi : public MIDIDriver {

  Vector<HMIDIIN> connected_sources;

  static void CALLBACK read(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance,
                            DWORD_PTR dwParam1, DWORD_PTR dwParam2);

public:
  virtual Error open();
  virtual void close();

  virtual PackedStringArray get_connected_inputs();

  MIDIDriverWinMidi();
  virtual ~MIDIDriverWinMidi();
};

#endif // MIDI_DRIVER_WINMIDI_H
#endif // WINMIDI_ENABLED

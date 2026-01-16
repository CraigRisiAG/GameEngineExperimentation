

/**
 * @class AudioDriverCoreAudio
 * @brief CoreAudio driver implementation for audio input/output on macOS and iOS.
 * 
 * This class provides audio playback and capture functionality using Apple's CoreAudio framework.
 * It handles audio unit initialization, device management, and callback-based audio processing.
 * 
 * @details
 * - Manages both output and input audio units for playback and recording
 * - Supports device selection and enumeration on macOS
 * - Uses mutex-protected access for thread-safe audio operations
 * - Implements callback-based audio rendering and input capture
 * - Supports configurable sample rates, channels, and buffer sizes
 * 
 * @note Audio unit callbacks are static methods to comply with CoreAudio's C-style callback interface.
 * @note OSX-specific features like device enumeration and selection require the OSX_ENABLED preprocessor flag.
 * 
 * @see AudioDriver - Base class for audio driver implementations
 * @see AudioServer - Audio server that manages audio drivers
 */
#ifdef COREAUDIO_ENABLED

#ifndef AUDIO_DRIVER_COREAUDIO_H
#define AUDIO_DRIVER_COREAUDIO_H

#include "servers/audio_server.h"

#include <AudioUnit/AudioUnit.h>
#ifdef OSX_ENABLED
#include <CoreAudio/AudioHardware.h>
#endif

class AudioDriverCoreAudio : public AudioDriver {

	AudioComponentInstance audio_unit;
	AudioComponentInstance input_unit;

	bool active;
	Mutex mutex;

	String device_name;
	String capture_device_name;

	int mix_rate;
	unsigned int channels;
	unsigned int capture_channels;
	unsigned int buffer_frames;

	Vector<int32_t> samples_in;
	Vector<int16_t> input_buf;

#ifdef OSX_ENABLED
	Array _get_device_list(bool capture = false);
	void _set_device(const String &device, bool capture = false);

	static OSStatus input_device_address_cb(AudioObjectID inObjectID,
			UInt32 inNumberAddresses, const AudioObjectPropertyAddress *inAddresses,
			void *inClientData);

	static OSStatus output_device_address_cb(AudioObjectID inObjectID,
			UInt32 inNumberAddresses, const AudioObjectPropertyAddress *inAddresses,
			void *inClientData);
#endif

	static OSStatus output_callback(void *inRefCon,
			AudioUnitRenderActionFlags *ioActionFlags,
			const AudioTimeStamp *inTimeStamp,
			UInt32 inBusNumber, UInt32 inNumberFrames,
			AudioBufferList *ioData);

	static OSStatus input_callback(void *inRefCon,
			AudioUnitRenderActionFlags *ioActionFlags,
			const AudioTimeStamp *inTimeStamp,
			UInt32 inBusNumber, UInt32 inNumberFrames,
			AudioBufferList *ioData);

	Error capture_init();
	void capture_finish();

public:
	const char *get_name() const {
		return "CoreAudio";
	};

	virtual Error init();
	virtual void start();
	virtual int get_mix_rate() const;
	virtual SpeakerMode get_speaker_mode() const;

	virtual void lock();
	virtual void unlock();
	virtual void finish();

	virtual Error capture_start();
	virtual Error capture_stop();

	bool try_lock();
	void stop();

#ifdef OSX_ENABLED
	virtual Array get_device_list();
	virtual String get_device();
	virtual void set_device(String device);

	virtual Array capture_get_device_list();
	virtual void capture_set_device(const String &p_name);
	virtual String capture_get_device();
#endif

	AudioDriverCoreAudio();
	~AudioDriverCoreAudio();
};

#endif

#endif

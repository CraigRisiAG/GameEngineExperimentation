
/**
 * AudioDriverOpenSL - OpenSL ES audio driver for Android platform
 * 
 * Implements audio playback and recording functionality using OpenSL ES API.
 * Manages audio buffers, mixing, and device communication through callback-based
 * queue processing.
 * 
 * @class AudioDriverOpenSL
 * @extends AudioDriver
 * 
 * Key Features:
 * - Dual-buffer audio playback system (BUFFER_COUNT = 2)
 * - Real-time audio mixing with configurable buffer size
 * - Audio recording capability with separate buffer queue
 * - Thread-safe operations via mutex locking
 * - Dynamic pause/resume functionality
 * 
 * Private Members:
 * - active: Indicates if driver is actively processing audio
 * - pause: Pause state flag
 * - buffer_size: Size of audio buffers in samples
 * - buffers[BUFFER_COUNT]: Ring buffers for audio playback data
 * - mixdown_buffer: Temporary buffer for audio mixing (32-bit)
 * - rec_buffer: Recording buffer for captured audio
 * - sl, EngineItf, OutputMix: OpenSL ES engine and output configuration
 * - player, recorder: OpenSL ES audio objects for playback/recording
 * - bufferQueueItf, recordBufferQueueItf: Queue interfaces for buffer management
 * 
 * Callback Methods:
 * - _buffer_callback(): Processes playback buffer completion events
 * - _record_buffer_callback(): Processes recording buffer completion events
 * - Static callback wrappers for C-style OpenSL ES callback interface
 * 
 * @note Requires SLES/OpenSLES.h and Android-specific extensions
 * @note Uses singleton pattern for global driver access
 */
#ifndef AUDIO_DRIVER_OPENSL_H
#define AUDIO_DRIVER_OPENSL_H

#include "core/os/mutex.h"
#include "servers/audio_server.h"

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class AudioDriverOpenSL : public AudioDriver {

	bool active;
	Mutex mutex;

	enum {

		BUFFER_COUNT = 2
	};

	bool pause;

	uint32_t buffer_size;
	int16_t *buffers[BUFFER_COUNT];
	int32_t *mixdown_buffer;
	int last_free;

	Vector<int16_t> rec_buffer;

	SLPlayItf playItf;
	SLRecordItf recordItf;
	SLObjectItf sl;
	SLEngineItf EngineItf;
	SLObjectItf OutputMix;
	SLVolumeItf volumeItf;
	SLObjectItf player;
	SLObjectItf recorder;
	SLAndroidSimpleBufferQueueItf bufferQueueItf;
	SLAndroidSimpleBufferQueueItf recordBufferQueueItf;
	SLDataSource audioSource;
	SLDataFormat_PCM pcm;
	SLDataSink audioSink;
	SLDataLocator_OutputMix locator_outputmix;
	SLBufferQueueState state;

	static AudioDriverOpenSL *s_ad;

	void _buffer_callback(
			SLAndroidSimpleBufferQueueItf queueItf);

	static void _buffer_callbacks(
			SLAndroidSimpleBufferQueueItf queueItf,
			void *pContext);

	void _record_buffer_callback(
			SLAndroidSimpleBufferQueueItf queueItf);

	static void _record_buffer_callbacks(
			SLAndroidSimpleBufferQueueItf queueItf,
			void *pContext);

	virtual Error capture_init_device();

public:
	void set_singleton();

	virtual const char *get_name() const;

	virtual Error init();
	virtual void start();
	virtual int get_mix_rate() const;
	virtual SpeakerMode get_speaker_mode() const;
	virtual void lock();
	virtual void unlock();
	virtual void finish();

	virtual void set_pause(bool p_pause);

	virtual Error capture_start();
	virtual Error capture_stop();

	AudioDriverOpenSL();
};

#endif // AUDIO_DRIVER_ANDROID_H

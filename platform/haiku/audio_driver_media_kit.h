
/**
 * @class AudioDriverMediaKit
 * @brief Audio driver implementation using Haiku's Media Kit.
 * 
 * Provides audio output functionality for the Haiku operating system by leveraging
 * the Media Kit's sound playback capabilities. Manages audio buffering, format
 * conversion, and playback thread synchronization.
 * 
 * @note This driver is only compiled when MEDIA_KIT_ENABLED is defined.
 * 
 * @see AudioDriver
 * @see BSoundPlayer
 */

/**
 * @brief Mutex for thread-safe access to audio driver resources.
 */

/**
 * @brief Pointer to the BSoundPlayer instance managing audio playback.
 */

/**
 * @brief Static buffer holding audio samples for playback.
 */

/**
 * @brief Audio playback callback function invoked by the Media Kit.
 * 
 * @param cookie User-defined context pointer.
 * @param buffer Pointer to the audio buffer to be filled.
 * @param size Size of the buffer in bytes.
 * @param format Audio format specification for the current playback session.
 */

/**
 * @brief Audio sampling rate in Hz (e.g., 44100, 48000).
 */

/**
 * @brief Speaker configuration mode (mono, stereo, surround, etc.).
 */

/**
 * @brief Size of each audio buffer in samples.
 */

/**
 * @brief Number of audio channels (1 for mono, 2 for stereo, etc.).
 */

/**
 * @brief Indicates whether audio playback is currently active.
 */

/**
 * @brief Retrieves the driver name.
 * @return The string "MediaKit".
 */

/**
 * @brief Initializes the audio driver and Media Kit resources.
 * @return OK on success, or an error code on failure.
 */

/**
 * @brief Starts audio playback.
 */

/**
 * @brief Retrieves the current audio mix rate.
 * @return The mix rate in Hz.
 */

/**
 * @brief Retrieves the current speaker mode configuration.
 * @return The active speaker mode.
 */

/**
 * @brief Acquires the audio driver mutex for thread-safe operations.
 */

/**
 * @brief Releases the audio driver mutex.
 */

/**
 * @brief Stops audio playback and releases all driver resources.
 */

/**
 * @brief Constructs an AudioDriverMediaKit instance.
 */

/**
 * @brief Destructs the AudioDriverMediaKit instance and cleans up resources.
 */
#include "servers/audio_server.h"

#ifdef MEDIA_KIT_ENABLED

#include "core/os/mutex.h"
#include "core/os/thread.h"

#include <kernel/image.h> // needed for image_id

#include <SoundPlayer.h>

class AudioDriverMediaKit : public AudioDriver {
	Mutex mutex;

	BSoundPlayer *player;
	static int32_t *samples_in;

	static void PlayBuffer(void *cookie, void *buffer, size_t size, const media_raw_audio_format &format);

	unsigned int mix_rate;
	SpeakerMode speaker_mode;
	unsigned int buffer_size;
	int channels;

	bool active;

public:
	const char *get_name() const {
		return "MediaKit";
	};

	virtual Error init();
	virtual void start();
	virtual int get_mix_rate() const;
	virtual SpeakerMode get_speaker_mode() const;
	virtual void lock();
	virtual void unlock();
	virtual void finish();

	AudioDriverMediaKit();
	~AudioDriverMediaKit();
};

#endif

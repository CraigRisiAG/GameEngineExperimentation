
/**
 * @file audio_driver_media_kit.cpp
 * @brief Audio driver implementation for Haiku OS using the MediaKit library.
 * 
 * This file provides audio output functionality for the game engine on Haiku OS
 * by implementing the AudioDriverMediaKit class. It manages audio initialization,
 * buffer processing, and playback through the BeOS/Haiku MediaKit BSoundPlayer API.
 * 
 * @class AudioDriverMediaKit
 * @brief Haiku OS audio driver using MediaKit BSoundPlayer.
 * 
 * Handles audio configuration, mixing, and real-time playback on Haiku systems.
 * Uses a callback-based approach where PlayBuffer is invoked by the BSoundPlayer
 * to fill audio buffers with mixed audio data.
 */

/**
 * @var int32_t *AudioDriverMediaKit::samples_in
 * @brief Static buffer storing mixed audio samples between callback invocations.
 */

/**
 * @fn Error AudioDriverMediaKit::init()
 * @brief Initializes the audio driver and creates the BSoundPlayer instance.
 * 
 * Configures audio parameters (mix rate, channels, buffer size), allocates
 * sample buffer memory, sets up the MediaKit audio format, and starts playback.
 * 
 * @return Error code: OK on success, ERR_CANT_OPEN if BSoundPlayer creation fails.
 */

/**
 * @fn void AudioDriverMediaKit::PlayBuffer(void *cookie, void *buffer, size_t size, const media_raw_audio_format &format)
 * @brief Static callback invoked by BSoundPlayer to fill audio buffers.
 * 
 * Processes audio by calling the audio server mixer when active, or fills the
 * buffer with silence. Copies the mixed samples to the output buffer.
 * 
 * @param cookie Pointer to the AudioDriverMediaKit instance.
 * @param buffer Output audio buffer to fill.
 * @param size Size of the buffer in bytes.
 * @param format MediaKit audio format specification.
 */

/**
 * @fn void AudioDriverMediaKit::start()
 * @brief Activates audio processing and playback.
 */

/**
 * @fn int AudioDriverMediaKit::get_mix_rate() const
 * @brief Returns the configured audio mix rate in Hz.
 * @return Mix rate value.
 */

/**
 * @fn SpeakerMode AudioDriverMediaKit::get_speaker_mode() const
 * @brief Returns the speaker configuration mode.
 * @return Current speaker mode (stereo).
 */

/**
 * @fn void AudioDriverMediaKit::lock()
 * @brief Acquires the audio mutex for thread-safe access.
 */

/**
 * @fn void AudioDriverMediaKit::unlock()
 * @brief Releases the audio mutex.
 */

/**
 * @fn void AudioDriverMediaKit::finish()
 * @brief Shuts down the audio driver and releases resources.
 * 
 * Deletes the BSoundPlayer instance and deallocates the sample buffer.
 */

/**
 * @fn AudioDriverMediaKit::AudioDriverMediaKit()
 * @brief Constructor. Initializes player pointer to NULL.
 */

/**
 * @fn AudioDriverMediaKit::~AudioDriverMediaKit()
 * @brief Destructor.
 */
#include "audio_driver_media_kit.h"

#ifdef MEDIA_KIT_ENABLED

#include "core/project_settings.h"

int32_t *AudioDriverMediaKit::samples_in = NULL;

Error AudioDriverMediaKit::init() {
	active = false;

	mix_rate = GLOBAL_DEF_RST("audio/mix_rate", DEFAULT_MIX_RATE);
	speaker_mode = SPEAKER_MODE_STEREO;
	channels = 2;

	int latency = GLOBAL_DEF_RST("audio/output_latency", DEFAULT_OUTPUT_LATENCY);
	buffer_size = next_power_of_2(latency * mix_rate / 1000);
	samples_in = memnew_arr(int32_t, buffer_size * channels);

	media_raw_audio_format format;
	format = media_raw_audio_format::wildcard;
	format.frame_rate = mix_rate;
	format.channel_count = channels;
	format.format = media_raw_audio_format::B_AUDIO_INT;
	format.byte_order = B_MEDIA_LITTLE_ENDIAN;
	format.buffer_size = buffer_size * sizeof(int32_t) * channels;

	player = new BSoundPlayer(
			&format,
			"godot_sound_server",
			AudioDriverMediaKit::PlayBuffer,
			NULL,
			this);

	if (player->InitCheck() != B_OK) {
		fprintf(stderr, "MediaKit ERR: can not create a BSoundPlayer instance\n");
		ERR_FAIL_COND_V(player == NULL, ERR_CANT_OPEN);
	}

	player->Start();

	return OK;
}

void AudioDriverMediaKit::PlayBuffer(void *cookie, void *buffer, size_t size, const media_raw_audio_format &format) {
	AudioDriverMediaKit *ad = (AudioDriverMediaKit *)cookie;
	int32_t *buf = (int32_t *)buffer;

	if (!ad->active) {
		for (unsigned int i = 0; i < ad->buffer_size * ad->channels; i++) {
			AudioDriverMediaKit::samples_in[i] = 0;
		}
	} else {
		ad->lock();
		ad->audio_server_process(ad->buffer_size, AudioDriverMediaKit::samples_in);
		ad->unlock();
	}

	for (unsigned int i = 0; i < ad->buffer_size * ad->channels; i++) {
		buf[i] = AudioDriverMediaKit::samples_in[i];
	}
}

void AudioDriverMediaKit::start() {
	active = true;
}

int AudioDriverMediaKit::get_mix_rate() const {
	return mix_rate;
}

AudioDriverMediaKit::SpeakerMode AudioDriverMediaKit::get_speaker_mode() const {
	return speaker_mode;
}

void AudioDriverMediaKit::lock() {
	if (!mutex)
		return;

	mutex.lock();
}

void AudioDriverMediaKit::unlock() {
	if (!mutex)
		return;

	mutex.unlock();
}

void AudioDriverMediaKit::finish() {
	delete player;

	if (samples_in) {
		memdelete_arr(samples_in);
	};
}

AudioDriverMediaKit::AudioDriverMediaKit() {
	player = NULL;
}

AudioDriverMediaKit::~AudioDriverMediaKit() {
}

#endif

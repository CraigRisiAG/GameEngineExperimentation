
/**
 * @class AudioDriverJavaScript
 * @brief JavaScript-based audio driver implementation for web platforms.
 *
 * This class provides audio input/output functionality for
 * JavaScript/WebAssembly environments. It inherits from AudioDriver and handles
 * audio mixing, capture, and playback through JavaScript audio APIs.
 *
 * @member internal_buffer Pointer to the internal audio buffer for mixing
 * operations.
 * @member _driver_id Unique identifier for this audio driver instance.
 * @member buffer_length Size of the audio buffer in samples.
 * @member singleton Static pointer to the singleton instance of the audio
 * driver.
 *
 * @method mix_to_js() Mixes audio data to be sent to JavaScript audio context.
 * @method process_capture(float sample) Processes captured audio sample from
 * input device.
 * @method get_name() Returns the name identifier of this audio driver
 * ("JavaScript").
 * @method init() Initializes the audio driver and sets up audio context.
 * @method start() Starts audio playback.
 * @method resume() Resumes audio playback after pause.
 * @method get_mix_rate() Returns the audio mixing sample rate in Hz.
 * @method get_speaker_mode() Returns the current speaker configuration mode.
 * @method lock() Acquires lock for thread-safe audio buffer access.
 * @method unlock() Releases lock on audio buffer.
 * @method finish() Cleans up and shuts down the audio driver.
 * @method capture_start() Initiates audio capture from input device.
 * @method capture_stop() Stops audio capture.
 *
 * @note This driver is specific to JavaScript/WebAssembly builds and may not be
 * available on other platforms.
 */
#ifndef AUDIO_DRIVER_JAVASCRIPT_H
#define AUDIO_DRIVER_JAVASCRIPT_H

#include "servers/audio_server.h"

class AudioDriverJavaScript : public AudioDriver {

  float *internal_buffer;

  int _driver_id;
  int buffer_length;

public:
  void mix_to_js();
  void process_capture(float sample);

  static AudioDriverJavaScript *singleton;

  virtual const char *get_name() const;

  virtual Error init();
  virtual void start();
  void resume();
  virtual int get_mix_rate() const;
  virtual SpeakerMode get_speaker_mode() const;
  virtual void lock();
  virtual void unlock();
  virtual void finish();

  virtual Error capture_start();
  virtual Error capture_stop();

  AudioDriverJavaScript();
};

#endif

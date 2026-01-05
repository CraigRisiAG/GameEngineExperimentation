
/// @class AudioDriverAndroid
/// @brief Android platform audio driver implementation.
///
/// Implements the AudioDriver interface for Android devices using JNI to interact
/// with Java/Kotlin audio APIs. Manages audio initialization, mixing, playback control,
/// and thread synchronization.
///
/// @details
/// This driver handles:
/// - Audio buffer management and pinning for JNI interop
/// - Thread-safe access to audio resources via mutex locking
/// - JNI method binding for audio operations (init, write, pause, quit)
/// - Audio mixing at a specified sample rate with speaker mode configuration
/// - Audio playback lifecycle (start, pause, finish)
///
/// @note This class uses static methods and members for JNI callback integration.
/// The audio buffer is pinned in memory to ensure stable JNI access.
///
/// @see AudioDriver
/// @see JavaGodotLibJNI
#ifndef AUDIO_DRIVER_ANDROID_H
#define AUDIO_DRIVER_ANDROID_H

#include "servers/audio_server.h"

#include "java_godot_lib_jni.h"

class AudioDriverAndroid : public AudioDriver {

	static Mutex mutex;
	static AudioDriverAndroid *s_ad;
	static jobject io;
	static jmethodID _init_audio;
	static jmethodID _write_buffer;
	static jmethodID _quit;
	static jmethodID _pause;
	static bool active;
	static bool quit;

	static jclass cls;

	static jobject audioBuffer;
	static void *audioBufferPinned;
	static int32_t *audioBuffer32;
	static int audioBufferFrames;
	static int mix_rate;

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

	static void setup(jobject p_io);
	static void thread_func(JNIEnv *env);

	AudioDriverAndroid();
};

#endif // AUDIO_DRIVER_ANDROID_H

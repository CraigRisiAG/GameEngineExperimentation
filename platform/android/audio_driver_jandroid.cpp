
/**
 * @file audio_driver_jandroid.cpp
 * @brief Android audio driver implementation using JNI for audio processing.
 * 
 * This file implements the AudioDriverAndroid class, which provides audio output
 * functionality on Android platforms by interfacing with Java audio APIs through JNI.
 * 
 * @class AudioDriverAndroid
 * @brief JNI-based audio driver for Android platform.
 * 
 * Manages audio initialization, mixing, and playback on Android devices.
 * Uses a separate thread to handle audio processing and communicates with Java
 * audio components through JNI method calls.
 * 
 * @member s_ad
 * @brief Static pointer to the singleton AudioDriverAndroid instance.
 * 
 * @member io
 * @brief JNI reference to the Java GodotIO audio handler object.
 * 
 * @member _init_audio
 * @brief JNI method ID for Java audioInit(int, int) method.
 * 
 * @member _write_buffer
 * @brief JNI method ID for Java audioWriteShortBuffer(short[]) method.
 * 
 * @member _quit
 * @brief JNI method ID for Java audioQuit() method.
 * 
 * @member _pause
 * @brief JNI method ID for Java audioPause(boolean) method.
 * 
 * @member active
 * @brief Flag indicating if the audio driver is actively processing audio.
 * 
 * @member cls
 * @brief Global JNI reference to the GodotIO Java class.
 * 
 * @member audioBufferFrames
 * @brief Number of audio frames in the buffer.
 * 
 * @member mix_rate
 * @brief Audio mixing sample rate in Hz (default 44100).
 * 
 * @member quit
 * @brief Flag signaling the audio thread to exit.
 * 
 * @member audioBuffer
 * @brief JNI reference to the Java short array audio buffer.
 * 
 * @member audioBufferPinned
 * @brief Pinned memory pointer to the audio buffer for direct access.
 * 
 * @member mutex
 * @brief Synchronization mutex for thread-safe audio buffer access.
 * 
 * @member audioBuffer32
 * @brief 32-bit audio buffer for internal processing before conversion to 16-bit.
 * 
 * @method get_name()
 * @brief Returns the audio driver name.
 * @return const char* - "Android"
 * 
 * @method init()
 * @brief Initializes the audio driver and allocates audio buffers.
 * Retrieves audio configuration from project settings and sets up JNI references.
 * @return Error - OK on success, ERR_INVALID_PARAMETER if buffer allocation fails.
 * 
 * @method start()
 * @brief Starts audio processing by setting the active flag.
 * 
 * @method setup(jobject p_io)
 * @brief Configures JNI method references for Java audio calls.
 * @param p_io - JNI reference to the GodotIO Java object.
 * 
 * @method thread_func(JNIEnv* env)
 * @brief Main audio processing thread function.
 * Continuously reads audio data from the audio server, converts to 16-bit format,
 * and sends to Java audio output. Runs until quit flag is set.
 * @param env - JNI environment pointer.
 * 
 * @method get_mix_rate() const
 * @brief Returns the current audio mixing sample rate.
 * @return int - Mix rate in Hz.
 * 
 * @method get_speaker_mode() const
 * @brief Returns the audio speaker configuration.
 * @return AudioDriver::SpeakerMode - SPEAKER_MODE_STEREO.
 * 
 * @method lock()
 * @brief Acquires the audio buffer mutex for synchronization.
 * 
 * @method unlock()
 * @brief Releases the audio buffer mutex.
 * 
 * @method finish()
 * @brief Cleans up audio resources and releases JNI references.
 * 
 * @method set_pause(bool p_pause)
 * @brief Pauses or resumes audio playback.
 * @param p_pause - True to pause, false to resume.
 * 
 * @method AudioDriverAndroid()
 * @brief Constructor. Initializes the driver as inactive and sets singleton instance.
 */
#include "audio_driver_jandroid.h"

#include "core/os/os.h"
#include "core/project_settings.h"
#include "thread_jandroid.h"

AudioDriverAndroid *AudioDriverAndroid::s_ad = NULL;

jobject AudioDriverAndroid::io;
jmethodID AudioDriverAndroid::_init_audio;
jmethodID AudioDriverAndroid::_write_buffer;
jmethodID AudioDriverAndroid::_quit;
jmethodID AudioDriverAndroid::_pause;
bool AudioDriverAndroid::active = false;
jclass AudioDriverAndroid::cls;
int AudioDriverAndroid::audioBufferFrames = 0;
int AudioDriverAndroid::mix_rate = 44100;
bool AudioDriverAndroid::quit = false;
jobject AudioDriverAndroid::audioBuffer = NULL;
void *AudioDriverAndroid::audioBufferPinned = NULL;
Mutex AudioDriverAndroid::mutex;
int32_t *AudioDriverAndroid::audioBuffer32 = NULL;

const char *AudioDriverAndroid::get_name() const {

	return "Android";
}

Error AudioDriverAndroid::init() {

	/*
	// TODO: pass in/return a (Java) device ID, also whether we're opening for input or output
	   this->spec.samples = Android_JNI_OpenAudioDevice(this->spec.freq, this->spec.format == AUDIO_U8 ? 0 : 1, this->spec.channels, this->spec.samples);
	   SDL_CalculateAudioSpec(&this->spec);

	   if (this->spec.samples == 0) {
	       // Init failed?
	       SDL_SetError("Java-side initialization failed!");
	       return 0;
	   }
*/

	//Android_JNI_SetupThread();

	//        __android_log_print(ANDROID_LOG_VERBOSE, "SDL", "SDL audio: opening device");

	JNIEnv *env = ThreadAndroid::get_env();
	int mix_rate = GLOBAL_DEF_RST("audio/mix_rate", 44100);

	int latency = GLOBAL_DEF_RST("audio/output_latency", 25);
	unsigned int buffer_size = next_power_of_2(latency * mix_rate / 1000);
	print_verbose("Audio buffer size: " + itos(buffer_size));

	audioBuffer = env->CallObjectMethod(io, _init_audio, mix_rate, buffer_size);

	ERR_FAIL_COND_V(audioBuffer == NULL, ERR_INVALID_PARAMETER);

	audioBuffer = env->NewGlobalRef(audioBuffer);

	jboolean isCopy = JNI_FALSE;
	audioBufferPinned = env->GetShortArrayElements((jshortArray)audioBuffer, &isCopy);
	audioBufferFrames = env->GetArrayLength((jshortArray)audioBuffer);
	audioBuffer32 = memnew_arr(int32_t, audioBufferFrames);

	return OK;
}

void AudioDriverAndroid::start() {
	active = true;
}

void AudioDriverAndroid::setup(jobject p_io) {

	JNIEnv *env = ThreadAndroid::get_env();
	io = p_io;

	jclass c = env->GetObjectClass(io);
	cls = (jclass)env->NewGlobalRef(c);

	_init_audio = env->GetMethodID(cls, "audioInit", "(II)Ljava/lang/Object;");
	_write_buffer = env->GetMethodID(cls, "audioWriteShortBuffer", "([S)V");
	_quit = env->GetMethodID(cls, "audioQuit", "()V");
	_pause = env->GetMethodID(cls, "audioPause", "(Z)V");
}

void AudioDriverAndroid::thread_func(JNIEnv *env) {

	jclass cls = env->FindClass("org/godotengine/godot/Godot");
	if (cls) {

		cls = (jclass)env->NewGlobalRef(cls);
	}
	jfieldID fid = env->GetStaticFieldID(cls, "io", "Lorg/godotengine/godot/GodotIO;");
	jobject ob = env->GetStaticObjectField(cls, fid);
	jobject gob = env->NewGlobalRef(ob);
	jclass c = env->GetObjectClass(gob);
	jclass lcls = (jclass)env->NewGlobalRef(c);
	_write_buffer = env->GetMethodID(lcls, "audioWriteShortBuffer", "([S)V");

	while (!quit) {

		int16_t *ptr = (int16_t *)audioBufferPinned;
		int fc = audioBufferFrames;

		if (!s_ad->active || mutex.try_lock() != OK) {

			for (int i = 0; i < fc; i++) {
				ptr[i] = 0;
			}

		} else {

			s_ad->audio_server_process(fc / 2, audioBuffer32);

			mutex.unlock();

			for (int i = 0; i < fc; i++) {

				ptr[i] = audioBuffer32[i] >> 16;
			}
		}
		env->ReleaseShortArrayElements((jshortArray)audioBuffer, (jshort *)ptr, JNI_COMMIT);
		env->CallVoidMethod(gob, _write_buffer, (jshortArray)audioBuffer);
	}
}

int AudioDriverAndroid::get_mix_rate() const {

	return mix_rate;
}

AudioDriver::SpeakerMode AudioDriverAndroid::get_speaker_mode() const {

	return SPEAKER_MODE_STEREO;
}

void AudioDriverAndroid::lock() {

	mutex.lock();
}

void AudioDriverAndroid::unlock() {

	mutex.unlock();
}

void AudioDriverAndroid::finish() {

	JNIEnv *env = ThreadAndroid::get_env();
	env->CallVoidMethod(io, _quit);

	if (audioBuffer) {
		env->DeleteGlobalRef(audioBuffer);
		audioBuffer = NULL;
		audioBufferPinned = NULL;
	}

	active = false;
}

void AudioDriverAndroid::set_pause(bool p_pause) {

	JNIEnv *env = ThreadAndroid::get_env();
	env->CallVoidMethod(io, _pause, p_pause);
}

AudioDriverAndroid::AudioDriverAndroid() {

	s_ad = this;
	active = false;
}


#include "audio_driver_dummy.h"

#include "core/os/os.h"
#include "core/project_settings.h"

Error AudioDriverDummy::init() {

  active = false;
  thread_exited = false;
  exit_thread = false;
  samples_in = NULL;

  mix_rate = DEFAULT_MIX_RATE;
  speaker_mode = SPEAKER_MODE_STEREO;
  channels = 2;

  int latency = GLOBAL_DEF_RST("audio/output_latency", DEFAULT_OUTPUT_LATENCY);
  buffer_frames = closest_power_of_2(latency * mix_rate / 1000);

  samples_in = memnew_arr(int32_t, buffer_frames *channels);

  thread = Thread::create(AudioDriverDummy::thread_func, this);

  return OK;
};

void AudioDriverDummy::thread_func(void *p_udata) {

  AudioDriverDummy *ad = (AudioDriverDummy *)p_udata;

  uint64_t usdelay = (ad->buffer_frames / float(ad->mix_rate)) * 1000000;

  while (!ad->exit_thread) {

    if (ad->active) {

      ad->lock();

      ad->audio_server_process(ad->buffer_frames, ad->samples_in);

      ad->unlock();
    };

    OS::get_singleton()->delay_usec(usdelay);
  };

  ad->thread_exited = true;
};

void AudioDriverDummy::start() { active = true; };

int AudioDriverDummy::get_mix_rate() const { return mix_rate; };

AudioDriver::SpeakerMode AudioDriverDummy::get_speaker_mode() const {

  return speaker_mode;
};

void AudioDriverDummy::lock() {

  if (!thread)
    return;
  mutex.lock();
};

void AudioDriverDummy::unlock() {

  if (!thread)
    return;
  mutex.unlock();
};

void AudioDriverDummy::finish() {

  if (!thread)
    return;

  exit_thread = true;
  Thread::wait_to_finish(thread);

  if (samples_in) {
    memdelete_arr(samples_in);
  };

  memdelete(thread);
  thread = NULL;
};

AudioDriverDummy::AudioDriverDummy() { thread = NULL; };

AudioDriverDummy::~AudioDriverDummy() {

};

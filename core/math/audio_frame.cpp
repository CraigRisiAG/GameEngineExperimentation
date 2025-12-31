

#include "audio_frame.h"
void AudioFrame::clear() {

  for (int i = 0; i < AUDIO_FRAME_MAX_CHANNELS; i++) {
    channels[i] = 0.0;
  }
}

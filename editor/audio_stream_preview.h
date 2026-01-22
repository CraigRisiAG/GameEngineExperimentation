


/// @class AudioStreamPreview
/// @brief Stores and provides access to audio stream preview data.
/// 
/// AudioStreamPreview encapsulates preview information for an audio stream,
/// including the preview waveform data and stream length. It provides methods
/// to query maximum and minimum amplitude values over time intervals.
///
/// @note This class is managed as a Reference and should be used with Ref<>.

/// @class AudioStreamPreviewGenerator
/// @brief Singleton that asynchronously generates audio stream previews.
///
/// AudioStreamPreviewGenerator manages the creation of preview data for audio streams
/// in a background thread to avoid blocking the main thread. It maintains a collection
/// of active preview generation tasks and emits signals when previews are complete.
///
/// @note This is a singleton class. Use get_singleton() to access the instance.
///
/// @method static AudioStreamPreviewGenerator *get_singleton()
/// @brief Returns the singleton instance of AudioStreamPreviewGenerator.
/// @return Pointer to the singleton instance.
///
/// @method Ref<AudioStreamPreview> generate_preview(const Ref<AudioStream> &p_stream)
/// @brief Initiates asynchronous preview generation for the given audio stream.
/// @param p_stream The audio stream to generate a preview for.
/// @return A reference to the AudioStreamPreview object (populated asynchronously).
#ifndef AUDIO_STREAM_PREVIEW_H
#define AUDIO_STREAM_PREVIEW_H

#include "core/os/thread.h"
#include "scene/main/node.h"
#include "servers/audio/audio_stream.h"

class AudioStreamPreview : public Reference {
	GDCLASS(AudioStreamPreview, Reference);
	friend class AudioStream;
	Vector<uint8_t> preview;
	float length;

	friend class AudioStreamPreviewGenerator;

public:
	float get_length() const;
	float get_max(float p_time, float p_time_next) const;
	float get_min(float p_time, float p_time_next) const;

	AudioStreamPreview();
};

class AudioStreamPreviewGenerator : public Node {
	GDCLASS(AudioStreamPreviewGenerator, Node);

	static AudioStreamPreviewGenerator *singleton;

	struct Preview {
		Ref<AudioStreamPreview> preview;
		Ref<AudioStream> base_stream;
		Ref<AudioStreamPlayback> playback;
		volatile bool generating;
		ObjectID id;
		Thread *thread;
	};

	Map<ObjectID, Preview> previews;

	static void _preview_thread(void *p_preview);

	void _update_emit(ObjectID p_id);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	static AudioStreamPreviewGenerator *get_singleton() { return singleton; }

	Ref<AudioStreamPreview> generate_preview(const Ref<AudioStream> &p_stream);

	AudioStreamPreviewGenerator();
};

#endif // AUDIO_STREAM_PREVIEW_H

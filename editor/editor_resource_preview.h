

/**
 * @class EditorResourcePreviewGenerator
 * @brief Base class for generating preview thumbnails of editor resources.
 * 
 * EditorResourcePreviewGenerator is an abstract base class that defines the interface
 * for creating preview textures of various resource types. Subclasses should override
 * the virtual methods to handle specific resource types and generate appropriate previews.
 */

/**
 * @fn virtual bool EditorResourcePreviewGenerator::handles(const String &p_type) const
 * @brief Checks if this generator can handle the specified resource type.
 * @param p_type The resource type string to check.
 * @return true if this generator can handle the resource type, false otherwise.
 */

/**
 * @fn virtual Ref<Texture2D> EditorResourcePreviewGenerator::generate(const RES &p_from, const Size2 &p_size) const
 * @brief Generates a preview texture from a resource object.
 * @param p_from The resource to generate a preview for.
 * @param p_size The desired size of the preview texture.
 * @return A Texture2D reference containing the generated preview, or null if generation failed.
 */

/**
 * @fn virtual Ref<Texture2D> EditorResourcePreviewGenerator::generate_from_path(const String &p_path, const Size2 &p_size) const
 * @brief Generates a preview texture from a file path.
 * @param p_path The file path of the resource to preview.
 * @param p_size The desired size of the preview texture.
 * @return A Texture2D reference containing the generated preview, or null if generation failed.
 */

/**
 * @fn virtual bool EditorResourcePreviewGenerator::generate_small_preview_automatically() const
 * @brief Determines if small previews should be automatically generated for this resource type.
 * @return true if small previews should be auto-generated, false otherwise.
 */

/**
 * @fn virtual bool EditorResourcePreviewGenerator::can_generate_small_preview() const
 * @brief Checks if this generator is capable of generating small preview thumbnails.
 * @return true if small preview generation is supported, false otherwise.
 */

/**
 * @class EditorResourcePreview
 * @brief Manages the generation and caching of resource preview thumbnails.
 * 
 * EditorResourcePreview is a singleton node that handles the queuing, generation, and
 * caching of preview thumbnails for editor resources. It uses a background thread to
 * asynchronously generate previews and notifies interested objects via callback functions
 * when previews are ready.
 */

/**
 * @fn static EditorResourcePreview* EditorResourcePreview::get_singleton()
 * @brief Retrieves the singleton instance of EditorResourcePreview.
 * @return Pointer to the EditorResourcePreview singleton.
 */

/**
 * @fn void EditorResourcePreview::queue_resource_preview(const String &p_path, Object *p_receiver, const StringName &p_receiver_func, const Variant &p_userdata)
 * @brief Queues a resource file for preview generation.
 * @param p_path The file path of the resource to preview.
 * @param p_receiver The object that will receive the preview callback.
 * @param p_receiver_func The callback function name to invoke with the generated preview.
 * @param p_userdata Custom user data to pass to the callback function.
 */

/**
 * @fn void EditorResourcePreview::queue_edited_resource_preview(const Ref<Resource> &p_res, Object *p_receiver, const StringName &p_receiver_func, const Variant &p_userdata)
 * @brief Queues an in-memory resource for preview generation.
 * @param p_res The resource object to generate a preview for.
 * @param p_receiver The object that will receive the preview callback.
 * @param p_receiver_func The callback function name to invoke with the generated preview.
 * @param p_userdata Custom user data to pass to the callback function.
 */

/**
 * @fn void EditorResourcePreview::add_preview_generator(const Ref<EditorResourcePreviewGenerator> &p_generator)
 * @brief Registers a preview generator for handling specific resource types.
 * @param p_generator The preview generator to add.
 */

/**
 * @fn void EditorResourcePreview::remove_preview_generator(const Ref<EditorResourcePreviewGenerator> &p_generator)
 * @brief Unregisters a preview generator.
 * @param p_generator The preview generator to remove.
 */

/**
 * @fn void EditorResourcePreview::check_for_invalidation(const String &p_path)
 * @brief Checks if cached previews for a resource are still valid and invalidates them if necessary.
 * @param p_path The file path of the resource to check.
 */

/**
 * @fn void EditorResourcePreview::start()
 * @brief Starts the background preview generation thread.
 */

/**
 * @fn void EditorResourcePreview::stop()
 * @brief Stops the background preview generation thread.
 */
#ifndef EDITORRESOURCEPREVIEW_H
#define EDITORRESOURCEPREVIEW_H

#include "core/os/semaphore.h"
#include "core/os/thread.h"
#include "scene/main/node.h"
#include "scene/resources/texture.h"

class EditorResourcePreviewGenerator : public Reference {

	GDCLASS(EditorResourcePreviewGenerator, Reference);

protected:
	static void _bind_methods();

public:
	virtual bool handles(const String &p_type) const;
	virtual Ref<Texture2D> generate(const RES &p_from, const Size2 &p_size) const;
	virtual Ref<Texture2D> generate_from_path(const String &p_path, const Size2 &p_size) const;

	virtual bool generate_small_preview_automatically() const;
	virtual bool can_generate_small_preview() const;

	EditorResourcePreviewGenerator();
};

class EditorResourcePreview : public Node {

	GDCLASS(EditorResourcePreview, Node);

	static EditorResourcePreview *singleton;

	struct QueueItem {
		Ref<Resource> resource;
		String path;
		ObjectID id;
		StringName function;
		Variant userdata;
	};

	List<QueueItem> queue;

	Mutex preview_mutex;
	Semaphore preview_sem;
	Thread *thread;
	volatile bool exit;
	volatile bool exited;

	struct Item {
		Ref<Texture2D> preview;
		Ref<Texture2D> small_preview;
		int order;
		uint32_t last_hash;
		uint64_t modified_time;
	};

	int order;

	Map<String, Item> cache;

	void _preview_ready(const String &p_str, const Ref<Texture2D> &p_texture, const Ref<Texture2D> &p_small_texture, ObjectID id, const StringName &p_func, const Variant &p_ud);
	void _generate_preview(Ref<ImageTexture> &r_texture, Ref<ImageTexture> &r_small_texture, const QueueItem &p_item, const String &cache_base);

	static void _thread_func(void *ud);
	void _thread();

	Vector<Ref<EditorResourcePreviewGenerator>> preview_generators;

protected:
	static void _bind_methods();

public:
	static EditorResourcePreview *get_singleton();

	//callback function is callback(String p_path,Ref<Texture2D> preview,Variant udata) preview null if could not load
	void queue_resource_preview(const String &p_path, Object *p_receiver, const StringName &p_receiver_func, const Variant &p_userdata);
	void queue_edited_resource_preview(const Ref<Resource> &p_res, Object *p_receiver, const StringName &p_receiver_func, const Variant &p_userdata);

	void add_preview_generator(const Ref<EditorResourcePreviewGenerator> &p_generator);
	void remove_preview_generator(const Ref<EditorResourcePreviewGenerator> &p_generator);
	void check_for_invalidation(const String &p_path);

	void start();
	void stop();

	EditorResourcePreview();
	~EditorResourcePreview();
};

#endif // EDITORRESOURCEPREVIEW_H

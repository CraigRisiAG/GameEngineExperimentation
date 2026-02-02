

/**
 * @class EditorInterface
 * @brief Provides a high-level interface for editor plugins to interact with the Godot editor.
 * 
 * EditorInterface is a singleton that exposes core editor functionality to plugins, including:
 * - Scene and resource management (opening, editing, saving scenes)
 * - Viewport and viewport control access
 * - File system and resource preview access
 * - Inspector and editor settings access
 * - Plugin state management
 * - Mesh preview generation
 * 
 * @method Array _make_mesh_previews(const Array &p_meshes, int p_preview_size)
 * @brief Wrapper method that converts Array input to Vector<Ref<Mesh>> for mesh preview generation.
 * @param p_meshes Array of Mesh resources to generate previews for
 * @param p_preview_size Size of the generated preview textures in pixels
 * @return Array of Texture2D objects representing the mesh previews
 * 
 * @method Vector<Ref<Texture2D>> make_mesh_previews(const Vector<Ref<Mesh>> &p_meshes, Vector<Transform> *p_transforms, int p_preview_size)
 * @brief Generates preview textures for mesh resources using the visual server.
 * @details Creates a temporary viewport with camera and lighting setup to render mesh previews.
 * Supports optional per-mesh transforms for custom positioning.
 * @param p_meshes Vector of Mesh resources to preview
 * @param p_transforms Optional custom transforms for each mesh
 * @param p_preview_size Desired preview texture size
 * @return Vector of generated Texture2D preview images
 * 
 * @method void set_main_screen_editor(const String &p_name)
 * @brief Switches the active editor screen by name.
 * @param p_name Name of the editor screen to activate
 * 
 * @method Control *get_editor_viewport()
 * @brief Returns the main editor viewport control.
 * @return Pointer to the viewport Control
 * 
 * @method void edit_resource(const Ref<Resource> &p_resource)
 * @brief Opens a resource in the appropriate editor.
 * @param p_resource Resource to edit
 * 
 * @method void open_scene_from_path(const String &scene_path)
 * @brief Opens a scene file from the given path.
 * @param scene_path Path to the scene file
 * 
 * @method void reload_scene_from_path(const String &scene_path)
 * @brief Reloads a scene from disk.
 * @param scene_path Path to the scene file
 * 
 * @method Node *get_edited_scene_root()
 * @brief Returns the root node of the currently edited scene.
 * @return Root Node pointer or NULL if no scene is open
 * 
 * @method Array get_open_scenes() const
 * @brief Returns an array of file paths for all open scenes.
 * @return Array of String scene file paths
 * 
 * @method ScriptEditor *get_script_editor()
 * @brief Returns the singleton instance of the script editor.
 * @return Pointer to ScriptEditor
 * 
 * @method void select_file(const String &p_file)
 * @brief Selects a file in the file system dock.
 * @param p_file File path to select
 * 
 * @method String get_selected_path() const
 * @brief Returns the currently selected path in the file system dock.
 * @return Selected file or folder path
 * 
 * @method String get_current_path() const
 * @brief Returns the current browsing path in the file system dock.
 * @return Current directory path
 * 
 * @method void inspect_object(Object *p_obj, const String &p_for_property)
 * @brief Displays an object in the editor inspector.
 * @param p_obj Object to inspect
 * @param p_for_property Optional specific property to focus on
 * 
 * @method EditorFileSystem *get_resource_file_system()
 * @brief Returns the singleton EditorFileSystem instance.
 * @return Pointer to EditorFileSystem
 * 
 * @method FileSystemDock *get_file_system_dock()
 * @brief Returns the file system dock panel.
 * @return Pointer to FileSystemDock
 * 
 * @method EditorSelection *get_selection()
 * @brief Returns the editor's selection manager.
 * @return Pointer to EditorSelection
 * 
 * @method Ref<EditorSettings> get_editor_settings()
 * @brief Returns the editor settings resource.
 * @return Reference to EditorSettings
 * 
 * @method EditorResourcePreview *get_resource_previewer()
 * @brief Returns the resource preview generator.
 * @return Pointer to EditorResourcePreview
 * 
 * @method Control *get_base_control()
 * @brief Returns the main GUI control of the editor.
 * @return Pointer to the base GUI Control
 * 
 * @method void set_plugin_enabled(const String &p_plugin, bool p_enabled)
 * @brief Enables or disables an addon plugin.
 * @param p_plugin Plugin name
 * @param p_enabled True to enable, false to disable
 * 
 * @method bool is_plugin_enabled(const String &p_plugin) const
 * @brief Checks if an addon plugin is currently enabled.
 * @param p_plugin Plugin name
 * @return True if enabled, false otherwise
 * 
 * @method EditorInspector *get_inspector() const
 * @brief Returns the editor's object inspector.
 * @return Pointer to EditorInspector
 * 
 * @method Error save_scene()
 * @brief Saves the currently edited scene to its file path.
 * @return OK on success, ERR_CANT_CREATE if no scene is open or has no path
 * 
 * @method void save_scene_as(const String &p_scene, bool p_with_preview)
 * @brief Saves the current scene to a specified path.
 * @param p_scene Destination file path
 * @param p_with_preview If true, generates a scene preview thumbnail
 * 
 * @method void set_distraction_free_mode(bool p_enter)
 * @brief Toggles distraction-free editing mode.
 * @param p_enter True to enter, false to exit
 * 
 * @class EditorPlugin
 * @brief Base class for creating editor plugins that extend the Godot editor.
 * 
 * EditorPlugin provides hooks for customizing the editor interface through:
 * - Custom types and autoload singletons
 * - UI controls added to various editor panels
 * - Scene and resource editing callbacks
 * - Input event handling for 2D and 3D viewports
 * - Import/export and gizmo plugin support
 * 
 * @method void add_custom_type(const String &p_type, const String &p_base, const Ref<Script> &p_script, const Ref<Texture2D> &p_icon)
 * @brief Registers a custom resource type in the editor.
 * @param p_type Custom type name
 * @param p_base Base resource class name
 * @param p_script Script defining the custom type behavior
 * @param p_icon Icon to display in the editor
 * 
 * @method void remove_custom_type(const String &p_type)
 * @brief Unregisters a previously registered custom type.
 * @param p_type Custom type name to remove
 * 
 * @method void add_autoload_singleton(const String &p_name, const String &p_path)
 * @brief Registers an autoload singleton accessible in all scenes.
 * @param p_name Autoload identifier name
 * @param p_path Path to the autoload script or scene
 * 
 * @method void remove_autoload_singleton(const String &p_name)
 * @brief Removes a registered autoload singleton.
 * @param p_name Autoload identifier name to remove
 * 
 * @method ToolButton *add_control_to_bottom_panel(Control *p_control, const String &p_title)
 * @brief Adds a control to the bottom editor panel with a toggle button.
 * @param p_control Control widget to add
 * @param p_title Title displayed on the toggle button
 * @return ToolButton for toggling panel visibility
 * 
 * @method void add_control_to_dock(DockSlot p_slot, Control *p_control)
 * @brief Adds a control to a dockable panel location.
 * @param p_slot Dock slot location (top-left, bottom-right, etc.)
 * @param p_control Control to dock
 * 
 * @method void remove_control_from_docks(Control *p_control)
 * @brief Removes a docked control from the editor.
 * @param p_control Control to remove
 * 
 * @method void remove_control_from_bottom_panel(Control *p_control)
 * @brief Removes a control from the bottom panel.
 * @param p_control Control to remove
 * 
 * @method void add_control_to_container(CustomControlContainer p_location, Control *p_control)
 * @brief Adds a control to a specific editor container location.
 * @param p_location Container location (toolbar, viewport menu, property editor, etc.)
 * @param p_control Control to add
 * 
 * @method void remove_control_from_container(CustomControlContainer p_location, Control *p_control)
 * @brief Removes a control from a specific container location.
 * @param p_location Container location identifier
 * @param p_control Control to remove
 * 
 * @method void add_tool_menu_item(const String &p_name, Object *p_handler, const String &p_callback, const Variant &p_ud)
 * @brief Adds a menu item to the Tools menu.
 * @param p_name Menu item label
 * @param p_handler Object containing the callback method
 * @param p_callback Method name to call when activated
 * @param p_ud Optional user data passed to callback
 * 
 * @method void add_tool_submenu_item(const String &p_name, Object *p_submenu)
 * @brief Adds a submenu to the Tools menu.
 * @param p_name Submenu label
 * @param p_submenu PopupMenu object containing submenu items
 * 
 * @method void remove_tool_menu_item(const String &p_name)
 * @brief Removes a Tools menu item.
 * @param p_name Menu item label to remove
 * 
 * @method void set_input_event_forwarding_always_enabled()
 * @brief Enables continuous input event forwarding to this plugin.
 * 
 * @method void set_force_draw_over_forwarding_enabled()
 * @brief Enables continuous draw-over viewport updates for this plugin.
 * 
 * @method void notify_scene_changed(const Node *scn_root)
 * @brief Emits signal when the scene has changed.
 * @param scn_root Root node of the changed scene
 * 
 * @method void notify_main_screen_changed(const String &screen_name)
 * @brief Emits signal when the main editor screen changes.
 * @param screen_name Name of the new screen
 * 
 * @method void notify_scene_closed(const String &scene_filepath)
 * @brief Emits signal when a scene is closed.
 * @param scene_filepath Path to the closed scene
 * 
 * @method void notify_resource_saved(const Ref<Resource> &p_resource)
 * @brief Emits signal when a resource is saved.
 * @param p_resource The saved resource
 * 
 * @method bool forward_canvas_gui_input(const Ref<InputEvent> &p_event)
 * @brief Allows plugin to handle 2D canvas input events.
 * @param p_event Input event to handle
 * @return True if event was consumed by plugin, false otherwise
 * 
 * @method void forward_canvas_draw_over_viewport(Control *p_overlay)
 * @brief Allows plugin to draw overlay graphics on 2D viewport.
 * @param p_overlay Control to draw on
 * 
 * @method void forward_canvas_force_draw_over_viewport(Control *p_overlay)
 * @brief Forces continuous drawing of overlay on 2D viewport.
 * @param p_overlay Control to draw on
 * 
 * @method int update_overlays() const
 * @brief Updates all viewport overlays.
 * @return Number of updated viewports
 * 
 * @method bool forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event)
 * @brief Allows plugin to handle 3D viewport input events.
 * @param p_camera Active camera in 3D viewport
 * @param p_event Input event to handle
 * @return True if event was consumed, false otherwise
 * 
 * @method void forward_spatial_draw_over_viewport(Control *p_overlay)
 * @brief Allows plugin to draw overlay graphics on 3D viewports.
 * @param p_overlay Control to draw on
 * 
 * @method void forward_spatial_force_draw_over_viewport(Control *p_overlay)
 * @brief Forces continuous drawing of overlay on 3D viewports.
 * @param p_overlay Control to draw on
 * 
 * @method String get_name() const
 * @brief Returns the display name of this plugin.
 * @return Plugin name string
 * 
 * @method const Ref<Texture2D> get_icon() const
 * @brief Returns the icon for this plugin.
 * @return Texture2D reference for plugin icon
 * 
 * @method bool has_main_screen() const
 * @brief Indicates whether plugin provides a main editor screen.
 * @return True if plugin has main screen, false otherwise
 * 
 * @method void make_visible(bool p_visible)
 * @brief Shows or hides the plugin's main screen.
 * @param p_visible True to show, false to hide
 * 
 * @method void edit(Object *p_object)
 * @brief Called when the plugin should edit a specific object.
 * @param p_object Object to edit
 * 
 * @method bool handles(Object *p_object) const
 * @brief Indicates whether plugin can handle editing a specific object.
 * @param p_object Object to check
 * @return True if plugin can handle, false otherwise
 * 
 * @method Dictionary get_state() const
 * @brief Returns the plugin's current editor state for serialization.
 * @return Dictionary containing state data
 * 
 * @method void set_state(const Dictionary &p_state)
 * @brief Restores plugin state from a dictionary.
 * @param p_state State dictionary to restore
 * 
 * @method void clear()
 * @brief Called to clear the plugin's content and reset state.
 * 
 * @method void save_external_data()
 * @brief Called to save any external resources managed by the plugin.
 * 
 * @method void apply_changes()
 * @brief Called to apply pending changes in the plugin editor.
 * 
 * @method void get_breakpoints(List<String> *p_breakpoints)
 * @brief Returns list of script breakpoint locations managed by plugin.
 * @param p_breakpoints List to populate with breakpoint paths
 * 
 * @method bool get_remove_list(List<Node *> *p_list)
 * @brief Retrieves list of nodes to be removed. Override to prevent deletion.
 * @param p_list List to populate
 * @return False to allow removal, true to prevent
 * 
 * @method void restore_global_state()
 * @brief Called to restore plugin's global editor state.
 * 
 * @method void save_global_state()
 * @brief Called to save plugin's global editor state.
 * 
 * @method void add_import_plugin(const Ref<EditorImportPlugin> &p_importer)
 * @brief Registers a custom resource importer plugin.
 * @param p_importer EditorImportPlugin to register
 * 
 * @method void remove_import_plugin(const Ref<EditorImportPlugin> &p_importer)
 * @brief Unregisters a resource importer plugin.
 * @param p_importer EditorImportPlugin to remove
 * 
 * @method void add_export_plugin(const Ref<EditorExportPlugin> &p_exporter)
 * @brief Registers a custom project exporter plugin.
 * @param p_exporter EditorExportPlugin to register
 * 
 * @method void remove_export_plugin(const Ref<EditorExportPlugin> &p_exporter)
 * @brief Unregisters an export plugin.
 * @param p_exporter EditorExportPlugin to remove
 * 
 * @method void add_spatial_gizmo_plugin(const Ref<EditorSpatialGizmoPlugin> &p_gizmo_plugin)
 * @brief Registers a 3D viewport gizmo plugin.
 * @param p_gizmo_plugin EditorSpatialGizmoPlugin to register
 * 
 * @method void remove_spatial_gizmo_plugin(const Ref<EditorSpatialGizmoPlugin> &p_gizmo_plugin)
 * @brief Unregisters a gizmo plugin.
 * @param p_gizmo_plugin EditorSpatialGizmoPlugin to remove
 * 
 * @method void add_inspector_plugin(const Ref<EditorInspectorPlugin> &p_plugin)
 * @brief Registers a custom property inspector plugin.
 * @param p_plugin EditorInspectorPlugin to register
 * 
 * @method void remove_inspector_plugin(const Ref<EditorInspectorPlugin> &p_plugin)
 * @brief Unregisters an inspector plugin.
 * @param p_plugin EditorInspectorPlugin to remove
 * 
 * @method void add_scene_import_plugin(const Ref<EditorSceneImporter> &p_importer)
 * @brief Registers a custom scene importer plugin.
 * @param p_importer EditorSceneImporter to register
 * 
 * @method void remove_scene_import_plugin(const Ref<EditorSceneImporter> &p_importer)
 * @brief Unregisters a scene importer plugin.
 * @param p_importer EditorSceneImporter to remove
 * 
 * @method void enable_plugin()
 * @brief Called when plugin is enabled in project settings.
 * @details Override to initialize plugin resources and register autoloads.
 * 
 * @method void disable_plugin()
 * @brief Called when plugin is disabled in project settings.
 * @details Override to cleanup plugin resources and unregister autoloads.
 * 
 * @method void set_window_layout(Ref<ConfigFile> p_layout)
 * @brief Called to restore editor window layout configuration.
 * @param p_layout ConfigFile with layout data
 * 
 * @method void get_window_layout(Ref<ConfigFile> p_layout)
 * @brief Called to save editor window layout configuration.
 * @param p_layout ConfigFile to populate with layout data
 * 
 * @method bool build()
 * @brief Called during project build process. Override to add custom build steps.
 * @return True to continue build, false to cancel
 * 
 * @method void queue_save_layout() const
 * @brief Requests that the editor save its current layout configuration.
 * 
 * @method void make_bottom_panel_item_visible(Control *p_item)
 * @brief Shows a bottom panel item and switches focus to it.
 * @param p_item Bottom panel Control to show
 * 
 * @method void hide_bottom_panel()
 * @brief Hides the bottom editor panel.
 * 
 * @method EditorInterface *get_editor_interface()
 * @brief Returns the editor interface singleton.
 * @return Pointer to EditorInterface
 * 
 * @method ScriptCreateDialog *get_script_create_dialog()
 * @brief Returns the script creation dialog.
 * @return Pointer to ScriptCreateDialog
 * 
 * @signal scene_changed(Node *scene_root)
 * @brief Emitted when the edited scene changes.
 * 
 * @signal scene_closed(String filepath)
 * @brief Emitted when a scene is closed.
 * 
 * @signal main_screen_changed(String screen_name)
 * @brief Emitted when the main editor screen changes.
 * 
 * @signal resource_saved(Resource resource)
 * @brief Emitted when a resource is saved.
 * 
 * @enum CustomControlContainer
 * @brief Locations where controls can be added to the editor UI.
 * @value CONTAINER_TOOLBAR Main editor toolbar
 * @value CONTAINER_SPATIAL_EDITOR_MENU 3D editor menu panel
 * @value CONTAINER_SPATIAL_EDITOR_SIDE_LEFT 3D editor left sidebar
 * @value CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT 3D editor right sidebar
 * @value CONTAINER_SPATIAL_EDITOR_BOTTOM 3D editor bottom panel
 * @value CONTAINER_CANVAS_EDITOR_MENU 2D editor menu panel
 * @value CONTAINER_CANVAS_EDITOR_SIDE_LEFT 2D editor left sidebar
 * @value CONTAINER_CANVAS_EDITOR_SIDE_RIGHT 2D editor right sidebar
 * @value CONTAINER_CANVAS_EDITOR_BOTTOM 2D editor bottom panel
 * @value CONTAINER_PROPERTY_EDITOR_BOTTOM Inspector addon area
 * @value CONTAINER_PROJECT_SETTING_TAB_LEFT Project settings left tabs
 * @value CONTAINER_PROJECT_SETTING_TAB_RIGHT Project settings right tabs
 * 
 * @enum DockSlot
 * @brief Dockable panel slot positions in the editor.
 * @value DOCK_SLOT_LEFT_UL Left panel upper position
 * @value DOCK_SLOT_LEFT_BL Left panel lower position
 * @value DOCK_SLOT_LEFT_UR Left panel upper-right position
 * @value DOCK_SLOT_LEFT_BR Left panel bottom-right position
 * @value DOCK_SLOT_RIGHT_UL Right panel upper position
 * @value DOCK_SLOT_RIGHT_BL Right panel lower position
 * @value DOCK_SLOT_RIGHT_UR Right panel upper-right position
 * @value DOCK_SLOT_RIGHT_BR Right panel bottom-right position
 * @value DOCK_SLOT_MAX Maximum dock slot value
 * 
 * @class EditorPlugins
 * @brief Manager for plugin creation functions.
 * @details Maintains registry of editor plugin creation callbacks used by the editor
 * to instantiate available plugins.
 */
#include "editor_plugin.h"

#include "editor/editor_export.h"
#include "editor/editor_node.h"
#include "editor/editor_settings.h"
#include "editor/filesystem_dock.h"
#include "editor/project_settings_editor.h"
#include "editor_resource_preview.h"
#include "main/main.h"
#include "plugins/canvas_item_editor_plugin.h"
#include "plugins/spatial_editor_plugin.h"
#include "scene/3d/camera.h"
#include "scene/gui/popup_menu.h"
#include "servers/visual_server.h"

Array EditorInterface::_make_mesh_previews(const Array &p_meshes, int p_preview_size) {

	Vector<Ref<Mesh>> meshes;

	for (int i = 0; i < p_meshes.size(); i++) {
		meshes.push_back(p_meshes[i]);
	}

	Vector<Ref<Texture2D>> textures = make_mesh_previews(meshes, NULL, p_preview_size);
	Array ret;
	for (int i = 0; i < textures.size(); i++) {
		ret.push_back(textures[i]);
	}

	return ret;
}

Vector<Ref<Texture2D>> EditorInterface::make_mesh_previews(const Vector<Ref<Mesh>> &p_meshes, Vector<Transform> *p_transforms, int p_preview_size) {

	int size = p_preview_size;

	RID scenario = VS::get_singleton()->scenario_create();

	RID viewport = VS::get_singleton()->viewport_create();
	VS::get_singleton()->viewport_set_update_mode(viewport, VS::VIEWPORT_UPDATE_ALWAYS);
	VS::get_singleton()->viewport_set_scenario(viewport, scenario);
	VS::get_singleton()->viewport_set_size(viewport, size, size);
	VS::get_singleton()->viewport_set_transparent_background(viewport, true);
	VS::get_singleton()->viewport_set_active(viewport, true);
	RID viewport_texture = VS::get_singleton()->viewport_get_texture(viewport);

	RID camera = VS::get_singleton()->camera_create();
	VS::get_singleton()->viewport_attach_camera(viewport, camera);

	RID light = VS::get_singleton()->directional_light_create();
	RID light_instance = VS::get_singleton()->instance_create2(light, scenario);

	RID light2 = VS::get_singleton()->directional_light_create();
	VS::get_singleton()->light_set_color(light2, Color(0.7, 0.7, 0.7));
	RID light_instance2 = VS::get_singleton()->instance_create2(light2, scenario);

	EditorProgress ep("mlib", TTR("Creating Mesh Previews"), p_meshes.size());

	Vector<Ref<Texture2D>> textures;

	for (int i = 0; i < p_meshes.size(); i++) {

		Ref<Mesh> mesh = p_meshes[i];
		if (!mesh.is_valid()) {
			textures.push_back(Ref<Texture2D>());
			continue;
		}

		Transform mesh_xform;
		if (p_transforms != NULL) {
			mesh_xform = (*p_transforms)[i];
		}

		RID inst = VS::get_singleton()->instance_create2(mesh->get_rid(), scenario);
		VS::get_singleton()->instance_set_transform(inst, mesh_xform);

		AABB aabb = mesh->get_aabb();
		Vector3 ofs = aabb.position + aabb.size * 0.5;
		aabb.position -= ofs;
		Transform xform;
		xform.basis = Basis().rotated(Vector3(0, 1, 0), -Math_PI / 6);
		xform.basis = Basis().rotated(Vector3(1, 0, 0), Math_PI / 6) * xform.basis;
		AABB rot_aabb = xform.xform(aabb);
		float m = MAX(rot_aabb.size.x, rot_aabb.size.y) * 0.5;
		if (m == 0) {
			textures.push_back(Ref<Texture2D>());
			continue;
		}
		xform.origin = -xform.basis.xform(ofs); //-ofs*m;
		xform.origin.z -= rot_aabb.size.z * 2;
		xform.invert();
		xform = mesh_xform * xform;

		VS::get_singleton()->camera_set_transform(camera, xform * Transform(Basis(), Vector3(0, 0, 3)));
		VS::get_singleton()->camera_set_orthogonal(camera, m * 2, 0.01, 1000.0);

		VS::get_singleton()->instance_set_transform(light_instance, xform * Transform().looking_at(Vector3(-2, -1, -1), Vector3(0, 1, 0)));
		VS::get_singleton()->instance_set_transform(light_instance2, xform * Transform().looking_at(Vector3(+1, -1, -2), Vector3(0, 1, 0)));

		ep.step(TTR("Thumbnail..."), i);
		Main::iteration();
		Main::iteration();
		Ref<Image> img = VS::get_singleton()->texture_2d_get(viewport_texture);
		ERR_CONTINUE(!img.is_valid() || img->empty());
		Ref<ImageTexture> it(memnew(ImageTexture));
		it->create_from_image(img);

		VS::get_singleton()->free(inst);

		textures.push_back(it);
	}

	VS::get_singleton()->free(viewport);
	VS::get_singleton()->free(light);
	VS::get_singleton()->free(light_instance);
	VS::get_singleton()->free(light2);
	VS::get_singleton()->free(light_instance2);
	VS::get_singleton()->free(camera);
	VS::get_singleton()->free(scenario);

	return textures;
}

void EditorInterface::set_main_screen_editor(const String &p_name) {
	EditorNode::get_singleton()->select_editor_by_name(p_name);
}

Control *EditorInterface::get_editor_viewport() {

	return EditorNode::get_singleton()->get_viewport();
}

void EditorInterface::edit_resource(const Ref<Resource> &p_resource) {

	EditorNode::get_singleton()->edit_resource(p_resource);
}

void EditorInterface::open_scene_from_path(const String &scene_path) {

	if (EditorNode::get_singleton()->is_changing_scene()) {
		return;
	}

	EditorNode::get_singleton()->open_request(scene_path);
}

void EditorInterface::reload_scene_from_path(const String &scene_path) {

	if (EditorNode::get_singleton()->is_changing_scene()) {
		return;
	}

	EditorNode::get_singleton()->reload_scene(scene_path);
}

Node *EditorInterface::get_edited_scene_root() {
	return EditorNode::get_singleton()->get_edited_scene();
}

Array EditorInterface::get_open_scenes() const {

	Array ret;
	Vector<EditorData::EditedScene> scenes = EditorNode::get_editor_data().get_edited_scenes();

	int scns_amount = scenes.size();
	for (int idx_scn = 0; idx_scn < scns_amount; idx_scn++) {
		if (scenes[idx_scn].root == NULL)
			continue;
		ret.push_back(scenes[idx_scn].root->get_filename());
	}
	return ret;
}

ScriptEditor *EditorInterface::get_script_editor() {
	return ScriptEditor::get_singleton();
}

void EditorInterface::select_file(const String &p_file) {
	EditorNode::get_singleton()->get_filesystem_dock()->select_file(p_file);
}

String EditorInterface::get_selected_path() const {
	return EditorNode::get_singleton()->get_filesystem_dock()->get_selected_path();
}

String EditorInterface::get_current_path() const {
	return EditorNode::get_singleton()->get_filesystem_dock()->get_current_path();
}

void EditorInterface::inspect_object(Object *p_obj, const String &p_for_property) {

	EditorNode::get_singleton()->push_item(p_obj, p_for_property);
}

EditorFileSystem *EditorInterface::get_resource_file_system() {
	return EditorFileSystem::get_singleton();
}

FileSystemDock *EditorInterface::get_file_system_dock() {
	return EditorNode::get_singleton()->get_filesystem_dock();
}

EditorSelection *EditorInterface::get_selection() {
	return EditorNode::get_singleton()->get_editor_selection();
}

Ref<EditorSettings> EditorInterface::get_editor_settings() {
	return EditorSettings::get_singleton();
}

EditorResourcePreview *EditorInterface::get_resource_previewer() {
	return EditorResourcePreview::get_singleton();
}

Control *EditorInterface::get_base_control() {

	return EditorNode::get_singleton()->get_gui_base();
}

void EditorInterface::set_plugin_enabled(const String &p_plugin, bool p_enabled) {
	EditorNode::get_singleton()->set_addon_plugin_enabled(p_plugin, p_enabled, true);
}

bool EditorInterface::is_plugin_enabled(const String &p_plugin) const {
	return EditorNode::get_singleton()->is_addon_plugin_enabled(p_plugin);
}

EditorInspector *EditorInterface::get_inspector() const {
	return EditorNode::get_singleton()->get_inspector();
}

Error EditorInterface::save_scene() {
	if (!get_edited_scene_root())
		return ERR_CANT_CREATE;
	if (get_edited_scene_root()->get_filename() == String())
		return ERR_CANT_CREATE;

	save_scene_as(get_edited_scene_root()->get_filename());
	return OK;
}

void EditorInterface::save_scene_as(const String &p_scene, bool p_with_preview) {

	EditorNode::get_singleton()->save_scene_to_path(p_scene, p_with_preview);
}

void EditorInterface::set_distraction_free_mode(bool p_enter) {
	EditorNode::get_singleton()->set_distraction_free_mode(p_enter);
}

EditorInterface *EditorInterface::singleton = NULL;

void EditorInterface::_bind_methods() {

	ClassDB::bind_method(D_METHOD("inspect_object", "object", "for_property"), &EditorInterface::inspect_object, DEFVAL(String()));
	ClassDB::bind_method(D_METHOD("get_selection"), &EditorInterface::get_selection);
	ClassDB::bind_method(D_METHOD("get_editor_settings"), &EditorInterface::get_editor_settings);
	ClassDB::bind_method(D_METHOD("get_script_editor"), &EditorInterface::get_script_editor);
	ClassDB::bind_method(D_METHOD("get_base_control"), &EditorInterface::get_base_control);
	ClassDB::bind_method(D_METHOD("edit_resource", "resource"), &EditorInterface::edit_resource);
	ClassDB::bind_method(D_METHOD("open_scene_from_path", "scene_filepath"), &EditorInterface::open_scene_from_path);
	ClassDB::bind_method(D_METHOD("reload_scene_from_path", "scene_filepath"), &EditorInterface::reload_scene_from_path);
	ClassDB::bind_method(D_METHOD("get_open_scenes"), &EditorInterface::get_open_scenes);
	ClassDB::bind_method(D_METHOD("get_edited_scene_root"), &EditorInterface::get_edited_scene_root);
	ClassDB::bind_method(D_METHOD("get_resource_previewer"), &EditorInterface::get_resource_previewer);
	ClassDB::bind_method(D_METHOD("get_resource_filesystem"), &EditorInterface::get_resource_file_system);
	ClassDB::bind_method(D_METHOD("get_editor_viewport"), &EditorInterface::get_editor_viewport);
	ClassDB::bind_method(D_METHOD("make_mesh_previews", "meshes", "preview_size"), &EditorInterface::_make_mesh_previews);
	ClassDB::bind_method(D_METHOD("select_file", "file"), &EditorInterface::select_file);
	ClassDB::bind_method(D_METHOD("get_selected_path"), &EditorInterface::get_selected_path);
	ClassDB::bind_method(D_METHOD("get_current_path"), &EditorInterface::get_current_path);
	ClassDB::bind_method(D_METHOD("get_file_system_dock"), &EditorInterface::get_file_system_dock);

	ClassDB::bind_method(D_METHOD("set_plugin_enabled", "plugin", "enabled"), &EditorInterface::set_plugin_enabled);
	ClassDB::bind_method(D_METHOD("is_plugin_enabled", "plugin"), &EditorInterface::is_plugin_enabled);

	ClassDB::bind_method(D_METHOD("get_inspector"), &EditorInterface::get_inspector);

	ClassDB::bind_method(D_METHOD("save_scene"), &EditorInterface::save_scene);
	ClassDB::bind_method(D_METHOD("save_scene_as", "path", "with_preview"), &EditorInterface::save_scene_as, DEFVAL(true));

	ClassDB::bind_method(D_METHOD("set_main_screen_editor", "name"), &EditorInterface::set_main_screen_editor);
	ClassDB::bind_method(D_METHOD("set_distraction_free_mode", "enter"), &EditorInterface::set_distraction_free_mode);
}

EditorInterface::EditorInterface() {
	singleton = this;
}

///////////////////////////////////////////
void EditorPlugin::add_custom_type(const String &p_type, const String &p_base, const Ref<Script> &p_script, const Ref<Texture2D> &p_icon) {

	EditorNode::get_editor_data().add_custom_type(p_type, p_base, p_script, p_icon);
}

void EditorPlugin::remove_custom_type(const String &p_type) {

	EditorNode::get_editor_data().remove_custom_type(p_type);
}

void EditorPlugin::add_autoload_singleton(const String &p_name, const String &p_path) {
	EditorNode::get_singleton()->get_project_settings()->get_autoload_settings()->autoload_add(p_name, p_path);
}

void EditorPlugin::remove_autoload_singleton(const String &p_name) {
	EditorNode::get_singleton()->get_project_settings()->get_autoload_settings()->autoload_remove(p_name);
}

ToolButton *EditorPlugin::add_control_to_bottom_panel(Control *p_control, const String &p_title) {
	ERR_FAIL_NULL_V(p_control, NULL);
	return EditorNode::get_singleton()->add_bottom_panel_item(p_title, p_control);
}

void EditorPlugin::add_control_to_dock(DockSlot p_slot, Control *p_control) {

	ERR_FAIL_NULL(p_control);
	EditorNode::get_singleton()->add_control_to_dock(EditorNode::DockSlot(p_slot), p_control);
}

void EditorPlugin::remove_control_from_docks(Control *p_control) {

	ERR_FAIL_NULL(p_control);
	EditorNode::get_singleton()->remove_control_from_dock(p_control);
}

void EditorPlugin::remove_control_from_bottom_panel(Control *p_control) {

	ERR_FAIL_NULL(p_control);
	EditorNode::get_singleton()->remove_bottom_panel_item(p_control);
}

void EditorPlugin::add_control_to_container(CustomControlContainer p_location, Control *p_control) {
	ERR_FAIL_NULL(p_control);

	switch (p_location) {

		case CONTAINER_TOOLBAR: {

			EditorNode::get_menu_hb()->add_child(p_control);
		} break;

		case CONTAINER_SPATIAL_EDITOR_MENU: {

			SpatialEditor::get_singleton()->add_control_to_menu_panel(p_control);

		} break;
		case CONTAINER_SPATIAL_EDITOR_SIDE_LEFT: {

			SpatialEditor::get_singleton()->get_palette_split()->add_child(p_control);
			SpatialEditor::get_singleton()->get_palette_split()->move_child(p_control, 0);

		} break;
		case CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT: {

			SpatialEditor::get_singleton()->get_palette_split()->add_child(p_control);
			SpatialEditor::get_singleton()->get_palette_split()->move_child(p_control, 1);

		} break;
		case CONTAINER_SPATIAL_EDITOR_BOTTOM: {

			SpatialEditor::get_singleton()->get_shader_split()->add_child(p_control);

		} break;
		case CONTAINER_CANVAS_EDITOR_MENU: {

			CanvasItemEditor::get_singleton()->add_control_to_menu_panel(p_control);

		} break;
		case CONTAINER_CANVAS_EDITOR_SIDE_LEFT: {

			CanvasItemEditor::get_singleton()->get_palette_split()->add_child(p_control);
			CanvasItemEditor::get_singleton()->get_palette_split()->move_child(p_control, 0);

		} break;
		case CONTAINER_CANVAS_EDITOR_SIDE_RIGHT: {

			CanvasItemEditor::get_singleton()->get_palette_split()->add_child(p_control);
			CanvasItemEditor::get_singleton()->get_palette_split()->move_child(p_control, 1);

		} break;
		case CONTAINER_CANVAS_EDITOR_BOTTOM: {

			CanvasItemEditor::get_singleton()->get_bottom_split()->add_child(p_control);

		} break;
		case CONTAINER_PROPERTY_EDITOR_BOTTOM: {

			EditorNode::get_singleton()->get_inspector_dock_addon_area()->add_child(p_control);

		} break;
		case CONTAINER_PROJECT_SETTING_TAB_LEFT: {

			ProjectSettingsEditor::get_singleton()->get_tabs()->add_child(p_control);
			ProjectSettingsEditor::get_singleton()->get_tabs()->move_child(p_control, 0);

		} break;
		case CONTAINER_PROJECT_SETTING_TAB_RIGHT: {

			ProjectSettingsEditor::get_singleton()->get_tabs()->add_child(p_control);
			ProjectSettingsEditor::get_singleton()->get_tabs()->move_child(p_control, 1);

		} break;
	}
}

void EditorPlugin::remove_control_from_container(CustomControlContainer p_location, Control *p_control) {
	ERR_FAIL_NULL(p_control);

	switch (p_location) {

		case CONTAINER_TOOLBAR: {

			EditorNode::get_menu_hb()->remove_child(p_control);
		} break;

		case CONTAINER_SPATIAL_EDITOR_MENU: {

			SpatialEditor::get_singleton()->remove_control_from_menu_panel(p_control);

		} break;
		case CONTAINER_SPATIAL_EDITOR_SIDE_LEFT:
		case CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT: {

			SpatialEditor::get_singleton()->get_palette_split()->remove_child(p_control);

		} break;
		case CONTAINER_SPATIAL_EDITOR_BOTTOM: {

			SpatialEditor::get_singleton()->get_shader_split()->remove_child(p_control);

		} break;
		case CONTAINER_CANVAS_EDITOR_MENU: {

			CanvasItemEditor::get_singleton()->remove_control_from_menu_panel(p_control);

		} break;
		case CONTAINER_CANVAS_EDITOR_SIDE_LEFT:
		case CONTAINER_CANVAS_EDITOR_SIDE_RIGHT: {

			CanvasItemEditor::get_singleton()->get_palette_split()->remove_child(p_control);

		} break;
		case CONTAINER_CANVAS_EDITOR_BOTTOM: {

			CanvasItemEditor::get_singleton()->get_bottom_split()->remove_child(p_control);

		} break;
		case CONTAINER_PROPERTY_EDITOR_BOTTOM: {

			EditorNode::get_singleton()->get_inspector_dock_addon_area()->remove_child(p_control);

		} break;
		case CONTAINER_PROJECT_SETTING_TAB_LEFT:
		case CONTAINER_PROJECT_SETTING_TAB_RIGHT: {

			ProjectSettingsEditor::get_singleton()->get_tabs()->remove_child(p_control);

		} break;
	}
}

void EditorPlugin::add_tool_menu_item(const String &p_name, Object *p_handler, const String &p_callback, const Variant &p_ud) {
	EditorNode::get_singleton()->add_tool_menu_item(p_name, p_handler, p_callback, p_ud);
}

void EditorPlugin::add_tool_submenu_item(const String &p_name, Object *p_submenu) {
	ERR_FAIL_NULL(p_submenu);
	PopupMenu *submenu = Object::cast_to<PopupMenu>(p_submenu);
	ERR_FAIL_NULL(submenu);
	EditorNode::get_singleton()->add_tool_submenu_item(p_name, submenu);
}

void EditorPlugin::remove_tool_menu_item(const String &p_name) {
	EditorNode::get_singleton()->remove_tool_menu_item(p_name);
}

void EditorPlugin::set_input_event_forwarding_always_enabled() {
	input_event_forwarding_always_enabled = true;
	EditorPluginList *always_input_forwarding_list = EditorNode::get_singleton()->get_editor_plugins_force_input_forwarding();
	always_input_forwarding_list->add_plugin(this);
}

void EditorPlugin::set_force_draw_over_forwarding_enabled() {
	force_draw_over_forwarding_enabled = true;
	EditorPluginList *always_draw_over_forwarding_list = EditorNode::get_singleton()->get_editor_plugins_force_over();
	always_draw_over_forwarding_list->add_plugin(this);
}

void EditorPlugin::notify_scene_changed(const Node *scn_root) {
	emit_signal("scene_changed", scn_root);
}

void EditorPlugin::notify_main_screen_changed(const String &screen_name) {

	if (screen_name == last_main_screen_name)
		return;

	emit_signal("main_screen_changed", screen_name);
	last_main_screen_name = screen_name;
}

void EditorPlugin::notify_scene_closed(const String &scene_filepath) {
	emit_signal("scene_closed", scene_filepath);
}

void EditorPlugin::notify_resource_saved(const Ref<Resource> &p_resource) {
	emit_signal("resource_saved", p_resource);
}

bool EditorPlugin::forward_canvas_gui_input(const Ref<InputEvent> &p_event) {

	if (get_script_instance() && get_script_instance()->has_method("forward_canvas_gui_input")) {
		return get_script_instance()->call("forward_canvas_gui_input", p_event);
	}
	return false;
}

void EditorPlugin::forward_canvas_draw_over_viewport(Control *p_overlay) {

	if (get_script_instance() && get_script_instance()->has_method("forward_canvas_draw_over_viewport")) {
		get_script_instance()->call("forward_canvas_draw_over_viewport", p_overlay);
	}
}

void EditorPlugin::forward_canvas_force_draw_over_viewport(Control *p_overlay) {

	if (get_script_instance() && get_script_instance()->has_method("forward_canvas_force_draw_over_viewport")) {
		get_script_instance()->call("forward_canvas_force_draw_over_viewport", p_overlay);
	}
}

// Updates the overlays of the 2D viewport or, if in 3D mode, of every 3D viewport.
int EditorPlugin::update_overlays() const {

	if (SpatialEditor::get_singleton()->is_visible()) {
		int count = 0;
		for (uint32_t i = 0; i < SpatialEditor::VIEWPORTS_COUNT; i++) {
			SpatialEditorViewport *vp = SpatialEditor::get_singleton()->get_editor_viewport(i);
			if (vp->is_visible()) {
				vp->update_surface();
				count++;
			}
		}
		return count;
	} else {
		// This will update the normal viewport itself as well
		CanvasItemEditor::get_singleton()->get_viewport_control()->update();
		return 1;
	}
}

bool EditorPlugin::forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) {

	if (get_script_instance() && get_script_instance()->has_method("forward_spatial_gui_input")) {
		return get_script_instance()->call("forward_spatial_gui_input", p_camera, p_event);
	}

	return false;
}

void EditorPlugin::forward_spatial_draw_over_viewport(Control *p_overlay) {

	if (get_script_instance() && get_script_instance()->has_method("forward_spatial_draw_over_viewport")) {
		get_script_instance()->call("forward_spatial_draw_over_viewport", p_overlay);
	}
}

void EditorPlugin::forward_spatial_force_draw_over_viewport(Control *p_overlay) {

	if (get_script_instance() && get_script_instance()->has_method("forward_spatial_force_draw_over_viewport")) {
		get_script_instance()->call("forward_spatial_force_draw_over_viewport", p_overlay);
	}
}
String EditorPlugin::get_name() const {

	if (get_script_instance() && get_script_instance()->has_method("get_plugin_name")) {
		return get_script_instance()->call("get_plugin_name");
	}

	return String();
}
const Ref<Texture2D> EditorPlugin::get_icon() const {

	if (get_script_instance() && get_script_instance()->has_method("get_plugin_icon")) {
		return get_script_instance()->call("get_plugin_icon");
	}

	return Ref<Texture2D>();
}
bool EditorPlugin::has_main_screen() const {

	if (get_script_instance() && get_script_instance()->has_method("has_main_screen")) {
		return get_script_instance()->call("has_main_screen");
	}

	return false;
}
void EditorPlugin::make_visible(bool p_visible) {

	if (get_script_instance() && get_script_instance()->has_method("make_visible")) {
		get_script_instance()->call("make_visible", p_visible);
	}
}

void EditorPlugin::edit(Object *p_object) {

	if (get_script_instance() && get_script_instance()->has_method("edit")) {
		if (p_object->is_class("Resource")) {
			get_script_instance()->call("edit", Ref<Resource>(Object::cast_to<Resource>(p_object)));
		} else {
			get_script_instance()->call("edit", p_object);
		}
	}
}

bool EditorPlugin::handles(Object *p_object) const {

	if (get_script_instance() && get_script_instance()->has_method("handles")) {
		return get_script_instance()->call("handles", p_object);
	}

	return false;
}
Dictionary EditorPlugin::get_state() const {

	if (get_script_instance() && get_script_instance()->has_method("get_state")) {
		return get_script_instance()->call("get_state");
	}

	return Dictionary();
}

void EditorPlugin::set_state(const Dictionary &p_state) {

	if (get_script_instance() && get_script_instance()->has_method("set_state")) {
		get_script_instance()->call("set_state", p_state);
	}
}

void EditorPlugin::clear() {

	if (get_script_instance() && get_script_instance()->has_method("clear")) {
		get_script_instance()->call("clear");
	}
}

// if editor references external resources/scenes, save them
void EditorPlugin::save_external_data() {

	if (get_script_instance() && get_script_instance()->has_method("save_external_data")) {
		get_script_instance()->call("save_external_data");
	}
}

// if changes are pending in editor, apply them
void EditorPlugin::apply_changes() {

	if (get_script_instance() && get_script_instance()->has_method("apply_changes")) {
		get_script_instance()->call("apply_changes");
	}
}

void EditorPlugin::get_breakpoints(List<String> *p_breakpoints) {

	if (get_script_instance() && get_script_instance()->has_method("get_breakpoints")) {
		PackedStringArray arr = get_script_instance()->call("get_breakpoints");
		for (int i = 0; i < arr.size(); i++)
			p_breakpoints->push_back(arr[i]);
	}
}
bool EditorPlugin::get_remove_list(List<Node *> *p_list) {

	return false;
}

void EditorPlugin::restore_global_state() {}
void EditorPlugin::save_global_state() {}

void EditorPlugin::add_import_plugin(const Ref<EditorImportPlugin> &p_importer) {
	ResourceFormatImporter::get_singleton()->add_importer(p_importer);
	EditorFileSystem::get_singleton()->call_deferred("scan");
}

void EditorPlugin::remove_import_plugin(const Ref<EditorImportPlugin> &p_importer) {
	ResourceFormatImporter::get_singleton()->remove_importer(p_importer);
	EditorFileSystem::get_singleton()->call_deferred("scan");
}

void EditorPlugin::add_export_plugin(const Ref<EditorExportPlugin> &p_exporter) {
	EditorExport::get_singleton()->add_export_plugin(p_exporter);
}

void EditorPlugin::remove_export_plugin(const Ref<EditorExportPlugin> &p_exporter) {
	EditorExport::get_singleton()->remove_export_plugin(p_exporter);
}

void EditorPlugin::add_spatial_gizmo_plugin(const Ref<EditorSpatialGizmoPlugin> &p_gizmo_plugin) {
	SpatialEditor::get_singleton()->add_gizmo_plugin(p_gizmo_plugin);
}

void EditorPlugin::remove_spatial_gizmo_plugin(const Ref<EditorSpatialGizmoPlugin> &p_gizmo_plugin) {
	SpatialEditor::get_singleton()->remove_gizmo_plugin(p_gizmo_plugin);
}

void EditorPlugin::add_inspector_plugin(const Ref<EditorInspectorPlugin> &p_plugin) {
	EditorInspector::add_inspector_plugin(p_plugin);
}

void EditorPlugin::remove_inspector_plugin(const Ref<EditorInspectorPlugin> &p_plugin) {
	EditorInspector::remove_inspector_plugin(p_plugin);
}

void EditorPlugin::add_scene_import_plugin(const Ref<EditorSceneImporter> &p_importer) {
	ResourceImporterScene::get_singleton()->add_importer(p_importer);
}

void EditorPlugin::remove_scene_import_plugin(const Ref<EditorSceneImporter> &p_importer) {
	ResourceImporterScene::get_singleton()->remove_importer(p_importer);
}

int find(const PackedStringArray &a, const String &v) {
	const String *r = a.ptr();
	for (int j = 0; j < a.size(); ++j) {
		if (r[j] == v) {
			return j;
		}
	}
	return -1;
}

void EditorPlugin::enable_plugin() {
	// Called when the plugin gets enabled in project settings, after it's added to the tree.
	// You can implement it to register autoloads.

	if (get_script_instance() && get_script_instance()->has_method("enable_plugin")) {
		get_script_instance()->call("enable_plugin");
	}
}

void EditorPlugin::disable_plugin() {
	// Last function called when the plugin gets disabled in project settings.
	// Implement it to cleanup things from the project, such as unregister autoloads.

	if (get_script_instance() && get_script_instance()->has_method("disable_plugin")) {
		get_script_instance()->call("disable_plugin");
	}
}

void EditorPlugin::set_window_layout(Ref<ConfigFile> p_layout) {

	if (get_script_instance() && get_script_instance()->has_method("set_window_layout")) {
		get_script_instance()->call("set_window_layout", p_layout);
	}
}

void EditorPlugin::get_window_layout(Ref<ConfigFile> p_layout) {

	if (get_script_instance() && get_script_instance()->has_method("get_window_layout")) {
		get_script_instance()->call("get_window_layout", p_layout);
	}
}

bool EditorPlugin::build() {

	if (get_script_instance() && get_script_instance()->has_method("build")) {
		return get_script_instance()->call("build");
	}

	return true;
}

void EditorPlugin::queue_save_layout() const {

	EditorNode::get_singleton()->save_layout();
}

void EditorPlugin::make_bottom_panel_item_visible(Control *p_item) {

	EditorNode::get_singleton()->make_bottom_panel_item_visible(p_item);
}

void EditorPlugin::hide_bottom_panel() {

	EditorNode::get_singleton()->hide_bottom_panel();
}

EditorInterface *EditorPlugin::get_editor_interface() {
	return EditorInterface::get_singleton();
}

ScriptCreateDialog *EditorPlugin::get_script_create_dialog() {
	return EditorNode::get_singleton()->get_script_create_dialog();
}

void EditorPlugin::_bind_methods() {

	ClassDB::bind_method(D_METHOD("add_control_to_container", "container", "control"), &EditorPlugin::add_control_to_container);
	ClassDB::bind_method(D_METHOD("add_control_to_bottom_panel", "control", "title"), &EditorPlugin::add_control_to_bottom_panel);
	ClassDB::bind_method(D_METHOD("add_control_to_dock", "slot", "control"), &EditorPlugin::add_control_to_dock);
	ClassDB::bind_method(D_METHOD("remove_control_from_docks", "control"), &EditorPlugin::remove_control_from_docks);
	ClassDB::bind_method(D_METHOD("remove_control_from_bottom_panel", "control"), &EditorPlugin::remove_control_from_bottom_panel);
	ClassDB::bind_method(D_METHOD("remove_control_from_container", "container", "control"), &EditorPlugin::remove_control_from_container);
	ClassDB::bind_method(D_METHOD("add_tool_menu_item", "name", "handler", "callback", "ud"), &EditorPlugin::add_tool_menu_item, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("add_tool_submenu_item", "name", "submenu"), &EditorPlugin::add_tool_submenu_item);
	ClassDB::bind_method(D_METHOD("remove_tool_menu_item", "name"), &EditorPlugin::remove_tool_menu_item);
	ClassDB::bind_method(D_METHOD("add_custom_type", "type", "base", "script", "icon"), &EditorPlugin::add_custom_type);
	ClassDB::bind_method(D_METHOD("remove_custom_type", "type"), &EditorPlugin::remove_custom_type);

	ClassDB::bind_method(D_METHOD("add_autoload_singleton", "name", "path"), &EditorPlugin::add_autoload_singleton);
	ClassDB::bind_method(D_METHOD("remove_autoload_singleton", "name"), &EditorPlugin::remove_autoload_singleton);

	ClassDB::bind_method(D_METHOD("update_overlays"), &EditorPlugin::update_overlays);

	ClassDB::bind_method(D_METHOD("make_bottom_panel_item_visible", "item"), &EditorPlugin::make_bottom_panel_item_visible);
	ClassDB::bind_method(D_METHOD("hide_bottom_panel"), &EditorPlugin::hide_bottom_panel);

	ClassDB::bind_method(D_METHOD("get_undo_redo"), &EditorPlugin::_get_undo_redo);
	ClassDB::bind_method(D_METHOD("queue_save_layout"), &EditorPlugin::queue_save_layout);
	ClassDB::bind_method(D_METHOD("add_import_plugin", "importer"), &EditorPlugin::add_import_plugin);
	ClassDB::bind_method(D_METHOD("remove_import_plugin", "importer"), &EditorPlugin::remove_import_plugin);
	ClassDB::bind_method(D_METHOD("add_scene_import_plugin", "scene_importer"), &EditorPlugin::add_scene_import_plugin);
	ClassDB::bind_method(D_METHOD("remove_scene_import_plugin", "scene_importer"), &EditorPlugin::remove_scene_import_plugin);
	ClassDB::bind_method(D_METHOD("add_export_plugin", "plugin"), &EditorPlugin::add_export_plugin);
	ClassDB::bind_method(D_METHOD("remove_export_plugin", "plugin"), &EditorPlugin::remove_export_plugin);
	ClassDB::bind_method(D_METHOD("add_spatial_gizmo_plugin", "plugin"), &EditorPlugin::add_spatial_gizmo_plugin);
	ClassDB::bind_method(D_METHOD("remove_spatial_gizmo_plugin", "plugin"), &EditorPlugin::remove_spatial_gizmo_plugin);
	ClassDB::bind_method(D_METHOD("add_inspector_plugin", "plugin"), &EditorPlugin::add_inspector_plugin);
	ClassDB::bind_method(D_METHOD("remove_inspector_plugin", "plugin"), &EditorPlugin::remove_inspector_plugin);
	ClassDB::bind_method(D_METHOD("set_input_event_forwarding_always_enabled"), &EditorPlugin::set_input_event_forwarding_always_enabled);
	ClassDB::bind_method(D_METHOD("set_force_draw_over_forwarding_enabled"), &EditorPlugin::set_force_draw_over_forwarding_enabled);

	ClassDB::bind_method(D_METHOD("get_editor_interface"), &EditorPlugin::get_editor_interface);
	ClassDB::bind_method(D_METHOD("get_script_create_dialog"), &EditorPlugin::get_script_create_dialog);

	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::BOOL, "forward_canvas_gui_input", PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "InputEvent")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("forward_canvas_draw_over_viewport", PropertyInfo(Variant::OBJECT, "overlay", PROPERTY_HINT_RESOURCE_TYPE, "Control")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("forward_canvas_force_draw_over_viewport", PropertyInfo(Variant::OBJECT, "overlay", PROPERTY_HINT_RESOURCE_TYPE, "Control")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::BOOL, "forward_spatial_gui_input", PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_RESOURCE_TYPE, "Camera"), PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "InputEvent")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::STRING, "get_plugin_name"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "get_plugin_icon"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::BOOL, "has_main_screen"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("make_visible", PropertyInfo(Variant::BOOL, "visible")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("edit", PropertyInfo(Variant::OBJECT, "object")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::BOOL, "handles", PropertyInfo(Variant::OBJECT, "object")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::DICTIONARY, "get_state"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("set_state", PropertyInfo(Variant::DICTIONARY, "state")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("clear"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("save_external_data"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("apply_changes"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::PACKED_STRING_ARRAY, "get_breakpoints"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("set_window_layout", PropertyInfo(Variant::OBJECT, "layout", PROPERTY_HINT_RESOURCE_TYPE, "ConfigFile")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("get_window_layout", PropertyInfo(Variant::OBJECT, "layout", PROPERTY_HINT_RESOURCE_TYPE, "ConfigFile")));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo(Variant::BOOL, "build"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("enable_plugin"));
	ClassDB::add_virtual_method(get_class_static(), MethodInfo("disable_plugin"));

	ADD_SIGNAL(MethodInfo("scene_changed", PropertyInfo(Variant::OBJECT, "scene_root", PROPERTY_HINT_RESOURCE_TYPE, "Node")));
	ADD_SIGNAL(MethodInfo("scene_closed", PropertyInfo(Variant::STRING, "filepath")));
	ADD_SIGNAL(MethodInfo("main_screen_changed", PropertyInfo(Variant::STRING, "screen_name")));
	ADD_SIGNAL(MethodInfo("resource_saved", PropertyInfo(Variant::OBJECT, "resource", PROPERTY_HINT_RESOURCE_TYPE, "Resource")));

	BIND_ENUM_CONSTANT(CONTAINER_TOOLBAR);
	BIND_ENUM_CONSTANT(CONTAINER_SPATIAL_EDITOR_MENU);
	BIND_ENUM_CONSTANT(CONTAINER_SPATIAL_EDITOR_SIDE_LEFT);
	BIND_ENUM_CONSTANT(CONTAINER_SPATIAL_EDITOR_SIDE_RIGHT);
	BIND_ENUM_CONSTANT(CONTAINER_SPATIAL_EDITOR_BOTTOM);
	BIND_ENUM_CONSTANT(CONTAINER_CANVAS_EDITOR_MENU);
	BIND_ENUM_CONSTANT(CONTAINER_CANVAS_EDITOR_SIDE_LEFT);
	BIND_ENUM_CONSTANT(CONTAINER_CANVAS_EDITOR_SIDE_RIGHT);
	BIND_ENUM_CONSTANT(CONTAINER_CANVAS_EDITOR_BOTTOM);
	BIND_ENUM_CONSTANT(CONTAINER_PROPERTY_EDITOR_BOTTOM);
	BIND_ENUM_CONSTANT(CONTAINER_PROJECT_SETTING_TAB_LEFT);
	BIND_ENUM_CONSTANT(CONTAINER_PROJECT_SETTING_TAB_RIGHT);

	BIND_ENUM_CONSTANT(DOCK_SLOT_LEFT_UL);
	BIND_ENUM_CONSTANT(DOCK_SLOT_LEFT_BL);
	BIND_ENUM_CONSTANT(DOCK_SLOT_LEFT_UR);
	BIND_ENUM_CONSTANT(DOCK_SLOT_LEFT_BR);
	BIND_ENUM_CONSTANT(DOCK_SLOT_RIGHT_UL);
	BIND_ENUM_CONSTANT(DOCK_SLOT_RIGHT_BL);
	BIND_ENUM_CONSTANT(DOCK_SLOT_RIGHT_UR);
	BIND_ENUM_CONSTANT(DOCK_SLOT_RIGHT_BR);
	BIND_ENUM_CONSTANT(DOCK_SLOT_MAX);
}

EditorPlugin::EditorPlugin() :
		undo_redo(NULL),
		input_event_forwarding_always_enabled(false),
		force_draw_over_forwarding_enabled(false),
		last_main_screen_name("") {
}

EditorPlugin::~EditorPlugin() {
}

EditorPluginCreateFunc EditorPlugins::creation_funcs[MAX_CREATE_FUNCS];

int EditorPlugins::creation_func_count = 0;

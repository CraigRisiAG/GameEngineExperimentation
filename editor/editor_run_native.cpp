

/**
 * @class EditorRunNative
 * @brief Manages native platform export and run functionality for the editor.
 *
 * Handles the creation and management of menu buttons for running exported projects
 * on various native platforms. Provides UI elements for selecting export presets and
 * run options, and coordinates the execution of native builds with debug settings.
 */

/**
 * @brief Processes notifications for tree entry and frame updates.
 * @param p_what The notification type to process.
 *
 * On NOTIFICATION_ENTER_TREE: Initializes menu buttons for each available export platform.
 * Creates small icon buttons for each platform and connects signal handlers.
 *
 * On NOTIFICATION_PROCESS: Polls export platforms for changes and updates menu visibility
 * and options based on available export presets.
 */
void EditorRunNative::_notification(int p_what)

/**
 * @brief Executes the native run process for a selected platform and option.
 * @param p_idx The option index within the platform's options (-1 for menu button press).
 * @param p_platform The index of the export platform to run on.
 *
 * Validates the main scene exists, retrieves the appropriate export preset, and initiates
 * the native execution with configured debug flags (remote debugging, collisions, navigation, etc.).
 * Stores resume information if the main scene is not available.
 */
void EditorRunNative::_run_native(int p_idx, int p_platform)

/**
 * @brief Resumes a previously interrupted native run operation.
 *
 * Re-executes _run_native with the stored resume index and platform from the last call.
 */
void EditorRunNative::resume_run_native()

/**
 * @brief Binds native methods and signals to the scripting system.
 *
 * Exposes the "native_run" signal for use in GDScript and other bound languages.
 */
void EditorRunNative::_bind_methods()

/**
 * @brief Checks if remote debug deployment is enabled in project settings.
 * @return True if remote debug deployment is enabled, false otherwise.
 */
bool EditorRunNative::is_deploy_debug_remote_enabled() const

/**
 * @brief Initializes the EditorRunNative instance.
 *
 * Sets up frame processing, initializes first-run flag, and clears resume state.
 */
EditorRunNative::EditorRunNative()
#include "editor_run_native.h"

#include "editor_export.h"
#include "editor_node.h"
#include "editor_scale.h"

void EditorRunNative::_notification(int p_what) {

	if (p_what == NOTIFICATION_ENTER_TREE) {

		for (int i = 0; i < EditorExport::get_singleton()->get_export_platform_count(); i++) {

			Ref<EditorExportPlatform> eep = EditorExport::get_singleton()->get_export_platform(i);
			if (eep.is_null())
				continue;
			Ref<ImageTexture> icon = eep->get_run_icon();
			if (!icon.is_null()) {
				Ref<Image> im = icon->get_data();
				im = im->duplicate();
				im->clear_mipmaps();
				if (!im->empty()) {

					im->resize(16 * EDSCALE, 16 * EDSCALE);
					Ref<ImageTexture> small_icon;
					small_icon.instance();
					small_icon->create_from_image(im);
					MenuButton *mb = memnew(MenuButton);
					mb->get_popup()->connect("id_pressed", callable_mp(this, &EditorRunNative::_run_native), varray(i));
					mb->connect("pressed", callable_mp(this, &EditorRunNative::_run_native), varray(-1, i));
					mb->set_icon(small_icon);
					add_child(mb);
					menus[i] = mb;
				}
			}
		}
	}

	if (p_what == NOTIFICATION_PROCESS) {

		bool changed = EditorExport::get_singleton()->poll_export_platforms() || first;

		if (changed) {

			for (Map<int, MenuButton *>::Element *E = menus.front(); E; E = E->next()) {

				Ref<EditorExportPlatform> eep = EditorExport::get_singleton()->get_export_platform(E->key());
				MenuButton *mb = E->get();
				int dc = eep->get_options_count();

				if (dc == 0) {
					mb->hide();
				} else {
					mb->get_popup()->clear();
					mb->show();
					mb->set_tooltip(eep->get_options_tooltip());
					if (dc > 1) {
						for (int i = 0; i < dc; i++) {
							mb->get_popup()->add_icon_item(eep->get_option_icon(i), eep->get_option_label(i));
							mb->get_popup()->set_item_tooltip(mb->get_popup()->get_item_count() - 1, eep->get_option_tooltip(i));
						}
					}
				}
			}

			first = false;
		}
	}
}

void EditorRunNative::_run_native(int p_idx, int p_platform) {

	if (!EditorNode::get_singleton()->ensure_main_scene(true)) {
		resume_idx = p_idx;
		resume_platform = p_platform;
		return;
	}

	Ref<EditorExportPlatform> eep = EditorExport::get_singleton()->get_export_platform(p_platform);
	ERR_FAIL_COND(eep.is_null());

	if (p_idx == -1) {
		if (eep->get_options_count() == 1) {
			menus[p_platform]->get_popup()->hide();
			p_idx = 0;
		} else {
			return;
		}
	}

	Ref<EditorExportPreset> preset;

	for (int i = 0; i < EditorExport::get_singleton()->get_export_preset_count(); i++) {

		Ref<EditorExportPreset> ep = EditorExport::get_singleton()->get_export_preset(i);
		if (ep->is_runnable() && ep->get_platform() == eep) {
			preset = ep;
			break;
		}
	}

	if (preset.is_null()) {
		EditorNode::get_singleton()->show_warning(TTR("No runnable export preset found for this platform.\nPlease add a runnable preset in the export menu."));
		return;
	}

	emit_signal("native_run");

	int flags = 0;

	bool deploy_debug_remote = is_deploy_debug_remote_enabled();
	bool deploy_dumb = EditorSettings::get_singleton()->get_project_metadata("debug_options", "run_file_server", false);
	bool debug_collisions = EditorSettings::get_singleton()->get_project_metadata("debug_options", "run_debug_collisons", false);
	bool debug_navigation = EditorSettings::get_singleton()->get_project_metadata("debug_options", "run_debug_navigation", false);

	if (deploy_debug_remote)
		flags |= EditorExportPlatform::DEBUG_FLAG_REMOTE_DEBUG;
	if (deploy_dumb)
		flags |= EditorExportPlatform::DEBUG_FLAG_DUMB_CLIENT;
	if (debug_collisions)
		flags |= EditorExportPlatform::DEBUG_FLAG_VIEW_COLLISONS;
	if (debug_navigation)
		flags |= EditorExportPlatform::DEBUG_FLAG_VIEW_NAVIGATION;

	eep->run(preset, p_idx, flags);
}

void EditorRunNative::resume_run_native() {
	_run_native(resume_idx, resume_platform);
}

void EditorRunNative::_bind_methods() {

	ADD_SIGNAL(MethodInfo("native_run"));
}

bool EditorRunNative::is_deploy_debug_remote_enabled() const {

	return EditorSettings::get_singleton()->get_project_metadata("debug_options", "run_deploy_remote_debug", false);
}

EditorRunNative::EditorRunNative() {
	set_process(true);
	first = true;
	resume_idx = 0;
	resume_platform = 0;
}

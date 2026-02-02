

/**
 * @file editor_run.cpp
 * @brief Editor runtime execution manager for the game engine editor.
 * 
 * This module handles launching and managing game instances from the editor,
 * including process spawning, debugging configuration, and window placement settings.
 * It supports multiple simultaneous instances, remote debugging, breakpoints,
 * and various window placement options.
 */

/**
 * @brief Gets the current execution status of the editor run session.
 * @return EditorRun::Status The current status (PLAY or STOP).
 */

/**
 * @brief Runs a game scene with specified configuration and debugging options.
 * 
 * Launches one or more instances of the game executable with arguments configured from
 * editor settings, including remote debugging, breakpoints, window placement, and custom arguments.
 * 
 * @param p_scene The scene file path to run. If empty, runs the default scene.
 * @param p_custom_args Additional command-line arguments to pass to the game executable.
 * @param p_breakpoints List of breakpoint locations for the debugger.
 * @param p_skip_breakpoints If true, skip all breakpoints during execution.
 * @return Error OK if successful, otherwise an error code.
 */

/**
 * @brief Checks if a given process ID belongs to a child process managed by this EditorRun instance.
 * 
 * @param p_pid The process ID to check.
 * @return bool True if the process is managed by this EditorRun instance, false otherwise.
 */

/**
 * @brief Terminates a specific child process and removes it from the managed list.
 * 
 * @param p_pid The process ID of the child process to terminate.
 */

/**
 * @brief Stops all running child processes and sets status to STOP.
 * 
 * Terminates all managed game instances and updates the run session status.
 */

/**
 * @brief Constructs an EditorRun instance with initial STOP status.
 */
#include "editor_run.h"

#include "core/project_settings.h"
#include "editor_settings.h"

EditorRun::Status EditorRun::get_status() const {

	return status;
}

Error EditorRun::run(const String &p_scene, const String &p_custom_args, const List<String> &p_breakpoints, const bool &p_skip_breakpoints) {

	List<String> args;

	String resource_path = ProjectSettings::get_singleton()->get_resource_path();
	String remote_host = EditorSettings::get_singleton()->get("network/debug/remote_host");
	int remote_port = (int)EditorSettings::get_singleton()->get("network/debug/remote_port");

	if (resource_path != "") {
		args.push_back("--path");
		args.push_back(resource_path.replace(" ", "%20"));
	}

	args.push_back("--remote-debug");
	args.push_back(remote_host + ":" + String::num(remote_port));

	args.push_back("--allow_focus_steal_pid");
	args.push_back(itos(OS::get_singleton()->get_process_id()));

	bool debug_collisions = EditorSettings::get_singleton()->get_project_metadata("debug_options", "run_debug_collisons", false);
	bool debug_navigation = EditorSettings::get_singleton()->get_project_metadata("debug_options", "run_debug_navigation", false);
	if (debug_collisions) {
		args.push_back("--debug-collisions");
	}

	if (debug_navigation) {
		args.push_back("--debug-navigation");
	}

	int screen = EditorSettings::get_singleton()->get("run/window_placement/screen");
	if (screen == 0) {
		// Same as editor
		screen = OS::get_singleton()->get_current_screen();
	} else if (screen == 1) {
		// Previous monitor (wrap to the other end if needed)
		screen = Math::wrapi(
				OS::get_singleton()->get_current_screen() - 1,
				0,
				OS::get_singleton()->get_screen_count());
	} else if (screen == 2) {
		// Next monitor (wrap to the other end if needed)
		screen = Math::wrapi(
				OS::get_singleton()->get_current_screen() + 1,
				0,
				OS::get_singleton()->get_screen_count());
	} else {
		// Fixed monitor ID
		// There are 3 special options, so decrement the option ID by 3 to get the monitor ID
		screen -= 3;
	}

	if (OS::get_singleton()->is_disable_crash_handler()) {
		args.push_back("--disable-crash-handler");
	}

	Rect2 screen_rect;
	screen_rect.position = OS::get_singleton()->get_screen_position(screen);
	screen_rect.size = OS::get_singleton()->get_screen_size(screen);

	Size2 desired_size;
	desired_size.x = ProjectSettings::get_singleton()->get("display/window/size/width");
	desired_size.y = ProjectSettings::get_singleton()->get("display/window/size/height");

	Size2 test_size;
	test_size.x = ProjectSettings::get_singleton()->get("display/window/size/test_width");
	test_size.y = ProjectSettings::get_singleton()->get("display/window/size/test_height");
	if (test_size.x > 0 && test_size.y > 0) {

		desired_size = test_size;
	}

	int window_placement = EditorSettings::get_singleton()->get("run/window_placement/rect");

	switch (window_placement) {
		case 0: { // top left

			args.push_back("--position");
			args.push_back(itos(screen_rect.position.x) + "," + itos(screen_rect.position.y));
		} break;
		case 1: { // centered
			int display_scale = 1;
#ifdef OSX_ENABLED
			if (OS::get_singleton()->get_screen_dpi(screen) >= 192 && OS::get_singleton()->get_screen_size(screen).x > 2000) {
				display_scale = 2;
			}
#endif

			Vector2 pos = screen_rect.position + ((screen_rect.size / display_scale - desired_size) / 2).floor();
			args.push_back("--position");
			args.push_back(itos(pos.x) + "," + itos(pos.y));
		} break;
		case 2: { // custom pos
			Vector2 pos = EditorSettings::get_singleton()->get("run/window_placement/rect_custom_position");
			pos += screen_rect.position;
			args.push_back("--position");
			args.push_back(itos(pos.x) + "," + itos(pos.y));
		} break;
		case 3: { // force maximized
			Vector2 pos = screen_rect.position;
			args.push_back("--position");
			args.push_back(itos(pos.x) + "," + itos(pos.y));
			args.push_back("--maximized");

		} break;
		case 4: { // force fullscreen

			Vector2 pos = screen_rect.position;
			args.push_back("--position");
			args.push_back(itos(pos.x) + "," + itos(pos.y));
			args.push_back("--fullscreen");
		} break;
	}

	if (p_breakpoints.size()) {

		args.push_back("--breakpoints");
		String bpoints;
		for (const List<String>::Element *E = p_breakpoints.front(); E; E = E->next()) {

			bpoints += E->get().replace(" ", "%20");
			if (E->next())
				bpoints += ",";
		}

		args.push_back(bpoints);
	}

	if (p_skip_breakpoints) {
		args.push_back("--skip-breakpoints");
	}

	if (p_scene != "") {
		args.push_back(p_scene);
	}

	if (p_custom_args != "") {
		Vector<String> cargs = p_custom_args.split(" ", false);
		for (int i = 0; i < cargs.size(); i++) {
			args.push_back(cargs[i].replace(" ", "%20"));
		}
	}

	String exec = OS::get_singleton()->get_executable_path();

	printf("Running: %ls", exec.c_str());
	for (List<String>::Element *E = args.front(); E; E = E->next()) {

		printf(" %ls", E->get().c_str());
	};
	printf("\n");

	int instances = EditorSettings::get_singleton()->get_project_metadata("debug_options", "run_debug_instances", 1);
	for (int i = 0; i < instances; i++) {
		OS::ProcessID pid = 0;
		Error err = OS::get_singleton()->execute(exec, args, false, &pid);
		ERR_FAIL_COND_V(err, err);
		pids.push_back(pid);
	}

	status = STATUS_PLAY;

	return OK;
}

bool EditorRun::has_child_process(OS::ProcessID p_pid) const {
	for (const List<OS::ProcessID>::Element *E = pids.front(); E; E = E->next()) {
		if (E->get() == p_pid)
			return true;
	}
	return false;
}

void EditorRun::stop_child_process(OS::ProcessID p_pid) {
	if (has_child_process(p_pid)) {
		OS::get_singleton()->kill(p_pid);
		pids.erase(p_pid);
	}
}

void EditorRun::stop() {

	if (status != STATUS_STOP && pids.size() > 0) {

		for (List<OS::ProcessID>::Element *E = pids.front(); E; E = E->next()) {
			OS::get_singleton()->kill(E->get());
		}
	}

	status = STATUS_STOP;
}

EditorRun::EditorRun() {

	status = STATUS_STOP;
}

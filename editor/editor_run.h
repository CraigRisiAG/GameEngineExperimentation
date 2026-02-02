

/**
 * @class EditorRun
 * @brief Manages the execution and lifecycle of editor play sessions.
 * 
 * This class handles running scenes in the editor, managing their execution state,
 * and controlling child processes associated with play sessions.
 */

/**
 * @enum Status
 * @brief Represents the current execution state of the editor.
 * @var STATUS_PLAY - Scene is currently running
 * @var STATUS_PAUSED - Scene execution is paused
 * @var STATUS_STOP - Scene is stopped
 */

/**
 * @brief Gets the current execution status.
 * @return Status The current status of the editor run session
 */

/**
 * @brief Runs a scene with optional custom arguments and breakpoints.
 * @param p_scene The path to the scene file to run
 * @param p_custom_args Custom command-line arguments to pass to the scene
 * @param p_breakpoints List of breakpoints to set before execution
 * @param p_skip_breakpoints If true, skip all breakpoints during execution (default: false)
 * @return Error Error code indicating success or failure of the run operation
 */

/**
 * @brief Notifies that native execution has started.
 * Sets the status to STATUS_PLAY.
 */

/**
 * @brief Stops the current scene execution and all child processes.
 */

/**
 * @brief Stops a specific child process by its ID.
 * @param p_pid The process ID to terminate
 */

/**
 * @brief Checks if a child process with the given ID exists.
 * @param p_pid The process ID to check
 * @return bool True if the process exists, false otherwise
 */

/**
 * @brief Gets the total number of active child processes.
 * @return int The count of child processes
 */

/**
 * @brief Constructs an EditorRun instance and initializes it to stopped state.
 */
#ifndef EDITOR_RUN_H
#define EDITOR_RUN_H

#include "core/os/os.h"
#include "scene/main/node.h"
class EditorRun {
public:
	enum Status {

		STATUS_PLAY,
		STATUS_PAUSED,
		STATUS_STOP
	};

	List<OS::ProcessID> pids;

private:
	Status status;

public:
	Status get_status() const;
	Error run(const String &p_scene, const String &p_custom_args, const List<String> &p_breakpoints, const bool &p_skip_breakpoints = false);
	void run_native_notify() { status = STATUS_PLAY; }
	void stop();

	void stop_child_process(OS::ProcessID p_pid);
	bool has_child_process(OS::ProcessID p_pid) const;
	int get_child_process_count() const { return pids.size(); }

	EditorRun();
};

#endif // EDITOR_RUN_H

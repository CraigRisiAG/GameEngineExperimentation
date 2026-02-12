

/**
 * @class BackgroundProgress
 * @brief A background progress tracking UI component that extends HBoxContainer.
 * 
 * Manages multiple concurrent background tasks with visual progress indicators.
 * Thread-safe implementation for handling progress updates from background threads.
 * 
 * @note This class is thread-safe and can be updated from multiple threads.
 */

/**
 * @struct BackgroundProgress::Task
 * @brief Internal structure to represent a single background task.
 * 
 * @member hb The horizontal box container holding the task UI elements.
 * @member progress The progress bar widget displaying task completion.
 */

/**
 * @fn void BackgroundProgress::add_task(const String &p_task, const String &p_label, int p_steps)
 * @brief Adds a new background task with a label and expected number of steps.
 * 
 * @param p_task Unique identifier for the task.
 * @param p_label Display label for the task.
 * @param p_steps Expected number of steps to completion.
 */

/**
 * @fn void BackgroundProgress::task_step(const String &p_task, int p_step = -1)
 * @brief Updates the progress of an existing task.
 * 
 * @param p_task Identifier of the task to update.
 * @param p_step Current step number. If -1, increments automatically.
 */

/**
 * @fn void BackgroundProgress::end_task(const String &p_task)
 * @brief Completes and removes a background task from tracking.
 * 
 * @param p_task Identifier of the task to end.
 */

/**
 * @class ProgressDialog
 * @brief A modal dialog for displaying and managing progress of foreground tasks.
 * 
 * Provides a popup window that displays progress bars for multiple tasks with
 * optional cancellation capability. Implements singleton pattern for global access.
 */

/**
 * @struct ProgressDialog::Task
 * @brief Internal structure representing a single progress dialog task.
 * 
 * @member task Unique task identifier.
 * @member vb Vertical box container for task UI elements.
 * @member progress Progress bar widget.
 * @member state Label displaying current task state/status.
 */

/**
 * @fn static ProgressDialog* ProgressDialog::get_singleton()
 * @brief Returns the global singleton instance of the progress dialog.
 * 
 * @return Pointer to the singleton ProgressDialog instance.
 */

/**
 * @fn void ProgressDialog::add_task(const String &p_task, const String &p_label, int p_steps, bool p_can_cancel = false)
 * @brief Adds a new task to the progress dialog.
 * 
 * @param p_task Unique identifier for the task.
 * @param p_label Display label for the task.
 * @param p_steps Expected number of steps to completion.
 * @param p_can_cancel Whether the user can cancel this task.
 */

/**
 * @fn bool ProgressDialog::task_step(const String &p_task, const String &p_state, int p_step = -1, bool p_force_redraw = true)
 * @brief Updates the progress and state of an existing task.
 * 
 * @param p_task Identifier of the task to update.
 * @param p_state Current state description to display.
 * @param p_step Current step number. If -1, increments automatically.
 * @param p_force_redraw Forces immediate UI redraw if true.
 * @return True if the task is still running, false if cancelled.
 */

/**
 * @fn void ProgressDialog::end_task(const String &p_task)
 * @brief Completes and removes a task from the progress dialog.
 * 
 * @param p_task Identifier of the task to end.
 */

/**
 * @fn ProgressDialog::ProgressDialog()
 * @brief Constructs a new ProgressDialog instance and initializes the singleton.
 */
#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/popup.h"
#include "scene/gui/progress_bar.h"

class BackgroundProgress : public HBoxContainer {

	GDCLASS(BackgroundProgress, HBoxContainer);

	_THREAD_SAFE_CLASS_

	struct Task {

		HBoxContainer *hb;
		ProgressBar *progress;
	};

	Map<String, Task> tasks;
	Map<String, int> updates;
	void _update();

protected:
	void _add_task(const String &p_task, const String &p_label, int p_steps);
	void _task_step(const String &p_task, int p_step = -1);
	void _end_task(const String &p_task);

	static void _bind_methods();

public:
	void add_task(const String &p_task, const String &p_label, int p_steps);
	void task_step(const String &p_task, int p_step = -1);
	void end_task(const String &p_task);

	BackgroundProgress() {}
};

class ProgressDialog : public Popup {

	GDCLASS(ProgressDialog, Popup);
	struct Task {

		String task;
		VBoxContainer *vb;
		ProgressBar *progress;
		Label *state;
	};
	HBoxContainer *cancel_hb;
	Button *cancel;

	Map<String, Task> tasks;
	VBoxContainer *main;
	uint64_t last_progress_tick;

	static ProgressDialog *singleton;
	void _popup();

	void _cancel_pressed();
	bool cancelled;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	static ProgressDialog *get_singleton() { return singleton; }
	void add_task(const String &p_task, const String &p_label, int p_steps, bool p_can_cancel = false);
	bool task_step(const String &p_task, const String &p_state, int p_step = -1, bool p_force_redraw = true);
	void end_task(const String &p_task);

	ProgressDialog();
};

#endif // PROGRESS_DIALOG_H

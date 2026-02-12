


/**
 * @class BackgroundProgress
 * @brief Manages background task progress tracking with thread-safe operations.
 * 
 * Provides functionality to add, update, and remove background tasks with visual progress indicators.
 * Uses a message queue to safely handle task state changes from multiple threads.
 * 
 * @method _add_task(const String &p_task, const String &p_label, int p_steps)
 *         Internal method to create a new background task with a progress bar.
 * 
 * @method _update()
 *         Processes pending task updates from the updates map.
 * 
 * @method _task_step(const String &p_task, int p_step)
 *         Internal method to update task progress. Negative step values increment by 1.
 * 
 * @method _end_task(const String &p_task)
 *         Internal method to remove a completed task and cleanup its UI elements.
 * 
 * @method add_task(const String &p_task, const String &p_label, int p_steps)
 *         Public method to queue a new background task for creation.
 * 
 * @method task_step(const String &p_task, int p_step)
 *         Public method to update task progress, queuing updates to prevent deadlock.
 * 
 * @method end_task(const String &p_task)
 *         Public method to queue task completion and removal.
 */

/**
 * @class ProgressDialog
 * @brief Modal dialog for displaying foreground task progress with cancellation support.
 * 
 * Displays progress bars for long-running editor operations with optional cancel button.
 * Manages task lifecycle and redraws at throttled intervals (200ms) for performance.
 * Singleton pattern implementation for centralized access.
 * 
 * @method add_task(const String &p_task, const String &p_label, int p_steps, bool p_can_cancel)
 *         Creates and displays a new progress task in the dialog.
 * 
 * @method task_step(const String &p_task, const String &p_state, int p_step, bool p_force_redraw)
 *         Updates task progress and state label. Returns cancellation status.
 * 
 * @method end_task(const String &p_task)
 *         Removes task and hides dialog if no tasks remain.
 * 
 * @method _cancel_pressed()
 *         Internal callback when cancel button is pressed.
 * 
 * @method _popup()
 *         Positions and displays the dialog centered on screen.
 * 
 * @method _notification(int p_what)
 *         Handles redraw notifications to apply panel styling.
 */
#include "progress_dialog.h"

#include "core/message_queue.h"
#include "core/os/os.h"
#include "editor_scale.h"
#include "main/main.h"

void BackgroundProgress::_add_task(const String &p_task, const String &p_label, int p_steps) {

	_THREAD_SAFE_METHOD_
	ERR_FAIL_COND_MSG(tasks.has(p_task), "Task '" + p_task + "' already exists.");
	BackgroundProgress::Task t;
	t.hb = memnew(HBoxContainer);
	Label *l = memnew(Label);
	l->set_text(p_label + " ");
	t.hb->add_child(l);
	t.progress = memnew(ProgressBar);
	t.progress->set_max(p_steps);
	t.progress->set_value(p_steps);
	Control *ec = memnew(Control);
	ec->set_h_size_flags(SIZE_EXPAND_FILL);
	ec->set_v_size_flags(SIZE_EXPAND_FILL);
	t.progress->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	ec->add_child(t.progress);
	ec->set_custom_minimum_size(Size2(80, 5) * EDSCALE);
	t.hb->add_child(ec);

	add_child(t.hb);

	tasks[p_task] = t;
}

void BackgroundProgress::_update() {

	_THREAD_SAFE_METHOD_

	for (Map<String, int>::Element *E = updates.front(); E; E = E->next()) {

		if (tasks.has(E->key())) {
			_task_step(E->key(), E->get());
		}
	}

	updates.clear();
}

void BackgroundProgress::_task_step(const String &p_task, int p_step) {

	_THREAD_SAFE_METHOD_

	ERR_FAIL_COND(!tasks.has(p_task));

	Task &t = tasks[p_task];
	if (p_step < 0)
		t.progress->set_value(t.progress->get_value() + 1);
	else
		t.progress->set_value(p_step);
}
void BackgroundProgress::_end_task(const String &p_task) {

	_THREAD_SAFE_METHOD_

	ERR_FAIL_COND(!tasks.has(p_task));
	Task &t = tasks[p_task];

	memdelete(t.hb);
	tasks.erase(p_task);
}

void BackgroundProgress::_bind_methods() {

	ClassDB::bind_method("_add_task", &BackgroundProgress::_add_task);
	ClassDB::bind_method("_task_step", &BackgroundProgress::_task_step);
	ClassDB::bind_method("_end_task", &BackgroundProgress::_end_task);
	ClassDB::bind_method("_update", &BackgroundProgress::_update);
}

void BackgroundProgress::add_task(const String &p_task, const String &p_label, int p_steps) {

	MessageQueue::get_singleton()->push_call(this, "_add_task", p_task, p_label, p_steps);
}
void BackgroundProgress::task_step(const String &p_task, int p_step) {

	//this code is weird, but it prevents deadlock.
	bool no_updates = true;
	{
		_THREAD_SAFE_METHOD_
		no_updates = updates.empty();
	}

	if (no_updates)
		MessageQueue::get_singleton()->push_call(this, "_update");

	{
		_THREAD_SAFE_METHOD_
		updates[p_task] = p_step;
	}
}

void BackgroundProgress::end_task(const String &p_task) {

	MessageQueue::get_singleton()->push_call(this, "_end_task", p_task);
}

////////////////////////////////////////////////

ProgressDialog *ProgressDialog::singleton = NULL;

void ProgressDialog::_notification(int p_what) {

	switch (p_what) {

		case NOTIFICATION_DRAW: {

			Ref<StyleBox> style = get_stylebox("panel", "PopupMenu");
			draw_style_box(style, Rect2(Point2(), get_size()));

		} break;
	}
}

void ProgressDialog::_popup() {

	Size2 ms = main->get_combined_minimum_size();
	ms.width = MAX(500 * EDSCALE, ms.width);

	Ref<StyleBox> style = get_stylebox("panel", "PopupMenu");
	ms += style->get_minimum_size();
	main->set_margin(MARGIN_LEFT, style->get_margin(MARGIN_LEFT));
	main->set_margin(MARGIN_RIGHT, -style->get_margin(MARGIN_RIGHT));
	main->set_margin(MARGIN_TOP, style->get_margin(MARGIN_TOP));
	main->set_margin(MARGIN_BOTTOM, -style->get_margin(MARGIN_BOTTOM));

	raise();
	popup_centered(ms);
}

void ProgressDialog::add_task(const String &p_task, const String &p_label, int p_steps, bool p_can_cancel) {

	if (MessageQueue::get_singleton()->is_flushing()) {
		ERR_PRINT("Do not use progress dialog (task) while flushing the message queue or using call_deferred()!");
		return;
	}

	ERR_FAIL_COND_MSG(tasks.has(p_task), "Task '" + p_task + "' already exists.");
	ProgressDialog::Task t;
	t.vb = memnew(VBoxContainer);
	VBoxContainer *vb2 = memnew(VBoxContainer);
	t.vb->add_margin_child(p_label, vb2);
	t.progress = memnew(ProgressBar);
	t.progress->set_max(p_steps);
	t.progress->set_value(p_steps);
	vb2->add_child(t.progress);
	t.state = memnew(Label);
	t.state->set_clip_text(true);
	vb2->add_child(t.state);
	main->add_child(t.vb);

	tasks[p_task] = t;
	if (p_can_cancel) {
		cancel_hb->show();
	} else {
		cancel_hb->hide();
	}
	cancel_hb->raise();
	cancelled = false;
	_popup();
	if (p_can_cancel) {
		cancel->grab_focus();
	}
}

bool ProgressDialog::task_step(const String &p_task, const String &p_state, int p_step, bool p_force_redraw) {

	ERR_FAIL_COND_V(!tasks.has(p_task), cancelled);

	if (!p_force_redraw) {
		uint64_t tus = OS::get_singleton()->get_ticks_usec();
		if (tus - last_progress_tick < 200000) //200ms
			return cancelled;
	}

	Task &t = tasks[p_task];
	if (p_step < 0)
		t.progress->set_value(t.progress->get_value() + 1);
	else
		t.progress->set_value(p_step);

	t.state->set_text(p_state);
	last_progress_tick = OS::get_singleton()->get_ticks_usec();
	if (cancel_hb->is_visible()) {
		OS::get_singleton()->force_process_input();
	}

	Main::iteration(); // this will not work on a lot of platforms, so it's only meant for the editor
	return cancelled;
}

void ProgressDialog::end_task(const String &p_task) {

	ERR_FAIL_COND(!tasks.has(p_task));
	Task &t = tasks[p_task];

	memdelete(t.vb);
	tasks.erase(p_task);

	if (tasks.empty())
		hide();
	else
		_popup();
}

void ProgressDialog::_cancel_pressed() {
	cancelled = true;
}

void ProgressDialog::_bind_methods() {
}

ProgressDialog::ProgressDialog() {

	main = memnew(VBoxContainer);
	add_child(main);
	main->set_anchors_and_margins_preset(Control::PRESET_WIDE);
	set_exclusive(true);
	last_progress_tick = 0;
	singleton = this;
	cancel_hb = memnew(HBoxContainer);
	main->add_child(cancel_hb);
	cancel_hb->hide();
	cancel = memnew(Button);
	cancel_hb->add_spacer();
	cancel_hb->add_child(cancel);
	cancel->set_text(TTR("Cancel"));
	cancel_hb->add_spacer();
	cancel->connect("pressed", callable_mp(this, &ProgressDialog::_cancel_pressed));
}

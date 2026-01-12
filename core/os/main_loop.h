

/**
 * @class MainLoop
 * @brief Main loop class that handles the core event loop and lifecycle of the application.
 * 
 * MainLoop is responsible for managing the application's main event loop, including
 * input handling, frame iteration, and idle processing. It receives system notifications
 * such as window focus changes, file drops, and OS-level events.
 * 
 * This class serves as the base class for implementing custom main loop behavior and
 * can be extended to handle specific application requirements.
 * 
 * @section Notifications
 * - NOTIFICATION_WM_MOUSE_ENTER: Mouse cursor entered the window
 * - NOTIFICATION_WM_MOUSE_EXIT: Mouse cursor left the window
 * - NOTIFICATION_WM_FOCUS_IN: Window gained focus
 * - NOTIFICATION_WM_FOCUS_OUT: Window lost focus
 * - NOTIFICATION_WM_QUIT_REQUEST: System quit request received
 * - NOTIFICATION_WM_GO_BACK_REQUEST: Back button request (mobile)
 * - NOTIFICATION_WM_UNFOCUS_REQUEST: Unfocus request
 * - NOTIFICATION_OS_MEMORY_WARNING: Low memory warning from OS
 * - NOTIFICATION_TRANSLATION_CHANGED: Language/translation changed
 * - NOTIFICATION_WM_ABOUT: About dialog requested
 * - NOTIFICATION_CRASH: Application crash notification
 * - NOTIFICATION_OS_IME_UPDATE: Input method editor update
 * - NOTIFICATION_APP_RESUMED: Application resumed from pause
 * - NOTIFICATION_APP_PAUSED: Application paused
 */
#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "core/os/input_event.h"
#include "core/reference.h"
#include "core/script_language.h"

class MainLoop : public Object {

	GDCLASS(MainLoop, Object);
	OBJ_CATEGORY("Main Loop");

	Ref<Script> init_script;

protected:
	static void _bind_methods();

public:
	enum {
		//make sure these are replicated in Node
		NOTIFICATION_WM_MOUSE_ENTER = 1002,
		NOTIFICATION_WM_MOUSE_EXIT = 1003,
		NOTIFICATION_WM_FOCUS_IN = 1004,
		NOTIFICATION_WM_FOCUS_OUT = 1005,
		NOTIFICATION_WM_QUIT_REQUEST = 1006,
		NOTIFICATION_WM_GO_BACK_REQUEST = 1007,
		NOTIFICATION_WM_UNFOCUS_REQUEST = 1008,
		NOTIFICATION_OS_MEMORY_WARNING = 1009,
		NOTIFICATION_TRANSLATION_CHANGED = 1010,
		NOTIFICATION_WM_ABOUT = 1011,
		NOTIFICATION_CRASH = 1012,
		NOTIFICATION_OS_IME_UPDATE = 1013,
		NOTIFICATION_APP_RESUMED = 1014,
		NOTIFICATION_APP_PAUSED = 1015,
	};

	virtual void input_event(const Ref<InputEvent> &p_event);
	virtual void input_text(const String &p_text);

	virtual void init();
	virtual bool iteration(float p_time);
	virtual bool idle(float p_time);
	virtual void finish();

	virtual void drop_files(const Vector<String> &p_files, int p_from_screen = 0);
	virtual void global_menu_action(const Variant &p_id, const Variant &p_meta);

	void set_init_script(const Ref<Script> &p_init_script);

	MainLoop();
	virtual ~MainLoop();
};

#endif // MAIN_LOOP_H



/// \class MainLoop
/// \brief Core class that manages the main event loop and script lifecycle for the game engine.
///
/// MainLoop provides the fundamental framework for handling input events, text input, frame iteration,
/// and idle processing. It integrates with the scripting system to allow script instances to override
/// virtual methods for custom game logic.
///
/// The lifecycle follows this sequence:
/// 1. init() - Initializes the main loop and sets up the script instance
/// 2. iteration(delta) - Called each frame with elapsed time
/// 3. idle(delta) - Called during idle frames
/// 4. finish() - Cleans up and finalizes the main loop
///
/// \method void _bind_methods()
/// \brief Binds C++ methods and signals to the scripting engine.
/// Registers callable methods, virtual methods for script override, notifications, and signals
/// so they can be accessed from scripts.
///
/// \method void set_init_script(const Ref<Script> &p_init_script)
/// \brief Sets the initialization script to be executed during init().
/// \param p_init_script The script to initialize with
///
/// \method void input_text(const String &p_text)
/// \brief Passes text input to the script instance via _input_text() callback.
/// \param p_text The input text string
///
/// \method void input_event(const Ref<InputEvent> &p_event)
/// \brief Passes input events to the script instance via _input_event() callback.
/// \param p_event The input event to process
///
/// \method void init()
/// \brief Initializes the main loop by setting the init script and calling _initialize() on script instances.
///
/// \method bool iteration(float p_time)
/// \brief Processes one frame iteration, delegating to script's _iteration() method.
/// \param p_time Delta time in seconds for this frame
/// \return Boolean result from script's _iteration() or false if no script instance
///
/// \method bool idle(float p_time)
/// \brief Processes idle time, delegating to script's _idle() method.
/// \param p_time Delta time in seconds for idle period
/// \return Boolean result from script's _idle() or false if no script instance
///
/// \method void drop_files(const Vector<String> &p_files, int p_from_screen)
/// \brief Handles file drop events via _drop_files() script callback.
/// \param p_files Vector of dropped file paths
/// \param p_from_screen Screen index the files were dropped from
///
/// \method void global_menu_action(const Variant &p_id, const Variant &p_meta)
/// \brief Handles global menu actions via _global_menu_action() script callback.
/// \param p_id Identifier for the menu action
/// \param p_meta Additional metadata for the action
///
/// \method void finish()
/// \brief Finalizes the main loop by calling _finalize() and clearing the script instance.
#include "main_loop.h"

#include "core/script_language.h"

void MainLoop::_bind_methods() {

	ClassDB::bind_method(D_METHOD("input_event", "event"), &MainLoop::input_event);
	ClassDB::bind_method(D_METHOD("input_text", "text"), &MainLoop::input_text);
	ClassDB::bind_method(D_METHOD("init"), &MainLoop::init);
	ClassDB::bind_method(D_METHOD("iteration", "delta"), &MainLoop::iteration);
	ClassDB::bind_method(D_METHOD("idle", "delta"), &MainLoop::idle);
	ClassDB::bind_method(D_METHOD("finish"), &MainLoop::finish);

	BIND_VMETHOD(MethodInfo("_input_event", PropertyInfo(Variant::OBJECT, "event", PROPERTY_HINT_RESOURCE_TYPE, "InputEvent")));
	BIND_VMETHOD(MethodInfo("_input_text", PropertyInfo(Variant::STRING, "text")));
	BIND_VMETHOD(MethodInfo("_initialize"));
	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_iteration", PropertyInfo(Variant::FLOAT, "delta")));
	BIND_VMETHOD(MethodInfo(Variant::BOOL, "_idle", PropertyInfo(Variant::FLOAT, "delta")));
	BIND_VMETHOD(MethodInfo("_drop_files", PropertyInfo(Variant::PACKED_STRING_ARRAY, "files"), PropertyInfo(Variant::INT, "from_screen")));
	BIND_VMETHOD(MethodInfo("_finalize"));

	BIND_VMETHOD(MethodInfo("_global_menu_action", PropertyInfo(Variant::NIL, "id"), PropertyInfo(Variant::NIL, "meta")));

	BIND_CONSTANT(NOTIFICATION_WM_MOUSE_ENTER);
	BIND_CONSTANT(NOTIFICATION_WM_MOUSE_EXIT);
	BIND_CONSTANT(NOTIFICATION_WM_FOCUS_IN);
	BIND_CONSTANT(NOTIFICATION_WM_FOCUS_OUT);
	BIND_CONSTANT(NOTIFICATION_WM_QUIT_REQUEST);
	BIND_CONSTANT(NOTIFICATION_WM_GO_BACK_REQUEST);
	BIND_CONSTANT(NOTIFICATION_WM_UNFOCUS_REQUEST);
	BIND_CONSTANT(NOTIFICATION_OS_MEMORY_WARNING);
	BIND_CONSTANT(NOTIFICATION_TRANSLATION_CHANGED);
	BIND_CONSTANT(NOTIFICATION_WM_ABOUT);
	BIND_CONSTANT(NOTIFICATION_CRASH);
	BIND_CONSTANT(NOTIFICATION_OS_IME_UPDATE);
	BIND_CONSTANT(NOTIFICATION_APP_RESUMED);
	BIND_CONSTANT(NOTIFICATION_APP_PAUSED);

	ADD_SIGNAL(MethodInfo("on_request_permissions_result", PropertyInfo(Variant::STRING, "permission"), PropertyInfo(Variant::BOOL, "granted")));
};

void MainLoop::set_init_script(const Ref<Script> &p_init_script) {

	init_script = p_init_script;
}

MainLoop::MainLoop() {
}

MainLoop::~MainLoop() {
}

void MainLoop::input_text(const String &p_text) {

	if (get_script_instance())
		get_script_instance()->call("_input_text", p_text);
}

void MainLoop::input_event(const Ref<InputEvent> &p_event) {

	if (get_script_instance())
		get_script_instance()->call("_input_event", p_event);
}

void MainLoop::init() {

	if (init_script.is_valid())
		set_script(init_script);

	if (get_script_instance())
		get_script_instance()->call("_initialize");
}
bool MainLoop::iteration(float p_time) {

	if (get_script_instance())
		return get_script_instance()->call("_iteration", p_time);

	return false;
}
bool MainLoop::idle(float p_time) {

	if (get_script_instance())
		return get_script_instance()->call("_idle", p_time);

	return false;
}

void MainLoop::drop_files(const Vector<String> &p_files, int p_from_screen) {

	if (get_script_instance())
		get_script_instance()->call("_drop_files", p_files, p_from_screen);
}

void MainLoop::global_menu_action(const Variant &p_id, const Variant &p_meta) {

	if (get_script_instance())
		get_script_instance()->call("_global_menu_action", p_id, p_meta);
}

void MainLoop::finish() {

	if (get_script_instance()) {
		get_script_instance()->call("_finalize");
		set_script(Variant()); //clear script
	}
}

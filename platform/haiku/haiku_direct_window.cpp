
/**
 * @file haiku_direct_window.cpp
 * @brief Haiku platform implementation of a direct window for Godot game engine.
 * 
 * This file implements the HaikuDirectWindow class, which manages a Haiku BDirectWindow
 * and handles all input events including mouse, keyboard, and window resize operations.
 * It processes these events and forwards them to the engine's input system.
 * 
 * @class HaikuDirectWindow
 * @brief A Haiku native window implementation that receives and processes input events.
 * 
 * @details
 * HaikuDirectWindow extends BDirectWindow and serves as the main window for the Godot
 * engine on the Haiku operating system. It manages:
 * - OpenGL view attachment and direct mode rendering
 * - Mouse input events (button presses, movement, wheel scrolling)
 * - Keyboard input events and modifier state changes
 * - Window resize events
 * - Main loop iteration and message dispatching
 * 
 * @method HaikuDirectWindow(BRect p_frame)
 * @brief Constructor that initializes the window with given frame dimensions.
 * @param p_frame The window frame rectangle.
 * 
 * @method void SetHaikuGLView(HaikuGLView *p_view)
 * @brief Attaches an OpenGL view to this window.
 * @param p_view Pointer to the HaikuGLView to attach.
 * 
 * @method void StartMessageRunner()
 * @brief Starts a message runner that triggers redraws at 60 FPS.
 * 
 * @method void StopMessageRunner()
 * @brief Stops the message runner and cleans up resources.
 * 
 * @method void SetInput(InputDefault *p_input)
 * @brief Sets the input handler for this window.
 * @param p_input Pointer to the input handler.
 * 
 * @method void SetMainLoop(MainLoop *p_main_loop)
 * @brief Sets the main game loop for this window.
 * @param p_main_loop Pointer to the main loop.
 * 
 * @method bool QuitRequested()
 * @brief Handles window close request and notifies the main loop.
 * @return Always false to prevent immediate window closure.
 * 
 * @method void DirectConnected(direct_buffer_info *info)
 * @brief Called when direct buffer access is established.
 * @param info Direct buffer information structure.
 * 
 * @method void MessageReceived(BMessage *message)
 * @brief Processes messages sent to this window.
 * @param message The message to process.
 * 
 * @method void DispatchMessage(BMessage *message, BHandler *handler)
 * @brief Dispatches messages to appropriate input event handlers.
 * @param message The message to dispatch.
 * @param handler The handler that will receive the message.
 * 
 * @method void HandleMouseButton(BMessage *message)
 * @brief Processes mouse button press/release events.
 * @param message Message containing mouse button data.
 * 
 * @method void HandleMouseMoved(BMessage *message)
 * @brief Processes mouse movement events and calculates relative movement.
 * @param message Message containing mouse position data.
 * 
 * @method void HandleMouseWheelChanged(BMessage *message)
 * @brief Processes mouse wheel scroll events.
 * @param message Message containing wheel delta data.
 * 
 * @method void HandleKeyboardEvent(BMessage *message)
 * @brief Processes keyboard key press/release events with Unicode support.
 * @param message Message containing keyboard data.
 * 
 * @method void HandleKeyboardModifierEvent(BMessage *message)
 * @brief Processes modifier key state changes (Shift, Ctrl, Alt, Cmd).
 * @param message Message containing modifier state data.
 * 
 * @method void HandleWindowResized(BMessage *message)
 * @brief Processes window resize events and updates video mode dimensions.
 * @param message Message containing window dimensions.
 * 
 * @method void GetKeyModifierState(Ref<InputEventWithModifiers> event, uint32 p_state)
 * @brief Updates input event with current modifier key states.
 * @param event The input event to update.
 * @param p_state The modifier state flags from Haiku.
 * 
 * @method int GetMouseButtonState(uint32 p_state)
 * @brief Converts Haiku mouse button state to engine button mask.
 * @param p_state Haiku mouse button state flags.
 * @return Button mask for the input system.
 */
#include <UnicodeChar.h>

#include "core/os/keyboard.h"
#include "haiku_direct_window.h"
#include "key_mapping_haiku.h"
#include "main/main.h"

HaikuDirectWindow::HaikuDirectWindow(BRect p_frame) :
		BDirectWindow(p_frame, "Godot", B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE) {
	last_mouse_pos_valid = false;
	last_buttons_state = 0;
	last_button_mask = 0;
	last_key_modifier_state = 0;

	view = NULL;
	update_runner = NULL;
	input = NULL;
	main_loop = NULL;
}

HaikuDirectWindow::~HaikuDirectWindow() {
}

void HaikuDirectWindow::SetHaikuGLView(HaikuGLView *p_view) {
	view = p_view;
}

void HaikuDirectWindow::StartMessageRunner() {
	update_runner = new BMessageRunner(BMessenger(this),
			new BMessage(REDRAW_MSG), 1000000 / 60 /* 60 fps */);
}

void HaikuDirectWindow::StopMessageRunner() {
	delete update_runner;
}

void HaikuDirectWindow::SetInput(InputDefault *p_input) {
	input = p_input;
}

void HaikuDirectWindow::SetMainLoop(MainLoop *p_main_loop) {
	main_loop = p_main_loop;
}

bool HaikuDirectWindow::QuitRequested() {
	StopMessageRunner();
	main_loop->notification(MainLoop::NOTIFICATION_WM_QUIT_REQUEST);
	return false;
}

void HaikuDirectWindow::DirectConnected(direct_buffer_info *info) {
	view->DirectConnected(info);
	view->EnableDirectMode(true);
}

void HaikuDirectWindow::MessageReceived(BMessage *message) {
	switch (message->what) {
		case REDRAW_MSG:
			if (Main::iteration()) {
				view->EnableDirectMode(false);
				Quit();
			}
			break;

		default:
			BDirectWindow::MessageReceived(message);
	}
}

void HaikuDirectWindow::DispatchMessage(BMessage *message, BHandler *handler) {
	switch (message->what) {
		case B_MOUSE_DOWN:
		case B_MOUSE_UP:
			HandleMouseButton(message);
			break;

		case B_MOUSE_MOVED:
			HandleMouseMoved(message);
			break;

		case B_MOUSE_WHEEL_CHANGED:
			HandleMouseWheelChanged(message);
			break;

		case B_KEY_DOWN:
		case B_KEY_UP:
			HandleKeyboardEvent(message);
			break;

		case B_MODIFIERS_CHANGED:
			HandleKeyboardModifierEvent(message);
			break;

		case B_WINDOW_RESIZED:
			HandleWindowResized(message);
			break;

		case LOCKGL_MSG:
			view->LockGL();
			break;

		case UNLOCKGL_MSG:
			view->UnlockGL();
			break;

		default:
			BDirectWindow::DispatchMessage(message, handler);
	}
}

void HaikuDirectWindow::HandleMouseButton(BMessage *message) {
	BPoint where;
	if (message->FindPoint("where", &where) != B_OK) {
		return;
	}

	uint32 modifiers = message->FindInt32("modifiers");
	uint32 buttons = message->FindInt32("buttons");
	uint32 button = buttons ^ last_buttons_state;
	last_buttons_state = buttons;

	// TODO: implement the mouse_mode checks
	/*
	if (mouse_mode == MOUSE_MODE_CAPTURED) {
		event.xbutton.x=last_mouse_pos.x;
		event.xbutton.y=last_mouse_pos.y;
	}
	*/

	Ref<InputEventMouseButton> mouse_event;
	mouse_event.instance();

	mouse_event->set_button_mask(GetMouseButtonState(buttons));
	mouse_event->set_position({ where.x, where.y });
	mouse_event->set_global_position({ where.x, where.y });
	GetKeyModifierState(mouse_event, modifiers);

	switch (button) {
		default:
		case B_PRIMARY_MOUSE_BUTTON:
			mouse_event->set_button_index(1);
			break;

		case B_SECONDARY_MOUSE_BUTTON:
			mouse_event->set_button_index(2);
			break;

		case B_TERTIARY_MOUSE_BUTTON:
			mouse_event->set_button_index(3);
			break;
	}

	mouse_event->set_pressed(message->what == B_MOUSE_DOWN);

	if (message->what == B_MOUSE_DOWN && mouse_event->get_button_index() == 1) {
		int32 clicks = message->FindInt32("clicks");

		if (clicks > 1) {
			mouse_event->set_doubleclick(true);
		}
	}

	input->parse_input_event(mouse_event);
}

void HaikuDirectWindow::HandleMouseMoved(BMessage *message) {
	BPoint where;
	if (message->FindPoint("where", &where) != B_OK) {
		return;
	}

	Point2i pos(where.x, where.y);
	uint32 modifiers = message->FindInt32("modifiers");
	uint32 buttons = message->FindInt32("buttons");

	if (!last_mouse_pos_valid) {
		last_mouse_position = pos;
		last_mouse_pos_valid = true;
	}

	Point2i rel = pos - last_mouse_position;

	Ref<InputEventMouseMotion> motion_event;
	motion_event.instance();
	GetKeyModifierState(motion_event, modifiers);

	motion_event->set_button_mask(GetMouseButtonState(buttons));
	motion_event->set_position({ pos.x, pos.y });
	input->set_mouse_position(pos);
	motion_event->set_global_position({ pos.x, pos.y });
	motion_event->set_speed({ input->get_last_mouse_speed().x,
			input->get_last_mouse_speed().y });

	motion_event->set_relative({ rel.x, rel.y });

	last_mouse_position = pos;

	input->parse_input_event(motion_event);
}

void HaikuDirectWindow::HandleMouseWheelChanged(BMessage *message) {
	float wheel_delta_y = 0;
	if (message->FindFloat("be:wheel_delta_y", &wheel_delta_y) != B_OK) {
		return;
	}

	Ref<InputEventMouseButton> mouse_event;
	mouse_event.instance();
	//GetKeyModifierState(mouse_event, modifiers);

	mouse_event->set_button_index(wheel_delta_y < 0 ? 4 : 5);
	mouse_event->set_button_mask(last_button_mask);
	mouse_event->set_position({ last_mouse_position.x,
			last_mouse_position.y });
	mouse_event->set_global_position({ last_mouse_position.x,
			last_mouse_position.y });

	mouse_event->set_pressed(true);
	input->parse_input_event(mouse_event);

	mouse_event->set_pressed(false);
	input->parse_input_event(mouse_event);
}

void HaikuDirectWindow::HandleKeyboardEvent(BMessage *message) {
	int32 raw_char = 0;
	int32 key = 0;
	int32 modifiers = 0;

	if (message->FindInt32("raw_char", &raw_char) != B_OK) {
		return;
	}

	if (message->FindInt32("key", &key) != B_OK) {
		return;
	}

	if (message->FindInt32("modifiers", &modifiers) != B_OK) {
		return;
	}

	Ref<InputEventKey> event;
	event.instance();
	GetKeyModifierState(event, modifiers);
	event->set_pressed(message->what == B_KEY_DOWN);
	event->set_keycode(KeyMappingHaiku::get_keysym(raw_char, key));
	event->set_physical_keycode(KeyMappingHaiku::get_keysym(raw_char, key));
	event->set_echo(message->HasInt32("be:key_repeat"));
	event->set_unicode(0);

	const char *bytes = NULL;
	if (message->FindString("bytes", &bytes) == B_OK) {
		event->set_unicode(BUnicodeChar::FromUTF8(&bytes));
	}

	//make it consistent across platforms.
	if (event->get_keycode() == KEY_BACKTAB) {
		event->set_keycode(KEY_TAB);
		event->set_physical_keycode(KEY_TAB);
		event->set_shift(true);
	}

	input->parse_input_event(event);
}

void HaikuDirectWindow::HandleKeyboardModifierEvent(BMessage *message) {
	int32 old_modifiers = 0;
	int32 modifiers = 0;

	if (message->FindInt32("be:old_modifiers", &old_modifiers) != B_OK) {
		return;
	}

	if (message->FindInt32("modifiers", &modifiers) != B_OK) {
		return;
	}

	int32 key = old_modifiers ^ modifiers;

	Ref<InputEventWithModifiers> event;
	event.instance();
	GetKeyModifierState(event, modifiers);

	event->set_shift(key & B_SHIFT_KEY);
	event->set_alt(key & B_OPTION_KEY);
	event->set_control(key & B_CONTROL_KEY);
	event->set_command(key & B_COMMAND_KEY);

	input->parse_input_event(event);
}

void HaikuDirectWindow::HandleWindowResized(BMessage *message) {
	int32 width = 0;
	int32 height = 0;

	if ((message->FindInt32("width", &width) != B_OK) || (message->FindInt32("height", &height) != B_OK)) {
		return;
	}

	current_video_mode->width = width;
	current_video_mode->height = height;
}

inline void HaikuDirectWindow::GetKeyModifierState(Ref<InputEventWithModifiers> event, uint32 p_state) {
	last_key_modifier_state = p_state;

	event->set_shift(p_state & B_SHIFT_KEY);
	event->set_control(p_state & B_CONTROL_KEY);
	event->set_alt(p_state & B_OPTION_KEY);
	event->set_metakey(p_state & B_COMMAND_KEY);

	return state;
}

inline int HaikuDirectWindow::GetMouseButtonState(uint32 p_state) {
	int state = 0;

	if (p_state & B_PRIMARY_MOUSE_BUTTON) {
		state |= 1 << 0;
	}

	if (p_state & B_SECONDARY_MOUSE_BUTTON) {
		state |= 1 << 1;
	}

	if (p_state & B_TERTIARY_MOUSE_BUTTON) {
		state |= 1 << 2;
	}

	last_button_mask = state;

	return state;
}


/**
 * @class HaikuDirectWindow
 * @brief A Haiku OS direct window implementation for handling graphics rendering and input events.
 * 
 * HaikuDirectWindow extends BDirectWindow to provide a specialized window for the Godot engine
 * on Haiku OS. It manages OpenGL rendering through HaikuGLView and handles various input events
 * including mouse and keyboard input. The window integrates with the engine's main loop and
 * input system.
 * 
 * @details
 * The class tracks mouse position, button states, and modifier keys to provide consistent input
 * handling. It uses a message runner for periodic updates and manages direct buffer access for
 * efficient rendering.
 * 
 * Key responsibilities:
 * - Handle mouse events (movement, buttons, wheel)
 * - Process keyboard input and modifier states
 * - Manage window resize events
 * - Coordinate with the OpenGL view for rendering
 * - Integrate with the main event loop and input system
 * 
 * @note This class is specific to the Haiku OS platform and uses Haiku's native API (Be API).
 */
#ifndef HAIKU_DIRECT_WINDOW_H
#define HAIKU_DIRECT_WINDOW_H

#include <kernel/image.h> // needed for image_id

#include <DirectWindow.h>

#include "core/os/os.h"
#include "main/input_default.h"

#include "haiku_gl_view.h"

#define REDRAW_MSG 'rdrw'
#define LOCKGL_MSG 'glck'
#define UNLOCKGL_MSG 'ulck'

class HaikuDirectWindow : public BDirectWindow {
private:
	Point2i last_mouse_position;
	bool last_mouse_pos_valid;
	uint32 last_buttons_state;
	uint32 last_key_modifier_state;
	int last_button_mask;
	OS::VideoMode *current_video_mode;

	MainLoop *main_loop;
	InputDefault *input;
	HaikuGLView *view;
	BMessageRunner *update_runner;

	void HandleMouseButton(BMessage *message);
	void HandleMouseMoved(BMessage *message);
	void HandleMouseWheelChanged(BMessage *message);
	void HandleWindowResized(BMessage *message);
	void HandleKeyboardEvent(BMessage *message);
	void HandleKeyboardModifierEvent(BMessage *message);
	inline void GetKeyModifierState(Ref<InputEventWithModifiers> event, uint32 p_state);
	inline int GetMouseButtonState(uint32 p_state);

public:
	HaikuDirectWindow(BRect p_frame);
	~HaikuDirectWindow();

	void SetHaikuGLView(HaikuGLView *p_view);
	void StartMessageRunner();
	void StopMessageRunner();
	void SetInput(InputDefault *p_input);
	void SetMainLoop(MainLoop *p_main_loop);
	inline void SetVideoMode(OS::VideoMode *video_mode) { current_video_mode = video_mode; };
	virtual bool QuitRequested();
	virtual void DirectConnected(direct_buffer_info *info);
	virtual void MessageReceived(BMessage *message);
	virtual void DispatchMessage(BMessage *message, BHandler *handler);

	inline Point2i GetLastMousePosition() { return last_mouse_position; };
	inline int GetLastButtonMask() { return last_button_mask; };
};

#endif

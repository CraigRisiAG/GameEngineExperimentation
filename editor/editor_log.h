

/**
 * @class EditorLog
 * @brief A UI panel for displaying and managing editor log messages.
 * 
 * EditorLog is a VBoxContainer-based widget that displays log messages from the editor,
 * including standard output, errors, warnings, and editor-specific messages. It provides
 * functionality to clear and copy log contents, and integrates with the error handling
 * and undo/redo systems.
 * 
 * @details
 * The log display is managed through a RichTextLabel for formatted text output. The UI
 * includes a title bar with control buttons and a tool button for additional functionality.
 * The class handles error callbacks and undo/redo notifications through static callback methods.
 * 
 * Thread-safe message handling is supported through thread ID tracking.
 */

/**
 * @enum MessageType
 * @brief Enumeration of message types that can be logged.
 * 
 * @value MSG_TYPE_STD Standard output message
 * @value MSG_TYPE_ERROR Error message
 * @value MSG_TYPE_WARNING Warning message
 * @value MSG_TYPE_EDITOR Editor-specific message
 */

/**
 * @fn void add_message(const String &p_msg, MessageType p_type)
 * @brief Adds a message to the log with the specified type.
 * 
 * @param p_msg The message text to add
 * @param p_type The type of message (default: MSG_TYPE_STD)
 */

/**
 * @fn void set_tool_button(ToolButton *p_tool_button)
 * @brief Sets the tool button for the log panel.
 * 
 * @param p_tool_button Pointer to the ToolButton to set
 */

/**
 * @fn void deinit()
 * @brief Deinitializes the editor log and cleans up resources.
 */

/**
 * @fn void clear()
 * @brief Clears all messages from the log.
 */

/**
 * @fn void copy()
 * @brief Copies the current log contents to clipboard.
 */
#ifndef EDITOR_LOG_H
#define EDITOR_LOG_H

#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/texture_button.h"
//#include "scene/gui/empty_control.h"
#include "core/os/thread.h"
#include "pane_drag.h"
#include "scene/gui/box_container.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tool_button.h"

class EditorLog : public VBoxContainer {

	GDCLASS(EditorLog, VBoxContainer);

	Button *clearbutton;
	Button *copybutton;
	Label *title;
	RichTextLabel *log;
	HBoxContainer *title_hb;
	//PaneDrag *pd;
	ToolButton *tool_button;

	static void _error_handler(void *p_self, const char *p_func, const char *p_file, int p_line, const char *p_error, const char *p_errorexp, ErrorHandlerType p_type);

	ErrorHandlerList eh;

	Thread::ID current;

	//void _dragged(const Point2& p_ofs);
	void _clear_request();
	void _copy_request();
	static void _undo_redo_cbk(void *p_self, const String &p_name);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	enum MessageType {
		MSG_TYPE_STD,
		MSG_TYPE_ERROR,
		MSG_TYPE_WARNING,
		MSG_TYPE_EDITOR
	};

	void add_message(const String &p_msg, MessageType p_type = MSG_TYPE_STD);
	void set_tool_button(ToolButton *p_tool_button);
	void deinit();

	void clear();
	void copy();
	EditorLog();
	~EditorLog();
};

#endif // EDITOR_LOG_H

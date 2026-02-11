

/**
 * @class InspectorDock
 * @brief Main dock panel for inspecting and editing object properties in the editor.
 * 
 * InspectorDock provides a comprehensive interface for viewing and modifying properties
 * of selected objects in the scene. It includes resource management, history navigation,
 * property searching, and object method invocation capabilities.
 * 
 * Features:
 * - Property inspection and editing via EditorInspector
 * - Resource creation, loading, and saving
 * - Edit history with forward/backward navigation
 * - Property search functionality
 * - Object context menu with helper methods
 * - Animation keyframing support
 * - Resource and parameter copying/pasting
 * - Warning/notification system
 * 
 * @note Inherits from VBoxContainer to organize sub-components vertically.
 * @see EditorInspector, EditorNode, EditorData
 */
#ifndef INSPECTOR_DOCK_H
#define INSPECTOR_DOCK_H

#include "editor/animation_track_editor.h"
#include "editor/connections_dialog.h"
#include "editor/create_dialog.h"
#include "editor/editor_data.h"
#include "editor/editor_inspector.h"
#include "editor/editor_path.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/control.h"
#include "scene/gui/label.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/tool_button.h"

class EditorNode;

class InspectorDock : public VBoxContainer {

	GDCLASS(InspectorDock, VBoxContainer);

	enum MenuOptions {
		RESOURCE_LOAD,
		RESOURCE_SAVE,
		RESOURCE_SAVE_AS,
		RESOURCE_MAKE_BUILT_IN,
		RESOURCE_COPY,
		RESOURCE_EDIT_CLIPBOARD,
		OBJECT_COPY_PARAMS,
		OBJECT_PASTE_PARAMS,
		OBJECT_UNIQUE_RESOURCES,
		OBJECT_REQUEST_HELP,

		COLLAPSE_ALL,
		EXPAND_ALL,

		OBJECT_METHOD_BASE = 500
	};

	EditorNode *editor;
	EditorData *editor_data;

	EditorInspector *inspector;

	Object *current;

	ToolButton *backward_button;
	ToolButton *forward_button;

	EditorFileDialog *load_resource_dialog;
	CreateDialog *new_resource_dialog;
	ToolButton *resource_new_button;
	ToolButton *resource_load_button;
	MenuButton *resource_save_button;
	MenuButton *history_menu;
	LineEdit *search;

	MenuButton *object_menu;
	EditorPath *editor_path;

	Button *warning;
	AcceptDialog *warning_dialog;

	void _menu_option(int p_option);

	void _new_resource();
	void _load_resource(const String &p_type = "");
	void _open_resource_selector() { _load_resource(); }; // just used to call from arg-less signal
	void _resource_file_selected(String p_file);
	void _save_resource(bool save_as) const;
	void _unref_resource() const;
	void _copy_resource() const;
	void _paste_resource() const;

	void _warning_pressed();
	void _resource_created();
	void _resource_selected(const RES &p_res, const String &p_property);
	void _edit_forward();
	void _edit_back();
	void _menu_collapseall();
	void _menu_expandall();
	void _select_history(int p_idx);
	void _prepare_history();

	void _property_keyed(const String &p_keyed, const Variant &p_value, bool p_advance);
	void _transform_keyed(Object *sp, const String &p_sub, const Transform &p_key);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void go_back();
	void update_keying();
	void edit_resource(const Ref<Resource> &p_resource);
	void open_resource(const String &p_type);
	void clear();
	void set_warning(const String &p_message);
	void update(Object *p_object);
	Container *get_addon_area();
	EditorInspector *get_inspector() { return inspector; }

	InspectorDock(EditorNode *p_editor, EditorData &p_editor_data);
	~InspectorDock();
};

#endif

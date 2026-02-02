

/**
 * @class EditorPluginSettings
 * @brief Manages the UI and settings for editor plugins.
 * 
 * EditorPluginSettings is a VBoxContainer-based class that provides a user interface
 * for viewing, creating, and configuring editor plugins. It maintains a list of plugins
 * and handles user interactions such as creating new plugins and editing existing ones.
 * 
 * @details
 * The class manages:
 * - A tree view displaying available plugins
 * - Buttons for creating and updating the plugin list
 * - A plugin configuration dialog for editing plugin settings
 * - Plugin activity state tracking
 * 
 * @note This class integrates with the undo/redo system for reversible operations.
 * 
 * @see PluginConfigDialog
 * @see EditorData
 * @see PropertyEditor
 */
#ifndef EDITORPLUGINSETTINGS_H
#define EDITORPLUGINSETTINGS_H

#include "core/undo_redo.h"
#include "editor/plugin_config_dialog.h"
#include "editor_data.h"
#include "property_editor.h"
#include "scene/gui/dialogs.h"

class EditorPluginSettings : public VBoxContainer {

	GDCLASS(EditorPluginSettings, VBoxContainer);

	enum {
		BUTTON_PLUGIN_EDIT
	};

	PluginConfigDialog *plugin_config_dialog;
	Button *create_plugin;
	Button *update_list;
	Tree *plugin_list;
	bool updating;

	void _plugin_activity_changed();
	void _create_clicked();
	void _cell_button_pressed(Object *p_item, int p_column, int p_id);

protected:
	void _notification(int p_what);

	static void _bind_methods();

public:
	void update_plugins();

	EditorPluginSettings();
};

#endif // EDITORPLUGINSETTINGS_H

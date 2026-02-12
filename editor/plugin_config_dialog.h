

/// @class PluginConfigDialog
/// @brief A dialog for configuring plugin settings and metadata.
/// 
/// PluginConfigDialog extends ConfirmationDialog to provide a user interface
/// for creating and editing plugin configurations. It manages various plugin
/// properties including name, subfolder, description, author, version, script
/// selection, and activation status.
/// 
/// The dialog supports two modes: creation mode (adding new plugins) and edit
/// mode (modifying existing plugins). It validates required fields and notifies
/// the application of configuration changes through confirmation events.
/// 
/// @member name_edit LineEdit for the plugin name
/// @member subfolder_edit LineEdit for the plugin subfolder path
/// @member desc_edit TextEdit for the plugin description
/// @member author_edit LineEdit for the plugin author name
/// @member version_edit LineEdit for the plugin version
/// @member script_option_edit OptionButton for selecting script language/type
/// @member script_edit LineEdit for the script file path
/// @member active_edit CheckBox for enabling/disabling the plugin
/// @member _edit_mode Boolean flag indicating if the dialog is in edit mode
/// 
/// @method config(const String &p_config_path) Initializes the dialog with
///         plugin configuration data from the specified path
/// @method _clear_fields() Resets all input fields to default values
/// @method _on_confirmed() Handles confirmation action and saves configuration
/// @method _on_cancelled() Handles cancellation action
/// @method _on_required_text_changed(const String &p_text) Validates required
///         text fields and updates UI state accordingly
#ifndef PLUGIN_CONFIG_DIALOG_H
#define PLUGIN_CONFIG_DIALOG_H

#include "scene/gui/check_box.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/option_button.h"
#include "scene/gui/text_edit.h"

class PluginConfigDialog : public ConfirmationDialog {

	GDCLASS(PluginConfigDialog, ConfirmationDialog);

	LineEdit *name_edit;
	LineEdit *subfolder_edit;
	TextEdit *desc_edit;
	LineEdit *author_edit;
	LineEdit *version_edit;
	OptionButton *script_option_edit;
	LineEdit *script_edit;
	CheckBox *active_edit;

	bool _edit_mode;

	void _clear_fields();
	void _on_confirmed();
	void _on_cancelled();
	void _on_required_text_changed(const String &p_text);

protected:
	virtual void _notification(int p_what);
	static void _bind_methods();

public:
	void config(const String &p_config_path);

	PluginConfigDialog();
	~PluginConfigDialog();
};

#endif // PLUGIN_CONFIG_DIALOG_H

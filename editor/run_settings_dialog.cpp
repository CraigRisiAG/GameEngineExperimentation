

/**
 * @file run_settings_dialog.cpp
 * @brief Implementation of the RunSettingsDialog class for configuring scene run settings.
 *
 * This dialog provides UI controls for selecting the run mode (Current Scene or Main Scene)
 * and specifying command-line arguments for the main scene execution.
 */

/**
 * @brief Displays the run settings dialog as a centered popup.
 *
 * Opens the dialog with a fixed size of 300x150 pixels, centered on the screen.
 */

/**
 * @brief Sets custom command-line arguments for scene execution.
 *
 * @param p_arguments The argument string to be set in the arguments text field.
 */

/**
 * @brief Gets the current custom command-line arguments.
 *
 * @return String The current arguments text from the arguments field.
 */

/**
 * @brief Binds class methods to the Godot class database.
 *
 * Used for exposing methods to the Godot scripting system. Currently empty
 * with one method commented out.
 */

/**
 * @brief Callback for when the run mode selection changes.
 *
 * Enables or disables the arguments text field based on the selected mode.
 * When "Current Scene" (index 0) is selected, arguments are disabled.
 * When "Main Scene" (index 1) is selected, arguments are enabled.
 *
 * @param idx The index of the newly selected run mode.
 */

/**
 * @brief Gets the currently selected run mode.
 *
 * @return int The index of the selected run mode (0 = Current Scene, 1 = Main Scene).
 */

/**
 * @brief Sets the run mode programmatically.
 *
 * @param p_run_mode The run mode index to select (0 = Current Scene, 1 = Main Scene).
 *                   Also sets the editability of the arguments field accordingly.
 */

/**
 * @brief Constructor for RunSettingsDialog.
 *
 * Initializes the dialog with:
 * - A vertical box container layout
 * - An option button for selecting run mode (Current Scene or Main Scene)
 * - A line edit field for entering main scene arguments (initially disabled)
 * - Sets the dialog title and OK button text
 */
#include "run_settings_dialog.h"

void RunSettingsDialog::popup_run_settings() {

	popup_centered(Size2(300, 150));
}

void RunSettingsDialog::set_custom_arguments(const String &p_arguments) {

	arguments->set_text(p_arguments);
}
String RunSettingsDialog::get_custom_arguments() const {

	return arguments->get_text();
}

void RunSettingsDialog::_bind_methods() {

	//ClassDB::bind_method("_browse_selected_file",&RunSettingsDialog::_browse_selected_file);
}

void RunSettingsDialog::_run_mode_changed(int idx) {

	if (idx == 0)
		arguments->set_editable(false);
	else
		arguments->set_editable(true);
}

int RunSettingsDialog::get_run_mode() const {

	return run_mode->get_selected();
}

void RunSettingsDialog::set_run_mode(int p_run_mode) {

	run_mode->select(p_run_mode);
	arguments->set_editable(p_run_mode);
}

RunSettingsDialog::RunSettingsDialog() {

	/* SNAP DIALOG */

	VBoxContainer *vbc = memnew(VBoxContainer);
	add_child(vbc);
	//set_child_rect(vbc);

	run_mode = memnew(OptionButton);
	vbc->add_margin_child(TTR("Run Mode:"), run_mode);
	run_mode->add_item(TTR("Current Scene"));
	run_mode->add_item(TTR("Main Scene"));
	run_mode->connect("item_selected", callable_mp(this, &RunSettingsDialog::_run_mode_changed));
	arguments = memnew(LineEdit);
	vbc->add_margin_child(TTR("Main Scene Arguments:"), arguments);
	arguments->set_editable(false);

	get_ok()->set_text(TTR("Close"));

	set_title(TTR("Scene Run Settings"));
}

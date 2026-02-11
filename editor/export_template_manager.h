

/// @class ExportTemplateManager
/// @brief Manages the download, installation, and removal of export templates for the game engine.
///
/// This class provides a user interface dialog for managing export templates. It allows users to:
/// - View available and installed export templates
/// - Download templates from remote mirrors
/// - Install templates from local files
/// - Uninstall previously installed templates
/// - Monitor download progress
///
/// @details
/// The manager communicates with remote HTTP servers to fetch available templates and their metadata.
/// It maintains a list of installed templates and handles the installation workflow, including
/// validation and progress tracking.
///
/// @note Inherits from ConfirmationDialog to provide a modal dialog interface.
/// @see ExportTemplateVersion, EditorSettings, HTTPRequest
#ifndef EXPORT_TEMPLATE_MANAGER_H
#define EXPORT_TEMPLATE_MANAGER_H

#include "editor/editor_settings.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/progress_bar.h"
#include "scene/gui/scroll_container.h"
#include "scene/main/http_request.h"

class ExportTemplateVersion;

class ExportTemplateManager : public ConfirmationDialog {
	GDCLASS(ExportTemplateManager, ConfirmationDialog);

	AcceptDialog *template_downloader;
	VBoxContainer *template_list;
	Label *template_list_state;
	ProgressBar *template_download_progress;

	ScrollContainer *installed_scroll;
	VBoxContainer *installed_vb;
	HBoxContainer *current_hb;
	FileDialog *template_open;

	ConfirmationDialog *remove_confirm;
	String to_remove;

	HTTPRequest *request_mirror;
	HTTPRequest *download_templates;

	Vector<uint8_t> download_data;

	float update_countdown;

	void _update_template_list();

	void _download_template(const String &p_version);
	void _uninstall_template(const String &p_version);
	void _uninstall_template_confirm();

	virtual void ok_pressed();
	bool _install_from_file(const String &p_file, bool p_use_progress = true);

	void _http_download_mirror_completed(int p_status, int p_code, const PackedStringArray &headers, const PackedByteArray &p_data);
	void _http_download_templates_completed(int p_status, int p_code, const PackedStringArray &headers, const PackedByteArray &p_data);

	void _begin_template_download(const String &p_url);

	void _window_template_downloader_closed();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	bool can_install_android_template();
	Error install_android_template();

	void popup_manager();

	ExportTemplateManager();
};

#endif // EXPORT_TEMPLATE_MANAGER_H

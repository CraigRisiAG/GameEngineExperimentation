

/// @class EditorRunNative
/// @brief Manages native platform execution and deployment from the editor.
///
/// EditorRunNative is a horizontal box container that provides UI controls for running
/// and debugging games on native platforms. It maintains platform-specific menu buttons
/// and tracks the state of resumed runs for seamless re-execution.
///
/// @member menus Map of platform identifiers to their corresponding menu buttons.
/// @member first Flag indicating if this is the first initialization.
/// @member resume_idx Index of the last resumed run session.
/// @member resume_platform Platform identifier for the resumed run.
///
/// @method _run_native(int p_idx, int p_platform) Executes the game on the specified platform.
/// @method is_deploy_debug_remote_enabled() const Checks if remote debug deployment is enabled.
/// @method resume_run_native() Resumes the last native run session.
/// @method EditorRunNative() Constructs a new EditorRunNative instance.
#ifndef EDITOR_RUN_NATIVE_H
#define EDITOR_RUN_NATIVE_H

#include "scene/gui/box_container.h"
#include "scene/gui/menu_button.h"

class EditorRunNative : public HBoxContainer {

	GDCLASS(EditorRunNative, HBoxContainer);

	Map<int, MenuButton *> menus;
	bool first;

	int resume_idx;
	int resume_platform;

	void _run_native(int p_idx, int p_platform);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	bool is_deploy_debug_remote_enabled() const;

	void resume_run_native();

	EditorRunNative();
};

#endif // EDITOR_RUN_NATIVE_H

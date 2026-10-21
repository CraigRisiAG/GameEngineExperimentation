
#ifndef DEBUGGER_EDITOR_PLUGIN_H
#define DEBUGGER_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"

class EditorNode;
class EditorFileServer;
class MenuButton;
class PopupMenu;

class DebuggerEditorPlugin : public EditorPlugin {

  GDCLASS(DebuggerEditorPlugin, EditorPlugin);

private:
  MenuButton *debug_menu;
  EditorFileServer *file_server;
  PopupMenu *instances_menu;

  enum MenuOptions {
    RUN_FILE_SERVER,
    RUN_LIVE_DEBUG,
    RUN_DEBUG_COLLISONS,
    RUN_DEBUG_NAVIGATION,
    RUN_DEPLOY_REMOTE_DEBUG,
    RUN_RELOAD_SCRIPTS,
  };

  void _update_debug_options();
  void _notification(int p_what);
  void _select_run_count(int p_index);
  void _menu_option(int p_option);

public:
  virtual String get_name() const { return "Debugger"; }
  bool has_main_screen() const { return false; }

  DebuggerEditorPlugin(EditorNode *p_node, MenuButton *p_menu);
  ~DebuggerEditorPlugin();
};

#endif // DEBUGGER_EDITOR_PLUGIN_H

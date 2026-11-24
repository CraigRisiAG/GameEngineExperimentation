// This file declares the GDNativeLibrarySingletonEditor class, which provides a
// user interface for managing GDNative library singletons within the Godot
// editor. It allows users to discover, enable, and disable GDNative singletons,
// and updates the project settings accordingly.
#ifndef GD_NATIVE_LIBRARY_EDITOR_H
#define GD_NATIVE_LIBRARY_EDITOR_H

#ifdef TOOLS_ENABLED
#include "editor/editor_file_system.h"
#include "editor/project_settings_editor.h"

class GDNativeLibrarySingletonEditor : public VBoxContainer {
  GDCLASS(GDNativeLibrarySingletonEditor, VBoxContainer);

private:
  Tree *libraries;
  UndoRedo *undo_redo;

  bool updating;

  static Set<String>
  _find_singletons_recursive(EditorFileSystemDirectory *p_dir);

protected:
  void _notification(int p_what);
  static void _bind_methods();

  void _discover_singletons();
  void _item_edited();
  void _update_libraries();

public:
  GDNativeLibrarySingletonEditor();
};

#endif
#endif // GD_NATIVE_LIBRARY_EDITOR_H

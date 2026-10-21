
#ifndef MESH_LIBRARY_EDITOR_PLUGIN_H
#define MESH_LIBRARY_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "scene/resources/mesh_library.h"

class MeshLibraryEditor : public Control {

  GDCLASS(MeshLibraryEditor, Control);

  Ref<MeshLibrary> mesh_library;

  EditorNode *editor;
  MenuButton *menu;
  ConfirmationDialog *cd;
  EditorFileDialog *file;
  int to_erase;

  enum {

    MENU_OPTION_ADD_ITEM,
    MENU_OPTION_REMOVE_ITEM,
    MENU_OPTION_UPDATE_FROM_SCENE,
    MENU_OPTION_IMPORT_FROM_SCENE
  };

  int option;
  void _import_scene_cbk(const String &p_str);
  void _menu_cbk(int p_option);
  void _menu_confirm();

  static void _import_scene(Node *p_scene, Ref<MeshLibrary> p_library,
                            bool p_merge);

protected:
  static void _bind_methods();

public:
  MenuButton *get_menu_button() const { return menu; }

  void edit(const Ref<MeshLibrary> &p_mesh_library);
  static Error update_library_file(Node *p_base_scene, Ref<MeshLibrary> ml,
                                   bool p_merge = true);

  MeshLibraryEditor(EditorNode *p_editor);
};

class MeshLibraryEditorPlugin : public EditorPlugin {

  GDCLASS(MeshLibraryEditorPlugin, EditorPlugin);

  MeshLibraryEditor *mesh_library_editor;
  EditorNode *editor;

public:
  virtual String get_name() const { return "MeshLibrary"; }
  bool has_main_screen() const { return false; }
  virtual void edit(Object *p_node);
  virtual bool handles(Object *p_node) const;
  virtual void make_visible(bool p_visible);

  MeshLibraryEditorPlugin(EditorNode *p_node);
};

#endif // MESH_LIBRARY_EDITOR_PLUGIN_H

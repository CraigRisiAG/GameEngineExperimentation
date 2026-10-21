
#ifndef THEME_EDITOR_PLUGIN_H
#define THEME_EDITOR_PLUGIN_H

#include "scene/gui/check_box.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/option_button.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/texture_rect.h"
#include "scene/resources/theme.h"

#include "editor/editor_node.h"

class ThemeEditor : public VBoxContainer {

  GDCLASS(ThemeEditor, VBoxContainer);

  Panel *main_panel;
  MarginContainer *main_container;
  Ref<Theme> theme;

  EditorFileDialog *file_dialog;

  double time_left;

  MenuButton *theme_menu;
  ConfirmationDialog *add_del_dialog;
  HBoxContainer *type_hbc;
  MenuButton *type_menu;
  LineEdit *type_edit;
  HBoxContainer *name_hbc;
  MenuButton *name_menu;
  LineEdit *name_edit;
  OptionButton *type_select;
  Label *type_select_label;
  Label *name_select_label;

  enum PopupMode {
    POPUP_ADD,
    POPUP_CLASS_ADD,
    POPUP_REMOVE,
    POPUP_CLASS_REMOVE,
    POPUP_CREATE_EMPTY,
    POPUP_CREATE_EDITOR_EMPTY,
    POPUP_IMPORT_EDITOR_THEME
  };

  int popup_mode;

  Tree *test_tree;

  void _save_template_cbk(String fname);
  void _dialog_cbk();
  void _type_menu_cbk(int p_option);
  void _name_menu_about_to_show();
  void _name_menu_cbk(int p_option);
  void _theme_menu_cbk(int p_option);
  void _propagate_redraw(Control *p_at);
  void _refresh_interval();

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  void edit(const Ref<Theme> &p_theme);

  ThemeEditor();
};

class ThemeEditorPlugin : public EditorPlugin {

  GDCLASS(ThemeEditorPlugin, EditorPlugin);

  ThemeEditor *theme_editor;
  EditorNode *editor;
  Button *button;

public:
  virtual String get_name() const { return "Theme"; }
  bool has_main_screen() const { return false; }
  virtual void edit(Object *p_node);
  virtual bool handles(Object *p_node) const;
  virtual void make_visible(bool p_visible);

  ThemeEditorPlugin(EditorNode *p_node);
};

#endif // THEME_EDITOR_PLUGIN_H

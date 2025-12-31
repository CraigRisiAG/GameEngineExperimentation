
/**
 * @file connections_dialog.h
 * @brief Signal connections management dialog and dock for the editor.
 * @author Juan Linietsky <reduzio@gmail.com>
 *
 * This file provides UI components for managing signal connections in the Godot
 * editor. It includes the ConnectDialog for creating/editing connections and
 * the ConnectionsDock for displaying and managing all connections of a selected
 * node.
 */

/**
 * @class ConnectDialog
 * @brief Dialog for creating and editing signal connections between nodes.
 *
 * Allows users to select a target node, choose a method to connect to,
 * configure connection flags (deferred, oneshot), and bind parameters to the
 * signal.
 */

/**
 * @struct ConnectionData
 * @brief Represents a complete signal connection with all its parameters.
 *
 * @member source The node emitting the signal
 * @member target The node receiving the signal callback
 * @member signal The name of the signal being connected
 * @member method The target method to be called
 * @member flags Connection flags (deferred, oneshot, etc.)
 * @member binds Additional variant parameters bound to the connection
 */

/**
 * @class ConnectionsDockTree
 * @brief Extended Tree widget with rich text label tooltip support for signal
 * documentation.
 *
 * Customizes tooltip rendering to display formatted signal documentation
 * when hovering over connection items.
 */

/**
 * @class ConnectionsDock
 * @brief Dock panel displaying all signal connections for a selected node.
 *
 * Provides interface for viewing, creating, editing, and removing signal
 * connections. Includes context menus and integration with the undo/redo
 * system.
 *
 * @member SignalMenuOption Right-click menu options for signals (CONNECT,
 * DISCONNECT_ALL)
 * @member SlotMenuOption Right-click menu options for individual connections
 * (EDIT, GO_TO_SCRIPT, DISCONNECT)
 */
@author Juan Linietsky<reduzio @gmail.com> * /

#ifndef CONNECTIONS_DIALOG_H
#define CONNECTIONS_DIALOG_H

#include "core/undo_redo.h"
#include "editor/editor_inspector.h"
#include "editor/scene_tree_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/check_button.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/popup.h"
#include "scene/gui/tree.h"

    class PopupMenu;
class ConnectDialogBinds;

class ConnectDialog : public ConfirmationDialog {

  GDCLASS(ConnectDialog, ConfirmationDialog);

public:
  struct ConnectionData {
    Node *source = nullptr;
    Node *target = nullptr;
    StringName signal;
    StringName method;
    uint32_t flags = 0;
    Vector<Variant> binds;

    ConnectionData() {}
    ConnectionData(const Connection &p_connection) {
      source = Object::cast_to<Node>(p_connection.signal.get_object());
      signal = p_connection.signal.get_name();
      target = Object::cast_to<Node>(p_connection.callable.get_object());
      method = p_connection.callable.get_method();
      flags = p_connection.flags;
      binds = p_connection.binds;
    }
    operator Connection() {
      Connection c;
      c.signal = ::Signal(source, signal);
      c.callable = Callable(target, method);
      c.flags = flags;
      c.binds = binds;
      return c;
    }
  };

private:
  Label *connect_to_label;
  LineEdit *from_signal;
  Node *source;
  StringName signal;
  LineEdit *dst_method;
  ConnectDialogBinds *cdbinds;
  bool bEditMode;
  NodePath dst_path;
  VBoxContainer *vbc_right;

  SceneTreeEditor *tree;
  AcceptDialog *error;
  EditorInspector *bind_editor;
  OptionButton *type_list;
  CheckBox *deferred;
  CheckBox *oneshot;
  CheckButton *advanced;

  Label *error_label;

  void ok_pressed();
  void _cancel_pressed();
  void _item_activated();
  void _text_entered(const String &_text);
  void _tree_node_selected();
  void _add_bind();
  void _remove_bind();
  void _advanced_pressed();
  void _update_ok_enabled();

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  Node *get_source() const;
  StringName get_signal_name() const;
  NodePath get_dst_path() const;
  void set_dst_node(Node *p_node);
  StringName get_dst_method_name() const;
  void set_dst_method(const StringName &p_method);
  Vector<Variant> get_binds() const;

  bool get_deferred() const;
  bool get_oneshot() const;
  bool is_editing() const;

  void init(ConnectionData c, bool bEdit = false);

  void popup_dialog(const String &p_for_signal);
  ConnectDialog();
  ~ConnectDialog();
};

//////////////////////////////////////////

// Custom Tree needed to use a RichTextLabel as tooltip control
// when display signal documentation.
class ConnectionsDockTree : public Tree {

  virtual Control *make_custom_tooltip(const String &p_text) const;
};

class ConnectionsDock : public VBoxContainer {

  GDCLASS(ConnectionsDock, VBoxContainer);

  // Right-click Pop-up Menu Options.
  enum SignalMenuOption { CONNECT, DISCONNECT_ALL };

  enum SlotMenuOption { EDIT, GO_TO_SCRIPT, DISCONNECT };

  Node *selectedNode;
  ConnectionsDockTree *tree;
  EditorNode *editor;

  ConfirmationDialog *disconnect_all_dialog;
  ConnectDialog *connect_dialog;
  Button *connect_button;
  PopupMenu *signal_menu;
  PopupMenu *slot_menu;
  UndoRedo *undo_redo;

  Map<StringName, Map<StringName, String>> descr_cache;

  void _make_or_edit_connection();
  void _connect(ConnectDialog::ConnectionData cToMake);
  void _disconnect(TreeItem &item);
  void _disconnect_all();

  void _tree_item_selected();
  void _tree_item_activated();
  bool _is_item_signal(TreeItem &item);

  void _open_connection_dialog(TreeItem &item);
  void _open_connection_dialog(ConnectDialog::ConnectionData cToEdit);
  void _go_to_script(TreeItem &item);

  void _handle_signal_menu_option(int option);
  void _handle_slot_menu_option(int option);
  void _rmb_pressed(Vector2 position);
  void _close();

protected:
  void _connect_pressed();
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_undoredo(UndoRedo *p_undo_redo) { undo_redo = p_undo_redo; }
  void set_node(Node *p_node);
  void update_tree();

  ConnectionsDock(EditorNode *p_editor = NULL);
  ~ConnectionsDock();
};

#endif

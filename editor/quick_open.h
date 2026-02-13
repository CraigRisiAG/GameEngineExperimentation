/// @class EditorQuickOpen
/// @brief A quick file/resource open dialog for the editor.
///
/// Provides a searchable dialog for quickly opening files and resources in the
/// editor. Inherits from ConfirmationDialog and integrates with the editor's
/// file system.
///
/// @note This class uses Godot's class binding system (GDCLASS macro).
///
/// @see ConfirmationDialog, EditorFileSystemDirectory, Tree, LineEdit
/// @fn void _update_search()
/// @brief Updates the search results based on current search box input.

/// @fn void _sbox_input(const Ref<InputEvent> &p_ie)
/// @brief Handles input events in the search box.
/// @param p_ie The input event to process.

/// @fn void _parse_fs(EditorFileSystemDirectory *efsd, Vector<Pair<String,
/// Ref<Texture2D>>> &list)
/// @brief Recursively parses the file system directory and populates the result
/// list.
/// @param efsd The editor file system directory to parse.
/// @param list Output vector to store file paths and their associated textures.

/// @fn Vector<Pair<String, Ref<Texture2D>>> _sort_fs(Vector<Pair<String,
/// Ref<Texture2D>>> &list)
/// @brief Sorts the file system search results.
/// @param list The unsorted file list with textures.
/// @return A sorted vector of file paths and textures.

/// @fn float _path_cmp(String search, String path) const
/// @brief Compares a search string against a file path for ranking.
/// @param search The search query string.
/// @param path The file path to compare.
/// @return A comparison score for ranking search results.

/// @fn void _confirmed()
/// @brief Called when the dialog is confirmed/accepted.

/// @fn void _text_changed(const String &p_newtext)
/// @brief Called when the search box text changes.
/// @param p_newtext The new text in the search box.

/// @fn StringName get_base_type() const
/// @brief Returns the base type filter for the quick open dialog.
/// @return The base type as a StringName.

/// @fn String get_selected() const
/// @brief Returns the currently selected file/resource path.
/// @return The selected item path as a String.

/// @fn Vector<String> get_selected_files() const
/// @brief Returns all selected files/resources when multi-selection is enabled.
/// @return A vector of selected item paths.

/// @fn void popup_dialog(const StringName &p_base, bool p_enable_multi = false,
/// bool p_add_dirs = false, bool p_dontclear = false)
/// @brief Opens the quick open dialog with specified parameters.
/// @param p_base The base type to filter resources.
/// @param p_enable_multi Enable multiple file selection (default: false).
/// @param p_add_dirs Include directories in search results (default: false).
/// @param p_dontclear Preserve previous search state (default: false).

/// @fn EditorQuickOpen()
/// @brief Constructs a new EditorQuickOpen dialog instance.

#ifndef EDITOR_QUICK_OPEN_H
#define EDITOR_QUICK_OPEN_H

#include "core/pair.h"
#include "editor_file_system.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"
class EditorQuickOpen : public ConfirmationDialog {

  GDCLASS(EditorQuickOpen, ConfirmationDialog);

  LineEdit *search_box;
  Tree *search_options;
  StringName base_type;
  StringName ei;
  StringName ot;
  bool add_directories;

  void _update_search();

  void _sbox_input(const Ref<InputEvent> &p_ie);
  void _parse_fs(EditorFileSystemDirectory *efsd,
                 Vector<Pair<String, Ref<Texture2D>>> &list);
  Vector<Pair<String, Ref<Texture2D>>>
  _sort_fs(Vector<Pair<String, Ref<Texture2D>>> &list);
  float _path_cmp(String search, String path) const;

  void _confirmed();
  void _text_changed(const String &p_newtext);

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  StringName get_base_type() const;

  String get_selected() const;
  Vector<String> get_selected_files() const;

  void popup_dialog(const StringName &p_base, bool p_enable_multi = false,
                    bool p_add_dirs = false, bool p_dontclear = false);
  EditorQuickOpen();
};

#endif // EDITOR_QUICK_OPEN_H

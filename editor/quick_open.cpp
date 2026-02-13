

/// @class EditorQuickOpen
/// @brief A dialog for quickly opening files and directories in the editor.
/// 
/// This class provides a dialog interface that allows users to search and select files
/// or directories from the project's file system. It features:
/// - Real-time search filtering with similarity-based ranking
/// - Support for single or multi-selection modes
/// - Directory and file filtering based on class type
/// - Keyboard navigation through search results
/// - Icon display for files and folders
///
/// The dialog emits a "quick_open" signal when a selection is confirmed.

/// @fn void popup_dialog(const StringName &p_base, bool p_enable_multi, bool p_add_dirs, bool p_dontclear)
/// @brief Opens the quick open dialog with specified configuration.
/// @param p_base The base class type to filter files by
/// @param p_enable_multi Whether to allow selecting multiple items
/// @param p_add_dirs Whether to include directories in the results
/// @param p_dontclear Whether to preserve the previous search text

/// @fn String get_selected() const
/// @brief Retrieves the path of the currently selected item.
/// @return The full resource path (res://) of the selected item, or empty string if nothing is selected

/// @fn Vector<String> get_selected_files() const
/// @brief Retrieves all selected file paths in multi-selection mode.
/// @return A vector of resource paths for all selected items

/// @fn StringName get_base_type() const
/// @brief Gets the base class type filter for this dialog.
/// @return The base class type used for filtering files

/// @fn void _update_search()
/// @brief Updates the search results based on current search text.
/// Rescans the file system and repopulates the results tree with sorted matches.

/// @fn Vector<Pair<String, Ref<Texture2D>>> _sort_fs(Vector<Pair<String, Ref<Texture2D>>> &list)
/// @brief Sorts search results by relevance using similarity scoring.
/// @param list The list of files to sort
/// @return The sorted list ranked by match quality

/// @fn float _path_cmp(String search, String path) const
/// @brief Compares search text against a path for relevance scoring.
/// @param search The search query text
/// @param path The file path to match against
/// @return A score value where higher indicates better match
#include "quick_open.h"

#include "core/os/keyboard.h"

void EditorQuickOpen::popup_dialog(const StringName &p_base, bool p_enable_multi, bool p_add_dirs, bool p_dontclear) {

	add_directories = p_add_dirs;
	popup_centered_ratio(0.6);
	if (p_dontclear)
		search_box->select_all();
	else
		search_box->clear();
	if (p_enable_multi)
		search_options->set_select_mode(Tree::SELECT_MULTI);
	else
		search_options->set_select_mode(Tree::SELECT_SINGLE);
	search_box->grab_focus();
	base_type = p_base;
	_update_search();
}

String EditorQuickOpen::get_selected() const {

	TreeItem *ti = search_options->get_selected();
	if (!ti)
		return String();

	return "res://" + ti->get_text(0);
}

Vector<String> EditorQuickOpen::get_selected_files() const {

	Vector<String> files;

	TreeItem *item = search_options->get_next_selected(search_options->get_root());
	while (item) {

		files.push_back("res://" + item->get_text(0));

		item = search_options->get_next_selected(item);
	}

	return files;
}

void EditorQuickOpen::_text_changed(const String &p_newtext) {

	_update_search();
}

void EditorQuickOpen::_sbox_input(const Ref<InputEvent> &p_ie) {

	Ref<InputEventKey> k = p_ie;
	if (k.is_valid()) {

		switch (k->get_keycode()) {
			case KEY_UP:
			case KEY_DOWN:
			case KEY_PAGEUP:
			case KEY_PAGEDOWN: {

				search_options->call("_gui_input", k);
				search_box->accept_event();

				TreeItem *root = search_options->get_root();
				if (!root->get_children())
					break;

				TreeItem *current = search_options->get_selected();

				TreeItem *item = search_options->get_next_selected(root);
				while (item) {
					item->deselect(0);
					item = search_options->get_next_selected(item);
				}

				current->select(0);

			} break;
		}
	}
}

float EditorQuickOpen::_path_cmp(String search, String path) const {

	if (search == path) {
		return 1.2f;
	}
	if (path.findn(search) != -1) {
		return 1.1f;
	}
	return path.to_lower().similarity(search.to_lower());
}

void EditorQuickOpen::_parse_fs(EditorFileSystemDirectory *efsd, Vector<Pair<String, Ref<Texture2D>>> &list) {

	if (!add_directories) {
		for (int i = 0; i < efsd->get_subdir_count(); i++) {

			_parse_fs(efsd->get_subdir(i), list);
		}
	}

	String search_text = search_box->get_text();

	if (add_directories) {
		String path = efsd->get_path();
		if (!path.ends_with("/"))
			path += "/";
		if (path != "res://") {
			path = path.substr(6, path.length());
			if (search_text.is_subsequence_ofi(path)) {
				Pair<String, Ref<Texture2D>> pair;
				pair.first = path;
				pair.second = get_icon("folder", "FileDialog");

				if (search_text != String() && list.size() > 0) {

					float this_sim = _path_cmp(search_text, path);
					float other_sim = _path_cmp(list[0].first, path);
					int pos = 1;

					while (pos < list.size() && this_sim <= other_sim) {
						other_sim = _path_cmp(list[pos++].first, path);
					}

					pos = this_sim >= other_sim ? pos - 1 : pos;
					list.insert(pos, pair);

				} else {
					list.push_back(pair);
				}
			}
		}
	}
	for (int i = 0; i < efsd->get_file_count(); i++) {

		String file = efsd->get_file_path(i);
		file = file.substr(6, file.length());

		if (ClassDB::is_parent_class(efsd->get_file_type(i), base_type) && (search_text.is_subsequence_ofi(file))) {
			Pair<String, Ref<Texture2D>> pair;
			pair.first = file;
			pair.second = get_icon((has_icon(efsd->get_file_type(i), ei) ? efsd->get_file_type(i) : ot), ei);
			list.push_back(pair);
		}
	}

	if (add_directories) {
		for (int i = 0; i < efsd->get_subdir_count(); i++) {

			_parse_fs(efsd->get_subdir(i), list);
		}
	}
}

Vector<Pair<String, Ref<Texture2D>>> EditorQuickOpen::_sort_fs(Vector<Pair<String, Ref<Texture2D>>> &list) {

	String search_text = search_box->get_text();
	Vector<Pair<String, Ref<Texture2D>>> sorted_list;

	if (search_text == String() || list.size() == 0)
		return list;

	Vector<float> scores;
	scores.resize(list.size());
	for (int i = 0; i < list.size(); i++)
		scores.write[i] = _path_cmp(search_text, list[i].first);

	while (list.size() > 0) {

		float best_score = 0.0f;
		int best_idx = 0;

		for (int i = 0; i < list.size(); i++) {
			float current_score = scores[i];
			if (current_score > best_score) {
				best_score = current_score;
				best_idx = i;
			}
		}

		sorted_list.push_back(list[best_idx]);
		list.remove(best_idx);
		scores.remove(best_idx);
	}

	return sorted_list;
}

void EditorQuickOpen::_update_search() {

	search_options->clear();
	TreeItem *root = search_options->create_item();
	EditorFileSystemDirectory *efsd = EditorFileSystem::get_singleton()->get_filesystem();
	Vector<Pair<String, Ref<Texture2D>>> list;

	_parse_fs(efsd, list);
	list = _sort_fs(list);

	for (int i = 0; i < list.size(); i++) {
		TreeItem *ti = search_options->create_item(root);
		ti->set_text(0, list[i].first);
		ti->set_icon(0, list[i].second);
	}

	if (root->get_children()) {
		TreeItem *ti = root->get_children();

		ti->select(0);
		ti->set_as_cursor(0);
	}

	get_ok()->set_disabled(root->get_children() == NULL);
}

void EditorQuickOpen::_confirmed() {

	TreeItem *ti = search_options->get_selected();
	if (!ti)
		return;
	emit_signal("quick_open");
	hide();
}

void EditorQuickOpen::_notification(int p_what) {

	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			connect("confirmed", callable_mp(this, &EditorQuickOpen::_confirmed));

			search_box->set_clear_button_enabled(true);
			[[fallthrough]];
		}
		case NOTIFICATION_THEME_CHANGED: {
			search_box->set_right_icon(get_icon("Search", "EditorIcons"));
		} break;
		case NOTIFICATION_EXIT_TREE: {
			disconnect("confirmed", callable_mp(this, &EditorQuickOpen::_confirmed));
		} break;
	}
}

StringName EditorQuickOpen::get_base_type() const {

	return base_type;
}

void EditorQuickOpen::_bind_methods() {

	ADD_SIGNAL(MethodInfo("quick_open"));
}

EditorQuickOpen::EditorQuickOpen() {

	VBoxContainer *vbc = memnew(VBoxContainer);
	add_child(vbc);
	search_box = memnew(LineEdit);
	vbc->add_margin_child(TTR("Search:"), search_box);
	search_box->connect("text_changed", callable_mp(this, &EditorQuickOpen::_text_changed));
	search_box->connect("gui_input", callable_mp(this, &EditorQuickOpen::_sbox_input));
	search_options = memnew(Tree);
	vbc->add_margin_child(TTR("Matches:"), search_options, true);
	get_ok()->set_text(TTR("Open"));
	get_ok()->set_disabled(true);
	register_text_enter(search_box);
	set_hide_on_ok(false);
	search_options->connect("item_activated", callable_mp(this, &EditorQuickOpen::_confirmed));
	search_options->set_hide_root(true);
	search_options->set_hide_folding(true);
	search_options->add_constant_override("draw_guides", 1);
	ei = "EditorIcons";
	ot = "Object";
	add_directories = false;
}

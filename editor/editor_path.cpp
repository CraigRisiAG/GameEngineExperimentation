

/// @class EditorPath
/// @brief Manages a visual navigation path for editor objects and sub-resources.
///
/// EditorPath provides a dropdown menu interface to navigate through object hierarchies
/// in the editor. It displays the current object and allows users to browse and select
/// sub-resources recursively.
///
/// @method _add_children_to_popup(Object *p_obj, int p_depth)
/// @brief Recursively populates the dropdown menu with child objects.
/// @param p_obj The parent object to extract properties from.
/// @param p_depth The current recursion depth (limited to 8 levels).
/// @details Iterates through editor properties, filters for resource types, and adds
///          valid objects to the popup menu with appropriate indentation based on depth.
///
/// @method _about_to_show()
/// @brief Prepares the popup menu before display.
/// @details Clears previous entries, retrieves the current history object, and populates
///          the menu with available sub-resources. Displays a message if no sub-resources exist.
///
/// @method update_path()
/// @brief Updates the display with the current object's icon and name.
/// @details Traverses the history path, sets the appropriate icon from the current object,
///          and displays formatted text with the object's name or class identifier.
///
/// @method _id_pressed(int p_idx)
/// @brief Handles popup menu item selection.
/// @param p_idx The index of the selected menu item.
/// @details Retrieves the corresponding object and pushes it to the editor for inspection.
///
/// @method _notification(int p_what)
/// @brief Responds to editor notifications.
/// @param p_what The notification type identifier.
///
/// @method _bind_methods()
/// @brief Binds methods to the scripting interface (empty implementation).
///
/// @constructor EditorPath(EditorHistory *p_history)
/// @brief Initializes the EditorPath with a reference to editor history.
/// @param p_history Pointer to the editor history manager for path tracking.
/// @details Sets up signals for popup events and configures text display properties.
#include "editor_path.h"

#include "editor_node.h"
#include "editor_scale.h"

void EditorPath::_add_children_to_popup(Object *p_obj, int p_depth) {

	if (p_depth > 8)
		return;

	List<PropertyInfo> pinfo;
	p_obj->get_property_list(&pinfo);
	for (List<PropertyInfo>::Element *E = pinfo.front(); E; E = E->next()) {

		if (!(E->get().usage & PROPERTY_USAGE_EDITOR))
			continue;
		if (E->get().hint != PROPERTY_HINT_RESOURCE_TYPE)
			continue;

		Variant value = p_obj->get(E->get().name);
		if (value.get_type() != Variant::OBJECT)
			continue;
		Object *obj = value;
		if (!obj)
			continue;

		Ref<Texture2D> icon = EditorNode::get_singleton()->get_object_icon(obj);

		int index = get_popup()->get_item_count();
		get_popup()->add_icon_item(icon, E->get().name.capitalize(), objects.size());
		get_popup()->set_item_h_offset(index, p_depth * 10 * EDSCALE);
		objects.push_back(obj->get_instance_id());

		_add_children_to_popup(obj, p_depth + 1);
	}
}

void EditorPath::_about_to_show() {

	Object *obj = ObjectDB::get_instance(history->get_path_object(history->get_path_size() - 1));
	if (!obj)
		return;

	objects.clear();
	get_popup()->clear();
	get_popup()->set_size(Size2(get_size().width, 1));

	_add_children_to_popup(obj);
	if (get_popup()->get_item_count() == 0) {
		get_popup()->add_item(TTR("No sub-resources found."));
		get_popup()->set_item_disabled(0, true);
	}
}

void EditorPath::update_path() {

	for (int i = 0; i < history->get_path_size(); i++) {

		Object *obj = ObjectDB::get_instance(history->get_path_object(i));
		if (!obj)
			continue;

		Ref<Texture2D> icon = EditorNode::get_singleton()->get_object_icon(obj);
		if (icon.is_valid())
			set_icon(icon);

		if (i == history->get_path_size() - 1) {
			String name;
			if (Object::cast_to<Resource>(obj)) {

				Resource *r = Object::cast_to<Resource>(obj);
				if (r->get_path().is_resource_file())
					name = r->get_path().get_file();
				else
					name = r->get_name();

				if (name == "")
					name = r->get_class();
			} else if (obj->is_class("EditorDebuggerRemoteObject"))
				name = obj->call("get_title");
			else if (Object::cast_to<Node>(obj))
				name = Object::cast_to<Node>(obj)->get_name();
			else if (Object::cast_to<Resource>(obj) && Object::cast_to<Resource>(obj)->get_name() != "")
				name = Object::cast_to<Resource>(obj)->get_name();
			else
				name = obj->get_class();

			set_text(" " + name); // An extra space so the text is not too close of the icon.
			set_tooltip(obj->get_class());
		}
	}
}

void EditorPath::_id_pressed(int p_idx) {

	ERR_FAIL_INDEX(p_idx, objects.size());

	Object *obj = ObjectDB::get_instance(objects[p_idx]);
	if (!obj)
		return;

	EditorNode::get_singleton()->push_item(obj);
}

void EditorPath::_notification(int p_what) {

	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED: {
			update_path();
		} break;
	}
}

void EditorPath::_bind_methods() {
}

EditorPath::EditorPath(EditorHistory *p_history) {

	history = p_history;
	set_clip_text(true);
	set_text_align(ALIGN_LEFT);
	get_popup()->connect("about_to_show", callable_mp(this, &EditorPath::_about_to_show));
	get_popup()->connect("id_pressed", callable_mp(this, &EditorPath::_id_pressed));
}



/// @class EditorDebuggerRemoteObject
/// @brief Represents a remote object being inspected in the debugger.
/// 
/// This class provides a wrapper around remote objects that are being debugged,
/// allowing property inspection and editing. It maintains a list of properties
/// and their current values synchronized with the remote debugger state.
/// 
/// @signal value_edited(int object_id, StringName property, Variant value)
///         Emitted when a property value is edited on this remote object.

/// @fn bool EditorDebuggerRemoteObject::_set(const StringName &p_name, const Variant &p_value)
/// @brief Sets a property value on the remote object.
/// @param p_name The name of the property to set.
/// @param p_value The new value for the property.
/// @return true if the property was successfully set, false otherwise.
/// @note Properties starting with "Constants/" cannot be edited.

/// @fn bool EditorDebuggerRemoteObject::_get(const StringName &p_name, Variant &r_ret) const
/// @brief Retrieves a property value from the remote object.
/// @param p_name The name of the property to retrieve.
/// @param r_ret Output parameter containing the property value.
/// @return true if the property exists and was retrieved, false otherwise.

/// @fn void EditorDebuggerRemoteObject::_get_property_list(List<PropertyInfo> *p_list) const
/// @brief Populates the property list for this remote object.
/// @param p_list Output list to be populated with PropertyInfo structures.

/// @fn String EditorDebuggerRemoteObject::get_title()
/// @brief Returns a human-readable title for this remote object.
/// @return A string containing the remote object's type name and ID, or "<null>" if invalid.

/// @fn Variant EditorDebuggerRemoteObject::get_variant(const StringName &p_name)
/// @brief Convenience method to get a property variant.
/// @param p_name The property name to retrieve.
/// @return The variant value of the property.

/// @class EditorDebuggerInspector
/// @brief Inspector panel for debugging remote objects in the editor.
/// 
/// Manages the inspection and editing of remote objects during debugging sessions.
/// Tracks multiple remote objects, handles property updates, and manages stack variables.
/// 
/// @signal object_selected(int id)
///         Emitted when a remote object is selected for inspection.
/// @signal object_edited(int id, StringName property, Variant value)
///         Emitted when a property of a remote object is edited.
/// @signal object_property_updated(int id, StringName property)
///         Emitted when a property of a remote object is updated by the debugger.

/// @fn EditorDebuggerInspector::EditorDebuggerInspector()
/// @brief Constructs the debugger inspector and initializes the variables object.

/// @fn EditorDebuggerInspector::~EditorDebuggerInspector()
/// @brief Destructs the debugger inspector and cleans up all cached remote objects.

/// @fn void EditorDebuggerInspector::_notification(int p_what)
/// @brief Handles editor notifications for initialization and tree entry.
/// @param p_what The notification type.

/// @fn void EditorDebuggerInspector::_object_edited(ObjectID p_id, const String &p_prop, const Variant &p_value)
/// @brief Internal handler for object property edits. Forwards to signal.
/// @param p_id The ID of the edited object.
/// @param p_prop The property name that was edited.
/// @param p_value The new value of the property.

/// @fn void EditorDebuggerInspector::_object_selected(ObjectID p_object)
/// @brief Internal handler for object selection. Forwards to signal.
/// @param p_object The ID of the selected object.

/// @fn ObjectID EditorDebuggerInspector::add_object(const Array &p_arr)
/// @brief Adds or updates a remote object in the inspector.
/// @param p_arr A serialized array containing the object's data from the debugger.
/// @return The ObjectID of the added/updated remote object.
/// @details Handles resource loading for object properties and script attachment.

/// @fn void EditorDebuggerInspector::clear_cache()
/// @brief Clears all cached remote objects and removes any current selection.

/// @fn Object *EditorDebuggerInspector::get_object(ObjectID p_id)
/// @brief Retrieves a cached remote object by ID.
/// @param p_id The ObjectID to look up.
/// @return Pointer to the remote object, or NULL if not found.

/// @fn void EditorDebuggerInspector::add_stack_variable(const Array &p_array)
/// @brief Adds a stack variable to the variables inspector panel.
/// @param p_array A serialized array containing the variable data.
/// @details Variables are categorized as Locals, Members, or Globals.

/// @fn void EditorDebuggerInspector::clear_stack_variables()
/// @brief Clears all stack variables from the variables inspector panel.

/// @fn String EditorDebuggerInspector::get_stack_variable(const String &p_var)
/// @brief Retrieves the value of a stack variable by name.
/// @param p_var The full name of the variable (including category prefix).
/// @return The string representation of the variable's value.
#include "editor_debugger_inspector.h"

#include "core/debugger/debugger_marshalls.h"
#include "core/io/marshalls.h"
#include "editor/editor_node.h"
#include "scene/debugger/scene_debugger.h"

bool EditorDebuggerRemoteObject::_set(const StringName &p_name, const Variant &p_value) {

	if (!editable || !prop_values.has(p_name) || String(p_name).begins_with("Constants/"))
		return false;

	prop_values[p_name] = p_value;
	emit_signal("value_edited", remote_object_id, p_name, p_value);
	return true;
}

bool EditorDebuggerRemoteObject::_get(const StringName &p_name, Variant &r_ret) const {

	if (!prop_values.has(p_name))
		return false;

	r_ret = prop_values[p_name];
	return true;
}

void EditorDebuggerRemoteObject::_get_property_list(List<PropertyInfo> *p_list) const {

	p_list->clear(); //sorry, no want category
	for (const List<PropertyInfo>::Element *E = prop_list.front(); E; E = E->next()) {
		p_list->push_back(E->get());
	}
}

String EditorDebuggerRemoteObject::get_title() {
	if (remote_object_id.is_valid())
		return TTR("Remote ") + String(type_name) + ": " + itos(remote_object_id);
	else
		return "<null>";
}

Variant EditorDebuggerRemoteObject::get_variant(const StringName &p_name) {
	Variant var;
	_get(p_name, var);
	return var;
}

void EditorDebuggerRemoteObject::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_title"), &EditorDebuggerRemoteObject::get_title);
	ClassDB::bind_method(D_METHOD("get_variant"), &EditorDebuggerRemoteObject::get_variant);
	ClassDB::bind_method(D_METHOD("clear"), &EditorDebuggerRemoteObject::clear);
	ClassDB::bind_method(D_METHOD("get_remote_object_id"), &EditorDebuggerRemoteObject::get_remote_object_id);

	ADD_SIGNAL(MethodInfo("value_edited", PropertyInfo(Variant::INT, "object_id"), PropertyInfo(Variant::STRING, "property"), PropertyInfo("value")));
}

EditorDebuggerInspector::EditorDebuggerInspector() {
	variables = memnew(EditorDebuggerRemoteObject);
	variables->editable = false;
}

EditorDebuggerInspector::~EditorDebuggerInspector() {
	clear_cache();
	memdelete(variables);
}

void EditorDebuggerInspector::_bind_methods() {
	ADD_SIGNAL(MethodInfo("object_selected", PropertyInfo(Variant::INT, "id")));
	ADD_SIGNAL(MethodInfo("object_edited", PropertyInfo(Variant::INT, "id"), PropertyInfo(Variant::STRING, "property"), PropertyInfo("value")));
	ADD_SIGNAL(MethodInfo("object_property_updated", PropertyInfo(Variant::INT, "id"), PropertyInfo(Variant::STRING, "property")));
}

void EditorDebuggerInspector::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE:
			connect("object_id_selected", callable_mp(this, &EditorDebuggerInspector::_object_selected));
			break;
		case NOTIFICATION_ENTER_TREE:
			edit(variables);
			break;
		default:
			break;
	}
}

void EditorDebuggerInspector::_object_edited(ObjectID p_id, const String &p_prop, const Variant &p_value) {

	emit_signal("object_edited", p_id, p_prop, p_value);
}

void EditorDebuggerInspector::_object_selected(ObjectID p_object) {

	emit_signal("object_selected", p_object);
}

ObjectID EditorDebuggerInspector::add_object(const Array &p_arr) {
	EditorDebuggerRemoteObject *debugObj = NULL;

	SceneDebuggerObject obj;
	obj.deserialize(p_arr);
	ERR_FAIL_COND_V(obj.id.is_null(), ObjectID());

	if (remote_objects.has(obj.id)) {
		debugObj = remote_objects[obj.id];
	} else {
		debugObj = memnew(EditorDebuggerRemoteObject);
		debugObj->remote_object_id = obj.id;
		debugObj->type_name = obj.class_name;
		remote_objects[obj.id] = debugObj;
		debugObj->connect("value_edited", callable_mp(this, &EditorDebuggerInspector::_object_edited));
	}

	int old_prop_size = debugObj->prop_list.size();

	debugObj->prop_list.clear();
	int new_props_added = 0;
	Set<String> changed;
	for (int i = 0; i < obj.properties.size(); i++) {

		PropertyInfo &pinfo = obj.properties[i].first;
		Variant &var = obj.properties[i].second;

		if (pinfo.type == Variant::OBJECT) {
			if (var.get_type() == Variant::STRING) {
				String path = var;
				if (path.find("::") != -1) {
					// built-in resource
					String base_path = path.get_slice("::", 0);
					if (ResourceLoader::get_resource_type(base_path) == "PackedScene") {
						if (!EditorNode::get_singleton()->is_scene_open(base_path)) {
							EditorNode::get_singleton()->load_scene(base_path);
						}
					} else {
						EditorNode::get_singleton()->load_resource(base_path);
					}
				}
				var = ResourceLoader::load(path);

				if (pinfo.hint_string == "Script") {
					if (debugObj->get_script() != var) {
						debugObj->set_script(REF());
						Ref<Script> script(var);
						if (!script.is_null()) {
							ScriptInstance *script_instance = script->placeholder_instance_create(debugObj);
							debugObj->set_script_and_instance(var, script_instance);
						}
					}
				}
			}
		}

		//always add the property, since props may have been added or removed
		debugObj->prop_list.push_back(pinfo);

		if (!debugObj->prop_values.has(pinfo.name)) {
			new_props_added++;
			debugObj->prop_values[pinfo.name] = var;
		} else {

			if (bool(Variant::evaluate(Variant::OP_NOT_EQUAL, debugObj->prop_values[pinfo.name], var))) {
				debugObj->prop_values[pinfo.name] = var;
				changed.insert(pinfo.name);
			}
		}
	}

	if (old_prop_size == debugObj->prop_list.size() && new_props_added == 0) {
		//only some may have changed, if so, then update those, if exist
		for (Set<String>::Element *E = changed.front(); E; E = E->next()) {
			emit_signal("object_property_updated", debugObj->remote_object_id, E->get());
		}
	} else {
		//full update, because props were added or removed
		debugObj->update();
	}
	return obj.id;
}

void EditorDebuggerInspector::clear_cache() {
	for (Map<ObjectID, EditorDebuggerRemoteObject *>::Element *E = remote_objects.front(); E; E = E->next()) {
		EditorNode *editor = EditorNode::get_singleton();
		if (editor->get_editor_history()->get_current() == E->value()->get_instance_id()) {
			editor->push_item(NULL);
		}
		memdelete(E->value());
	}
	remote_objects.clear();
}

Object *EditorDebuggerInspector::get_object(ObjectID p_id) {
	if (remote_objects.has(p_id))
		return remote_objects[p_id];
	return NULL;
}

void EditorDebuggerInspector::add_stack_variable(const Array &p_array) {

	DebuggerMarshalls::ScriptStackVariable var;
	var.deserialize(p_array);
	String n = var.name;
	Variant v = var.value;

	PropertyHint h = PROPERTY_HINT_NONE;
	String hs = String();

	if (v.get_type() == Variant::OBJECT) {
		v = Object::cast_to<EncodedObjectAsID>(v)->get_object_id();
		h = PROPERTY_HINT_OBJECT_ID;
		hs = "Object";
	}
	String type;
	switch (var.type) {
		case 0:
			type = "Locals/";
			break;
		case 1:
			type = "Members/";
			break;
		case 2:
			type = "Globals/";
			break;
		default:
			type = "Unknown/";
	}

	PropertyInfo pinfo;
	pinfo.name = type + n;
	pinfo.type = v.get_type();
	pinfo.hint = h;
	pinfo.hint_string = hs;

	variables->prop_list.push_back(pinfo);
	variables->prop_values[type + n] = v;
	variables->update();
	edit(variables);
}

void EditorDebuggerInspector::clear_stack_variables() {
	variables->clear();
	variables->update();
}

String EditorDebuggerInspector::get_stack_variable(const String &p_var) {
	return variables->get_variant(p_var);
}

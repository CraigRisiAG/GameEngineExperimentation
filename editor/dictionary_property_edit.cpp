


/// @file dictionary_property_edit.cpp
/// @brief Implementation of DictionaryPropertyEdit class for editing dictionary properties in the editor.
///
/// This file provides functionality to edit dictionary properties in the Godot engine editor.
/// It allows users to view, modify, and manage key-value pairs within a dictionary property
/// of an object through the property inspector interface.
///
/// @class DictionaryPropertyEdit
/// @brief Handles editing of dictionary properties in the editor UI.
///
/// Provides methods to get, set, and notify changes to dictionary entries.
/// Supports undo/redo functionality for dictionary modifications.

/// @brief Notifies of a change without parameters.
/// Triggers the change notification system.
void DictionaryPropertyEdit::_notif_change();

/// @brief Notifies of a change with a string parameter.
/// @param p_v The string value associated with the change notification.
void DictionaryPropertyEdit::_notif_changev(const String &p_v);

/// @brief Attempts to change a dictionary key.
/// @param p_old_key The current key to be changed.
/// @param p_new_key The new key value.
/// @note Currently not implemented; dictionary key modification is not supported.
void DictionaryPropertyEdit::_set_key(const Variant &p_old_key, const Variant &p_new_key);

/// @brief Sets a value in the dictionary for the given key.
/// @param p_key The dictionary key.
/// @param p_value The new value to set.
void DictionaryPropertyEdit::_set_value(const Variant &p_key, const Variant &p_value);

/// @brief Retrieves the dictionary from the associated object.
/// @return The dictionary variant, or an empty dictionary if retrieval fails.
Variant DictionaryPropertyEdit::get_dictionary() const;

/// @brief Populates the property list with dictionary keys and values.
/// @param p_list Pointer to the property list to be filled.
void DictionaryPropertyEdit::_get_property_list(List<PropertyInfo> *p_list) const;

/// @brief Initializes the editor for a specific object property.
/// @param p_obj The object containing the dictionary property.
/// @param p_prop The name of the property to edit.
void DictionaryPropertyEdit::edit(Object *p_obj, const StringName &p_prop);

/// @brief Retrieves the node associated with the edited object.
/// @return Pointer to the node, or NULL if conversion fails.
Node *DictionaryPropertyEdit::get_node();

/// @brief Determines if undo/redo operations should be disabled.
/// @return Always returns true; undo/redo is disabled for this editor.
bool DictionaryPropertyEdit::_dont_undo_redo();

/// @brief Binds methods to the class for script exposure.
void DictionaryPropertyEdit::_bind_methods();

/// @brief Sets a property value on the dictionary.
/// @param p_name The property name (format: "index: key" or "index: value").
/// @param p_value The value to set.
/// @return True if the property was successfully set, false otherwise.
bool DictionaryPropertyEdit::_set(const StringName &p_name, const Variant &p_value);

/// @brief Gets a property value from the dictionary.
/// @param p_name The property name (format: "index: key" or "index: value").
/// @param r_ret Output parameter containing the retrieved value.
/// @return True if the property was successfully retrieved, false otherwise.
bool DictionaryPropertyEdit::_get(const StringName &p_name, Variant &r_ret) const;

/// @brief Default constructor.
DictionaryPropertyEdit::DictionaryPropertyEdit();
#include "dictionary_property_edit.h"
#include "editor_node.h"

void DictionaryPropertyEdit::_notif_change() {
	_change_notify();
}

void DictionaryPropertyEdit::_notif_changev(const String &p_v) {
	_change_notify(p_v.utf8().get_data());
}

void DictionaryPropertyEdit::_set_key(const Variant &p_old_key, const Variant &p_new_key) {

	// TODO: Set key of a dictionary is not allowed yet
}

void DictionaryPropertyEdit::_set_value(const Variant &p_key, const Variant &p_value) {

	Dictionary dict = get_dictionary();
	dict[p_key] = p_value;
	Object *o = ObjectDB::get_instance(obj);
	if (!o)
		return;

	o->set(property, dict);
}

Variant DictionaryPropertyEdit::get_dictionary() const {

	Object *o = ObjectDB::get_instance(obj);
	if (!o)
		return Dictionary();
	Variant dict = o->get(property);
	if (dict.get_type() != Variant::DICTIONARY)
		return Dictionary();
	return dict;
}

void DictionaryPropertyEdit::_get_property_list(List<PropertyInfo> *p_list) const {

	Dictionary dict = get_dictionary();

	Array keys = dict.keys();
	keys.sort();

	for (int i = 0; i < keys.size(); i++) {
		String index = itos(i);

		const Variant &key = keys[i];
		PropertyInfo pi(key.get_type(), index + ": key");
		p_list->push_back(pi);

		const Variant &value = dict[key];
		pi = PropertyInfo(value.get_type(), index + ": value");
		p_list->push_back(pi);
	}
}

void DictionaryPropertyEdit::edit(Object *p_obj, const StringName &p_prop) {

	property = p_prop;
	obj = p_obj->get_instance_id();
}

Node *DictionaryPropertyEdit::get_node() {

	Object *o = ObjectDB::get_instance(obj);
	if (!o)
		return NULL;

	return cast_to<Node>(o);
}

bool DictionaryPropertyEdit::_dont_undo_redo() {
	return true;
}

void DictionaryPropertyEdit::_bind_methods() {

	ClassDB::bind_method(D_METHOD("_set_key"), &DictionaryPropertyEdit::_set_key);
	ClassDB::bind_method(D_METHOD("_set_value"), &DictionaryPropertyEdit::_set_value);
	ClassDB::bind_method(D_METHOD("_notif_change"), &DictionaryPropertyEdit::_notif_change);
	ClassDB::bind_method(D_METHOD("_notif_changev"), &DictionaryPropertyEdit::_notif_changev);
	ClassDB::bind_method(D_METHOD("_dont_undo_redo"), &DictionaryPropertyEdit::_dont_undo_redo);
}

bool DictionaryPropertyEdit::_set(const StringName &p_name, const Variant &p_value) {

	Dictionary dict = get_dictionary();
	Array keys = dict.keys();
	keys.sort();

	String pn = p_name;
	int slash = pn.find(": ");
	if (slash != -1 && pn.length() > slash) {
		String type = pn.substr(slash + 2, pn.length());
		int index = pn.substr(0, slash).to_int();
		if (type == "key" && index < keys.size()) {

			const Variant &key = keys[index];
			UndoRedo *ur = EditorNode::get_undo_redo();

			ur->create_action(TTR("Change Dictionary Key"));
			ur->add_do_method(this, "_set_key", key, p_value);
			ur->add_undo_method(this, "_set_key", p_value, key);
			ur->add_do_method(this, "_notif_changev", p_name);
			ur->add_undo_method(this, "_notif_changev", p_name);
			ur->commit_action();

			return true;
		} else if (type == "value" && index < keys.size()) {
			const Variant &key = keys[index];
			if (dict.has(key)) {

				Variant value = dict[key];
				UndoRedo *ur = EditorNode::get_undo_redo();

				ur->create_action(TTR("Change Dictionary Value"));
				ur->add_do_method(this, "_set_value", key, p_value);
				ur->add_undo_method(this, "_set_value", key, value);
				ur->add_do_method(this, "_notif_changev", p_name);
				ur->add_undo_method(this, "_notif_changev", p_name);
				ur->commit_action();

				return true;
			}
		}
	}

	return false;
}

bool DictionaryPropertyEdit::_get(const StringName &p_name, Variant &r_ret) const {

	Dictionary dict = get_dictionary();
	Array keys = dict.keys();
	keys.sort();

	String pn = p_name;
	int slash = pn.find(": ");

	if (slash != -1 && pn.length() > slash) {

		String type = pn.substr(slash + 2, pn.length());
		int index = pn.substr(0, slash).to_int();

		if (type == "key" && index < keys.size()) {
			r_ret = keys[index];
			return true;
		} else if (type == "value" && index < keys.size()) {
			const Variant &key = keys[index];
			if (dict.has(key)) {
				r_ret = dict[key];
				return true;
			}
		}
	}

	return false;
}

DictionaryPropertyEdit::DictionaryPropertyEdit() {
}

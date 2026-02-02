

/**
 * @file editor_properties_array_dict.h
 * @brief Editor property classes for managing array and dictionary types in the inspector.
 * 
 * This header defines editor property classes that provide UI components for editing
 * array and dictionary variant types in the Godot editor inspector panel.
 */

/**
 * @class EditorPropertyArrayObject
 * @brief Reference object wrapper for array variant data.
 * 
 * Provides property access to array elements through the _set and _get methods,
 * allowing individual array elements to be edited as properties in the inspector.
 */

/**
 * @class EditorPropertyDictionaryObject
 * @brief Reference object wrapper for dictionary variant data.
 * 
 * Wraps dictionary data and manages new key-value pair state. Provides property
 * access to dictionary entries and supports adding new items to the dictionary.
 */

/**
 * @class EditorPropertyArray
 * @brief Editor property widget for editing array variants.
 * 
 * Extends EditorProperty to provide a complete UI for editing arrays, including:
 * - Array element type management and conversion
 * - Pagination for large arrays
 * - Dynamic array length adjustment
 * - Type change functionality with popup menu
 * 
 * @property array_type The base type of the array elements
 * @property subtype Optional sub-type for array elements
 * @property page_len Number of elements shown per page
 * @property page_idx Current page index
 */

/**
 * @class EditorPropertyDictionary
 * @brief Editor property widget for editing dictionary variants.
 * 
 * Extends EditorProperty to provide a UI for editing dictionaries, including:
 * - Key-value pair management
 * - Type conversion for keys and values
 * - Pagination for large dictionaries
 * - Dynamic dictionary size adjustment
 * - Addition and removal of key-value pairs
 * 
 * @property page_len Number of entries shown per page
 * @property page_idx Current page index
 */
#ifndef EDITOR_PROPERTIES_ARRAY_DICT_H
#define EDITOR_PROPERTIES_ARRAY_DICT_H

#include "editor/editor_inspector.h"
#include "editor/editor_spin_slider.h"
#include "scene/gui/button.h"

class EditorPropertyArrayObject : public Reference {

	GDCLASS(EditorPropertyArrayObject, Reference);

	Variant array;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;

public:
	void set_array(const Variant &p_array);
	Variant get_array();

	EditorPropertyArrayObject();
};

class EditorPropertyDictionaryObject : public Reference {

	GDCLASS(EditorPropertyDictionaryObject, Reference);

	Variant new_item_key;
	Variant new_item_value;
	Dictionary dict;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;

public:
	void set_dict(const Dictionary &p_dict);
	Dictionary get_dict();

	void set_new_item_key(const Variant &p_new_item);
	Variant get_new_item_key();

	void set_new_item_value(const Variant &p_new_item);
	Variant get_new_item_value();

	EditorPropertyDictionaryObject();
};

class EditorPropertyArray : public EditorProperty {
	GDCLASS(EditorPropertyArray, EditorProperty);

	PopupMenu *change_type;
	bool updating;

	Ref<EditorPropertyArrayObject> object;
	int page_len;
	int page_idx;
	int changing_type_idx;
	Button *edit;
	VBoxContainer *vbox;
	EditorSpinSlider *length;
	EditorSpinSlider *page;
	HBoxContainer *page_hb;
	Variant::Type array_type;
	Variant::Type subtype;
	PropertyHint subtype_hint;
	String subtype_hint_string;

	void _page_changed(double p_page);
	void _length_changed(double p_page);
	void _edit_pressed();
	void _property_changed(const String &p_property, Variant p_value, const String &p_name = "", bool p_changing = false);
	void _change_type(Object *p_button, int p_index);
	void _change_type_menu(int p_index);

	void _object_id_selected(const StringName &p_property, ObjectID p_id);
	void _remove_pressed(int p_index);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void setup(Variant::Type p_array_type, const String &p_hint_string = "");
	virtual void update_property();
	EditorPropertyArray();
};

class EditorPropertyDictionary : public EditorProperty {
	GDCLASS(EditorPropertyDictionary, EditorProperty);

	PopupMenu *change_type;
	bool updating;

	Ref<EditorPropertyDictionaryObject> object;
	int page_len;
	int page_idx;
	int changing_type_idx;
	Button *edit;
	VBoxContainer *vbox;
	EditorSpinSlider *length;
	EditorSpinSlider *page;
	HBoxContainer *page_hb;

	void _page_changed(double p_page);
	void _edit_pressed();
	void _property_changed(const String &p_property, Variant p_value, const String &p_name = "", bool p_changing = false);
	void _change_type(Object *p_button, int p_index);
	void _change_type_menu(int p_index);

	void _add_key_value();
	void _object_id_selected(const StringName &p_property, ObjectID p_id);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual void update_property();
	EditorPropertyDictionary();
};

#endif // EDITOR_PROPERTIES_ARRAY_DICT_H

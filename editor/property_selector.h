

/// @class PropertySelector
/// @brief A dialog for selecting properties or methods from various sources.
///
/// PropertySelector is a ConfirmationDialog that provides a searchable interface
/// for selecting properties or methods. It supports selection from base types,
/// scripts, basic types, and object instances. The dialog includes a search box
/// with real-time filtering and help information for selected items.
///
/// @details
/// The selector can operate in two modes: property selection or method selection.
/// It filters options based on user input in the search box and displays results
/// in a tree view. A help panel provides additional information about selected items.
///
/// Supported selection sources:
/// - Base types (by class name)
/// - Script objects
/// - Basic Variant types
/// - Object instances
///
/// Type filtering can be applied to restrict results to specific Variant types.
///
/// @signals
/// - property_selected(String) - Emitted when a property is selected
/// - method_selected(String) - Emitted when a method is selected
///
/// @see PropertyEditor, EditorHelpBit
#ifndef PROPERTYSELECTOR_H
#define PROPERTYSELECTOR_H

#include "editor/property_editor.h"
#include "editor_help.h"
#include "scene/gui/rich_text_label.h"

class PropertySelector : public ConfirmationDialog {
	GDCLASS(PropertySelector, ConfirmationDialog);

	LineEdit *search_box;
	Tree *search_options;

	void _text_changed(const String &p_newtext);
	void _sbox_input(const Ref<InputEvent> &p_ie);
	void _update_search();
	void _confirmed();
	void _item_selected();
	void _hide_requested();

	EditorHelpBit *help_bit;

	bool properties;
	String selected;
	Variant::Type type;
	String base_type;
	ObjectID script;
	Object *instance;
	bool virtuals_only;

	Vector<Variant::Type> type_filter;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void select_method_from_base_type(const String &p_base, const String &p_current = "", bool p_virtuals_only = false);
	void select_method_from_script(const Ref<Script> &p_script, const String &p_current = "");
	void select_method_from_basic_type(Variant::Type p_type, const String &p_current = "");
	void select_method_from_instance(Object *p_instance, const String &p_current = "");

	void select_property_from_base_type(const String &p_base, const String &p_current = "");
	void select_property_from_script(const Ref<Script> &p_script, const String &p_current = "");
	void select_property_from_basic_type(Variant::Type p_type, const String &p_current = "");
	void select_property_from_instance(Object *p_instance, const String &p_current = "");

	void set_type_filter(const Vector<Variant::Type> &p_type_filter);

	PropertySelector();
};

#endif // PROPERTYSELECTOR_H

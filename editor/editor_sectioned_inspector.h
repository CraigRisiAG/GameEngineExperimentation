

/// @class SectionedInspector
/// @brief A sectioned inspector UI component that organizes object properties into categorized sections.
///
/// SectionedInspector extends HSplitContainer to provide a split-view interface with a tree-based
/// section navigator on the left and a detailed inspector panel on the right. It allows users to
/// browse and edit object properties organized by category, with support for searching and filtering.
///
/// @member obj The ObjectID of the currently inspected object.
/// @member sections Pointer to the Tree widget displaying available property sections.
/// @member filter Pointer to the SectionedInspectorFilter for filtering properties.
/// @member section_map Map of section names to their corresponding TreeItem pointers.
/// @member inspector Pointer to the EditorInspector displaying detailed properties.
/// @member search_box Pointer to the LineEdit widget for search functionality.
/// @member selected_category The currently selected property category/section.
///
/// @method register_search_box(LineEdit *p_box) Registers an external search box for filtering properties.
/// @method get_inspector() Returns a pointer to the underlying EditorInspector.
/// @method edit(Object *p_object) Sets the object to be inspected and updates the UI.
/// @method get_full_item_path(const String &p_item) Retrieves the full hierarchical path of a property item.
/// @method set_current_section(const String &p_section) Changes the currently displayed section.
/// @method get_current_section() const Returns the name of the currently selected section.
/// @method update_category_list() Refreshes the list of available property categories.
#ifndef EDITOR_SECTIONED_INSPECTOR_H
#define EDITOR_SECTIONED_INSPECTOR_H

#include "editor/editor_inspector.h"
#include "scene/gui/split_container.h"
#include "scene/gui/tree.h"

class SectionedInspectorFilter;

class SectionedInspector : public HSplitContainer {

	GDCLASS(SectionedInspector, HSplitContainer);

	ObjectID obj;

	Tree *sections;
	SectionedInspectorFilter *filter;

	Map<String, TreeItem *> section_map;
	EditorInspector *inspector;
	LineEdit *search_box;

	String selected_category;

	static void _bind_methods();
	void _section_selected();

	void _search_changed(const String &p_what);

public:
	void register_search_box(LineEdit *p_box);
	EditorInspector *get_inspector();
	void edit(Object *p_object);
	String get_full_item_path(const String &p_item);

	void set_current_section(const String &p_section);
	String get_current_section() const;

	void update_category_list();

	SectionedInspector();
	~SectionedInspector();
};
#endif // EDITOR_SECTIONED_INSPECTOR_H

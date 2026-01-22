

/// @class ArrayPropertyEdit
/// @brief A property editor for array-type properties in the Godot editor.
///
/// This class provides functionality to edit array properties of objects
/// through the Godot editor's property inspector. It handles pagination, type
/// checking, and value modification for array elements.
///
/// @details
/// The editor manages:
/// - Array page navigation for large arrays
/// - Type validation and hints for array elements
/// - Undo/redo support for array modifications
/// - Dynamic property generation based on array size

/// @var page
/// @brief Current page number for pagination of large arrays.

/// @var obj
/// @brief Object ID of the object being edited.

/// @var property
/// @brief Name of the array property being edited.

/// @var vtypes
/// @brief String representation of valid value types for array elements.

/// @var subtype_hint_string
/// @brief Additional hint string for array element type validation.

/// @var subtype_hint
/// @brief Type hint for array elements.

/// @var subtype
/// @brief The Variant type of array elements.

/// @var default_type
/// @brief Default type for new array elements.

/// @fn void edit(Object *p_obj, const StringName &p_prop, const String
/// &p_hint_string, Variant::Type p_deftype)
/// @brief Initialize the editor for a specific array property.
/// @param p_obj The object containing the array property.
/// @param p_prop The name of the array property.
/// @param p_hint_string Type hint string for validation.
/// @param p_deftype Default type for array elements.

/// @fn Node *get_node()
/// @brief Retrieve the node associated with the edited object.
/// @return Pointer to the Node, or null if not applicable.
#ifndef ARRAY_PROPERTY_EDIT_H
#define ARRAY_PROPERTY_EDIT_H

#include "scene/main/node.h"

class ArrayPropertyEdit : public Reference {

  GDCLASS(ArrayPropertyEdit, Reference);

  int page;
  ObjectID obj;
  StringName property;
  String vtypes;
  String subtype_hint_string;
  PropertyHint subtype_hint;
  Variant::Type subtype;
  Variant get_array() const;
  Variant::Type default_type;

  void _notif_change();
  void _notif_changev(const String &p_v);
  void _set_size(int p_size);
  void _set_value(int p_idx, const Variant &p_value);

  bool _dont_undo_redo();

protected:
  static void _bind_methods();
  bool _set(const StringName &p_name, const Variant &p_value);
  bool _get(const StringName &p_name, Variant &r_ret) const;
  void _get_property_list(List<PropertyInfo> *p_list) const;

public:
  void edit(Object *p_obj, const StringName &p_prop,
            const String &p_hint_string, Variant::Type p_deftype);

  Node *get_node();

  ArrayPropertyEdit();
};

#endif // ARRAY_PROPERTY_EDIT_H

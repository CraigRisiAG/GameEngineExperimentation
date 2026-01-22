

#ifndef EDITOR_DEBUGGER_INSPECTOR_H
#define EDITOR_DEBUGGER_INSPECTOR_H
#include "editor/editor_inspector.h"

class EditorDebuggerRemoteObject : public Object {

  GDCLASS(EditorDebuggerRemoteObject, Object);

protected:
  bool _set(const StringName &p_name, const Variant &p_value);
  bool _get(const StringName &p_name, Variant &r_ret) const;
  void _get_property_list(List<PropertyInfo> *p_list) const;
  static void _bind_methods();

public:
  bool editable = false;
  ObjectID remote_object_id;
  String type_name;
  List<PropertyInfo> prop_list;
  Map<StringName, Variant> prop_values;

  ObjectID get_remote_object_id() { return remote_object_id; };
  String get_title();

  Variant get_variant(const StringName &p_name);

  void clear() {
    prop_list.clear();
    prop_values.clear();
  }

  void update() { _change_notify(); }

  EditorDebuggerRemoteObject() {};
};

class EditorDebuggerInspector : public EditorInspector {

  GDCLASS(EditorDebuggerInspector, EditorInspector);

private:
  ObjectID inspected_object_id;
  Map<ObjectID, EditorDebuggerRemoteObject *> remote_objects;
  EditorDebuggerRemoteObject *variables;

  void _object_selected(ObjectID p_object);
  void _object_edited(ObjectID p_id, const String &p_prop,
                      const Variant &p_value);

protected:
  void _notification(int p_what);
  static void _bind_methods();

public:
  EditorDebuggerInspector();
  ~EditorDebuggerInspector();

  // Remote Object cache
  ObjectID add_object(const Array &p_arr);
  Object *get_object(ObjectID p_id);
  void clear_cache();

  // Stack Dump variables
  String get_stack_variable(const String &p_var);
  void add_stack_variable(const Array &p_arr);
  void clear_stack_variables();
};

#endif // EDITOR_DEBUGGER_INSPECTOR_H

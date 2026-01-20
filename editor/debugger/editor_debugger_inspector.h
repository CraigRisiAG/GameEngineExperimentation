


/// @class EditorDebuggerRemoteObject
/// @brief Represents a remote object from a debugger session.
/// 
/// This class wraps remote objects that are inspected during debugging,
/// providing property access and synchronization with the remote debugger.
/// It stores property metadata and values for display in the inspector.
class EditorDebuggerRemoteObject : public Object {

/// @class EditorDebuggerInspector
/// @brief Inspector panel for examining remote objects during debugging.
/// 
/// This class extends EditorInspector to provide debugging capabilities,
/// managing remote object inspection and stack variable display.
/// It maintains a cache of remote objects and handles variable updates
/// from the debugger backend.
/// 
/// @see EditorInspector
/// @see EditorDebuggerRemoteObject
class EditorDebuggerInspector : public EditorInspector {
	
	/// @brief Handles selection of a remote object for inspection.
	/// @param p_object The ObjectID of the selected remote object.
	void _object_selected(ObjectID p_object);
	
	/// @brief Called when a property of an inspected object is edited.
	/// @param p_id The ObjectID of the edited object.
	/// @param p_prop The property name that was edited.
	/// @param p_value The new property value.
	void _object_edited(ObjectID p_id, const String &p_prop, const Variant &p_value);
	
	/// @brief Adds a remote object to the cache.
	/// @param p_arr Array containing remote object data.
	/// @return The ObjectID of the added remote object.
	ObjectID add_object(const Array &p_arr);
	
	/// @brief Retrieves a cached remote object.
	/// @param p_id The ObjectID of the object to retrieve.
	/// @return Pointer to the remote object, or nullptr if not found.
	Object *get_object(ObjectID p_id);
	
	/// @brief Clears the remote object cache.
	void clear_cache();
	
	/// @brief Retrieves a variable from the current stack frame.
	/// @param p_var The variable name.
	/// @return The variable value as a string.
	String get_stack_variable(const String &p_var);
	
	/// @brief Adds a variable to the stack variable list.
	/// @param p_arr Array containing stack variable data.
	void add_stack_variable(const Array &p_arr);
	
	/// @brief Clears all stack variables.
	void clear_stack_variables();
};

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

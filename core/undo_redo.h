

/**
 * @class UndoRedo
 * @brief Manages undo/redo functionality for objects and resources.
 * 
 * The UndoRedo class provides a complete undo/redo system that tracks operations
 * performed on objects and allows them to be reversed or reapplied. It supports
 * method calls, property changes, and resource references.
 * 
 * @section Usage
 * Create an action with create_action(), add operations (methods, properties, references),
 * then commit_action() to finalize. Use undo() and redo() to navigate history.
 * 
 * @section MergeMode
 * - MERGE_DISABLE: No merging of consecutive actions
 * - MERGE_ENDS: Merge actions with same name at history ends
 * - MERGE_ALL: Merge all actions with same name
 */

/**
 * @enum MergeMode
 * @brief Determines how consecutive undo/redo actions are merged.
 * @var MERGE_DISABLE Do not merge actions
 * @var MERGE_ENDS Merge consecutive actions with identical names
 * @var MERGE_ALL Merge all actions sharing the same name
 */

/**
 * @struct Operation
 * @brief Represents a single operation within an action (method, property, or reference).
 * @var type The type of operation (method call, property change, or resource reference)
 * @var resref Reference to a resource if applicable
 * @var object Object ID that the operation targets
 * @var name Name of the method or property
 * @var args Arguments for the operation
 */

/**
 * @struct Action
 * @brief Represents a complete action containing do and undo operations.
 * @var name Human-readable name of the action
 * @var do_ops List of operations to perform (redo)
 * @var undo_ops List of operations to undo
 * @var last_tick Timestamp of the action for merge mode operations
 */
#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include "core/object.h"
#include "core/resource.h"

class UndoRedo : public Object {

	GDCLASS(UndoRedo, Object);
	OBJ_SAVE_TYPE(UndoRedo);

public:
	enum MergeMode {
		MERGE_DISABLE,
		MERGE_ENDS,
		MERGE_ALL
	};

	typedef void (*CommitNotifyCallback)(void *p_ud, const String &p_name);
	Variant _add_do_method(const Variant **p_args, int p_argcount, Callable::CallError &r_error);
	Variant _add_undo_method(const Variant **p_args, int p_argcount, Callable::CallError &r_error);

	typedef void (*MethodNotifyCallback)(void *p_ud, Object *p_base, const StringName &p_name, VARIANT_ARG_DECLARE);
	typedef void (*PropertyNotifyCallback)(void *p_ud, Object *p_base, const StringName &p_property, const Variant &p_value);

private:
	struct Operation {

		enum Type {
			TYPE_METHOD,
			TYPE_PROPERTY,
			TYPE_REFERENCE
		};

		Type type;
		Ref<Resource> resref;
		ObjectID object;
		StringName name;
		Variant args[VARIANT_ARG_MAX];
	};

	struct Action {
		String name;
		List<Operation> do_ops;
		List<Operation> undo_ops;
		uint64_t last_tick;
	};

	Vector<Action> actions;
	int current_action;
	int action_level;
	MergeMode merge_mode;
	bool merging;
	uint64_t version;

	void _pop_history_tail();
	void _process_operation_list(List<Operation>::Element *E);
	void _discard_redo();

	CommitNotifyCallback callback;
	void *callback_ud;
	void *method_callbck_ud;
	void *prop_callback_ud;

	MethodNotifyCallback method_callback;
	PropertyNotifyCallback property_callback;

	int committing;

protected:
	static void _bind_methods();

public:
	void create_action(const String &p_name = "", MergeMode p_mode = MERGE_DISABLE);

	void add_do_method(Object *p_object, const StringName &p_method, VARIANT_ARG_LIST);
	void add_undo_method(Object *p_object, const StringName &p_method, VARIANT_ARG_LIST);
	void add_do_property(Object *p_object, const StringName &p_property, const Variant &p_value);
	void add_undo_property(Object *p_object, const StringName &p_property, const Variant &p_value);
	void add_do_reference(Object *p_object);
	void add_undo_reference(Object *p_object);

	bool is_committing_action() const;
	void commit_action();

	bool redo();
	bool undo();
	String get_current_action_name() const;
	void clear_history(bool p_increase_version = true);

	bool has_undo();
	bool has_redo();

	uint64_t get_version() const;

	void set_commit_notify_callback(CommitNotifyCallback p_callback, void *p_ud);

	void set_method_notify_callback(MethodNotifyCallback p_method_callback, void *p_ud);
	void set_property_notify_callback(PropertyNotifyCallback p_property_callback, void *p_ud);

	UndoRedo();
	~UndoRedo();
};

VARIANT_ENUM_CAST(UndoRedo::MergeMode);

#endif // UNDO_REDO_H

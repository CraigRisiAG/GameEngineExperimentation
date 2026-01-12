

/// @class FuncRef
/// @brief A reference wrapper for calling methods on objects by name.
///
/// FuncRef provides a way to store and invoke a method on a specific object instance
/// without holding a direct reference to the object. It uses the object's instance ID
/// to retrieve the object at call time, allowing for safe deferred method invocation.
///
/// @method call_func(...)
/// @brief Calls the referenced function with variable arguments.
/// @param p_args Pointer to array of Variant arguments
/// @param p_argcount Number of arguments
/// @param r_error Reference to CallError struct for error reporting
/// @return Variant result from the function call
/// @note Returns null Variant if instance is invalid
///
/// @method call_funcv(const Array &p_args)
/// @brief Calls the referenced function with arguments provided as an Array.
/// @param p_args Array containing the function arguments
/// @return Variant result from the function call
/// @note Asserts that the instance ID is valid before attempting the call
///
/// @method set_instance(Object *p_obj)
/// @brief Sets the target object instance for this function reference.
/// @param p_obj Pointer to the object instance (must not be null)
/// @note Stores the instance ID internally, not a direct object reference
///
/// @method set_function(const StringName &p_func)
/// @brief Sets the name of the method to call on the target object.
/// @param p_func The name of the method as a StringName
///
/// @method is_valid() const
/// @brief Checks if this function reference is valid and callable.
/// @return true if the instance exists and has the specified method, false otherwise
#include "func_ref.h"

Variant FuncRef::call_func(const Variant **p_args, int p_argcount, Callable::CallError &r_error) {

	if (id.is_null()) {
		r_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return Variant();
	}
	Object *obj = ObjectDB::get_instance(id);

	if (!obj) {
		r_error.error = Callable::CallError::CALL_ERROR_INSTANCE_IS_NULL;
		return Variant();
	}

	return obj->call(function, p_args, p_argcount, r_error);
}

Variant FuncRef::call_funcv(const Array &p_args) {

	ERR_FAIL_COND_V(id.is_null(), Variant());

	Object *obj = ObjectDB::get_instance(id);

	ERR_FAIL_COND_V(!obj, Variant());

	return obj->callv(function, p_args);
}

void FuncRef::set_instance(Object *p_obj) {

	ERR_FAIL_NULL(p_obj);
	id = p_obj->get_instance_id();
}

void FuncRef::set_function(const StringName &p_func) {

	function = p_func;
}

bool FuncRef::is_valid() const {
	if (id.is_null())
		return false;

	Object *obj = ObjectDB::get_instance(id);
	if (!obj)
		return false;

	return obj->has_method(function);
}

void FuncRef::_bind_methods() {

	{
		MethodInfo mi;
		mi.name = "call_func";
		Vector<Variant> defargs;
		ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "call_func", &FuncRef::call_func, mi, defargs);
	}

	ClassDB::bind_method(D_METHOD("call_funcv", "arg_array"), &FuncRef::call_funcv);

	ClassDB::bind_method(D_METHOD("set_instance", "instance"), &FuncRef::set_instance);
	ClassDB::bind_method(D_METHOD("set_function", "name"), &FuncRef::set_function);
	ClassDB::bind_method(D_METHOD("is_valid"), &FuncRef::is_valid);
}

FuncRef::FuncRef() {
}




/**
 * @class FuncRef
 * @brief A reference wrapper for calling functions on objects.
 * 
 * FuncRef is a Reference-derived class that allows storing a reference to a specific
 * function on an object and calling it later through the stored reference.
 * 
 * @details
 * This class maintains an ObjectID to track the target object and a StringName
 * to identify the function to be called. It provides multiple ways to invoke
 * the stored function with different argument passing mechanisms.
 */

/**
 * @brief Invokes the referenced function with the provided arguments.
 * 
 * @param p_args Array of Variant pointers representing the function arguments.
 * @param p_argcount The number of arguments in p_args.
 * @param r_error Output parameter that receives any CallError that occurs during invocation.
 * @return Variant The return value of the called function.
 */

/**
 * @brief Invokes the referenced function with arguments provided as an Array.
 * 
 * @param p_args An Array of Variants to pass as function arguments.
 * @return Variant The return value of the called function.
 */

/**
 * @brief Sets the object instance whose function will be called.
 * 
 * @param p_obj Pointer to the Object that contains the function to be called.
 */

/**
 * @brief Sets the name of the function to be called on the instance.
 * 
 * @param p_func The name of the function as a StringName.
 */

/**
 * @brief Checks whether this FuncRef refers to a valid, callable function.
 * 
 * @return bool True if the referenced object and function are valid; false otherwise.
 */
#ifndef FUNC_REF_H
#define FUNC_REF_H

#include "core/reference.h"

class FuncRef : public Reference {

	GDCLASS(FuncRef, Reference);
	ObjectID id;
	StringName function;

protected:
	static void _bind_methods();

public:
	Variant call_func(const Variant **p_args, int p_argcount, Callable::CallError &r_error);
	Variant call_funcv(const Array &p_args);
	void set_instance(Object *p_obj);
	void set_function(const StringName &p_func);
	bool is_valid() const;
	FuncRef();
};

#endif // FUNC_REF_H

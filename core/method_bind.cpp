


/**
 * @file method_bind.cpp
 * @brief Implementation of MethodBind class for method introspection and binding.
 * 
 * This file provides the core functionality for binding C++ methods to the engine's
 * reflection system. It manages method metadata, argument information, and method properties.
 * 
 * @note object.h must be included before method_bind.h to avoid potential circular dependencies.
 * 
 * @class MethodBind
 * @brief Binds C++ methods for introspection and dynamic invocation.
 * 
 * Provides functionality to:
 * - Store and retrieve method metadata (name, argument info, return type)
 * - Manage method properties (const-ness, return value presence)
 * - Handle default arguments for method parameters
 * - Generate and maintain argument type information in DEBUG mode
 * 
 * @method get_argument_info(int p_argument)
 * @brief Retrieves PropertyInfo for a specific argument.
 * @param p_argument Zero-based index of the argument.
 * @return PropertyInfo containing type and name information for the argument.
 * @note Only available in DEBUG_METHODS_ENABLED mode.
 * 
 * @method get_return_info()
 * @brief Retrieves PropertyInfo for the method's return type.
 * @return PropertyInfo describing the return type.
 * @note Only available in DEBUG_METHODS_ENABLED mode.
 * 
 * @method _set_const(bool p_const)
 * @brief Marks the method as const or non-const.
 * @param p_const True if the method is const-qualified.
 * 
 * @method _set_returns(bool p_returns)
 * @brief Specifies whether the method returns a value.
 * @param p_returns True if the method has a return value.
 * 
 * @method get_name() / set_name()
 * @brief Gets or sets the method's name.
 * @return The method's StringName identifier.
 * 
 * @method set_argument_names() / get_argument_names()
 * @brief Manages argument name metadata.
 * @note Only available in DEBUG_METHODS_ENABLED mode.
 * 
 * @method set_default_arguments()
 * @brief Sets default argument values for method parameters.
 * @param p_defargs Vector of Variant values representing default arguments.
 * 
 * @method _generate_argument_types(int p_count)
 * @brief Generates and caches argument type information.
 * @param p_count The number of arguments to generate type info for.
 * @note Only available in DEBUG_METHODS_ENABLED mode.
 * 
 * @constructor MethodBind()
 * @brief Initializes a MethodBind instance with default values.
 * 
 * @destructor ~MethodBind()
 * @brief Cleans up allocated argument type information.
 */
// object.h needs to be the first include *before* method_bind.h
// FIXME: Find out why and fix potential cyclical dependencies.
#include "core/object.h"

#include "method_bind.h"

#ifdef DEBUG_METHODS_ENABLED
PropertyInfo MethodBind::get_argument_info(int p_argument) const {

	ERR_FAIL_INDEX_V(p_argument, get_argument_count(), PropertyInfo());

	PropertyInfo info = _gen_argument_type_info(p_argument);
	info.name = p_argument < arg_names.size() ? String(arg_names[p_argument]) : String("arg" + itos(p_argument));
	return info;
}

PropertyInfo MethodBind::get_return_info() const {

	return _gen_argument_type_info(-1);
}

#endif
void MethodBind::_set_const(bool p_const) {

	_const = p_const;
}

void MethodBind::_set_returns(bool p_returns) {

	_returns = p_returns;
}

StringName MethodBind::get_name() const {
	return name;
}
void MethodBind::set_name(const StringName &p_name) {
	name = p_name;
}

#ifdef DEBUG_METHODS_ENABLED
void MethodBind::set_argument_names(const Vector<StringName> &p_names) {

	arg_names = p_names;
}
Vector<StringName> MethodBind::get_argument_names() const {

	return arg_names;
}

#endif

void MethodBind::set_default_arguments(const Vector<Variant> &p_defargs) {
	default_arguments = p_defargs;
	default_argument_count = default_arguments.size();
}

#ifdef DEBUG_METHODS_ENABLED
void MethodBind::_generate_argument_types(int p_count) {

	set_argument_count(p_count);

	Variant::Type *argt = memnew_arr(Variant::Type, p_count + 1);
	argt[0] = _gen_argument_type(-1); // return type

	for (int i = 0; i < p_count; i++) {
		argt[i + 1] = _gen_argument_type(i);
	}

	argument_types = argt;
}

#endif

MethodBind::MethodBind() {
	static int last_id = 0;
	method_id = last_id++;
	hint_flags = METHOD_FLAGS_DEFAULT;
	argument_count = 0;
	default_argument_count = 0;
#ifdef DEBUG_METHODS_ENABLED
	argument_types = NULL;
#endif
	_const = false;
	_returns = false;
}

MethodBind::~MethodBind() {
#ifdef DEBUG_METHODS_ENABLED
	if (argument_types)
		memdelete_arr(argument_types);
#endif
}

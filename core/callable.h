

/**
 * @file callable.h
 * @brief Abstraction layer for callable objects and signals
 * 
 * This file provides the core infrastructure for handling callable objects,
 * custom callables, and signals in the game engine. It enables efficient
 * invocation of methods and custom callable objects, supporting both standard
 * object-method pairs and customized callable implementations.
 */

/**
 * @class Callable
 * @brief Abstract representation of objects that can be called
 * 
 * Provides a unified interface for invoking methods on objects and custom
 * callables. Designed to optimize the standard case of object-method pairs
 * while supporting custom callable implementations. The class maintains a
 * maximum size of 16 bytes on 64-bit systems for efficiency.
 * 
 * @note Supports three states: null, custom, and standard callables
 */

/**
 * @struct Callable::CallError
 * @brief Error information for callable invocations
 * 
 * Contains error codes, problematic argument index, and expected values
 * to provide detailed feedback on failed method calls.
 */

/**
 * @class CallableCustom
 * @brief Base class for custom callable implementations
 * 
 * Provides the interface for creating custom callable objects with
 * reference counting. Derived classes must implement comparison,
 * hashing, string representation, and invocation logic.
 * 
 * @note Thread-safe reference counting via SafeRefCount
 */

/**
 * @class Signal
 * @brief Proxy object for object signal handling
 * 
 * Represents a named signal on an object, allocated on-demand by
 * scripting languages for storage in Variant types. Manages signal
 * emission, connection, and disconnection of callables.
 * 
 * @note Not used directly by the engine; primarily for script binding
 */
#ifndef CALLABLE_H
#define CALLABLE_H

#include "core/list.h"
#include "core/object_id.h"
#include "core/string_name.h"

class Object;
class Variant;
class CallableCustom;

// This is an abstraction of things that can be called.
// It is used for signals and other cases where efficient calling of functions
// is required. It is designed for the standard case (object and method)
// but can be optimized or customized.

class Callable {

	//needs to be max 16 bytes in 64 bits
	StringName method;
	union {
		uint64_t object;
		CallableCustom *custom;
	};

public:
	struct CallError {
		enum Error {
			CALL_OK,
			CALL_ERROR_INVALID_METHOD,
			CALL_ERROR_INVALID_ARGUMENT, // expected is variant type
			CALL_ERROR_TOO_MANY_ARGUMENTS, // expected is number of arguments
			CALL_ERROR_TOO_FEW_ARGUMENTS, // expected is number of arguments
			CALL_ERROR_INSTANCE_IS_NULL,
		};
		Error error;
		int argument;
		int expected;
	};

	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, CallError &r_call_error) const;
	void call_deferred(const Variant **p_arguments, int p_argcount) const;

	_FORCE_INLINE_ bool is_null() const {
		return method == StringName() && object == 0;
	}
	_FORCE_INLINE_ bool is_custom() const {
		return method == StringName() && custom != 0;
	}
	_FORCE_INLINE_ bool is_standard() const {
		return method != StringName();
	}

	Object *get_object() const;
	ObjectID get_object_id() const;
	StringName get_method() const;
	CallableCustom *get_custom() const;

	uint32_t hash() const;

	bool operator==(const Callable &p_callable) const;
	bool operator!=(const Callable &p_callable) const;
	bool operator<(const Callable &p_callable) const;

	void operator=(const Callable &p_callable);

	operator String() const;

	Callable(const Object *p_object, const StringName &p_method);
	Callable(ObjectID p_object, const StringName &p_method);
	Callable(CallableCustom *p_custom);
	Callable(const Callable &p_callable);
	Callable();
	~Callable();
};

class CallableCustom {
	friend class Callable;
	SafeRefCount ref_count;
	bool referenced;

public:
	typedef bool (*CompareEqualFunc)(const CallableCustom *p_a, const CallableCustom *p_b);
	typedef bool (*CompareLessFunc)(const CallableCustom *p_a, const CallableCustom *p_b);

	//for every type that inherits, these must always be the same for this type
	virtual uint32_t hash() const = 0;
	virtual String get_as_text() const = 0;
	virtual CompareEqualFunc get_compare_equal_func() const = 0;
	virtual CompareLessFunc get_compare_less_func() const = 0;
	virtual ObjectID get_object() const = 0; //must always be able to provide an object
	virtual void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const = 0;

	CallableCustom();
	virtual ~CallableCustom() {}
};

// This is just a proxy object to object signals, its only
// allocated on demand by/for scripting languages so it can
// be put inside a Variant, but it is not
// used by the engine itself.

class Signal {
	StringName name;
	ObjectID object;

public:
	_FORCE_INLINE_ bool is_null() const {
		return object.is_null() && name == StringName();
	}
	Object *get_object() const;
	ObjectID get_object_id() const;
	StringName get_name() const;

	bool operator==(const Signal &p_signal) const;
	bool operator!=(const Signal &p_signal) const;
	bool operator<(const Signal &p_signal) const;

	operator String() const;

	Error emit(const Variant **p_arguments, int p_argcount) const;
	Error connect(const Callable &p_callable, const Vector<Variant> &p_binds = Vector<Variant>(), uint32_t p_flags = 0);
	void disconnect(const Callable &p_callable);
	bool is_connected(const Callable &p_callable) const;

	Array get_connections() const;
	Signal(const Object *p_object, const StringName &p_name);
	Signal(ObjectID p_object, const StringName &p_name);
	Signal();
};

#endif // CALLABLE_H

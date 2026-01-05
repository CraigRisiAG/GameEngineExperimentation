
/**
 * @file jni_utils.h
 * @brief JNI utility functions and classes for interfacing with Java from C++
 * 
 * This header provides utilities for converting between Godot Variant types and JNI types,
 * as well as a singleton wrapper for calling Java methods from C++.
 */

/**
 * @struct jvalret
 * @brief Structure to hold the result of a Variant to JValue conversion
 * 
 * @member obj jobject - Java object reference (NULL if conversion didn't produce an object)
 * @member val jvalue - The converted JNI value
 */

/**
 * @brief Converts a Godot Variant to a JNI jvalue
 * @param env JNI environment pointer
 * @param p_type The Variant type to convert from
 * @param p_arg Pointer to the Variant argument to convert
 * @param force_jobject If true, forces conversion to a jobject
 * @return jvalret structure containing the converted value and any created object reference
 */

/**
 * @brief Retrieves the class name from a JNI class reference
 * @param env JNI environment pointer
 * @param cls The JNI class to get the name from
 * @param array Output parameter indicating if the class is an array type
 * @return String containing the class name
 */

/**
 * @brief Converts a Java object to a Godot Variant
 * @param env JNI environment pointer
 * @param obj The Java object to convert
 * @return Variant containing the converted value
 */

/**
 * @brief Determines the Godot Variant type from a JNI type string
 * @param p_type String representation of the JNI type
 * @return The corresponding Variant::Type
 */

/**
 * @brief Gets the JNI type signature string for a given type
 * @param p_type String representation of the type
 * @return C-string containing the JNI type signature
 */

/**
 * @class JNISingleton
 * @brief Wrapper class for calling Java methods from C++ code
 * 
 * Manages a reference to a Java object instance and provides functionality to call
 * its methods with type conversion between Godot Variants and JNI types.
 * 
 * @method call Invokes a Java method by name with Variant arguments and returns a Variant result
 * @method get_instance Returns the wrapped Java object instance
 * @method set_instance Sets the Java object instance to wrap
 * @method add_method Registers a Java method with its signature and return type
 */
#ifndef JNI_UTILS_H
#define JNI_UTILS_H

#include "string_android.h"
#include <core/engine.h>
#include <core/variant.h>
#include <jni.h>

struct jvalret {

	jobject obj;
	jvalue val;
	jvalret() { obj = NULL; }
};

jvalret _variant_to_jvalue(JNIEnv *env, Variant::Type p_type, const Variant *p_arg, bool force_jobject = false);

String _get_class_name(JNIEnv *env, jclass cls, bool *array);

Variant _jobject_to_variant(JNIEnv *env, jobject obj);

Variant::Type get_jni_type(const String &p_type);

const char *get_jni_sig(const String &p_type);

class JNISingleton : public Object {

	GDCLASS(JNISingleton, Object);

	struct MethodData {

		jmethodID method;
		Variant::Type ret_type;
		Vector<Variant::Type> argtypes;
	};

	jobject instance;
	Map<StringName, MethodData> method_map;

public:
	virtual Variant call(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error) {

		ERR_FAIL_COND_V(!instance, Variant());

		r_error.error = Callable::CallError::CALL_OK;

		Map<StringName, MethodData>::Element *E = method_map.find(p_method);
		if (!E) {

			r_error.error = Callable::CallError::CALL_ERROR_INVALID_METHOD;
			return Variant();
		}

		int ac = E->get().argtypes.size();
		if (ac < p_argcount) {

			r_error.error = Callable::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
			r_error.argument = ac;
			return Variant();
		}

		if (ac > p_argcount) {

			r_error.error = Callable::CallError::CALL_ERROR_TOO_MANY_ARGUMENTS;
			r_error.argument = ac;
			return Variant();
		}

		for (int i = 0; i < p_argcount; i++) {

			if (!Variant::can_convert(p_args[i]->get_type(), E->get().argtypes[i])) {

				r_error.error = Callable::CallError::CALL_ERROR_INVALID_ARGUMENT;
				r_error.argument = i;
				r_error.expected = E->get().argtypes[i];
			}
		}

		jvalue *v = NULL;

		if (p_argcount) {

			v = (jvalue *)alloca(sizeof(jvalue) * p_argcount);
		}

		JNIEnv *env = ThreadAndroid::get_env();

		int res = env->PushLocalFrame(16);

		ERR_FAIL_COND_V(res != 0, Variant());

		List<jobject> to_erase;
		for (int i = 0; i < p_argcount; i++) {

			jvalret vr = _variant_to_jvalue(env, E->get().argtypes[i], p_args[i]);
			v[i] = vr.val;
			if (vr.obj)
				to_erase.push_back(vr.obj);
		}

		Variant ret;

		switch (E->get().ret_type) {

			case Variant::NIL: {

				env->CallVoidMethodA(instance, E->get().method, v);
			} break;
			case Variant::BOOL: {

				ret = env->CallBooleanMethodA(instance, E->get().method, v) == JNI_TRUE;
			} break;
			case Variant::INT: {

				ret = env->CallIntMethodA(instance, E->get().method, v);
			} break;
			case Variant::FLOAT: {

				ret = env->CallFloatMethodA(instance, E->get().method, v);
			} break;
			case Variant::STRING: {

				jobject o = env->CallObjectMethodA(instance, E->get().method, v);
				ret = jstring_to_string((jstring)o, env);
				env->DeleteLocalRef(o);
			} break;
			case Variant::PACKED_STRING_ARRAY: {

				jobjectArray arr = (jobjectArray)env->CallObjectMethodA(instance, E->get().method, v);

				ret = _jobject_to_variant(env, arr);

				env->DeleteLocalRef(arr);
			} break;
			case Variant::PACKED_INT32_ARRAY: {

				jintArray arr = (jintArray)env->CallObjectMethodA(instance, E->get().method, v);

				int fCount = env->GetArrayLength(arr);
				Vector<int> sarr;
				sarr.resize(fCount);

				int *w = sarr.ptrw();
				env->GetIntArrayRegion(arr, 0, fCount, w);
				ret = sarr;
				env->DeleteLocalRef(arr);
			} break;
			case Variant::PACKED_FLOAT32_ARRAY: {

				jfloatArray arr = (jfloatArray)env->CallObjectMethodA(instance, E->get().method, v);

				int fCount = env->GetArrayLength(arr);
				Vector<float> sarr;
				sarr.resize(fCount);

				float *w = sarr.ptrw();
				env->GetFloatArrayRegion(arr, 0, fCount, w);
				ret = sarr;
				env->DeleteLocalRef(arr);
			} break;

#ifndef _MSC_VER
#warning This is missing 64 bits arrays, I have no idea how to do it in JNI
#endif
			case Variant::DICTIONARY: {

				jobject obj = env->CallObjectMethodA(instance, E->get().method, v);
				ret = _jobject_to_variant(env, obj);
				env->DeleteLocalRef(obj);

			} break;
			default: {

				env->PopLocalFrame(NULL);
				ERR_FAIL_V(Variant());
			} break;
		}

		while (to_erase.size()) {
			env->DeleteLocalRef(to_erase.front()->get());
			to_erase.pop_front();
		}

		env->PopLocalFrame(NULL);

		return ret;
	}

	jobject get_instance() const {

		return instance;
	}
	void set_instance(jobject p_instance) {

		instance = p_instance;
	}

	void add_method(const StringName &p_name, jmethodID p_method, const Vector<Variant::Type> &p_args, Variant::Type p_ret_type) {

		MethodData md;
		md.method = p_method;
		md.argtypes = p_args;
		md.ret_type = p_ret_type;
		method_map[p_name] = md;
	}

	JNISingleton() {
		instance = NULL;
	}
};

#endif // JNI_UTILS_H

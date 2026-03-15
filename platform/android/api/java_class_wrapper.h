

/**
 * @file java_class_wrapper.h
 * @brief Android JNI reflection and invocation wrappers for exposing Java classes and instances to the engine.
 *
 * This header defines a small bridge layer used on Android to:
 * - Wrap Java classes discovered through JNI and reflection.
 * - Cache method and constant metadata.
 * - Convert between Java/JNI types and engine Variant types.
 * - Invoke Java static and instance methods through a Variant-based call interface.
 *
 * The main types are:
 * - JavaClass: Represents a reflected Java class and its callable/static members.
 * - JavaObject: Represents an instance of a wrapped Java class.
 * - JavaClassWrapper: Singleton-style helper responsible for class loading, reflection, and wrapper creation.
 */

/**
 * @class JavaClass
 * @brief Variant-callable wrapper around a Java class.
 *
 * JavaClass stores reflected metadata for a Java class, including:
 * - Available methods and overloads.
 * - Method signatures and return types.
 * - Constant/static field values.
 * - The underlying JNI class reference.
 *
 * On Android builds, it uses reflection metadata gathered by JavaClassWrapper
 * to resolve overloads and convert arguments/results between JNI and Variant.
 *
 * A JavaClass may be used to:
 * - Call static Java methods.
 * - Serve as the base type for JavaObject instances.
 * - Expose Java constants to script-facing systems.
 */

/**
 * @enum ArgumentType
 * @brief Internal encoding for reflected Java argument and return types.
 *
 * These values describe primitive Java types, strings, objects, and arrays.
 * Additional bit flags are used to indicate:
 * - Array types.
 * - Boxed numeric/object wrapper types.
 * - A mask for extracting the base type.
 *
 * This encoding is used during overload resolution and Variant conversion.
 */

/**
 * @struct MethodInfo
 * @brief Reflection metadata for a Java method overload.
 *
 * Stores enough information to select and invoke a Java method through JNI:
 * - Whether the method is static.
 * - Encoded parameter types.
 * - Parameter signature strings for class/object types.
 * - Encoded return type.
 * - JNI method ID for fast invocation.
 */

/**
 * @brief Converts an internal Java signature code to an engine Variant type.
 *
 * This helper maps reflected Java/JNI type information to the most appropriate
 * Variant::Type and assigns a likelihood score used when resolving overloaded
 * methods. Higher likelihood values indicate a stronger expected match.
 *
 * @param p_sig Encoded Java type signature.
 * @param r_type Output Variant type corresponding to the Java type.
 * @param likelihood Output score indicating how strongly the type maps to the Variant type.
 */

/**
 * @brief Converts a JNI object reference into a Variant.
 *
 * Handles object-to-Variant conversion for boxed primitives, strings, arrays,
 * and wrapped Java objects depending on the provided encoded signature.
 *
 * @param env Active JNI environment.
 * @param obj JNI object to convert.
 * @param var Output Variant receiving the converted value.
 * @param p_sig Encoded Java type signature describing the expected object type.
 * @return true if the conversion succeeded; false otherwise.
 */

/**
 * @brief Invokes a reflected Java method.
 *
 * Resolves the best matching overload for the supplied arguments, performs
 * Variant-to-JNI argument conversion, calls the method on either a class or
 * an instance, and converts the return value back to Variant.
 *
 * @param p_instance Target Java object instance, or null for static calls.
 * @param p_method Name of the Java method to invoke.
 * @param p_args Array of argument pointers.
 * @param p_argcount Number of arguments in @p p_args.
 * @param r_error Output call error information.
 * @param ret Output Variant receiving the return value.
 * @return true if a suitable overload was found and invoked successfully.
 */

/**
 * @brief Dispatches a Variant-based method call to the wrapped Java class.
 *
 * This is the public call entry point used by the engine when invoking static
 * Java methods through a JavaClass wrapper.
 *
 * @param p_method Name of the method to call.
 * @param p_args Array of argument pointers.
 * @param p_argcount Number of arguments.
 * @param r_error Output call error state.
 * @return The converted return value from the Java method, or an empty Variant on failure.
 */

/**
 * @brief Constructs an empty JavaClass wrapper.
 *
 * The wrapper is typically populated by JavaClassWrapper after reflecting a
 * Java class and collecting its methods and constants.
 */

/**
 * @class JavaObject
 * @brief Wrapper around a Java object instance.
 *
 * JavaObject represents a concrete Java instance associated with a JavaClass
 * wrapper. It exposes the same Variant-based call interface used elsewhere in
 * the engine, allowing reflected instance methods to be invoked dynamically.
 *
 * The object retains:
 * - A reference to its base JavaClass metadata.
 * - The underlying JNI object reference.
 */

/**
 * @brief Dispatches a Variant-based method call to the wrapped Java instance.
 *
 * Uses the associated JavaClass metadata to resolve and invoke an instance
 * method on the stored JNI object reference.
 *
 * @param p_method Name of the method to call.
 * @param p_args Array of argument pointers.
 * @param p_argcount Number of arguments.
 * @param r_error Output call error state.
 * @return The converted return value from the Java method, or an empty Variant on failure.
 */

/**
 * @brief Creates a wrapper for a Java object instance.
 *
 * @param p_base Reflected JavaClass metadata describing the object's class.
 * @param p_instance Pointer to the JNI object reference to wrap.
 */

/**
 * @brief Releases the wrapped JNI object resources.
 *
 * Responsible for cleaning up any owned JNI references associated with the instance.
 */

/**
 * @class JavaClassWrapper
 * @brief Factory and reflection cache for JavaClass wrappers.
 *
 * JavaClassWrapper is the central Android JNI helper responsible for:
 * - Loading Java classes through the Android activity/class loader.
 * - Reflecting methods, fields, modifiers, and type information.
 * - Building JavaClass wrappers and caching them by class name.
 * - Providing access to frequently used JNI reflection method IDs.
 *
 * This type acts as the main entry point for obtaining wrapped Java classes.
 */

/**
 * @brief Determines the internal signature encoding for a reflected Java type.
 *
 * Inspects a reflected Java type object and produces:
 * - An encoded internal type signature.
 * - A string signature for object/class types when needed.
 *
 * This information is used to build method metadata and perform conversions.
 *
 * @param env Active JNI environment.
 * @param obj Reflected Java type object.
 * @param sig Output encoded type signature.
 * @param strsig Output class/type signature string for non-primitive types.
 * @return true if the type was recognized and encoded successfully.
 */

/**
 * @brief Binds methods and properties for engine-side exposure.
 *
 * Registers this type with the engine's object/class database.
 */

/**
 * @brief Returns the global JavaClassWrapper instance.
 *
 * @return Pointer to the singleton wrapper instance.
 */

/**
 * @brief Wraps a Java class by name and returns a cached/reflected JavaClass.
 *
 * If the class has already been wrapped, the cached wrapper is returned.
 * Otherwise, the class is loaded through the Android class loader, reflected,
 * cached, and returned.
 *
 * @param p_class Fully qualified Java class name.
 * @return A reference to the wrapped JavaClass, or an empty reference on failure.
 */

/**
 * @brief Constructs the Android Java reflection wrapper.
 *
 * Initializes JNI reflection helpers, caches frequently used method IDs, and
 * stores the activity/class loader context required to locate application classes.
 *
 * @param p_activity Android activity object used to access the class loader.
 */

/**
 * @brief Constructs a non-Android placeholder wrapper.
 *
 * Exists to allow the type to be declared on non-Android builds where JNI
 * functionality is unavailable.
 */
#ifndef JAVA_CLASS_WRAPPER_H
#define JAVA_CLASS_WRAPPER_H

#include "core/reference.h"

#ifdef ANDROID_ENABLED
#include <android/log.h>
#include <jni.h>
#endif

#ifdef ANDROID_ENABLED
class JavaObject;
#endif

class JavaClass : public Reference {

	GDCLASS(JavaClass, Reference);

#ifdef ANDROID_ENABLED
	enum ArgumentType{

		ARG_TYPE_VOID,
		ARG_TYPE_BOOLEAN,
		ARG_TYPE_BYTE,
		ARG_TYPE_CHAR,
		ARG_TYPE_SHORT,
		ARG_TYPE_INT,
		ARG_TYPE_LONG,
		ARG_TYPE_FLOAT,
		ARG_TYPE_DOUBLE,
		ARG_TYPE_STRING, //special case
		ARG_TYPE_CLASS,
		ARG_ARRAY_BIT = 1 << 16,
		ARG_NUMBER_CLASS_BIT = 1 << 17,
		ARG_TYPE_MASK = (1 << 16) - 1
	};

	Map<StringName, Variant> constant_map;

	struct MethodInfo {

		bool _static;
		Vector<uint32_t> param_types;
		Vector<StringName> param_sigs;
		uint32_t return_type;
		jmethodID method;
	};

	_FORCE_INLINE_ static void _convert_to_variant_type(int p_sig, Variant::Type &r_type, float &likelihood) {

		likelihood = 1.0;
		r_type = Variant::NIL;

		switch (p_sig) {

			case ARG_TYPE_VOID: r_type = Variant::NIL; break;
			case ARG_TYPE_BOOLEAN | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_BOOLEAN: r_type = Variant::BOOL; break;
			case ARG_TYPE_BYTE | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_BYTE:
				r_type = Variant::INT;
				likelihood = 0.1;
				break;
			case ARG_TYPE_CHAR | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_CHAR:
				r_type = Variant::INT;
				likelihood = 0.2;
				break;
			case ARG_TYPE_SHORT | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_SHORT:
				r_type = Variant::INT;
				likelihood = 0.3;
				break;
			case ARG_TYPE_INT | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_INT:
				r_type = Variant::INT;
				likelihood = 1.0;
				break;
			case ARG_TYPE_LONG | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_LONG:
				r_type = Variant::INT;
				likelihood = 0.5;
				break;
			case ARG_TYPE_FLOAT | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_FLOAT:
				r_type = Variant::FLOAT;
				likelihood = 1.0;
				break;
			case ARG_TYPE_DOUBLE | ARG_NUMBER_CLASS_BIT:
			case ARG_TYPE_DOUBLE:
				r_type = Variant::FLOAT;
				likelihood = 0.5;
				break;
			case ARG_TYPE_STRING: r_type = Variant::STRING; break;
			case ARG_TYPE_CLASS: r_type = Variant::OBJECT; break;
			case ARG_ARRAY_BIT | ARG_TYPE_VOID: r_type = Variant::NIL; break;
			case ARG_ARRAY_BIT | ARG_TYPE_BOOLEAN: r_type = Variant::ARRAY; break;
			case ARG_ARRAY_BIT | ARG_TYPE_BYTE:
				r_type = Variant::PACKED_BYTE_ARRAY;
				likelihood = 1.0;
				break;
			case ARG_ARRAY_BIT | ARG_TYPE_CHAR:
				r_type = Variant::PACKED_BYTE_ARRAY;
				likelihood = 0.5;
				break;
			case ARG_ARRAY_BIT | ARG_TYPE_SHORT:
				r_type = Variant::PACKED_INT32_ARRAY;
				likelihood = 0.3;
				break;
			case ARG_ARRAY_BIT | ARG_TYPE_INT:
				r_type = Variant::PACKED_INT32_ARRAY;
				likelihood = 1.0;
				break;
			case ARG_ARRAY_BIT | ARG_TYPE_LONG:
				r_type = Variant::PACKED_INT32_ARRAY;
				likelihood = 0.5;
				break;
			case ARG_ARRAY_BIT | ARG_TYPE_FLOAT:
				r_type = Variant::PACKED_FLOAT32_ARRAY;
				likelihood = 1.0;
				break;
			case ARG_ARRAY_BIT | ARG_TYPE_DOUBLE:
				r_type = Variant::PACKED_FLOAT32_ARRAY;
				likelihood = 0.5;
				break;
			case ARG_ARRAY_BIT | ARG_TYPE_STRING: r_type = Variant::PACKED_STRING_ARRAY; break;
			case ARG_ARRAY_BIT | ARG_TYPE_CLASS: r_type = Variant::ARRAY; break;
		}
	}

	_FORCE_INLINE_ static bool _convert_object_to_variant(JNIEnv *env, jobject obj, Variant &var, uint32_t p_sig);

	bool _call_method(JavaObject *p_instance, const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error, Variant &ret);

	friend class JavaClassWrapper;
	Map<StringName, List<MethodInfo>> methods;
	jclass _class;
#endif

public:
	virtual Variant call(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error);

	JavaClass();
};

class JavaObject : public Reference {

	GDCLASS(JavaObject, Reference);

#ifdef ANDROID_ENABLED
	Ref<JavaClass> base_class;
	friend class JavaClass;

	jobject instance;
#endif

public:
	virtual Variant call(const StringName &p_method, const Variant **p_args, int p_argcount, Callable::CallError &r_error);

#ifdef ANDROID_ENABLED
	JavaObject(const Ref<JavaClass> &p_base, jobject *p_instance);
	~JavaObject();
#endif
};

class JavaClassWrapper : public Object {

	GDCLASS(JavaClassWrapper, Object);

#ifdef ANDROID_ENABLED
	Map<String, Ref<JavaClass>> class_cache;
	friend class JavaClass;
	jclass activityClass;
	jmethodID findClass;
	jmethodID getDeclaredMethods;
	jmethodID getFields;
	jmethodID getParameterTypes;
	jmethodID getReturnType;
	jmethodID getModifiers;
	jmethodID getName;
	jmethodID Class_getName;
	jmethodID Field_getName;
	jmethodID Field_getModifiers;
	jmethodID Field_get;
	jmethodID Boolean_booleanValue;
	jmethodID Byte_byteValue;
	jmethodID Character_characterValue;
	jmethodID Short_shortValue;
	jmethodID Integer_integerValue;
	jmethodID Long_longValue;
	jmethodID Float_floatValue;
	jmethodID Double_doubleValue;
	jobject classLoader;

	bool _get_type_sig(JNIEnv *env, jobject obj, uint32_t &sig, String &strsig);
#endif

	static JavaClassWrapper *singleton;

protected:
	static void _bind_methods();

public:
	static JavaClassWrapper *get_singleton() { return singleton; }

	Ref<JavaClass> wrap(const String &p_class);

#ifdef ANDROID_ENABLED
	JavaClassWrapper(jobject p_activity = NULL);
#else
	JavaClassWrapper();
#endif
};

#endif // JAVA_CLASS_WRAPPER_H

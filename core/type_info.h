

/**
 * @file type_info.h
 * @brief Type information and reflection utilities for the game engine.
 * 
 * This header provides compile-time type introspection and metadata management for the engine's
 * type system. It enables runtime type information (RTTI) capabilities when DEBUG_METHODS_ENABLED
 * is defined, allowing for type checking, inheritance detection, and variant type conversion.
 * 
 * @details
 * The file includes:
 * - EnableIf<C, T>: Template metaprogramming utility for conditional type definitions
 * - TypesAreSame<A, B>: Compile-time type equality checking
 * - TypeInherits<B, D>: Compile-time inheritance relationship detection
 * - GetTypeInfo<T>: Primary template for retrieving type metadata and variant information
 * - Metadata enum: Flags for specifying integer and floating-point type precision
 * 
 * Supported types include:
 * - Primitive types: bool, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, wchar_t
 * - Engine types: String, Vector2, Vector3, Transform, Quat, Color, StringName, NodePath, RID, Callable, Signal
 * - Collection types: Dictionary, Array, PackedByteArray, PackedIntXXArray, PackedFloatXXArray, PackedStringArray, PackedVectorXArray, PackedColorArray
 * - Special types: Variant, ObjectID, Object pointers
 * 
 * Macros:
 * - MAKE_TYPE_INFO(m_type, m_var_type): Generates GetTypeInfo specialization for a type
 * - MAKE_TYPE_INFO_WITH_META(m_type, m_var_type, m_metadata): Generates specialization with metadata
 * - MAKE_TEMPLATE_TYPE_INFO(m_template, m_type, m_var_type): Generates specialization for template types
 * - MAKE_ENUM_TYPE_INFO(m_enum): Generates specialization for enum types
 * - CLASS_INFO(m_type): Retrieves class info for a given type
 * 
 * @note When compilation fails with instantiation errors for GetTypeInfo<T>, ensure that the
 * forward declaration of T can see the actual class definition, especially for Object-derived classes.
 * This typically requires including the full class declaration in the source file where GetTypeInfo<T>
 * is instantiated.
 */
#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#ifdef DEBUG_METHODS_ENABLED

template <bool C, typename T = void>
struct EnableIf {

	typedef T type;
};

template <typename T>
struct EnableIf<false, T> {
};

template <typename, typename>
struct TypesAreSame {

	static bool const value = false;
};

template <typename A>
struct TypesAreSame<A, A> {

	static bool const value = true;
};

template <typename B, typename D>
struct TypeInherits {

	static D *get_d();

	static char (&test(B *))[1];
	static char (&test(...))[2];

	static bool const value = sizeof(test(get_d())) == sizeof(char) &&
							  !TypesAreSame<B volatile const, void volatile const>::value;
};

namespace GodotTypeInfo {
enum Metadata {
	METADATA_NONE,
	METADATA_INT_IS_INT8,
	METADATA_INT_IS_INT16,
	METADATA_INT_IS_INT32,
	METADATA_INT_IS_INT64,
	METADATA_INT_IS_UINT8,
	METADATA_INT_IS_UINT16,
	METADATA_INT_IS_UINT32,
	METADATA_INT_IS_UINT64,
	METADATA_REAL_IS_FLOAT,
	METADATA_REAL_IS_DOUBLE
};
}

// If the compiler fails because it's trying to instantiate the primary 'GetTypeInfo' template
// instead of one of the specializations, it's most likely because the type 'T' is not supported.
// If 'T' is a class that inherits 'Object', make sure it can see the actual class declaration
// instead of a forward declaration. You can always forward declare 'T' in a header file, and then
// include the actual declaration of 'T' in the source file where 'GetTypeInfo<T>' is instantiated.
template <class T, typename = void>
struct GetTypeInfo;

#define MAKE_TYPE_INFO(m_type, m_var_type)                                            \
	template <>                                                                       \
	struct GetTypeInfo<m_type> {                                                      \
		static const Variant::Type VARIANT_TYPE = m_var_type;                         \
		static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE; \
		static inline PropertyInfo get_class_info() {                                 \
			return PropertyInfo(VARIANT_TYPE, String());                              \
		}                                                                             \
	};                                                                                \
	template <>                                                                       \
	struct GetTypeInfo<const m_type &> {                                              \
		static const Variant::Type VARIANT_TYPE = m_var_type;                         \
		static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE; \
		static inline PropertyInfo get_class_info() {                                 \
			return PropertyInfo(VARIANT_TYPE, String());                              \
		}                                                                             \
	};

#define MAKE_TYPE_INFO_WITH_META(m_type, m_var_type, m_metadata)    \
	template <>                                                     \
	struct GetTypeInfo<m_type> {                                    \
		static const Variant::Type VARIANT_TYPE = m_var_type;       \
		static const GodotTypeInfo::Metadata METADATA = m_metadata; \
		static inline PropertyInfo get_class_info() {               \
			return PropertyInfo(VARIANT_TYPE, String());            \
		}                                                           \
	};                                                              \
	template <>                                                     \
	struct GetTypeInfo<const m_type &> {                            \
		static const Variant::Type VARIANT_TYPE = m_var_type;       \
		static const GodotTypeInfo::Metadata METADATA = m_metadata; \
		static inline PropertyInfo get_class_info() {               \
			return PropertyInfo(VARIANT_TYPE, String());            \
		}                                                           \
	};

MAKE_TYPE_INFO(bool, Variant::BOOL)
MAKE_TYPE_INFO_WITH_META(uint8_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_UINT8)
MAKE_TYPE_INFO_WITH_META(int8_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_INT8)
MAKE_TYPE_INFO_WITH_META(uint16_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_UINT16)
MAKE_TYPE_INFO_WITH_META(int16_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_INT16)
MAKE_TYPE_INFO_WITH_META(uint32_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_UINT32)
MAKE_TYPE_INFO_WITH_META(int32_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_INT32)
MAKE_TYPE_INFO_WITH_META(uint64_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_UINT64)
MAKE_TYPE_INFO_WITH_META(int64_t, Variant::INT, GodotTypeInfo::METADATA_INT_IS_INT64)
MAKE_TYPE_INFO(wchar_t, Variant::INT)
MAKE_TYPE_INFO_WITH_META(float, Variant::FLOAT, GodotTypeInfo::METADATA_REAL_IS_FLOAT)
MAKE_TYPE_INFO_WITH_META(double, Variant::FLOAT, GodotTypeInfo::METADATA_REAL_IS_DOUBLE)

MAKE_TYPE_INFO(String, Variant::STRING)
MAKE_TYPE_INFO(Vector2, Variant::VECTOR2)
MAKE_TYPE_INFO(Rect2, Variant::RECT2)
MAKE_TYPE_INFO(Vector3, Variant::VECTOR3)
MAKE_TYPE_INFO(Transform2D, Variant::TRANSFORM2D)
MAKE_TYPE_INFO(Plane, Variant::PLANE)
MAKE_TYPE_INFO(Quat, Variant::QUAT)
MAKE_TYPE_INFO(AABB, Variant::AABB)
MAKE_TYPE_INFO(Basis, Variant::BASIS)
MAKE_TYPE_INFO(Transform, Variant::TRANSFORM)
MAKE_TYPE_INFO(Color, Variant::COLOR)
MAKE_TYPE_INFO(StringName, Variant::STRING_NAME)
MAKE_TYPE_INFO(NodePath, Variant::NODE_PATH)
MAKE_TYPE_INFO(RID, Variant::_RID)
MAKE_TYPE_INFO(Callable, Variant::CALLABLE)
MAKE_TYPE_INFO(Signal, Variant::SIGNAL)
MAKE_TYPE_INFO(Dictionary, Variant::DICTIONARY)
MAKE_TYPE_INFO(Array, Variant::ARRAY)
MAKE_TYPE_INFO(PackedByteArray, Variant::PACKED_BYTE_ARRAY)
MAKE_TYPE_INFO(PackedInt32Array, Variant::PACKED_INT32_ARRAY)
MAKE_TYPE_INFO(PackedInt64Array, Variant::PACKED_INT64_ARRAY)
MAKE_TYPE_INFO(PackedFloat32Array, Variant::PACKED_FLOAT32_ARRAY)
MAKE_TYPE_INFO(PackedFloat64Array, Variant::PACKED_FLOAT64_ARRAY)
MAKE_TYPE_INFO(PackedStringArray, Variant::PACKED_STRING_ARRAY)
MAKE_TYPE_INFO(PackedVector2Array, Variant::PACKED_VECTOR2_ARRAY)
MAKE_TYPE_INFO(PackedVector3Array, Variant::PACKED_VECTOR3_ARRAY)
MAKE_TYPE_INFO(PackedColorArray, Variant::PACKED_COLOR_ARRAY)

MAKE_TYPE_INFO(IP_Address, Variant::STRING)

//objectID
template <>
struct GetTypeInfo<ObjectID> {
	static const Variant::Type VARIANT_TYPE = Variant::INT;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_INT_IS_UINT64;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::INT, String(), PROPERTY_HINT_INT_IS_OBJECTID);
	}
};

//for variant
template <>
struct GetTypeInfo<Variant> {
	static const Variant::Type VARIANT_TYPE = Variant::NIL;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::NIL, String(), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_NIL_IS_VARIANT);
	}
};

template <>
struct GetTypeInfo<const Variant &> {
	static const Variant::Type VARIANT_TYPE = Variant::NIL;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::NIL, String(), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_NIL_IS_VARIANT);
	}
};

#define MAKE_TEMPLATE_TYPE_INFO(m_template, m_type, m_var_type)                       \
	template <>                                                                       \
	struct GetTypeInfo<m_template<m_type>> {                                          \
		static const Variant::Type VARIANT_TYPE = m_var_type;                         \
		static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE; \
		static inline PropertyInfo get_class_info() {                                 \
			return PropertyInfo(VARIANT_TYPE, String());                              \
		}                                                                             \
	};                                                                                \
	template <>                                                                       \
	struct GetTypeInfo<const m_template<m_type> &> {                                  \
		static const Variant::Type VARIANT_TYPE = m_var_type;                         \
		static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE; \
		static inline PropertyInfo get_class_info() {                                 \
			return PropertyInfo(VARIANT_TYPE, String());                              \
		}                                                                             \
	};

MAKE_TEMPLATE_TYPE_INFO(Vector, Variant, Variant::ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, RID, Variant::ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, Plane, Variant::ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, Face3, Variant::PACKED_VECTOR3_ARRAY)
MAKE_TEMPLATE_TYPE_INFO(Vector, StringName, Variant::PACKED_STRING_ARRAY)

template <typename T>
struct GetTypeInfo<T *, typename EnableIf<TypeInherits<Object, T>::value>::type> {
	static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(StringName(T::get_class_static()));
	}
};

template <typename T>
struct GetTypeInfo<const T *, typename EnableIf<TypeInherits<Object, T>::value>::type> {
	static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(StringName(T::get_class_static()));
	}
};

#define TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_impl)                                                                                                                                 \
	template <>                                                                                                                                                                   \
	struct GetTypeInfo<m_impl> {                                                                                                                                                  \
		static const Variant::Type VARIANT_TYPE = Variant::INT;                                                                                                                   \
		static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;                                                                                             \
		static inline PropertyInfo get_class_info() {                                                                                                                             \
			return PropertyInfo(Variant::INT, String(), PROPERTY_HINT_NONE, String(), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM, String(#m_enum).replace("::", ".")); \
		}                                                                                                                                                                         \
	};

#define MAKE_ENUM_TYPE_INFO(m_enum)                 \
	TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_enum)       \
	TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_enum const) \
	TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, m_enum &)     \
	TEMPL_MAKE_ENUM_TYPE_INFO(m_enum, const m_enum &)

template <typename T>
inline StringName __constant_get_enum_name(T param, const String &p_constant) {
	if (GetTypeInfo<T>::VARIANT_TYPE == Variant::NIL)
		ERR_PRINT("Missing VARIANT_ENUM_CAST for constant's enum: " + p_constant);
	return GetTypeInfo<T>::get_class_info().class_name;
}

#define CLASS_INFO(m_type) (GetTypeInfo<m_type *>::get_class_info())

#else

#define MAKE_ENUM_TYPE_INFO(m_enum)
#define CLASS_INFO(m_type)

#endif // DEBUG_METHODS_ENABLED

#endif // TYPE_INFO_H

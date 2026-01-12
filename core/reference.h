

/**
 * @file reference.h
 * @brief Reference counting and smart pointer management for game engine objects
 * 
 * This header provides a reference counting system for managing object lifecycles in the game engine.
 * It includes:
 * 
 * - Reference: Base class implementing reference counting with SafeRefCount
 *   - Tracks initialization state and reference count
 *   - Methods: is_referenced(), init_ref(), reference(), unreference(), reference_get_count()
 * 
 * - Ref<T>: Template-based smart pointer for automatic memory management
 *   - Automatically increments/decrements reference count on assignment
 *   - Supports type casting with Ref<T_Other>
 *   - Provides pointer-like operators: ->, *, [], and comparison operators
 *   - Methods: is_valid(), is_null(), unref(), instance(), reference_ptr()
 *   - Supports conversion to/from Variant for script integration
 * 
 * - WeakRef: Weak reference class for breaking circular dependencies
 *   - Holds ObjectID instead of direct pointer
 *   - Methods: get_ref(), set_obj(), set_ref()
 * 
 * - PtrToArg<Ref<T>>: Template specialization for native function calls
 *   - Enables seamless Ref<T> conversion in native C++ bindings
 * 
 * - GetTypeInfo<Ref<T>>: Template specialization for type introspection
 *   - Provides metadata for script property hints and validation
 * 
 * @note Reference counting is not thread-safe; access should be protected by mutexes in multithreaded contexts
 * @see core/object.h, core/safe_refcount.h, core/class_db.h
 */
#ifndef REFERENCE_H
#define REFERENCE_H

#include "core/class_db.h"
#include "core/object.h"
#include "core/safe_refcount.h"

class Reference : public Object {

	GDCLASS(Reference, Object);
	SafeRefCount refcount;
	SafeRefCount refcount_init;

protected:
	static void _bind_methods();

public:
	_FORCE_INLINE_ bool is_referenced() const { return refcount_init.get() != 1; }
	bool init_ref();
	bool reference(); // returns false if refcount is at zero and didn't get increased
	bool unreference();
	int reference_get_count() const;

	Reference();
	~Reference();
};

template <class T>
class Ref {

	T *reference;

	void ref(const Ref &p_from) {

		if (p_from.reference == reference)
			return;

		unref();

		reference = p_from.reference;
		if (reference)
			reference->reference();
	}

	void ref_pointer(T *p_ref) {

		ERR_FAIL_COND(!p_ref);

		if (p_ref->init_ref())
			reference = p_ref;
	}

	//virtual Reference * get_reference() const { return reference; }
public:
	_FORCE_INLINE_ bool operator==(const T *p_ptr) const {
		return reference == p_ptr;
	}
	_FORCE_INLINE_ bool operator!=(const T *p_ptr) const {
		return reference != p_ptr;
	}

	_FORCE_INLINE_ bool operator<(const Ref<T> &p_r) const {

		return reference < p_r.reference;
	}
	_FORCE_INLINE_ bool operator==(const Ref<T> &p_r) const {

		return reference == p_r.reference;
	}
	_FORCE_INLINE_ bool operator!=(const Ref<T> &p_r) const {

		return reference != p_r.reference;
	}

	_FORCE_INLINE_ T *operator->() {

		return reference;
	}

	_FORCE_INLINE_ T *operator*() {

		return reference;
	}

	_FORCE_INLINE_ const T *operator->() const {

		return reference;
	}

	_FORCE_INLINE_ const T *ptr() const {

		return reference;
	}
	_FORCE_INLINE_ T *ptr() {

		return reference;
	}

	_FORCE_INLINE_ const T *operator*() const {

		return reference;
	}

	operator Variant() const {

		return Variant(reference);
	}

	void operator=(const Ref &p_from) {

		ref(p_from);
	}

	template <class T_Other>
	void operator=(const Ref<T_Other> &p_from) {

		Reference *refb = const_cast<Reference *>(static_cast<const Reference *>(p_from.ptr()));
		if (!refb) {
			unref();
			return;
		}
		Ref r;
		r.reference = Object::cast_to<T>(refb);
		ref(r);
		r.reference = NULL;
	}

	void operator=(const Variant &p_variant) {

		Object *object = p_variant.get_validated_object();

		if (object == reference) {
			return;
		}

		unref();

		if (!object) {
			return;
		}

		T *r = Object::cast_to<T>(object);
		if (r && r->reference()) {
			reference = r;
		}
	}

	template <class T_Other>
	void reference_ptr(T_Other *p_ptr) {
		if (reference == p_ptr) {
			return;
		}
		unref();

		T *r = Object::cast_to<T>(p_ptr);
		if (r) {
			ref_pointer(r);
		}
	}

	Ref(const Ref &p_from) {

		reference = NULL;
		ref(p_from);
	}

	template <class T_Other>
	Ref(const Ref<T_Other> &p_from) {

		reference = NULL;
		Reference *refb = const_cast<Reference *>(static_cast<const Reference *>(p_from.ptr()));
		if (!refb) {
			unref();
			return;
		}
		Ref r;
		r.reference = Object::cast_to<T>(refb);
		ref(r);
		r.reference = NULL;
	}

	Ref(T *p_reference) {

		reference = NULL;
		if (p_reference)
			ref_pointer(p_reference);
	}

	Ref(const Variant &p_variant) {

		Object *object = p_variant.get_validated_object();

		if (!object) {
			reference = nullptr;
			return;
		}

		T *r = Object::cast_to<T>(object);
		if (r && r->reference()) {
			reference = r;
		} else {
			reference = nullptr;
		}
	}

	inline bool is_valid() const { return reference != NULL; }
	inline bool is_null() const { return reference == NULL; }

	void unref() {
		//TODO this should be moved to mutexes, since this engine does not really
		// do a lot of referencing on references and stuff
		// mutexes will avoid more crashes?

		if (reference && reference->unreference()) {

			memdelete(reference);
		}
		reference = NULL;
	}

	void instance() {
		ref(memnew(T));
	}

	Ref() {

		reference = NULL;
	}

	~Ref() {

		unref();
	}
};

typedef Ref<Reference> REF;

class WeakRef : public Reference {

	GDCLASS(WeakRef, Reference);

	ObjectID ref;

protected:
	static void _bind_methods();

public:
	Variant get_ref() const;
	void set_obj(Object *p_object);
	void set_ref(const REF &p_ref);

	WeakRef();
};

#ifdef PTRCALL_ENABLED

template <class T>
struct PtrToArg<Ref<T>> {

	_FORCE_INLINE_ static Ref<T> convert(const void *p_ptr) {

		return Ref<T>(const_cast<T *>(reinterpret_cast<const T *>(p_ptr)));
	}

	_FORCE_INLINE_ static void encode(Ref<T> p_val, const void *p_ptr) {

		*(Ref<Reference> *)p_ptr = p_val;
	}
};

template <class T>
struct PtrToArg<const Ref<T> &> {

	_FORCE_INLINE_ static Ref<T> convert(const void *p_ptr) {

		return Ref<T>((T *)p_ptr);
	}
};

#endif // PTRCALL_ENABLED

#ifdef DEBUG_METHODS_ENABLED

template <class T>
struct GetTypeInfo<Ref<T>> {
	static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;

	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::OBJECT, String(), PROPERTY_HINT_RESOURCE_TYPE, T::get_class_static());
	}
};

template <class T>
struct GetTypeInfo<const Ref<T> &> {
	static const Variant::Type VARIANT_TYPE = Variant::OBJECT;
	static const GodotTypeInfo::Metadata METADATA = GodotTypeInfo::METADATA_NONE;

	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::OBJECT, String(), PROPERTY_HINT_RESOURCE_TYPE, T::get_class_static());
	}
};

#endif // DEBUG_METHODS_ENABLED

#endif // REFERENCE_H

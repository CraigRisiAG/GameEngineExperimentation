

/// \class Reference
/// \brief A reference-counted base class for managing object lifecycles automatically.
///
/// Reference is a specialized Object subclass that implements automatic memory management
/// through reference counting. Objects that inherit from Reference are automatically deleted
/// when their reference count reaches zero.
///
/// \method bool init_ref()
/// Initializes the reference count for this object. Should be called once after creation
/// to properly set up the reference counting mechanism. Compensates for the initial reference
/// to ensure correct reference counting behavior.
/// \return true if initialization was successful, false otherwise.
///
/// \method int reference_get_count() const
/// Returns the current reference count value for this object.
/// \return The number of active references to this object.
///
/// \method bool reference()
/// Increments the reference count and notifies script instances and language bindings
/// that a reference has been added. This is called automatically by REF smart pointers.
/// \return true if the reference operation was successful, false otherwise.
///
/// \method bool unreference()
/// Decrements the reference count and notifies script instances and language bindings
/// that a reference has been removed. When the count reaches zero, the object is flagged
/// for deletion.
/// \return true if the object should be deleted (count reached zero), false otherwise.
///
/// \class WeakRef
/// \brief A weak reference wrapper that holds a non-owning reference to an Object.
///
/// WeakRef stores an ObjectID to an object without incrementing its reference count.
/// The referenced object can be deleted independently, and get_ref() will safely return
/// a null Variant if the object no longer exists.
///
/// \method Variant get_ref() const
/// Retrieves the referenced object as a Variant, or a null Variant if the object has been deleted.
/// \return A Variant containing the referenced object, or an empty Variant.
///
/// \method void set_obj(Object *p_object)
/// Sets the weak reference to point to the specified object.
/// \param p_object The object to weakly reference, or nullptr to clear the reference.
///
/// \method void set_ref(const REF &p_ref)
/// Sets the weak reference from a Reference smart pointer.
/// \param p_ref A smart pointer to a Reference object.
#include "reference.h"

#include "core/script_language.h"

bool Reference::init_ref() {

	if (reference()) {

		if (!is_referenced() && refcount_init.unref()) {
			unreference(); // first referencing is already 1, so compensate for the ref above
		}

		return true;
	} else {

		return false;
	}
}

void Reference::_bind_methods() {

	ClassDB::bind_method(D_METHOD("init_ref"), &Reference::init_ref);
	ClassDB::bind_method(D_METHOD("reference"), &Reference::reference);
	ClassDB::bind_method(D_METHOD("unreference"), &Reference::unreference);
}

int Reference::reference_get_count() const {
	return refcount.get();
}

bool Reference::reference() {

	uint32_t rc_val = refcount.refval();
	bool success = rc_val != 0;

	if (success && rc_val <= 2 /* higher is not relevant */) {
		if (get_script_instance()) {
			get_script_instance()->refcount_incremented();
		}
		if (instance_binding_count > 0 && !ScriptServer::are_languages_finished()) {
			for (int i = 0; i < MAX_SCRIPT_INSTANCE_BINDINGS; i++) {
				if (_script_instance_bindings[i]) {
					ScriptServer::get_language(i)->refcount_incremented_instance_binding(this);
				}
			}
		}
	}

	return success;
}

bool Reference::unreference() {

	uint32_t rc_val = refcount.unrefval();
	bool die = rc_val == 0;

	if (rc_val <= 1 /* higher is not relevant */) {
		if (get_script_instance()) {
			bool script_ret = get_script_instance()->refcount_decremented();
			die = die && script_ret;
		}
		if (instance_binding_count > 0 && !ScriptServer::are_languages_finished()) {
			for (int i = 0; i < MAX_SCRIPT_INSTANCE_BINDINGS; i++) {
				if (_script_instance_bindings[i]) {
					bool script_ret = ScriptServer::get_language(i)->refcount_decremented_instance_binding(this);
					die = die && script_ret;
				}
			}
		}
	}

	return die;
}

Reference::Reference() :
		Object(true) {

	refcount.init();
	refcount_init.init();
}

Reference::~Reference() {
}

Variant WeakRef::get_ref() const {

	if (ref.is_null())
		return Variant();

	Object *obj = ObjectDB::get_instance(ref);
	if (!obj)
		return Variant();
	Reference *r = cast_to<Reference>(obj);
	if (r) {

		return REF(r);
	}

	return obj;
}

void WeakRef::set_obj(Object *p_object) {
	ref = p_object ? p_object->get_instance_id() : ObjectID();
}

void WeakRef::set_ref(const REF &p_ref) {

	ref = p_ref.is_valid() ? p_ref->get_instance_id() : ObjectID();
}

WeakRef::WeakRef() {
}

void WeakRef::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_ref"), &WeakRef::get_ref);
}

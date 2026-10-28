
#ifndef GD_MONO_PROPERTY_H
#define GD_MONO_PROPERTY_H

#include "gd_mono.h"
#include "gd_mono_header.h"
#include "i_mono_class_member.h"

class GDMonoProperty : public IMonoClassMember {

  GDMonoClass *owner;
  MonoProperty *mono_property;

  StringName name;
  ManagedType type;

  bool attrs_fetched;
  MonoCustomAttrInfo *attributes;

public:
  virtual GDMonoClass *get_enclosing_class() const final { return owner; }

  virtual MemberType get_member_type() const final {
    return MEMBER_TYPE_PROPERTY;
  }

  virtual StringName get_name() const final { return name; }

  virtual bool is_static() final;
  virtual Visibility get_visibility() final;

  virtual bool has_attribute(GDMonoClass *p_attr_class) final;
  virtual MonoObject *get_attribute(GDMonoClass *p_attr_class) final;
  void fetch_attributes();

  bool has_getter();
  bool has_setter();

  _FORCE_INLINE_ ManagedType get_type() const { return type; }

  void set_value(MonoObject *p_object, MonoObject *p_value,
                 MonoException **r_exc = NULL);
  void set_value(MonoObject *p_object, void **p_params,
                 MonoException **r_exc = NULL);
  MonoObject *get_value(MonoObject *p_object, MonoException **r_exc = NULL);

  bool get_bool_value(MonoObject *p_object);
  int get_int_value(MonoObject *p_object);
  String get_string_value(MonoObject *p_object);

  GDMonoProperty(MonoProperty *p_mono_property, GDMonoClass *p_owner);
  ~GDMonoProperty();
};

#endif // GD_MONO_PROPERTY_H

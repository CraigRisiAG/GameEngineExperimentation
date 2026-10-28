
#ifndef GDMONOFIELD_H
#define GDMONOFIELD_H

#include "gd_mono.h"
#include "gd_mono_header.h"
#include "i_mono_class_member.h"

class GDMonoField : public IMonoClassMember {

  GDMonoClass *owner;
  MonoClassField *mono_field;

  StringName name;
  ManagedType type;

  bool attrs_fetched;
  MonoCustomAttrInfo *attributes;

public:
  virtual GDMonoClass *get_enclosing_class() const final { return owner; }

  virtual MemberType get_member_type() const final { return MEMBER_TYPE_FIELD; }

  virtual StringName get_name() const final { return name; }

  virtual bool is_static() final;
  virtual Visibility get_visibility() final;

  virtual bool has_attribute(GDMonoClass *p_attr_class) final;
  virtual MonoObject *get_attribute(GDMonoClass *p_attr_class) final;
  void fetch_attributes();

  _FORCE_INLINE_ ManagedType get_type() const { return type; }

  void set_value(MonoObject *p_object, MonoObject *p_value);
  void set_value_raw(MonoObject *p_object, void *p_ptr);
  void set_value_from_variant(MonoObject *p_object, const Variant &p_value);

  MonoObject *get_value(MonoObject *p_object);

  bool get_bool_value(MonoObject *p_object);
  int get_int_value(MonoObject *p_object);
  String get_string_value(MonoObject *p_object);

  GDMonoField(MonoClassField *p_mono_field, GDMonoClass *p_owner);
  ~GDMonoField();
};

#endif // GDMONOFIELD_H

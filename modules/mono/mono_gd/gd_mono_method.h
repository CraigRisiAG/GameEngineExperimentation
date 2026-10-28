
#ifndef GD_MONO_METHOD_H
#define GD_MONO_METHOD_H

#include "gd_mono.h"
#include "gd_mono_header.h"
#include "i_mono_class_member.h"

class GDMonoMethod : public IMonoClassMember {

  StringName name;

  int params_count;
  ManagedType return_type;
  Vector<ManagedType> param_types;

  bool method_info_fetched;
  MethodInfo method_info;

  bool attrs_fetched;
  MonoCustomAttrInfo *attributes;

  void _update_signature();
  void _update_signature(MonoMethodSignature *p_method_sig);

  friend class GDMonoClass;

  MonoMethod *mono_method;

public:
  virtual GDMonoClass *get_enclosing_class() const final;

  virtual MemberType get_member_type() const final {
    return MEMBER_TYPE_METHOD;
  }

  virtual StringName get_name() const final { return name; }

  virtual bool is_static() final;

  virtual Visibility get_visibility() final;

  virtual bool has_attribute(GDMonoClass *p_attr_class) final;
  virtual MonoObject *get_attribute(GDMonoClass *p_attr_class) final;
  void fetch_attributes();

  _FORCE_INLINE_ MonoMethod *get_mono_ptr() const { return mono_method; }

  _FORCE_INLINE_ int get_parameters_count() const { return params_count; }
  _FORCE_INLINE_ ManagedType get_return_type() const { return return_type; }

  MonoObject *invoke(MonoObject *p_object, const Variant **p_params,
                     MonoException **r_exc = NULL) const;
  MonoObject *invoke(MonoObject *p_object, MonoException **r_exc = NULL) const;
  MonoObject *invoke_raw(MonoObject *p_object, void **p_params,
                         MonoException **r_exc = NULL) const;

  String get_full_name(bool p_signature = false) const;
  String get_full_name_no_class() const;
  String get_ret_type_full_name() const;
  String get_signature_desc(bool p_namespaces = false) const;

  void get_parameter_names(Vector<StringName> &names) const;
  void get_parameter_types(Vector<ManagedType> &types) const;

  const MethodInfo &get_method_info();

  GDMonoMethod(StringName p_name, MonoMethod *p_method);
  ~GDMonoMethod();
};

#endif // GD_MONO_METHOD_H

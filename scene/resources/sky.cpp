
#include "sky.h"

#include "core/io/image_loader.h"

void Sky::set_radiance_size(RadianceSize p_size) {
  ERR_FAIL_INDEX(p_size, RADIANCE_SIZE_MAX);

  radiance_size = p_size;
  static const int size[RADIANCE_SIZE_MAX] = {32,  64,   128, 256,
                                              512, 1024, 2048};
  VS::get_singleton()->sky_set_radiance_size(sky, size[radiance_size]);
}

Sky::RadianceSize Sky::get_radiance_size() const { return radiance_size; }

void Sky::set_process_mode(ProcessMode p_mode) {
  mode = p_mode;
  VS::get_singleton()->sky_set_mode(sky, VS::SkyMode(mode));
}

Sky::ProcessMode Sky::get_process_mode() const { return mode; }

void Sky::set_material(const Ref<Material> &p_material) {
  sky_material = p_material;
  RID material_rid;
  if (sky_material.is_valid())
    material_rid = sky_material->get_rid();
  VS::get_singleton()->sky_set_material(sky, material_rid);
}

Ref<Material> Sky::get_material() const { return sky_material; }

RID Sky::get_rid() const { return sky; }

void Sky::_bind_methods() {

  ClassDB::bind_method(D_METHOD("set_radiance_size", "size"),
                       &Sky::set_radiance_size);
  ClassDB::bind_method(D_METHOD("get_radiance_size"), &Sky::get_radiance_size);

  ClassDB::bind_method(D_METHOD("set_process_mode", "mode"),
                       &Sky::set_process_mode);
  ClassDB::bind_method(D_METHOD("get_process_mode"), &Sky::get_process_mode);

  ClassDB::bind_method(D_METHOD("set_material", "material"),
                       &Sky::set_material);
  ClassDB::bind_method(D_METHOD("get_material"), &Sky::get_material);

  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sky_material",
                            PROPERTY_HINT_RESOURCE_TYPE,
                            "ShaderMaterial,PanoramaSkyMaterial,"
                            "ProceduralSkyMaterial,PhysicalSkyMaterial"),
               "set_material", "get_material");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "radiance_size", PROPERTY_HINT_ENUM,
                            "32,64,128,256,512,1024,2048"),
               "set_radiance_size", "get_radiance_size");
  ADD_PROPERTY(PropertyInfo(Variant::INT, "process_mode", PROPERTY_HINT_ENUM,
                            "HighQuality,RealTime"),
               "set_process_mode", "get_process_mode");

  BIND_ENUM_CONSTANT(RADIANCE_SIZE_32);
  BIND_ENUM_CONSTANT(RADIANCE_SIZE_64);
  BIND_ENUM_CONSTANT(RADIANCE_SIZE_128);
  BIND_ENUM_CONSTANT(RADIANCE_SIZE_256);
  BIND_ENUM_CONSTANT(RADIANCE_SIZE_512);
  BIND_ENUM_CONSTANT(RADIANCE_SIZE_1024);
  BIND_ENUM_CONSTANT(RADIANCE_SIZE_2048);
  BIND_ENUM_CONSTANT(RADIANCE_SIZE_MAX);

  BIND_ENUM_CONSTANT(PROCESS_MODE_QUALITY);
  BIND_ENUM_CONSTANT(PROCESS_MODE_REALTIME);
}

Sky::Sky() {
  mode = PROCESS_MODE_QUALITY;
  radiance_size = RADIANCE_SIZE_256;
  sky = VS::get_singleton()->sky_create();
}

Sky::~Sky() { VS::get_singleton()->free(sky); }
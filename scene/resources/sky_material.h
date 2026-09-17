
#include "core/rid.h"
#include "scene/resources/material.h"

#ifndef SKY_MATERIAL_H
#define SKY_MATERIAL_H

class ProceduralSkyMaterial : public Material {

  GDCLASS(ProceduralSkyMaterial, Material);

private:
  Color sky_top_color;
  Color sky_horizon_color;
  float sky_curve;
  float sky_energy;

  Color ground_bottom_color;
  Color ground_horizon_color;
  float ground_curve;
  float ground_energy;

  float sun_angle_min;
  float sun_angle_max;
  float sun_curve;

  RID shader;

protected:
  static void _bind_methods();
  virtual bool _can_do_next_pass() const;

public:
  void set_sky_top_color(const Color &p_sky_top);
  Color get_sky_top_color() const;

  void set_sky_horizon_color(const Color &p_sky_horizon);
  Color get_sky_horizon_color() const;

  void set_sky_curve(float p_curve);
  float get_sky_curve() const;

  void set_sky_energy(float p_energy);
  float get_sky_energy() const;

  void set_ground_bottom_color(const Color &p_ground_bottom);
  Color get_ground_bottom_color() const;

  void set_ground_horizon_color(const Color &p_ground_horizon);
  Color get_ground_horizon_color() const;

  void set_ground_curve(float p_curve);
  float get_ground_curve() const;

  void set_ground_energy(float p_energy);
  float get_ground_energy() const;

  void set_sun_angle_min(float p_angle);
  float get_sun_angle_min() const;

  void set_sun_angle_max(float p_angle);
  float get_sun_angle_max() const;

  void set_sun_curve(float p_curve);
  float get_sun_curve() const;

  virtual Shader::Mode get_shader_mode() const;
  RID get_shader_rid() const;

  ProceduralSkyMaterial();
  ~ProceduralSkyMaterial();
};

//////////////////////////////////////////////////////
/* PanoramaSkyMaterial */

class PanoramaSkyMaterial : public Material {
  GDCLASS(PanoramaSkyMaterial, Material);

private:
  Ref<Texture2D> panorama;
  RID shader;

protected:
  static void _bind_methods();
  virtual bool _can_do_next_pass() const;

public:
  void set_panorama(const Ref<Texture2D> &p_panorama);
  Ref<Texture2D> get_panorama() const;

  virtual Shader::Mode get_shader_mode() const;
  RID get_shader_rid() const;

  PanoramaSkyMaterial();
  ~PanoramaSkyMaterial();
};

//////////////////////////////////////////////////////
/* PanoramaSkyMaterial */

class PhysicalSkyMaterial : public Material {
  GDCLASS(PhysicalSkyMaterial, Material);

private:
  RID shader;

  float rayleigh;
  Color rayleigh_color;
  float mie;
  float mie_eccentricity;
  Color mie_color;
  float turbidity;
  float sun_disk_scale;
  Color ground_color;
  float exposure;
  float dither_strength;

protected:
  static void _bind_methods();
  virtual bool _can_do_next_pass() const;

public:
  void set_rayleigh_coefficient(float p_rayleigh);
  float get_rayleigh_coefficient() const;

  void set_rayleigh_color(Color p_rayleigh_color);
  Color get_rayleigh_color() const;

  void set_turbidity(float p_turbidity);
  float get_turbidity() const;

  void set_mie_coefficient(float p_mie);
  float get_mie_coefficient() const;

  void set_mie_eccentricity(float p_eccentricity);
  float get_mie_eccentricity() const;

  void set_mie_color(Color p_mie_color);
  Color get_mie_color() const;

  void set_sun_disk_scale(float p_sun_disk_scale);
  float get_sun_disk_scale() const;

  void set_ground_color(Color p_ground_color);
  Color get_ground_color() const;

  void set_exposure(float p_exposure);
  float get_exposure() const;

  void set_dither_strength(float p_dither_strength);
  float get_dither_strength() const;

  virtual Shader::Mode get_shader_mode() const;
  RID get_shader_rid() const;

  PhysicalSkyMaterial();
  ~PhysicalSkyMaterial();
};

#endif /* !SKY_MATERIAL_H */


#ifndef TEXTURE_BUTTON_H
#define TEXTURE_BUTTON_H

#include "scene/gui/base_button.h"
#include "scene/resources/bit_map.h"
class TextureButton : public BaseButton {

  GDCLASS(TextureButton, BaseButton);

public:
  enum StretchMode {
    STRETCH_SCALE,
    STRETCH_TILE,
    STRETCH_KEEP,
    STRETCH_KEEP_CENTERED,
    STRETCH_KEEP_ASPECT,
    STRETCH_KEEP_ASPECT_CENTERED,
    STRETCH_KEEP_ASPECT_COVERED,
  };

private:
  Ref<Texture2D> normal;
  Ref<Texture2D> pressed;
  Ref<Texture2D> hover;
  Ref<Texture2D> disabled;
  Ref<Texture2D> focused;
  Ref<BitMap> click_mask;
  bool expand;
  StretchMode stretch_mode;

  Rect2 _texture_region;
  Rect2 _position_rect;
  bool _tile;

protected:
  virtual Size2 get_minimum_size() const;
  virtual bool has_point(const Point2 &p_point) const;
  void _notification(int p_what);
  static void _bind_methods();

public:
  void set_normal_texture(const Ref<Texture2D> &p_normal);
  void set_pressed_texture(const Ref<Texture2D> &p_pressed);
  void set_hover_texture(const Ref<Texture2D> &p_hover);
  void set_disabled_texture(const Ref<Texture2D> &p_disabled);
  void set_focused_texture(const Ref<Texture2D> &p_focused);
  void set_click_mask(const Ref<BitMap> &p_click_mask);

  Ref<Texture2D> get_normal_texture() const;
  Ref<Texture2D> get_pressed_texture() const;
  Ref<Texture2D> get_hover_texture() const;
  Ref<Texture2D> get_disabled_texture() const;
  Ref<Texture2D> get_focused_texture() const;
  Ref<BitMap> get_click_mask() const;

  bool get_expand() const;
  void set_expand(bool p_expand);

  void set_stretch_mode(StretchMode p_stretch_mode);
  StretchMode get_stretch_mode() const;

  TextureButton();
};

VARIANT_ENUM_CAST(TextureButton::StretchMode);
#endif // TEXTURE_BUTTON_H

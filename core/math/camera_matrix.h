
/**
 * @class CameraMatrix
 * @brief Represents a 4x4 projection matrix for camera transformations.
 *
 * This class encapsulates camera projection matrices used for perspective and
 * orthogonal projections in 3D graphics. It provides methods to set up various
 * projection types, extract camera properties, and perform matrix operations.
 *
 * @member matrix[4][4] The 4x4 projection matrix data.
 *
 * @enum Planes
 * Enumeration for the six frustum planes.
 * - PLANE_NEAR: Near clipping plane
 * - PLANE_FAR: Far clipping plane
 * - PLANE_LEFT: Left frustum plane
 * - PLANE_TOP: Top frustum plane
 * - PLANE_RIGHT: Right frustum plane
 * - PLANE_BOTTOM: Bottom frustum plane
 *
 * @method set_identity() Sets the matrix to identity.
 * @method set_zero() Sets all matrix elements to zero.
 * @method set_light_bias() Applies light bias transformation to the matrix.
 * @method set_depth_correction(bool p_flip_y) Applies depth correction,
 * optionally flipping Y axis.
 * @method set_light_atlas_rect(const Rect2 &p_rect) Sets up matrix for light
 * atlas rectangle.
 * @method set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t
 * p_z_near, real_t p_z_far, bool p_flip_fov) Sets perspective projection.
 * @method set_perspective(..., int p_eye, real_t p_intraocular_dist, real_t
 * p_convergence_dist) Sets stereo perspective projection.
 * @method set_for_hmd(int p_eye, real_t p_aspect, real_t p_intraocular_dist,
 * real_t p_display_width, real_t p_display_to_lens, real_t p_oversample, real_t
 * p_z_near, real_t p_z_far) Sets HMD-specific projection.
 * @method set_orthogonal(...) Sets orthogonal projection with various parameter
 * configurations.
 * @method set_frustum(...) Sets frustum projection.
 * @method get_z_far() const Returns the far clipping plane distance.
 * @method get_z_near() const Returns the near clipping plane distance.
 * @method get_aspect() const Returns the aspect ratio.
 * @method get_fov() const Returns the field of view.
 * @method is_orthogonal() const Checks if this is an orthogonal projection.
 * @method get_projection_planes(const Transform &p_transform) const Returns
 * frustum planes in world space.
 * @method get_endpoints(const Transform &p_transform, Vector3 *p_8points) const
 * Extracts the 8 endpoints of the frustum.
 * @method get_viewport_half_extents() const Returns half extents of the
 * viewport.
 * @method get_far_plane_size(real_t &r_width, real_t &r_height) const
 * Calculates far plane dimensions.
 * @method invert() Inverts the matrix in-place.
 * @method inverse() const Returns the inverse matrix without modifying
 * original.
 * @method operator*(const CameraMatrix &p_matrix) const Multiplies two camera
 * matrices.
 * @method xform4(const Plane &p_vec4) const Transforms a 4D plane vector.
 * @method xform(const Vector3 &p_vec3) const Transforms a 3D vector, applying
 * perspective division.
 * @method scale_translate_to_fit(const AABB &p_aabb) Adjusts matrix to fit an
 * axis-aligned bounding box.
 * @method make_scale(const Vector3 &p_scale) Creates a scale transformation
 * matrix.
 * @method get_pixels_per_meter(int p_for_pixel_width) const Calculates pixel
 * density per meter.
 * @method flip_y() Flips the Y-axis of the projection.
 * @method operator==(const CameraMatrix &p_cam) const Equality comparison.
 * @method operator!=(const CameraMatrix &p_cam) const Inequality comparison.
 */
#ifndef CAMERA_MATRIX_H
#define CAMERA_MATRIX_H

#include "core/math/rect2.h"
#include "core/math/transform.h"

struct CameraMatrix {

  enum Planes {
    PLANE_NEAR,
    PLANE_FAR,
    PLANE_LEFT,
    PLANE_TOP,
    PLANE_RIGHT,
    PLANE_BOTTOM
  };

  real_t matrix[4][4];

  void set_identity();
  void set_zero();
  void set_light_bias();
  void set_depth_correction(bool p_flip_y = true);
  void set_light_atlas_rect(const Rect2 &p_rect);
  void set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near,
                       real_t p_z_far, bool p_flip_fov = false);
  void set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near,
                       real_t p_z_far, bool p_flip_fov, int p_eye,
                       real_t p_intraocular_dist, real_t p_convergence_dist);
  void set_for_hmd(int p_eye, real_t p_aspect, real_t p_intraocular_dist,
                   real_t p_display_width, real_t p_display_to_lens,
                   real_t p_oversample, real_t p_z_near, real_t p_z_far);
  void set_orthogonal(real_t p_left, real_t p_right, real_t p_bottom,
                      real_t p_top, real_t p_znear, real_t p_zfar);
  void set_orthogonal(real_t p_size, real_t p_aspect, real_t p_znear,
                      real_t p_zfar, bool p_flip_fov = false);
  void set_frustum(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top,
                   real_t p_near, real_t p_far);
  void set_frustum(real_t p_size, real_t p_aspect, Vector2 p_offset,
                   real_t p_near, real_t p_far, bool p_flip_fov = false);

  static real_t get_fovy(real_t p_fovx, real_t p_aspect) {

    return Math::rad2deg(
        Math::atan(p_aspect * Math::tan(Math::deg2rad(p_fovx) * 0.5)) * 2.0);
  }

  real_t get_z_far() const;
  real_t get_z_near() const;
  real_t get_aspect() const;
  real_t get_fov() const;
  bool is_orthogonal() const;

  Vector<Plane> get_projection_planes(const Transform &p_transform) const;

  bool get_endpoints(const Transform &p_transform, Vector3 *p_8points) const;
  Vector2 get_viewport_half_extents() const;
  void get_far_plane_size(real_t &r_width, real_t &r_height) const;

  void invert();
  CameraMatrix inverse() const;

  CameraMatrix operator*(const CameraMatrix &p_matrix) const;

  Plane xform4(const Plane &p_vec4) const;
  _FORCE_INLINE_ Vector3 xform(const Vector3 &p_vec3) const;

  operator String() const;

  void scale_translate_to_fit(const AABB &p_aabb);
  void make_scale(const Vector3 &p_scale);
  int get_pixels_per_meter(int p_for_pixel_width) const;
  operator Transform() const;

  void flip_y();

  bool operator==(const CameraMatrix &p_cam) const {
    for (uint32_t i = 0; i < 4; i++) {
      for (uint32_t j = 0; j < 4; j++) {
        if (matrix[i][j] != p_cam.matrix[i][j]) {
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const CameraMatrix &p_cam) const { return !(*this == p_cam); }

  CameraMatrix();
  CameraMatrix(const Transform &p_transform);
  ~CameraMatrix();
};

Vector3 CameraMatrix::xform(const Vector3 &p_vec3) const {

  Vector3 ret;
  ret.x = matrix[0][0] * p_vec3.x + matrix[1][0] * p_vec3.y +
          matrix[2][0] * p_vec3.z + matrix[3][0];
  ret.y = matrix[0][1] * p_vec3.x + matrix[1][1] * p_vec3.y +
          matrix[2][1] * p_vec3.z + matrix[3][1];
  ret.z = matrix[0][2] * p_vec3.x + matrix[1][2] * p_vec3.y +
          matrix[2][2] * p_vec3.z + matrix[3][2];
  real_t w = matrix[0][3] * p_vec3.x + matrix[1][3] * p_vec3.y +
             matrix[2][3] * p_vec3.z + matrix[3][3];
  return ret / w;
}

#endif // CAMERA_MATRIX_H

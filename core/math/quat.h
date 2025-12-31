/// @class Quat
/// @brief Represents a quaternion for 3D rotations.
///
/// A quaternion is a mathematical entity consisting of four components (x, y,
/// z, w) that efficiently represents 3D rotations without gimbal lock. The w
/// component is the scalar part, while x, y, z form the vector part.
///
/// @var real_t Quat::x
/// @brief The x component of the quaternion vector part.
///
/// @var real_t Quat::y
/// @brief The y component of the quaternion vector part.
///
/// @var real_t Quat::z
/// @brief The z component of the quaternion vector part.
///
/// @var real_t Quat::w
/// @brief The scalar (scalar) component of the quaternion.
///
/// @fn real_t Quat::length_squared() const
/// @brief Returns the squared length (magnitude) of the quaternion.
/// @return The squared length of this quaternion.
///
/// @fn real_t Quat::length() const
/// @brief Returns the length (magnitude) of the quaternion.
/// @return The length of this quaternion.
///
/// @fn bool Quat::is_equal_approx(const Quat &p_quat) const
/// @brief Checks if this quaternion is approximately equal to another.
/// @param p_quat The quaternion to compare with.
/// @return true if approximately equal, false otherwise.
///
/// @fn void Quat::normalize()
/// @brief Normalizes this quaternion in-place to unit length.
///
/// @fn Quat Quat::normalized() const
/// @brief Returns a normalized copy of this quaternion.
/// @return A new normalized quaternion.
///
/// @fn bool Quat::is_normalized() const
/// @brief Checks if this quaternion is normalized (unit length).
/// @return true if normalized, false otherwise.
///
/// @fn Quat Quat::inverse() const
/// @brief Returns the inverse (conjugate) of this quaternion.
/// @return The inverse quaternion.
///
/// @fn real_t Quat::dot(const Quat &q) const
/// @brief Computes the dot product with another quaternion.
/// @param q The quaternion to compute dot product with.
/// @return The dot product result.
///
/// @fn void Quat::set_euler_xyz(const Vector3 &p_euler)
/// @brief Sets the quaternion from Euler angles in XYZ order.
/// @param p_euler The Euler angles in radians.
///
/// @fn Vector3 Quat::get_euler_xyz() const
/// @brief Returns the quaternion as Euler angles in XYZ order.
/// @return Euler angles in radians.
///
/// @fn void Quat::set_euler_yxz(const Vector3 &p_euler)
/// @brief Sets the quaternion from Euler angles in YXZ order.
/// @param p_euler The Euler angles in radians.
///
/// @fn Vector3 Quat::get_euler_yxz() const
/// @brief Returns the quaternion as Euler angles in YXZ order.
/// @return Euler angles in radians.
///
/// @fn void Quat::set_euler(const Vector3 &p_euler)
/// @brief Sets the quaternion from Euler angles (default YXZ order).
/// @param p_euler The Euler angles in radians.
///
/// @fn Vector3 Quat::get_euler() const
/// @brief Returns the quaternion as Euler angles (default YXZ order).
/// @return Euler angles in radians.
///
/// @fn Quat Quat::slerp(const Quat &q, const real_t &t) const
/// @brief Spherical linear interpolation between this and another quaternion.
/// @param q The target quaternion.
/// @param t The interpolation parameter (0.0 to 1.0).
/// @return The interpolated quaternion.
///
/// @fn Quat Quat::slerpni(const Quat &q, const real_t &t) const
/// @brief Spherical linear interpolation (normalized interpolation).
/// @param q The target quaternion.
/// @param t The interpolation parameter (0.0 to 1.0).
/// @return The interpolated quaternion.
///
/// @fn Quat Quat::cubic_slerp(const Quat &q, const Quat &prep, const Quat
/// &postq, const real_t &t) const
/// @brief Cubic spherical interpolation for smooth rotational curves.
/// @param q The target quaternion.
/// @param prep The previous quaternion.
/// @param postq The post quaternion.
/// @param t The interpolation parameter (0.0 to 1.0).
/// @return The interpolated quaternion.
///
/// @fn void Quat::set_axis_angle(const Vector3 &axis, const real_t &angle)
/// @brief Sets the quaternion from an axis and rotation angle.
/// @param axis The rotation axis (should be normalized).
/// @param angle The rotation angle in radians.
///
/// @fn void Quat::get_axis_angle(Vector3 &r_axis, real_t &r_angle) const
/// @brief Extracts the rotation axis and angle from this quaternion.
/// @param r_axis Output parameter for the rotation axis.
/// @param r_angle Output parameter for the rotation angle in radians.
///
/// @fn Vector3 Quat::xform(const Vector3 &v) const
/// @brief Transforms a vector by this quaternion rotation.
/// @note The quaternion must be normalized for correct results.
/// @param v The vector to transform.
/// @return The transformed vector.
///
/// @fn Quat::Quat(real_t p_x, real_t p_y, real_t p_z, real_t p_w)
/// @brief Constructs a quaternion with explicit components.
/// @param p_x The x component.
/// @param p_y The y component.
/// @param p_z The z component.
/// @param p_w The w component.
///
/// @fn Quat::Quat(const Vector3 &axis, const real_t &angle)
/// @brief Constructs a quaternion from an axis and angle.
/// @param axis The rotation axis.
/// @param angle The rotation angle in radians.
///
/// @fn Quat::Quat(const Vector3 &euler)
/// @brief Constructs a quaternion from Euler angles.
/// @param euler The Euler angles in radians.
///
/// @fn Quat::Quat(const Vector3 &v0, const Vector3 &v1)
/// @brief Constructs a quaternion representing the shortest rotation from v0 to
/// v1.
/// @param v0 The source vector.
/// @param v1 The target vector.
///
/// @fn Quat::Quat()
/// @brief Constructs an identity quaternion (no rotation).
*
// Circular dependency between Vector3 and Basis :/
#include "core/math/vector3.h"

#ifndef QUAT_H
#define QUAT_H

#include "core/math/math_defs.h"
#include "core/math/math_funcs.h"
#include "core/ustring.h"

    class Quat {
public:
  real_t x, y, z, w;

  _FORCE_INLINE_ real_t length_squared() const;
  bool is_equal_approx(const Quat &p_quat) const;
  real_t length() const;
  void normalize();
  Quat normalized() const;
  bool is_normalized() const;
  Quat inverse() const;
  _FORCE_INLINE_ real_t dot(const Quat &q) const;

  void set_euler_xyz(const Vector3 &p_euler);
  Vector3 get_euler_xyz() const;
  void set_euler_yxz(const Vector3 &p_euler);
  Vector3 get_euler_yxz() const;

  void set_euler(const Vector3 &p_euler) { set_euler_yxz(p_euler); };
  Vector3 get_euler() const { return get_euler_yxz(); };

  Quat slerp(const Quat &q, const real_t &t) const;
  Quat slerpni(const Quat &q, const real_t &t) const;
  Quat cubic_slerp(const Quat &q, const Quat &prep, const Quat &postq,
                   const real_t &t) const;

  void set_axis_angle(const Vector3 &axis, const real_t &angle);
  _FORCE_INLINE_ void get_axis_angle(Vector3 &r_axis, real_t &r_angle) const {
    r_angle = 2 * Math::acos(w);
    real_t r = ((real_t)1) / Math::sqrt(1 - w * w);
    r_axis.x = x * r;
    r_axis.y = y * r;
    r_axis.z = z * r;
  }

  void operator*=(const Quat &q);
  Quat operator*(const Quat &q) const;

  Quat operator*(const Vector3 &v) const {
    return Quat(w * v.x + y * v.z - z * v.y, w * v.y + z * v.x - x * v.z,
                w * v.z + x * v.y - y * v.x, -x * v.x - y * v.y - z * v.z);
  }

  _FORCE_INLINE_ Vector3 xform(const Vector3 &v) const {
#ifdef MATH_CHECKS
    ERR_FAIL_COND_V_MSG(!is_normalized(), v,
                        "The quaternion must be normalized.");
#endif
    Vector3 u(x, y, z);
    Vector3 uv = u.cross(v);
    return v + ((uv * w) + u.cross(uv)) * ((real_t)2);
  }

  _FORCE_INLINE_ void operator+=(const Quat &q);
  _FORCE_INLINE_ void operator-=(const Quat &q);
  _FORCE_INLINE_ void operator*=(const real_t &s);
  _FORCE_INLINE_ void operator/=(const real_t &s);
  _FORCE_INLINE_ Quat operator+(const Quat &q2) const;
  _FORCE_INLINE_ Quat operator-(const Quat &q2) const;
  _FORCE_INLINE_ Quat operator-() const;
  _FORCE_INLINE_ Quat operator*(const real_t &s) const;
  _FORCE_INLINE_ Quat operator/(const real_t &s) const;

  _FORCE_INLINE_ bool operator==(const Quat &p_quat) const;
  _FORCE_INLINE_ bool operator!=(const Quat &p_quat) const;

  operator String() const;

  inline void set(real_t p_x, real_t p_y, real_t p_z, real_t p_w) {
    x = p_x;
    y = p_y;
    z = p_z;
    w = p_w;
  }
  inline Quat(real_t p_x, real_t p_y, real_t p_z, real_t p_w)
      : x(p_x), y(p_y), z(p_z), w(p_w) {}
  Quat(const Vector3 &axis, const real_t &angle) {
    set_axis_angle(axis, angle);
  }

  Quat(const Vector3 &euler) { set_euler(euler); }
  Quat(const Quat &q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

  Quat operator=(const Quat &q) {
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
    return *this;
  }

  Quat(const Vector3 &v0, const Vector3 &v1) // shortest arc
  {
    Vector3 c = v0.cross(v1);
    real_t d = v0.dot(v1);

    if (d < -1.0 + CMP_EPSILON) {
      x = 0;
      y = 1;
      z = 0;
      w = 0;
    } else {

      real_t s = Math::sqrt((1.0 + d) * 2.0);
      real_t rs = 1.0 / s;

      x = c.x * rs;
      y = c.y * rs;
      z = c.z * rs;
      w = s * 0.5;
    }
  }

  inline Quat() : x(0), y(0), z(0), w(1) {}
};

real_t Quat::dot(const Quat &q) const {
  return x * q.x + y * q.y + z * q.z + w * q.w;
}

real_t Quat::length_squared() const { return dot(*this); }

void Quat::operator+=(const Quat &q) {
  x += q.x;
  y += q.y;
  z += q.z;
  w += q.w;
}

void Quat::operator-=(const Quat &q) {
  x -= q.x;
  y -= q.y;
  z -= q.z;
  w -= q.w;
}

void Quat::operator*=(const real_t &s) {
  x *= s;
  y *= s;
  z *= s;
  w *= s;
}

void Quat::operator/=(const real_t &s) { *this *= 1.0 / s; }

Quat Quat::operator+(const Quat &q2) const {
  const Quat &q1 = *this;
  return Quat(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
}

Quat Quat::operator-(const Quat &q2) const {
  const Quat &q1 = *this;
  return Quat(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
}

Quat Quat::operator-() const {
  const Quat &q2 = *this;
  return Quat(-q2.x, -q2.y, -q2.z, -q2.w);
}

Quat Quat::operator*(const real_t &s) const {
  return Quat(x * s, y * s, z * s, w * s);
}

Quat Quat::operator/(const real_t &s) const { return *this * (1.0 / s); }

bool Quat::operator==(const Quat &p_quat) const {
  return x == p_quat.x && y == p_quat.y && z == p_quat.z && w == p_quat.w;
}

bool Quat::operator!=(const Quat &p_quat) const {
  return x != p_quat.x || y != p_quat.y || z != p_quat.z || w != p_quat.w;
}

#endif // QUAT_H

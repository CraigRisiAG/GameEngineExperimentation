
/**
 * @class Plane
 * @brief Represents a geometric plane in 3D space defined by a normal vector
 * and distance.
 *
 * A plane is defined by the equation: normal · point = d
 * where normal is the plane's normal vector and d is the distance from the
 * origin.
 *
 * @member normal Vector3 - The normal vector of the plane (perpendicular to the
 * plane surface)
 * @member d real_t - The distance component of the plane equation
 *
 * @section Methods
 *
 * @method set_normal(const Vector3 &p_normal)
 * Sets the normal vector of the plane.
 *
 * @method Vector3 get_normal() const
 * Returns the normal vector of the plane.
 *
 * @method void normalize()
 * Normalizes the plane's normal vector in-place.
 *
 * @method Plane normalized() const
 * Returns a new plane with a normalized normal vector.
 *
 * @method Vector3 center() const
 * Returns the center point of the plane (normal * d).
 *
 * @method Vector3 get_any_point() const
 * Returns any point that lies on the plane.
 *
 * @method Vector3 get_any_perpendicular_normal() const
 * Returns a vector perpendicular to the plane's normal.
 *
 * @method bool is_point_over(const Vector3 &p_point) const
 * Determines if a point is on the positive side of the plane (normal · point >
 * d).
 *
 * @method real_t distance_to(const Vector3 &p_point) const
 * Calculates the signed distance from a point to the plane.
 *
 * @method bool has_point(const Vector3 &p_point, real_t _epsilon = CMP_EPSILON)
 * const Checks if a point lies on the plane within a specified epsilon
 * tolerance.
 *
 * @method bool intersect_3(const Plane &p_plane1, const Plane &p_plane2,
 * Vector3 *r_result = 0) const Finds the intersection point of three planes.
 *
 * @method bool intersects_ray(const Vector3 &p_from, const Vector3 &p_dir,
 * Vector3 *p_intersection) const Checks if a ray intersects the plane and
 * returns the intersection point.
 *
 * @method bool intersects_segment(const Vector3 &p_begin, const Vector3 &p_end,
 * Vector3 *p_intersection) const Checks if a line segment intersects the plane
 * and returns the intersection point.
 *
 * @method Vector3 project(const Vector3 &p_point) const
 * Projects a point onto the plane.
 *
 * @method Plane operator-() const
 * Returns the negated plane (flips the normal and d).
 *
 * @method bool is_equal_approx(const Plane &p_plane) const
 * Checks if two planes are approximately equal.
 *
 * @method bool operator==(const Plane &p_plane) const
 * Checks exact equality with another plane.
 *
 * @method bool operator!=(const Plane &p_plane) const
 * Checks inequality with another plane.
 *
 * @method operator String() const
 * Converts the plane to a string representation.
 *
 * @constructor Plane()
 * Default constructor. Creates a plane at the origin with d = 0.
 *
 * @constructor Plane(real_t p_a, real_t p_b, real_t p_c, real_t p_d)
 * Constructs a plane from individual normal components and distance.
 *
 * @constructor Plane(const Vector3 &p_normal, real_t p_d)
 * Constructs a plane from a normal vector and distance value.
 *
 * @constructor Plane(const Vector3 &p_point, const Vector3 &p_normal)
 * Constructs a plane from a point and normal vector.
 *
 * @constructor Plane(const Vector3 &p_point1, const Vector3 &p_point2, const
 * Vector3 &p_point3, ClockDirection p_dir = CLOCKWISE) Constructs a plane from
 * three points with specified winding direction.
 */
#ifndef PLANE_H
#define PLANE_H

#include "core/math/vector3.h"

class Plane {
public:
  Vector3 normal;
  real_t d;

  void set_normal(const Vector3 &p_normal);
  _FORCE_INLINE_ Vector3 get_normal() const {
    return normal;
  }; /// Point is coplanar, CMP_EPSILON for precision

  void normalize();
  Plane normalized() const;

  /* Plane-Point operations */

  _FORCE_INLINE_ Vector3 center() const { return normal * d; }
  Vector3 get_any_point() const;
  Vector3 get_any_perpendicular_normal() const;

  _FORCE_INLINE_ bool
  is_point_over(const Vector3 &p_point) const; ///< Point is over plane
  _FORCE_INLINE_ real_t distance_to(const Vector3 &p_point) const;
  _FORCE_INLINE_ bool has_point(const Vector3 &p_point,
                                real_t _epsilon = CMP_EPSILON) const;

  /* intersections */

  bool intersect_3(const Plane &p_plane1, const Plane &p_plane2,
                   Vector3 *r_result = 0) const;
  bool intersects_ray(const Vector3 &p_from, const Vector3 &p_dir,
                      Vector3 *p_intersection) const;
  bool intersects_segment(const Vector3 &p_begin, const Vector3 &p_end,
                          Vector3 *p_intersection) const;

  _FORCE_INLINE_ Vector3 project(const Vector3 &p_point) const {

    return p_point - normal * distance_to(p_point);
  }

  /* misc */

  Plane operator-() const { return Plane(-normal, -d); }
  bool is_equal_approx(const Plane &p_plane) const;

  _FORCE_INLINE_ bool operator==(const Plane &p_plane) const;
  _FORCE_INLINE_ bool operator!=(const Plane &p_plane) const;
  operator String() const;

  _FORCE_INLINE_ Plane() : d(0) {}
  _FORCE_INLINE_ Plane(real_t p_a, real_t p_b, real_t p_c, real_t p_d)
      : normal(p_a, p_b, p_c), d(p_d) {}

  _FORCE_INLINE_ Plane(const Vector3 &p_normal, real_t p_d);
  _FORCE_INLINE_ Plane(const Vector3 &p_point, const Vector3 &p_normal);
  _FORCE_INLINE_ Plane(const Vector3 &p_point1, const Vector3 &p_point2,
                       const Vector3 &p_point3,
                       ClockDirection p_dir = CLOCKWISE);
};

bool Plane::is_point_over(const Vector3 &p_point) const {

  return (normal.dot(p_point) > d);
}

real_t Plane::distance_to(const Vector3 &p_point) const {

  return (normal.dot(p_point) - d);
}

bool Plane::has_point(const Vector3 &p_point, real_t _epsilon) const {

  real_t dist = normal.dot(p_point) - d;
  dist = ABS(dist);
  return (dist <= _epsilon);
}

Plane::Plane(const Vector3 &p_normal, real_t p_d) : normal(p_normal), d(p_d) {}

Plane::Plane(const Vector3 &p_point, const Vector3 &p_normal)
    : normal(p_normal), d(p_normal.dot(p_point)) {}

Plane::Plane(const Vector3 &p_point1, const Vector3 &p_point2,
             const Vector3 &p_point3, ClockDirection p_dir) {

  if (p_dir == CLOCKWISE)
    normal = (p_point1 - p_point3).cross(p_point1 - p_point2);
  else
    normal = (p_point1 - p_point2).cross(p_point1 - p_point3);

  normal.normalize();
  d = normal.dot(p_point1);
}

bool Plane::operator==(const Plane &p_plane) const {

  return normal == p_plane.normal && d == p_plane.d;
}

bool Plane::operator!=(const Plane &p_plane) const {

  return normal != p_plane.normal || d != p_plane.d;
}

#endif // PLANE_H



/// \class AABB
/// \brief Axis-Aligned Bounding Box (AABB) class for 3D spatial calculations.
///
/// Represents a box in 3D space defined by a position (minimum corner) and size (dimensions).
/// Provides methods for geometric operations including intersection testing, merging, and axis queries.
///
/// \note All intersection and geometric operations assume the AABB is axis-aligned (not rotated).

/// \fn real_t AABB::get_area() const
/// \brief Calculates the volume of the bounding box.
/// \return The volume as the product of width, height, and depth (size.x * size.y * size.z).

/// \fn bool AABB::operator==(const AABB &p_rval) const
/// \brief Checks if two AABBs are equal.
/// \param p_rval The AABB to compare against.
/// \return True if both position and size are identical, false otherwise.

/// \fn bool AABB::operator!=(const AABB &p_rval) const
/// \brief Checks if two AABBs are not equal.
/// \param p_rval The AABB to compare against.
/// \return True if position or size differs, false if identical.

/// \fn void AABB::merge_with(const AABB &p_aabb)
/// \brief Merges this AABB with another, expanding to contain both.
/// \param p_aabb The AABB to merge with.
/// \note Modifies this AABB in-place to be the smallest box containing both original boxes.

/// \fn bool AABB::is_equal_approx(const AABB &p_aabb) const
/// \brief Checks if two AABBs are approximately equal within floating-point tolerance.
/// \param p_aabb The AABB to compare against.
/// \return True if position and size are approximately equal, false otherwise.

/// \fn AABB AABB::intersection(const AABB &p_aabb) const
/// \brief Computes the intersection of this AABB with another.
/// \param p_aabb The AABB to intersect with.
/// \return A new AABB representing the overlapping region, or an empty AABB if no intersection exists.

/// \fn bool AABB::intersects_ray(const Vector3 &p_from, const Vector3 &p_dir, Vector3 *r_clip, Vector3 *r_normal) const
/// \brief Tests if a ray intersects this AABB.
/// \param p_from The ray's starting position.
/// \param p_dir The ray's direction vector.
/// \param r_clip Optional output parameter for the intersection point.
/// \param r_normal Optional output parameter for the surface normal at intersection.
/// \return True if the ray intersects the box, false otherwise.

/// \fn bool AABB::intersects_segment(const Vector3 &p_from, const Vector3 &p_to, Vector3 *r_clip, Vector3 *r_normal) const
/// \brief Tests if a line segment intersects this AABB.
/// \param p_from The segment's starting point.
/// \param p_to The segment's ending point.
/// \param r_clip Optional output parameter for the intersection point.
/// \param r_normal Optional output parameter for the surface normal at intersection.
/// \return True if the segment intersects the box, false otherwise.

/// \fn bool AABB::intersects_plane(const Plane &p_plane) const
/// \brief Tests if a plane intersects this AABB.
/// \param p_plane The plane to test against.
/// \return True if the plane intersects the box (has points on both sides), false otherwise.

/// \fn Vector3 AABB::get_longest_axis() const
/// \brief Returns a unit vector along the longest axis of the box.
/// \return A unit vector (1,0,0), (0,1,0), or (0,0,1) indicating the longest dimension.

/// \fn int AABB::get_longest_axis_index() const
/// \brief Returns the index of the longest axis.
/// \return 0 for X-axis, 1 for Y-axis, or 2 for Z-axis.

/// \fn Vector3 AABB::get_shortest_axis() const
/// \brief Returns a unit vector along the shortest axis of the box.
/// \return A unit vector (1,0,0), (0,1,0), or (0,0,1) indicating the shortest dimension.

/// \fn int AABB::get_shortest_axis_index() const
/// \brief Returns the index of the shortest axis.
/// \return 0 for X-axis, 1 for Y-axis, or 2 for Z-axis.

/// \fn AABB AABB::merge(const AABB &p_with) const
/// \brief Creates a new AABB that contains both this box and another.
/// \param p_with The AABB to merge with.
/// \return A new merged AABB without modifying the original.

/// \fn AABB AABB::expand(const Vector3 &p_vector) const
/// \brief Creates a new AABB expanded to include a point.
/// \param p_vector The point to expand to.
/// \return A new expanded AABB without modifying the original.

/// \fn AABB AABB::grow(real_t p_by) const
/// \brief Creates a new AABB grown uniformly in all directions.
/// \param p_by The distance to grow by.
/// \return A new grown AABB without modifying the original.

/// \fn void AABB::get_edge(int p_edge, Vector3 &r_from, Vector3 &r_to) const
/// \brief Retrieves the endpoints of an edge of the bounding box.
/// \param p_edge The edge index (0-11 for the 12 edges of a box).
/// \param r_from Output parameter for the edge's starting point.
/// \param r_to Output parameter for the edge's ending point.
/// \note p_edge must be in range [0, 12). Fails with ERR_FAIL_INDEX if out of range.

/// \fn AABB::operator String() const
/// \brief Converts the AABB to a string representation.
/// \return A string in the format "position - size".
#include "aabb.h"

#include "core/print_string.h"

real_t AABB::get_area() const { return size.x * size.y * size.z; }

bool AABB::operator==(const AABB &p_rval) const {

  return ((position == p_rval.position) && (size == p_rval.size));
}
bool AABB::operator!=(const AABB &p_rval) const {

  return ((position != p_rval.position) || (size != p_rval.size));
}

void AABB::merge_with(const AABB &p_aabb) {

  Vector3 beg_1, beg_2;
  Vector3 end_1, end_2;
  Vector3 min, max;

  beg_1 = position;
  beg_2 = p_aabb.position;
  end_1 = Vector3(size.x, size.y, size.z) + beg_1;
  end_2 = Vector3(p_aabb.size.x, p_aabb.size.y, p_aabb.size.z) + beg_2;

  min.x = (beg_1.x < beg_2.x) ? beg_1.x : beg_2.x;
  min.y = (beg_1.y < beg_2.y) ? beg_1.y : beg_2.y;
  min.z = (beg_1.z < beg_2.z) ? beg_1.z : beg_2.z;

  max.x = (end_1.x > end_2.x) ? end_1.x : end_2.x;
  max.y = (end_1.y > end_2.y) ? end_1.y : end_2.y;
  max.z = (end_1.z > end_2.z) ? end_1.z : end_2.z;

  position = min;
  size = max - min;
}

bool AABB::is_equal_approx(const AABB &p_aabb) const {

  return position.is_equal_approx(p_aabb.position) &&
         size.is_equal_approx(p_aabb.size);
}

AABB AABB::intersection(const AABB &p_aabb) const {

  Vector3 src_min = position;
  Vector3 src_max = position + size;
  Vector3 dst_min = p_aabb.position;
  Vector3 dst_max = p_aabb.position + p_aabb.size;

  Vector3 min, max;

  if (src_min.x > dst_max.x || src_max.x < dst_min.x)
    return AABB();
  else {

    min.x = (src_min.x > dst_min.x) ? src_min.x : dst_min.x;
    max.x = (src_max.x < dst_max.x) ? src_max.x : dst_max.x;
  }

  if (src_min.y > dst_max.y || src_max.y < dst_min.y)
    return AABB();
  else {

    min.y = (src_min.y > dst_min.y) ? src_min.y : dst_min.y;
    max.y = (src_max.y < dst_max.y) ? src_max.y : dst_max.y;
  }

  if (src_min.z > dst_max.z || src_max.z < dst_min.z)
    return AABB();
  else {

    min.z = (src_min.z > dst_min.z) ? src_min.z : dst_min.z;
    max.z = (src_max.z < dst_max.z) ? src_max.z : dst_max.z;
  }

  return AABB(min, max - min);
}

bool AABB::intersects_ray(const Vector3 &p_from, const Vector3 &p_dir,
                          Vector3 *r_clip, Vector3 *r_normal) const {

  Vector3 c1, c2;
  Vector3 end = position + size;
  real_t near = -1e20;
  real_t far = 1e20;
  int axis = 0;

  for (int i = 0; i < 3; i++) {
    if (p_dir[i] == 0) {
      if ((p_from[i] < position[i]) || (p_from[i] > end[i])) {
        return false;
      }
    } else { // ray not parallel to planes in this direction
      c1[i] = (position[i] - p_from[i]) / p_dir[i];
      c2[i] = (end[i] - p_from[i]) / p_dir[i];

      if (c1[i] > c2[i]) {
        SWAP(c1, c2);
      }
      if (c1[i] > near) {
        near = c1[i];
        axis = i;
      }
      if (c2[i] < far) {
        far = c2[i];
      }
      if ((near > far) || (far < 0)) {
        return false;
      }
    }
  }

  if (r_clip)
    *r_clip = c1;
  if (r_normal) {
    *r_normal = Vector3();
    (*r_normal)[axis] = p_dir[axis] ? -1 : 1;
  }

  return true;
}

bool AABB::intersects_segment(const Vector3 &p_from, const Vector3 &p_to,
                              Vector3 *r_clip, Vector3 *r_normal) const {

  real_t min = 0, max = 1;
  int axis = 0;
  real_t sign = 0;

  for (int i = 0; i < 3; i++) {
    real_t seg_from = p_from[i];
    real_t seg_to = p_to[i];
    real_t box_begin = position[i];
    real_t box_end = box_begin + size[i];
    real_t cmin, cmax;
    real_t csign;

    if (seg_from < seg_to) {

      if (seg_from > box_end || seg_to < box_begin)
        return false;
      real_t length = seg_to - seg_from;
      cmin = (seg_from < box_begin) ? ((box_begin - seg_from) / length) : 0;
      cmax = (seg_to > box_end) ? ((box_end - seg_from) / length) : 1;
      csign = -1.0;

    } else {

      if (seg_to > box_end || seg_from < box_begin)
        return false;
      real_t length = seg_to - seg_from;
      cmin = (seg_from > box_end) ? (box_end - seg_from) / length : 0;
      cmax = (seg_to < box_begin) ? (box_begin - seg_from) / length : 1;
      csign = 1.0;
    }

    if (cmin > min) {
      min = cmin;
      axis = i;
      sign = csign;
    }
    if (cmax < max)
      max = cmax;
    if (max < min)
      return false;
  }

  Vector3 rel = p_to - p_from;

  if (r_normal) {
    Vector3 normal;
    normal[axis] = sign;
    *r_normal = normal;
  }

  if (r_clip)
    *r_clip = p_from + rel * min;

  return true;
}

bool AABB::intersects_plane(const Plane &p_plane) const {

  Vector3 points[8] = {
      Vector3(position.x, position.y, position.z),
      Vector3(position.x, position.y, position.z + size.z),
      Vector3(position.x, position.y + size.y, position.z),
      Vector3(position.x, position.y + size.y, position.z + size.z),
      Vector3(position.x + size.x, position.y, position.z),
      Vector3(position.x + size.x, position.y, position.z + size.z),
      Vector3(position.x + size.x, position.y + size.y, position.z),
      Vector3(position.x + size.x, position.y + size.y, position.z + size.z),
  };

  bool over = false;
  bool under = false;

  for (int i = 0; i < 8; i++) {

    if (p_plane.distance_to(points[i]) > 0)
      over = true;
    else
      under = true;
  }

  return under && over;
}

Vector3 AABB::get_longest_axis() const {

  Vector3 axis(1, 0, 0);
  real_t max_size = size.x;

  if (size.y > max_size) {
    axis = Vector3(0, 1, 0);
    max_size = size.y;
  }

  if (size.z > max_size) {
    axis = Vector3(0, 0, 1);
  }

  return axis;
}
int AABB::get_longest_axis_index() const {

  int axis = 0;
  real_t max_size = size.x;

  if (size.y > max_size) {
    axis = 1;
    max_size = size.y;
  }

  if (size.z > max_size) {
    axis = 2;
  }

  return axis;
}

Vector3 AABB::get_shortest_axis() const {

  Vector3 axis(1, 0, 0);
  real_t max_size = size.x;

  if (size.y < max_size) {
    axis = Vector3(0, 1, 0);
    max_size = size.y;
  }

  if (size.z < max_size) {
    axis = Vector3(0, 0, 1);
  }

  return axis;
}
int AABB::get_shortest_axis_index() const {

  int axis = 0;
  real_t max_size = size.x;

  if (size.y < max_size) {
    axis = 1;
    max_size = size.y;
  }

  if (size.z < max_size) {
    axis = 2;
  }

  return axis;
}

AABB AABB::merge(const AABB &p_with) const {

  AABB aabb = *this;
  aabb.merge_with(p_with);
  return aabb;
}
AABB AABB::expand(const Vector3 &p_vector) const {
  AABB aabb = *this;
  aabb.expand_to(p_vector);
  return aabb;
}
AABB AABB::grow(real_t p_by) const {

  AABB aabb = *this;
  aabb.grow_by(p_by);
  return aabb;
}

void AABB::get_edge(int p_edge, Vector3 &r_from, Vector3 &r_to) const {

  ERR_FAIL_INDEX(p_edge, 12);
  switch (p_edge) {

  case 0: {

    r_from = Vector3(position.x + size.x, position.y, position.z);
    r_to = Vector3(position.x, position.y, position.z);
  } break;
  case 1: {

    r_from = Vector3(position.x + size.x, position.y, position.z + size.z);
    r_to = Vector3(position.x + size.x, position.y, position.z);
  } break;
  case 2: {
    r_from = Vector3(position.x, position.y, position.z + size.z);
    r_to = Vector3(position.x + size.x, position.y, position.z + size.z);

  } break;
  case 3: {

    r_from = Vector3(position.x, position.y, position.z);
    r_to = Vector3(position.x, position.y, position.z + size.z);

  } break;
  case 4: {

    r_from = Vector3(position.x, position.y + size.y, position.z);
    r_to = Vector3(position.x + size.x, position.y + size.y, position.z);
  } break;
  case 5: {

    r_from = Vector3(position.x + size.x, position.y + size.y, position.z);
    r_to =
        Vector3(position.x + size.x, position.y + size.y, position.z + size.z);
  } break;
  case 6: {
    r_from =
        Vector3(position.x + size.x, position.y + size.y, position.z + size.z);
    r_to = Vector3(position.x, position.y + size.y, position.z + size.z);

  } break;
  case 7: {

    r_from = Vector3(position.x, position.y + size.y, position.z + size.z);
    r_to = Vector3(position.x, position.y + size.y, position.z);

  } break;
  case 8: {

    r_from = Vector3(position.x, position.y, position.z + size.z);
    r_to = Vector3(position.x, position.y + size.y, position.z + size.z);

  } break;
  case 9: {

    r_from = Vector3(position.x, position.y, position.z);
    r_to = Vector3(position.x, position.y + size.y, position.z);

  } break;
  case 10: {

    r_from = Vector3(position.x + size.x, position.y, position.z);
    r_to = Vector3(position.x + size.x, position.y + size.y, position.z);

  } break;
  case 11: {

    r_from = Vector3(position.x + size.x, position.y, position.z + size.z);
    r_to =
        Vector3(position.x + size.x, position.y + size.y, position.z + size.z);

  } break;
  }
}

AABB::operator String() const { return String() + position + " - " + size; }

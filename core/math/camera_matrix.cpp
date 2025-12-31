
/**
 * @class CameraMatrix
 * @brief Represents a 4x4 matrix used for camera transformations and projections.
 * 
 * This class provides functionality for setting up various camera projection types
 * (perspective, orthogonal, frustum), extracting projection planes, and performing
 * matrix operations. It supports both standard and stereoscopic (VR/HMD) camera setups.
 * 
 * @details The matrix is stored in column-major order and can be used for:
 *   - Perspective and orthogonal projections
 *   - Stereoscopic rendering for VR/HMD devices
 *   - Frustum culling with plane extraction
 *   - Matrix transformations and inversions
 *   - Viewport and field-of-view calculations
 */

/**
 * @fn void set_identity()
 * @brief Sets the matrix to the identity matrix.
 */

/**
 * @fn void set_zero()
 * @brief Sets all matrix elements to zero.
 */

/**
 * @fn Plane xform4(const Plane &p_vec4) const
 * @brief Transforms a plane by this matrix.
 * @param p_vec4 The plane to transform.
 * @return The transformed plane.
 */

/**
 * @fn void set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov)
 * @brief Sets up a standard perspective projection matrix.
 * @param p_fovy_degrees Field of view angle in degrees (vertical).
 * @param p_aspect Aspect ratio (width/height).
 * @param p_z_near Distance to near clipping plane.
 * @param p_z_far Distance to far clipping plane.
 * @param p_flip_fov Whether to flip the field of view calculation.
 */

/**
 * @fn void set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov, int p_eye, real_t p_intraocular_dist, real_t p_convergence_dist)
 * @brief Sets up a stereoscopic perspective projection matrix for VR rendering.
 * @param p_fovy_degrees Field of view angle in degrees (vertical).
 * @param p_aspect Aspect ratio (width/height).
 * @param p_z_near Distance to near clipping plane.
 * @param p_z_far Distance to far clipping plane.
 * @param p_flip_fov Whether to flip the field of view calculation.
 * @param p_eye Eye index (1 = left, 2 = right, other = mono).
 * @param p_intraocular_dist Distance between the eyes.
 * @param p_convergence_dist Convergence distance for stereoscopic rendering.
 */

/**
 * @fn void set_for_hmd(int p_eye, real_t p_aspect, real_t p_intraocular_dist, real_t p_display_width, real_t p_display_to_lens, real_t p_oversample, real_t p_z_near, real_t p_z_far)
 * @brief Sets up a projection matrix optimized for head-mounted displays (HMD).
 * @param p_eye Eye index (1 = left, 2 = right, other = mono).
 * @param p_aspect Aspect ratio (width/height).
 * @param p_intraocular_dist Distance between the eyes.
 * @param p_display_width Width of the display.
 * @param p_display_to_lens Distance from display to lens.
 * @param p_oversample Oversampling factor to increase field of view.
 * @param p_z_near Distance to near clipping plane.
 * @param p_z_far Distance to far clipping plane.
 */

/**
 * @fn void set_orthogonal(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_znear, real_t p_zfar)
 * @brief Sets up an orthogonal projection matrix with explicit boundaries.
 * @param p_left Left plane boundary.
 * @param p_right Right plane boundary.
 * @param p_bottom Bottom plane boundary.
 * @param p_top Top plane boundary.
 * @param p_znear Distance to near clipping plane.
 * @param p_zfar Distance to far clipping plane.
 */

/**
 * @fn void set_orthogonal(real_t p_size, real_t p_aspect, real_t p_znear, real_t p_zfar, bool p_flip_fov)
 * @brief Sets up an orthogonal projection matrix with size and aspect ratio.
 * @param p_size Size of the viewport.
 * @param p_aspect Aspect ratio (width/height).
 * @param p_znear Distance to near clipping plane.
 * @param p_zfar Distance to far clipping plane.
 * @param p_flip_fov Whether to flip the field of view calculation.
 */

/**
 * @fn void set_frustum(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_near, real_t p_far)
 * @brief Sets up a frustum projection matrix with explicit boundaries.
 * @param p_left Left plane boundary.
 * @param p_right Right plane boundary.
 * @param p_bottom Bottom plane boundary.
 * @param p_top Top plane boundary.
 * @param p_near Distance to near clipping plane.
 * @param p_far Distance to far clipping plane.
 */

/**
 * @fn void set_frustum(real_t p_size, real_t p_aspect, Vector2 p_offset, real_t p_near, real_t p_far, bool p_flip_fov)
 * @brief Sets up a frustum projection matrix with offset.
 * @param p_size Size of the frustum.
 * @param p_aspect Aspect ratio (width/height).
 * @param p_offset Offset applied to frustum planes.
 * @param p_near Distance to near clipping plane.
 * @param p_far Distance to far clipping plane.
 * @param p_flip_fov Whether to flip the field of view calculation.
 */

/**
 * @fn real_t get_z_far() const
 * @brief Extracts the far clipping plane distance from the matrix.
 * @return The far clipping plane distance.
 */

/**
 * @fn real_t get_z_near() const
 * @brief Extracts the near clipping plane distance from the matrix.
 * @return The near clipping plane distance.
 */

/**
 * @fn Vector2 get_viewport_half_extents() const
 * @brief Calculates the half extents of the viewport at the near plane.
 * @return Vector2 containing half width and height.
 */

/**
 * @fn void get_far_plane_size(real_t &r_width, real_t &r_height) const
 * @brief Calculates the size of the far clipping plane.
 * @param r_width Output parameter for plane width.
 * @param r_height Output parameter for plane height.
 */

/**
 * @fn bool get_endpoints(const Transform &p_transform, Vector3 *p_8points) const
 * @brief Extracts the 8 corner points of the frustum.
 * @param p_transform Transform to apply to the points.
 * @param p_8points Array to store the 8 corner points.
 * @return True if successful, false otherwise.
 */

/**
 * @fn Vector<Plane> get_projection_planes(const Transform &p_transform) const
 * @brief Extracts the 6 frustum planes from the matrix.
 * @param p_transform Transform to apply to the planes.
 * @return Vector containing the 6 projection planes (near, far, left, top, right, bottom).
 */

/**
 * @fn CameraMatrix inverse() const
 * @brief Returns the inverse of this matrix.
 * @return A new CameraMatrix that is the inverse of this one.
 */

/**
 * @fn void invert()
 * @brief Inverts this matrix in place using Gaussian elimination with partial pivoting.
 */

/**
 * @fn void flip_y()
 * @brief Flips the Y-axis of the projection (negates the second row).
 */

/**
 * @fn CameraMatrix operator*(const CameraMatrix &p_matrix) const
 * @brief Multiplies this matrix by another matrix.
 * @param p_matrix The matrix to multiply with.
 * @return The resulting matrix product.
 */

/**
 * @fn void set_depth_correction(bool p_flip_y)
 * @brief Sets up a depth correction matrix for rendering API compatibility.
 * @param p_flip_y Whether to flip the Y-axis.
 */

/**
 * @fn void set_light_bias()
 * @brief Sets up a matrix for light space rendering with bias.
 */

/**
 * @fn void set_light_atlas_rect(const Rect2 &p_rect)
 * @brief Sets up a matrix for rendering to a light atlas rectangle.
 * @param p_rect The rectangular area in the light atlas.
 */

/**
 * @fn operator String() const
 * @brief Converts the matrix to a string representation.
 * @return String representation of the matrix.
 */

/**
 * @fn real_t get_aspect() const
 * @brief Extracts the aspect ratio from the projection.
 * @return The aspect ratio (width/height).
 */

/**
 * @fn int get_pixels_per_meter(int p_for_pixel_width) const
 * @brief Calculates pixels per meter for a given pixel width.
 * @param p_for_pixel_width The pixel width to use for calculation.
 * @return Pixels per meter value.
 */

/**
 * @fn bool is_orthogonal() const
 * @brief Checks if this matrix represents an orthogonal projection.
 * @return True if orthogonal, false if perspective.
 */

/**
 * @fn real_t get_fov() const
 * @brief Extracts the field of view angle from the matrix.
 * @return Field of view in degrees.
 */

/**
 * @fn void make_scale(const Vector3 &p_scale)
 * @brief Sets up a scaling matrix.
 * @param p_scale The scale factors for each axis.
 */

/**
 * @fn void scale_translate_to_fit(const AABB &p_aabb)
 * @brief Creates a matrix that scales and translates to fit an AABB.
 * @param p_aabb The bounding box to fit.
 */

/**
 * @fn operator Transform() const
 * @brief Converts the matrix to a Transform object.
 * @return A Transform representation of this matrix.
 */

/**
 * @fn CameraMatrix(const Transform &p_transform)
 * @brief Constructs a CameraMatrix from a Transform.
 * @param p_transform The transform to convert.
 */

/**
 * @fn CameraMatrix()
 * @brief Default constructor that initializes the matrix to identity.
 */

/**
 * @fn ~CameraMatrix()
 * @brief Destructor.
 */
#include "camera_matrix.h"

#include "core/math/math_funcs.h"
#include "core/print_string.h"

void CameraMatrix::set_identity() {

	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			matrix[i][j] = (i == j) ? 1 : 0;
		}
	}
}

void CameraMatrix::set_zero() {

	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			matrix[i][j] = 0;
		}
	}
}

Plane CameraMatrix::xform4(const Plane &p_vec4) const {

	Plane ret;

	ret.normal.x = matrix[0][0] * p_vec4.normal.x + matrix[1][0] * p_vec4.normal.y + matrix[2][0] * p_vec4.normal.z + matrix[3][0] * p_vec4.d;
	ret.normal.y = matrix[0][1] * p_vec4.normal.x + matrix[1][1] * p_vec4.normal.y + matrix[2][1] * p_vec4.normal.z + matrix[3][1] * p_vec4.d;
	ret.normal.z = matrix[0][2] * p_vec4.normal.x + matrix[1][2] * p_vec4.normal.y + matrix[2][2] * p_vec4.normal.z + matrix[3][2] * p_vec4.d;
	ret.d = matrix[0][3] * p_vec4.normal.x + matrix[1][3] * p_vec4.normal.y + matrix[2][3] * p_vec4.normal.z + matrix[3][3] * p_vec4.d;
	return ret;
}

void CameraMatrix::set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov) {

	if (p_flip_fov) {
		p_fovy_degrees = get_fovy(p_fovy_degrees, 1.0 / p_aspect);
	}

	real_t sine, cotangent, deltaZ;
	real_t radians = p_fovy_degrees / 2.0 * Math_PI / 180.0;

	deltaZ = p_z_far - p_z_near;
	sine = Math::sin(radians);

	if ((deltaZ == 0) || (sine == 0) || (p_aspect == 0)) {
		return;
	}
	cotangent = Math::cos(radians) / sine;

	set_identity();

	matrix[0][0] = cotangent / p_aspect;
	matrix[1][1] = cotangent;
	matrix[2][2] = -(p_z_far + p_z_near) / deltaZ;
	matrix[2][3] = -1;
	matrix[3][2] = -2 * p_z_near * p_z_far / deltaZ;
	matrix[3][3] = 0;
}

void CameraMatrix::set_perspective(real_t p_fovy_degrees, real_t p_aspect, real_t p_z_near, real_t p_z_far, bool p_flip_fov, int p_eye, real_t p_intraocular_dist, real_t p_convergence_dist) {
	if (p_flip_fov) {
		p_fovy_degrees = get_fovy(p_fovy_degrees, 1.0 / p_aspect);
	}

	real_t left, right, modeltranslation, ymax, xmax, frustumshift;

	ymax = p_z_near * tan(p_fovy_degrees * Math_PI / 360.0f);
	xmax = ymax * p_aspect;
	frustumshift = (p_intraocular_dist / 2.0) * p_z_near / p_convergence_dist;

	switch (p_eye) {
		case 1: { // left eye
			left = -xmax + frustumshift;
			right = xmax + frustumshift;
			modeltranslation = p_intraocular_dist / 2.0;
		}; break;
		case 2: { // right eye
			left = -xmax - frustumshift;
			right = xmax - frustumshift;
			modeltranslation = -p_intraocular_dist / 2.0;
		}; break;
		default: { // mono, should give the same result as set_perspective(p_fovy_degrees,p_aspect,p_z_near,p_z_far,p_flip_fov)
			left = -xmax;
			right = xmax;
			modeltranslation = 0.0;
		}; break;
	};

	set_frustum(left, right, -ymax, ymax, p_z_near, p_z_far);

	// translate matrix by (modeltranslation, 0.0, 0.0)
	CameraMatrix cm;
	cm.set_identity();
	cm.matrix[3][0] = modeltranslation;
	*this = *this * cm;
}

void CameraMatrix::set_for_hmd(int p_eye, real_t p_aspect, real_t p_intraocular_dist, real_t p_display_width, real_t p_display_to_lens, real_t p_oversample, real_t p_z_near, real_t p_z_far) {
	// we first calculate our base frustum on our values without taking our lens magnification into account.
	real_t f1 = (p_intraocular_dist * 0.5) / p_display_to_lens;
	real_t f2 = ((p_display_width - p_intraocular_dist) * 0.5) / p_display_to_lens;
	real_t f3 = (p_display_width / 4.0) / p_display_to_lens;

	// now we apply our oversample factor to increase our FOV. how much we oversample is always a balance we strike between performance and how much
	// we're willing to sacrifice in FOV.
	real_t add = ((f1 + f2) * (p_oversample - 1.0)) / 2.0;
	f1 += add;
	f2 += add;
	f3 *= p_oversample;

	// always apply KEEP_WIDTH aspect ratio
	f3 *= p_aspect;

	switch (p_eye) {
		case 1: { // left eye
			set_frustum(-f2 * p_z_near, f1 * p_z_near, -f3 * p_z_near, f3 * p_z_near, p_z_near, p_z_far);
		}; break;
		case 2: { // right eye
			set_frustum(-f1 * p_z_near, f2 * p_z_near, -f3 * p_z_near, f3 * p_z_near, p_z_near, p_z_far);
		}; break;
		default: { // mono, does not apply here!
		}; break;
	};
};

void CameraMatrix::set_orthogonal(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_znear, real_t p_zfar) {

	set_identity();

	matrix[0][0] = 2.0 / (p_right - p_left);
	matrix[3][0] = -((p_right + p_left) / (p_right - p_left));
	matrix[1][1] = 2.0 / (p_top - p_bottom);
	matrix[3][1] = -((p_top + p_bottom) / (p_top - p_bottom));
	matrix[2][2] = -2.0 / (p_zfar - p_znear);
	matrix[3][2] = -((p_zfar + p_znear) / (p_zfar - p_znear));
	matrix[3][3] = 1.0;
}

void CameraMatrix::set_orthogonal(real_t p_size, real_t p_aspect, real_t p_znear, real_t p_zfar, bool p_flip_fov) {

	if (!p_flip_fov) {
		p_size *= p_aspect;
	}

	set_orthogonal(-p_size / 2, +p_size / 2, -p_size / p_aspect / 2, +p_size / p_aspect / 2, p_znear, p_zfar);
}

void CameraMatrix::set_frustum(real_t p_left, real_t p_right, real_t p_bottom, real_t p_top, real_t p_near, real_t p_far) {

	ERR_FAIL_COND(p_right <= p_left);
	ERR_FAIL_COND(p_top <= p_bottom);
	ERR_FAIL_COND(p_far <= p_near);

	real_t *te = &matrix[0][0];
	real_t x = 2 * p_near / (p_right - p_left);
	real_t y = 2 * p_near / (p_top - p_bottom);

	real_t a = (p_right + p_left) / (p_right - p_left);
	real_t b = (p_top + p_bottom) / (p_top - p_bottom);
	real_t c = -(p_far + p_near) / (p_far - p_near);
	real_t d = -2 * p_far * p_near / (p_far - p_near);

	te[0] = x;
	te[1] = 0;
	te[2] = 0;
	te[3] = 0;
	te[4] = 0;
	te[5] = y;
	te[6] = 0;
	te[7] = 0;
	te[8] = a;
	te[9] = b;
	te[10] = c;
	te[11] = -1;
	te[12] = 0;
	te[13] = 0;
	te[14] = d;
	te[15] = 0;
}

void CameraMatrix::set_frustum(real_t p_size, real_t p_aspect, Vector2 p_offset, real_t p_near, real_t p_far, bool p_flip_fov) {
	if (!p_flip_fov) {
		p_size *= p_aspect;
	}

	set_frustum(-p_size / 2 + p_offset.x, +p_size / 2 + p_offset.x, -p_size / p_aspect / 2 + p_offset.y, +p_size / p_aspect / 2 + p_offset.y, p_near, p_far);
}

real_t CameraMatrix::get_z_far() const {

	const real_t *matrix = (const real_t *)this->matrix;
	Plane new_plane = Plane(matrix[3] - matrix[2],
			matrix[7] - matrix[6],
			matrix[11] - matrix[10],
			matrix[15] - matrix[14]);

	new_plane.normal = -new_plane.normal;
	new_plane.normalize();

	return new_plane.d;
}
real_t CameraMatrix::get_z_near() const {

	const real_t *matrix = (const real_t *)this->matrix;
	Plane new_plane = Plane(matrix[3] + matrix[2],
			matrix[7] + matrix[6],
			matrix[11] + matrix[10],
			-matrix[15] - matrix[14]);

	new_plane.normalize();
	return new_plane.d;
}

Vector2 CameraMatrix::get_viewport_half_extents() const {

	const real_t *matrix = (const real_t *)this->matrix;
	///////--- Near Plane ---///////
	Plane near_plane = Plane(matrix[3] + matrix[2],
			matrix[7] + matrix[6],
			matrix[11] + matrix[10],
			-matrix[15] - matrix[14]);
	near_plane.normalize();

	///////--- Right Plane ---///////
	Plane right_plane = Plane(matrix[3] - matrix[0],
			matrix[7] - matrix[4],
			matrix[11] - matrix[8],
			-matrix[15] + matrix[12]);
	right_plane.normalize();

	Plane top_plane = Plane(matrix[3] - matrix[1],
			matrix[7] - matrix[5],
			matrix[11] - matrix[9],
			-matrix[15] + matrix[13]);
	top_plane.normalize();

	Vector3 res;
	near_plane.intersect_3(right_plane, top_plane, &res);

	return Vector2(res.x, res.y);
}

void CameraMatrix::get_far_plane_size(real_t &r_width, real_t &r_height) const {

	const real_t *matrix = (const real_t *)this->matrix;
	///////--- Far Plane ---///////
	Plane far_plane = Plane(matrix[3] - matrix[2],
			matrix[7] - matrix[6],
			matrix[11] - matrix[10],
			-matrix[15] + matrix[14]);
	far_plane.normalize();

	///////--- Right Plane ---///////
	Plane right_plane = Plane(matrix[3] - matrix[0],
			matrix[7] - matrix[4],
			matrix[11] - matrix[8],
			-matrix[15] + matrix[12]);
	right_plane.normalize();

	Plane top_plane = Plane(matrix[3] - matrix[1],
			matrix[7] - matrix[5],
			matrix[11] - matrix[9],
			-matrix[15] + matrix[13]);
	top_plane.normalize();

	Vector3 res;
	far_plane.intersect_3(right_plane, top_plane, &res);

	r_width = res.x;
	r_height = res.y;
}

bool CameraMatrix::get_endpoints(const Transform &p_transform, Vector3 *p_8points) const {

	Vector<Plane> planes = get_projection_planes(Transform());
	const Planes intersections[8][3] = {
		{ PLANE_FAR, PLANE_LEFT, PLANE_TOP },
		{ PLANE_FAR, PLANE_LEFT, PLANE_BOTTOM },
		{ PLANE_FAR, PLANE_RIGHT, PLANE_TOP },
		{ PLANE_FAR, PLANE_RIGHT, PLANE_BOTTOM },
		{ PLANE_NEAR, PLANE_LEFT, PLANE_TOP },
		{ PLANE_NEAR, PLANE_LEFT, PLANE_BOTTOM },
		{ PLANE_NEAR, PLANE_RIGHT, PLANE_TOP },
		{ PLANE_NEAR, PLANE_RIGHT, PLANE_BOTTOM },
	};

	for (int i = 0; i < 8; i++) {

		Vector3 point;
		bool res = planes[intersections[i][0]].intersect_3(planes[intersections[i][1]], planes[intersections[i][2]], &point);
		ERR_FAIL_COND_V(!res, false);
		p_8points[i] = p_transform.xform(point);
	}

	return true;
}

Vector<Plane> CameraMatrix::get_projection_planes(const Transform &p_transform) const {

	/** Fast Plane Extraction from combined modelview/projection matrices.
	 * References:
	 * https://web.archive.org/web/20011221205252/http://www.markmorley.com/opengl/frustumculling.html
	 * https://web.archive.org/web/20061020020112/http://www2.ravensoft.com/users/ggribb/plane%20extraction.pdf
	 */

	Vector<Plane> planes;

	const real_t *matrix = (const real_t *)this->matrix;

	Plane new_plane;

	///////--- Near Plane ---///////
	new_plane = Plane(matrix[3] + matrix[2],
			matrix[7] + matrix[6],
			matrix[11] + matrix[10],
			matrix[15] + matrix[14]);

	new_plane.normal = -new_plane.normal;
	new_plane.normalize();

	planes.push_back(p_transform.xform(new_plane));

	///////--- Far Plane ---///////
	new_plane = Plane(matrix[3] - matrix[2],
			matrix[7] - matrix[6],
			matrix[11] - matrix[10],
			matrix[15] - matrix[14]);

	new_plane.normal = -new_plane.normal;
	new_plane.normalize();

	planes.push_back(p_transform.xform(new_plane));

	///////--- Left Plane ---///////
	new_plane = Plane(matrix[3] + matrix[0],
			matrix[7] + matrix[4],
			matrix[11] + matrix[8],
			matrix[15] + matrix[12]);

	new_plane.normal = -new_plane.normal;
	new_plane.normalize();

	planes.push_back(p_transform.xform(new_plane));

	///////--- Top Plane ---///////
	new_plane = Plane(matrix[3] - matrix[1],
			matrix[7] - matrix[5],
			matrix[11] - matrix[9],
			matrix[15] - matrix[13]);

	new_plane.normal = -new_plane.normal;
	new_plane.normalize();

	planes.push_back(p_transform.xform(new_plane));

	///////--- Right Plane ---///////
	new_plane = Plane(matrix[3] - matrix[0],
			matrix[7] - matrix[4],
			matrix[11] - matrix[8],
			matrix[15] - matrix[12]);

	new_plane.normal = -new_plane.normal;
	new_plane.normalize();

	planes.push_back(p_transform.xform(new_plane));

	///////--- Bottom Plane ---///////
	new_plane = Plane(matrix[3] + matrix[1],
			matrix[7] + matrix[5],
			matrix[11] + matrix[9],
			matrix[15] + matrix[13]);

	new_plane.normal = -new_plane.normal;
	new_plane.normalize();

	planes.push_back(p_transform.xform(new_plane));

	return planes;
}

CameraMatrix CameraMatrix::inverse() const {

	CameraMatrix cm = *this;
	cm.invert();
	return cm;
}

void CameraMatrix::invert() {

	int i, j, k;
	int pvt_i[4], pvt_j[4]; /* Locations of pivot matrix */
	real_t pvt_val; /* Value of current pivot element */
	real_t hold; /* Temporary storage */
	real_t determinat; /* Determinant */

	determinat = 1.0;
	for (k = 0; k < 4; k++) {
		/** Locate k'th pivot element **/
		pvt_val = matrix[k][k]; /** Initialize for search **/
		pvt_i[k] = k;
		pvt_j[k] = k;
		for (i = k; i < 4; i++) {
			for (j = k; j < 4; j++) {
				if (Math::absd(matrix[i][j]) > Math::absd(pvt_val)) {
					pvt_i[k] = i;
					pvt_j[k] = j;
					pvt_val = matrix[i][j];
				}
			}
		}

		/** Product of pivots, gives determinant when finished **/
		determinat *= pvt_val;
		if (Math::absd(determinat) < 1e-7) {
			return; //(false);  /** Matrix is singular (zero determinant). **/
		}

		/** "Interchange" rows (with sign change stuff) **/
		i = pvt_i[k];
		if (i != k) { /** If rows are different **/
			for (j = 0; j < 4; j++) {
				hold = -matrix[k][j];
				matrix[k][j] = matrix[i][j];
				matrix[i][j] = hold;
			}
		}

		/** "Interchange" columns **/
		j = pvt_j[k];
		if (j != k) { /** If columns are different **/
			for (i = 0; i < 4; i++) {
				hold = -matrix[i][k];
				matrix[i][k] = matrix[i][j];
				matrix[i][j] = hold;
			}
		}

		/** Divide column by minus pivot value **/
		for (i = 0; i < 4; i++) {
			if (i != k) matrix[i][k] /= (-pvt_val);
		}

		/** Reduce the matrix **/
		for (i = 0; i < 4; i++) {
			hold = matrix[i][k];
			for (j = 0; j < 4; j++) {
				if (i != k && j != k) matrix[i][j] += hold * matrix[k][j];
			}
		}

		/** Divide row by pivot **/
		for (j = 0; j < 4; j++) {
			if (j != k) matrix[k][j] /= pvt_val;
		}

		/** Replace pivot by reciprocal (at last we can touch it). **/
		matrix[k][k] = 1.0 / pvt_val;
	}

	/* That was most of the work, one final pass of row/column interchange */
	/* to finish */
	for (k = 4 - 2; k >= 0; k--) { /* Don't need to work with 1 by 1 corner*/
		i = pvt_j[k]; /* Rows to swap correspond to pivot COLUMN */
		if (i != k) { /* If rows are different */
			for (j = 0; j < 4; j++) {
				hold = matrix[k][j];
				matrix[k][j] = -matrix[i][j];
				matrix[i][j] = hold;
			}
		}

		j = pvt_i[k]; /* Columns to swap correspond to pivot ROW */
		if (j != k) /* If columns are different */
			for (i = 0; i < 4; i++) {
				hold = matrix[i][k];
				matrix[i][k] = -matrix[i][j];
				matrix[i][j] = hold;
			}
	}
}

void CameraMatrix::flip_y() {
	for (int i = 0; i < 4; i++) {
		matrix[1][i] = -matrix[1][i];
	}
}

CameraMatrix::CameraMatrix() {

	set_identity();
}

CameraMatrix CameraMatrix::operator*(const CameraMatrix &p_matrix) const {

	CameraMatrix new_matrix;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			real_t ab = 0;
			for (int k = 0; k < 4; k++)
				ab += matrix[k][i] * p_matrix.matrix[j][k];
			new_matrix.matrix[j][i] = ab;
		}
	}

	return new_matrix;
}

void CameraMatrix::set_depth_correction(bool p_flip_y) {

	real_t *m = &matrix[0][0];

	m[0] = 1;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;
	m[4] = 0.0;
	m[5] = p_flip_y ? -1 : 1;
	m[6] = 0.0;
	m[7] = 0.0;
	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = 0.5;
	m[11] = 0.0;
	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.5;
	m[15] = 1.0;
}

void CameraMatrix::set_light_bias() {

	real_t *m = &matrix[0][0];

	m[0] = 0.5;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;
	m[4] = 0.0;
	m[5] = 0.5;
	m[6] = 0.0;
	m[7] = 0.0;
	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = 0.5;
	m[11] = 0.0;
	m[12] = 0.5;
	m[13] = 0.5;
	m[14] = 0.5;
	m[15] = 1.0;
}

void CameraMatrix::set_light_atlas_rect(const Rect2 &p_rect) {

	real_t *m = &matrix[0][0];

	m[0] = p_rect.size.width;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;
	m[4] = 0.0;
	m[5] = p_rect.size.height;
	m[6] = 0.0;
	m[7] = 0.0;
	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = 1.0;
	m[11] = 0.0;
	m[12] = p_rect.position.x;
	m[13] = p_rect.position.y;
	m[14] = 0.0;
	m[15] = 1.0;
}

CameraMatrix::operator String() const {

	String str;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			str += String((j > 0) ? ", " : "\n") + rtos(matrix[i][j]);

	return str;
}

real_t CameraMatrix::get_aspect() const {

	Vector2 vp_he = get_viewport_half_extents();
	return vp_he.x / vp_he.y;
}

int CameraMatrix::get_pixels_per_meter(int p_for_pixel_width) const {

	Vector3 result = xform(Vector3(1, 0, -1));

	return int((result.x * 0.5 + 0.5) * p_for_pixel_width);
}

bool CameraMatrix::is_orthogonal() const {

	return matrix[3][3] == 1.0;
}

real_t CameraMatrix::get_fov() const {
	const real_t *matrix = (const real_t *)this->matrix;

	Plane right_plane = Plane(matrix[3] - matrix[0],
			matrix[7] - matrix[4],
			matrix[11] - matrix[8],
			-matrix[15] + matrix[12]);
	right_plane.normalize();

	if ((matrix[8] == 0) && (matrix[9] == 0)) {
		return Math::rad2deg(Math::acos(Math::abs(right_plane.normal.x))) * 2.0;
	} else {
		// our frustum is asymmetrical need to calculate the left planes angle separately..
		Plane left_plane = Plane(matrix[3] + matrix[0],
				matrix[7] + matrix[4],
				matrix[11] + matrix[8],
				matrix[15] + matrix[12]);
		left_plane.normalize();

		return Math::rad2deg(Math::acos(Math::abs(left_plane.normal.x))) + Math::rad2deg(Math::acos(Math::abs(right_plane.normal.x)));
	}
}

void CameraMatrix::make_scale(const Vector3 &p_scale) {

	set_identity();
	matrix[0][0] = p_scale.x;
	matrix[1][1] = p_scale.y;
	matrix[2][2] = p_scale.z;
}

void CameraMatrix::scale_translate_to_fit(const AABB &p_aabb) {

	Vector3 min = p_aabb.position;
	Vector3 max = p_aabb.position + p_aabb.size;

	matrix[0][0] = 2 / (max.x - min.x);
	matrix[1][0] = 0;
	matrix[2][0] = 0;
	matrix[3][0] = -(max.x + min.x) / (max.x - min.x);

	matrix[0][1] = 0;
	matrix[1][1] = 2 / (max.y - min.y);
	matrix[2][1] = 0;
	matrix[3][1] = -(max.y + min.y) / (max.y - min.y);

	matrix[0][2] = 0;
	matrix[1][2] = 0;
	matrix[2][2] = 2 / (max.z - min.z);
	matrix[3][2] = -(max.z + min.z) / (max.z - min.z);

	matrix[0][3] = 0;
	matrix[1][3] = 0;
	matrix[2][3] = 0;
	matrix[3][3] = 1;
}

CameraMatrix::operator Transform() const {

	Transform tr;
	const real_t *m = &matrix[0][0];

	tr.basis.elements[0][0] = m[0];
	tr.basis.elements[1][0] = m[1];
	tr.basis.elements[2][0] = m[2];

	tr.basis.elements[0][1] = m[4];
	tr.basis.elements[1][1] = m[5];
	tr.basis.elements[2][1] = m[6];

	tr.basis.elements[0][2] = m[8];
	tr.basis.elements[1][2] = m[9];
	tr.basis.elements[2][2] = m[10];

	tr.origin.x = m[12];
	tr.origin.y = m[13];
	tr.origin.z = m[14];

	return tr;
}

CameraMatrix::CameraMatrix(const Transform &p_transform) {

	const Transform &tr = p_transform;
	real_t *m = &matrix[0][0];

	m[0] = tr.basis.elements[0][0];
	m[1] = tr.basis.elements[1][0];
	m[2] = tr.basis.elements[2][0];
	m[3] = 0.0;
	m[4] = tr.basis.elements[0][1];
	m[5] = tr.basis.elements[1][1];
	m[6] = tr.basis.elements[2][1];
	m[7] = 0.0;
	m[8] = tr.basis.elements[0][2];
	m[9] = tr.basis.elements[1][2];
	m[10] = tr.basis.elements[2][2];
	m[11] = 0.0;
	m[12] = tr.origin.x;
	m[13] = tr.origin.y;
	m[14] = tr.origin.z;
	m[15] = 1.0;
}

CameraMatrix::~CameraMatrix() {
}

/*
REAL_MATH.C
*/

/* ---------- headers */

#include "cseries.h"
#include "real_math.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

static __inline real magnitude_squared3d_right_associated(
	real_vector3d const *vector)
{
	return
		vector->i * vector->i +
		(vector->j * vector->j + vector->k * vector->k);
}

/* ---------- globals */

static real_matrix4x3 const private_identity4x3 =
{
	1.f,
	{
		{ 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f }
	},
};

static real_matrix4x3 const private_negative_identity4x3 =
{
	1.f,
	{
		{ -1.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f }
	},
};

static real_quaternion const private_identity_quaternion =
{
	{ { 0.f, 0.f, 0.f } },
	1.f,
};

static real_rectangle3d const private_null_rectangle =
{
	{ REAL_MAX, REAL_MIN, REAL_MAX, REAL_MIN, REAL_MAX, REAL_MIN },
};

short const global_projection3d_mappings[3][2][2] =
{
	{
		{ 2, 1 },
		{ 1, 2 },
	},
	{
		{ 0, 2 },
		{ 2, 0 },
	},
	{
		{ 1, 0 },
		{ 0, 1 },
	}
};

const real_point2d *global_origin2d = (const real_point2d *)&private_identity4x3.up;
const real_point2d *global_x_axis2d = (const real_point2d *)&private_identity4x3.forward;
const real_point2d *global_y_axis2d = (const real_point2d *)&private_identity4x3.left;
const real_point2d *global_negative_x_axis2d = (const real_point2d *)&private_negative_identity4x3.forward;
const real_point2d *global_negative_y_axis2d = (const real_point2d *)&private_negative_identity4x3.left;

const real_vector2d *global_zero_vector2d = (const real_vector2d *)&private_identity4x3.up;
const real_vector2d *global_forward2d = (const real_vector2d *)&private_identity4x3.forward;
const real_vector2d *global_left2d = (const real_vector2d *)&private_identity4x3.left;
const real_vector2d *global_backward2d = (const real_vector2d *)&private_negative_identity4x3.forward;
const real_vector2d *global_right2d = (const real_vector2d *)&private_negative_identity4x3.left;

const real_point3d *global_origin3d = (const real_point3d *)&private_identity4x3.position;

const real_point3d *global_x_axis3d = (const real_point3d *)&private_identity4x3.forward;
const real_point3d *global_y_axis3d = (const real_point3d *)&private_identity4x3.left;
const real_point3d *global_z_axis3d = (const real_point3d *)&private_identity4x3.up;
const real_point3d *global_negative_x_axis3d = (const real_point3d *)&private_negative_identity4x3.forward;
const real_point3d *global_negative_y_axis3d = (const real_point3d *)&private_negative_identity4x3.left;
const real_point3d *global_negative_z_axis3d = (const real_point3d *)&private_negative_identity4x3.up;

const real_vector3d *global_zero_vector3d = (const real_vector3d *)&private_identity4x3.position;

const real_vector3d *global_forward3d = (const real_vector3d *)&private_identity4x3.forward;
const real_vector3d *global_left3d = (const real_vector3d *)&private_identity4x3.left;
const real_vector3d *global_up3d = (const real_vector3d *)&private_identity4x3.up;

const real_vector3d *global_backward3d = (const real_vector3d *)&private_negative_identity4x3.forward;
const real_vector3d *global_right3d = (const real_vector3d *)&private_negative_identity4x3.left;
const real_vector3d *global_down3d = (const real_vector3d *)&private_negative_identity4x3.up;

const real_euler_angles2d *global_zero_angles2d = (const real_euler_angles2d *)&private_identity4x3.up;
const real_euler_angles3d *global_zero_angles3d = (const real_euler_angles3d *)&private_identity4x3.up;
const real_quaternion *global_identity_quaternion = &private_identity_quaternion;
const real_matrix4x3 *global_identity4x3 = &private_identity4x3;
const real_matrix4x3 *global_negative_identity4x3 = &private_negative_identity4x3;
const real_rectangle2d *global_null_rectangle2d = (const real_rectangle2d *)&private_null_rectangle;
const real_rectangle3d *global_null_rectangle3d = &private_null_rectangle;

/* ---------- public code */

void real_math_initialize(
	void)
{
	random_math_initialize();
	periodic_functions_initialize();
	return;
}

void real_math_dispose(
	void)
{
	random_math_dispose();
	periodic_functions_dispose();
	return;
}

void real_math_reset_precision(
	void)
{
	_control87(CW_DEFAULT, 0xFFFFF);
	return;
}

real_vector2d *perpendicular2d(
	real_vector2d const *a,
	real_vector2d *result)
{
	result->i = -a->j;
	result->j = a->i;
	return result;
}

real_vector3d *perpendicular3d(
	real_vector3d const *a,
	real_vector3d *result)
{
	real i = fabs(a->i);
	real j = fabs(a->j);
	real k = fabs(a->k);

	if (i <= j && i <= k)
	{
		result->i = 0.f;
		result->j = a->k;
		result->k = -a->j;
	}
	else
	{
		if (j <= k)
		{
			result->i = -a->k;
			result->j = 0.f;
			result->k = a->i;
		}
		else
		{
			result->i = a->j;
			result->j = -a->i;
			result->k = 0.f;
		}
	}

	return result;
}

real_vector4d *perpendicular4d(
	real_vector4d const *a,
	real_vector4d *result)
{
	result->i = a->k;
	result->j = a->l;
	result->k = -a->i;
	result->l = -a->j;
	return result;
}

real_vector3d *rotate_vector_about_axis(
	real_vector3d *v,
	real_vector3d const *n,
	real sine,
	real cosine)
{
	real one_minus_cosine_times_v_dot_n = dot_product3d(n, v) * (1-cosine);

	real v_cross_n_i = n->k*v->j - v->k*n->j;
	real v_cross_n_j = n->i*v->k - v->i*n->k;
	real v_cross_n_k = n->j*v->i - v->j*n->i;

	v->i = cosine*v->i + one_minus_cosine_times_v_dot_n*n->i - sine*v_cross_n_i;
	v->j = cosine*v->j + one_minus_cosine_times_v_dot_n*n->j - sine*v_cross_n_j;
	v->k = cosine*v->k + one_minus_cosine_times_v_dot_n*n->k - sine*v_cross_n_k;

	return v;
}

void vectors_interpolate(
	real_vector3d const *a,
	real_vector3d const *b,
	real t,
	real_vector3d *result)
{
	real one_minus_t = 1-t;
	result->i = one_minus_t*a->i + t*b->i;
	result->j = one_minus_t*a->j + t*b->j;
	result->k = one_minus_t*a->k + t*b->k;
	return;
}

void points_interpolate(
	real_point3d const *a,
	real_point3d const *b,
	real t,
	real_point3d *result)
{
	real one_minus_t = 1-t;
	result->x = one_minus_t*a->x + t*b->x;
	result->y = one_minus_t*a->y + t*b->y;
	result->z = one_minus_t*a->z + t*b->z;
	return;
}

void scalars_interpolate(
	real a,
	real b,
	real t,
	real *result)
{
	*result = (1-t) * a+b * t;
	return;
}

void scalars_interpolate_and_clamp_0_to_1(
	real a,
	real b,
	real t,
	real *result)
{
	*result = PIN((1.f-t)*a + t*b, 0.f, 1.f);
	return;
}

void component_vectors_from_normal3d(
	real_vector3d const *vector,
	real_vector3d const *normal,
	real_vector3d *parallel,
	real_vector3d *perpendicular)
{
	real_vector3d storage;
	real vn = dot_product3d(vector, normal);

	if (parallel== NULL)
	{
		parallel = &storage;
	}

	scale_vector3d(normal, vn, parallel);

	if (perpendicular!= NULL)
	{
		subtract_vectors3d(vector, parallel, perpendicular);
	}
	return;
}

void component_vectors_from_direction3d(
	real_vector3d const *vector,
	real_vector3d const *direction,
	real_vector3d *parallel,
	real_vector3d *perpendicular)
{
	real dd = dot_product3d(direction, direction);
	if (dd != 0.f)
	{
		real vd_dd = dot_product3d(vector, direction) / dd;
		scale_vector3d(direction, vd_dd, parallel);
		subtract_vectors3d(vector, parallel, perpendicular);
	}
	else
	{
		set_real_vector3d(parallel, 0.f, 0.f, 0.f);
		*perpendicular = *vector;
	}
	return;
}

void quaternions_multiply(
	real_quaternion const *q0,
	real_quaternion const *q1,
	real_quaternion *result)
{
	real_quaternion temporary;

	if (q0== result)
	{
		temporary = *q0;
		q0= &temporary;
	}

	if (q1== result)
	{
		temporary = *q1;
		q1= &temporary;
	}

	result->v.i =
					+ q0->v.j*q1->v.k
					+ q0->v.i*q1->w
					+ q0->w  *q1->v.i 
					- q0->v.k*q1->v.j
					;

	result->v.j =
					+ q0->w  *q1->v.j 
					+ q0->v.j*q1->w
					+ q0->v.k*q1->v.i 
					- q0->v.i*q1->v.k
					;

	result->v.k =
					+ q0->v.k*q1->w 
					+ q0->v.i*q1->v.j
					+ q0->w  *q1->v.k 
					- q0->v.j*q1->v.i
					;

	result->w =
					+ q0->w  *q1->w 
					- q0->v.i*q1->v.i 
					- q0->v.j*q1->v.j 
					- q0->v.k*q1->v.k
					;

	return;
}

void quaternions_interpolate(
	real_quaternion const *q0,
	real_quaternion const *q1,
	real t,
	real_quaternion *result)
{
	real v = 1.f-t;
	
	if (q0->v.i*q1->v.i + q0->v.j*q1->v.j + q0->v.k*q1->v.k + q0->w*q1->w < 0.f)
	{
		t = -t;
	}

	result->v.i = q0->v.i*v + q1->v.i*t;
	result->v.j = q0->v.j*v + q1->v.j*t;
	result->v.k = q0->v.k*v + q1->v.k*t;
	result->w = q0->w*v + q1->w*t;
	return;
}

void quaternion_transform_point(
	real_quaternion const *q,
	real_point3d const *p,
	real_point3d *result)
{

	real w_squared = q->w*q->w;
	real two_w_squared_minus_one = 2.f * w_squared - 1.f;
	real v_dot_p = (q->v.i*p->x + q->v.k*p->z + q->v.j*p->y);
	
	real two_v_dot_p = v_dot_p+v_dot_p;
	real two_w = q->w+q->w;

	real v_cross_r_i = (q->v.k*p->x) - (p->z*q->v.i);
	real v_cross_r_j = (q->v.i*p->y) - (q->v.j*p->x);
	real v_cross_r_k = (q->v.j*p->z) - (q->v.k*p->y);

	result->x = v_cross_r_k * two_w+two_v_dot_p*q->v.i + two_w_squared_minus_one*p->x;
	result->y = two_v_dot_p*q->v.j + two_w_squared_minus_one*p->y + v_cross_r_i*two_w;
	result->z = two_v_dot_p*q->v.k + two_w_squared_minus_one*p->z + v_cross_r_j*two_w;
	return;
}

void vectors3d_from_euler_angles3d(
	real_vector3d *forward,
	real_vector3d *up,
	real_euler_angles3d const *angles)
{
	real_matrix4x3 matrix;
	real_point3d dummy;

	match_assert("c:\\halo\\SOURCE\\math\\real_math.c", 832, forward);
	match_assert("c:\\halo\\SOURCE\\math\\real_math.c", 833, up);
	match_assert("c:\\halo\\SOURCE\\math\\real_math.c", 834, angles);

	matrix4x3_rotation_from_angles(&matrix, angles->yaw, angles->pitch, angles->roll);
	matrix4x3_to_point_and_vectors(&matrix, &dummy, forward, up);
	return;
}

boolean fast_vector_intersects_sphere(
	real_point3d const *point,
	real_vector3d const *vector,
	real_point3d const *center,
	real radius)
{
	real_point3d p = {point->x-center->x, point->y-center->y, point->z - center->z};
	real c = (p.x*p.x) + (p.y*p.y) + (p.z*p.z) - (radius*radius);
	real_vector3d direction;

	if (c < 0.f)
	{
		return TRUE;
	}
	else
	{
		real b;

		direction = *vector;
		b = direction.i*p.x + direction.j*p.y + direction.k*p.z;
		
		if (b >= 0.f)
		{
			return FALSE;
		}
		else
		{
			real a =
				direction.i*direction.i +
				direction.j*direction.j +
				direction.k*direction.k;
			real disc = b * b - a * c;
			
			if (disc <= 0.f)
			{
				return FALSE;
			}
			else
			{
				real neg_a_minus_b = -a - b;

				if (neg_a_minus_b < 0.f)
				{
					return TRUE;
				}
				else
				{
					return neg_a_minus_b * neg_a_minus_b < disc;
				}
			}
		}
	}
}

real fast_vector_intersection_with_sphere(
	real_point3d const *point,
	real_vector3d const *vector,
	real_point3d const *center,
	real radius)
{
	real_point3d p =
	{
		point->x - center->x,
		point->y - center->y,
		point->z - center->z
	};
	real c = p.x * p.x + p.y * p.y + p.z * p.z - radius * radius;
	real_vector3d direction;
	real b;
	real a;
	real discriminant;

	if (c < 0.0f)
		return 0.0f;

	direction = *vector;
	b =
		direction.i * p.x +
		direction.j * p.y +
		direction.k * p.z;
	if (b >= 0.0f)
		return REAL_MAX;

	a =
		direction.i * direction.i +
		direction.j * direction.j +
		direction.k * direction.k;
	discriminant = b * b - a * c;
	if (discriminant <= 0.0f)
		return REAL_MAX;

	return (-b - square_root(discriminant)) / a;
}

boolean point_in_rectangle2d(
	real_point2d const *point,
	real_rectangle2d const *bounds)
{
	return point->x >= bounds->x0
		&& point->x <= bounds->x1
		&& point->y >= bounds->y0
		&& point->y <= bounds->y1;
}

boolean point_in_rectangle3d(
	real_point3d const *point,
	real_rectangle3d const *bounds)
{
	return point->x >= bounds->x0
		&& point->x <= bounds->x1
		&& point->y >= bounds->y0
		&& point->y <= bounds->y1
		&& point->z >= bounds->z0
		&& point->z <= bounds->z1;
}

boolean circle_intersects_rectangle2d(
	real_point2d const *center,
	real radius,
	real_rectangle2d const *bounds)
{
	real dx;
	real dy;

	if (center->x <= bounds->x1)
	{
		if (center->x < bounds->x0)
		{
			dx = bounds->x0 - center->x;
		}
		else
		{
			dx = 0.f;
		}
	}
	else
	{
		dx = center->x - bounds->x1;
	}

	if (center->y <= bounds->y1)
	{
		if (center->y < bounds->y0)
		{
			dy = bounds->y0 - center->y;
		}
		else
		{
			dy = 0.f;
		}
	}
	else
	{
		dy = center->y - bounds->y1;
	}

	return dx * dx + dy * dy <= radius * radius;
}

boolean point_in_cone2d(
	real_point2d const *point,
	real_point2d const *center,
	real_vector2d const *direction,
	real length,
	real cosine)
{
	real_vector2d offset;
	real projection;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		1375,
		cosine>=0.0f);

	vector_from_points2d(center, point, &offset);
	projection = dot_product2d(&offset, direction);

	return (boolean)(projection >= 0.f &&
		projection <= length &&
		projection * projection >= magnitude_squared2d(&offset) * cosine * cosine);
}

boolean point_in_cone3d(
	real_point3d const *point,
	real_point3d const *center,
	real_vector3d const *direction,
	real length,
	real cosine)
{
	real_vector3d offset;
	real projection;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		1404,
		cosine>=0.0f);

	vector_from_points3d(center, point, &offset);
	projection = dot_product3d(&offset, direction);

	return (boolean)(projection >= 0.f &&
		projection <= length &&
		projection * projection >= magnitude_squared3d(&offset) * cosine * cosine);
}

boolean point_in_sector2d(
	real_point2d const *point,
	real_point2d const *center,
	real_vector2d const *direction,
	real radius,
	real cosine)
{
	real_vector2d offset;
	real distance_squared;
	real projection;
	boolean result;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		1433,
		cosine>=0.0f);

	vector_from_points2d(center, point, &offset);
	distance_squared = magnitude_squared2d(&offset);
	if (distance_squared <= radius * radius)
	{
		projection = dot_product2d(&offset, direction);
		if (projection >= 0.f &&
			projection * projection >= distance_squared * cosine * cosine)
		{
			result = TRUE;
		}
		else
		{
			result = FALSE;
		}
	}
	else
	{
		result = FALSE;
	}

	return result;
}

boolean point_in_sector3d(
	real_point3d const *point,
	real_point3d const *center,
	real_vector3d const *direction,
	real radius,
	real cosine)
{
	real_vector3d offset;
	real distance_squared;
	real projection;
	boolean result;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		1462,
		cosine>=0.0f);

	vector_from_points3d(center, point, &offset);
	distance_squared = magnitude_squared3d(&offset);
	if (distance_squared <= radius * radius)
	{
		projection = dot_product3d(&offset, direction);
		if (projection >= 0.f &&
			projection * projection >= distance_squared * cosine * cosine)
		{
			result = TRUE;
		}
		else
		{
			result = FALSE;
		}
	}
	else
	{
		result = FALSE;
	}

	return result;
}

boolean circle_intersects_cone2d(
	real_point2d const *circle_center,
	real circle_radius,
	real_point2d const *cone_center,
	real_vector2d const *cone_direction,
	real cone_length,
	real sine,
	real cosine)
{
	real_vector2d offset;
	real distance_squared;
	real projection;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2167,
		sine>0.0f && cosine>=0.0f);

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2168,
		valid_realcmp(sine * sine + cosine * cosine, 1.0f),
		csprintf(
			temporary,
			"%s, %s: assert_valid_real_sine_cosine(%f, %f)",
			"sine",
			"cosine",
			sine,
			cosine));

	vector_from_points2d(cone_center, circle_center, &offset);
	projection = dot_product2d(&offset, cone_direction);
	distance_squared = offset.j * offset.j;
	distance_squared += offset.i * offset.i;

	return (boolean)(
		projection >= -circle_radius &&
		projection <= circle_radius + cone_length &&
		(circle_radius * sine * 2.0f + projection) * projection +
				circle_radius * circle_radius >=
			distance_squared * cosine * cosine);
}

boolean circle_intersects_sector2d(
	real_point2d const *circle_center,
	real circle_radius,
	real_point2d const *sector_center,
	real_vector2d const *sector_direction,
	real sector_radius,
	real sine,
	real cosine)
{
	real_vector2d offset;
	real_point2d adjusted_center;
	real projection;
	real adjustment;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2231,
		sine>0.0f && cosine>=0.0f);

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2232,
		valid_realcmp(sine * sine + cosine * cosine, 1.0f),
		csprintf(
			temporary,
			"%s, %s: assert_valid_real_sine_cosine(%f, %f)",
			"sine",
			"cosine",
			sine,
			cosine));

	vector_from_points2d(sector_center, circle_center, &offset);
	projection = dot_product2d(&offset, sector_direction);
	if (projection >= -circle_radius &&
		projection <= circle_radius + sector_radius)
	{
		adjustment = circle_radius / sine;
		point_from_line2d(
			sector_center,
			sector_direction,
			-adjustment,
			&adjusted_center);
		if (point_in_sector2d(
			circle_center,
			&adjusted_center,
			sector_direction,
			sector_radius + circle_radius + adjustment,
			cosine))
		{
			return TRUE;
		}
	}

	return FALSE;
}

boolean sphere_intersects_cone3d(
	real_point3d const *sphere_center,
	real sphere_radius,
	real_point3d const *cone_center,
	real_vector3d const *cone_direction,
	real cone_length,
	real sine,
	real cosine)
{
	real_vector3d offset;
	real distance_squared;
	real projection;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2199,
		sine>0.0f && cosine>=0.0f);

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2200,
		valid_realcmp(sine * sine + cosine * cosine, 1.0f),
		csprintf(
			temporary,
			"%s, %s: assert_valid_real_sine_cosine(%f, %f)",
			"sine",
			"cosine",
			sine,
			cosine));

	vector_from_points3d(cone_center, sphere_center, &offset);
	projection = offset.k * cone_direction->k;
	projection += offset.j * cone_direction->j;
	projection += offset.i * cone_direction->i;
	distance_squared = offset.k * offset.k;
	distance_squared += offset.j * offset.j;
	distance_squared += offset.i * offset.i;

	return (boolean)(
		projection >= -sphere_radius &&
		projection <= sphere_radius + cone_length &&
		(sphere_radius * sine * 2.0f + projection) * projection +
				sphere_radius * sphere_radius >=
			distance_squared * cosine * cosine);
}

boolean sphere_intersects_sector3d(
	real_point3d const *sphere_center,
	real sphere_radius,
	real_point3d const *sector_center,
	real_vector3d const *sector_direction,
	real sector_radius,
	real sine,
	real cosine)
{
	real_vector3d offset;
	real_point3d adjusted_center;
	real projection;
	real adjustment;
	real negative_adjustment;

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2269,
		sine>0.0f && cosine>=0.0f);

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2270,
		valid_realcmp(sine * sine + cosine * cosine, 1.0f),
		csprintf(
			temporary,
			"%s, %s: assert_valid_real_sine_cosine(%f, %f)",
			"sine",
			"cosine",
			sine,
			cosine));

	vector_from_points3d(sector_center, sphere_center, &offset);
	projection =
		offset.k * sector_direction->k +
		offset.i * sector_direction->i;
	projection += offset.j * sector_direction->j;
	if (projection >= -sphere_radius &&
		projection <= sphere_radius + sector_radius)
	{
		adjustment = sphere_radius / sine;
		negative_adjustment = -adjustment;
		adjusted_center.x =
			sector_direction->i * negative_adjustment + sector_center->x;
		adjusted_center.y =
			sector_direction->j * negative_adjustment + sector_center->y;
		if (point_in_sector3d(
			sphere_center,
			&adjusted_center,
			sector_direction,
			(adjusted_center.z =
				sector_direction->k * negative_adjustment + sector_center->z,
			sector_radius + sphere_radius + adjustment),
			cosine))
		{
			return (boolean)TRUE;
		}
	}

	return FALSE;
}

boolean point_in_triangle2d(
	real_point2d const *point,
	real_point2d const *triangle0,
	real_point2d const *triangle1,
	real_point2d const *triangle2,
	real *t0,
	real *t1)
{
	real_vector2d edge0;
	real_vector2d edge1;
	real_vector2d offset;
	real cross0;
	real cross1;
	real determinant;
	boolean result;

	vector_from_points2d(triangle0, triangle1, &edge0);
	vector_from_points2d(triangle0, point, &offset);
	cross0 = cross_product2d(&edge0, &offset);
	if (cross0 >= 0.f)
	{
		vector_from_points2d(triangle0, triangle2, &edge1);
		cross1 = cross_product2d(&offset, &edge1);
		if (cross1 >= 0.f)
		{
			determinant = cross_product2d(&edge0, &edge1);
			if (cross0 + cross1 <= determinant)
			{
				real inverse_determinant = 1.f / determinant;
				*t0 = cross1 * inverse_determinant;
				*t1 = cross0 * inverse_determinant;
				result = TRUE;
			}
			else
			{
				result = FALSE;
			}
		}
		else
		{
			result = FALSE;
		}
	}
	else
	{
		result = FALSE;
	}

	return result;
}

boolean point_in_triangle3d(
	real_point3d const *point,
	real_point3d const *triangle0,
	real_point3d const *triangle1,
	real_point3d const *triangle2,
	real *t0,
	real *t1)
{
	real_vector3d edge0;
	real_vector3d edge1;
	real_vector3d offset;
	real_vector3d normal;
	real_point2d projected_edge0;
	real_point2d projected_edge1;
	real_point2d projected_offset;
	real plane_distance;
	real cross0;
	real cross1;
	real determinant;
	boolean result;
	short projection;
	boolean projection_sign;

	vector_from_points3d(triangle0, point, &offset);
	vector_from_points3d(triangle0, triangle1, &edge0);
	vector_from_points3d(triangle0, triangle2, &edge1);
	cross_product3d(&edge0, &edge1, &normal);
	plane_distance = dot_product3d(&normal, &offset);
	if (plane_distance * plane_distance <
		magnitude_squared3d(&normal) * _real_epsilon)
	{
		projection = projection_from_vector3d(&normal);
		projection_sign = projection_sign_from_vector3d(&normal, projection);
		project_point3d(
			(real_point3d const *)&edge0,
			projection,
			projection_sign,
			&projected_edge0);
		project_point3d(
			(real_point3d const *)&offset,
			projection,
			projection_sign,
			&projected_offset);
		cross0 = cross_product2d(
			(real_vector2d const *)&projected_edge0,
			(real_vector2d const *)&projected_offset);
		if (cross0 >= 0.0f)
		{
			project_point3d(
				(real_point3d const *)&edge1,
				projection,
				projection_sign,
				&projected_edge1);
			cross1 = cross_product2d(
				(real_vector2d const *)&projected_offset,
				(real_vector2d const *)&projected_edge1);
			if (cross1 >= 0.0f)
			{
				determinant = cross_product2d(
					(real_vector2d const *)&projected_edge0,
					(real_vector2d const *)&projected_edge1);
				if (cross0 + cross1 <= determinant)
				{
					real inverse_determinant;

					inverse_determinant = 1.0f / determinant;
					*t0 = cross1 * inverse_determinant;
					*t1 = cross0 * inverse_determinant;
					result = TRUE;
				}
				else
				{
					result = FALSE;
				}
			}
			else
			{
				result = FALSE;
			}
		}
		else
		{
			result = FALSE;
		}
	}
	else
	{
		result = FALSE;
	}

	return result;
}

boolean circle_intersects_triangle2d(
	real_point2d const *center,
	real radius,
	real_point2d const *triangle0,
	real_point2d const *triangle1,
	real_point2d const *triangle2)
{
	real_vector2d edge;
	real_vector2d offset;
	boolean result = TRUE;

	vector_from_points2d(triangle0, center, &offset);
	vector_from_points2d(triangle0, triangle1, &edge);
	if (cross_product2d(&offset, &edge) > 0.0f)
	{
		if (point_in_pill2d(center, triangle0, &edge, radius))
			return TRUE;
		result = FALSE;
	}

	vector_from_points2d(triangle1, center, &offset);
	vector_from_points2d(triangle1, triangle2, &edge);
	if (cross_product2d(&offset, &edge) < 0.0f)
	{
		if (point_in_pill2d(center, triangle1, &edge, radius))
			return TRUE;
		result = FALSE;
	}

	vector_from_points2d(triangle2, center, &offset);
	vector_from_points2d(triangle2, triangle0, &edge);
	if (cross_product2d(&offset, &edge) > 0.0f)
	{
		if (point_in_pill2d(center, triangle2, &edge, radius))
			return TRUE;
		result = FALSE;
	}

	return result;
}

boolean pill_intersects_triangle2d(
	real_point2d const *pill_base,
	real_vector2d const *pill_height,
	real pill_width,
	real_point2d const *triangle0,
	real_point2d const *triangle1,
	real_point2d const *triangle2)
{
	real_vector2d edge;
	real_vector2d offset;
	boolean result = TRUE;

	vector_from_points2d(triangle0, pill_base, &offset);
	vector_from_points2d(triangle0, triangle1, &edge);
	if (cross_product2d(&offset, &edge) > 0.0f)
	{
		if (vector_intersects_pill2d(
			triangle0,
			&edge,
			pill_base,
			pill_height,
			pill_width))
		{
			return TRUE;
		}
		result = FALSE;
	}

	vector_from_points2d(triangle1, pill_base, &offset);
	vector_from_points2d(triangle1, triangle2, &edge);
	if (cross_product2d(&offset, &edge) > 0.0f)
	{
		if (vector_intersects_pill2d(
			triangle1,
			&edge,
			pill_base,
			pill_height,
			pill_width))
		{
			return TRUE;
		}
		result = FALSE;
	}

	vector_from_points2d(triangle2, pill_base, &offset);
	vector_from_points2d(triangle2, triangle0, &edge);
	if (cross_product2d(&offset, &edge) > 0.0f)
	{
		if (vector_intersects_pill2d(
			triangle2,
			&edge,
			pill_base,
			pill_height,
			pill_width))
		{
			return TRUE;
		}
		result = FALSE;
	}

	return result;
}

boolean pill_intersects_rectangle2d(
	real_point2d const *pill_base,
	real_vector2d const *pill_height,
	real pill_width,
	real_rectangle2d const *bounds)
{
	real_point2d edge_base;
	real_vector2d edge_height;
	boolean result;

	result = TRUE;
	if (pill_base->x < bounds->x0)
	{
		set_real_point2d(&edge_base, bounds->x0, bounds->y0);
		set_real_vector2d(&edge_height, 0.0f, bounds->y1 - bounds->y0);
		if (vector_intersects_pill2d(
			&edge_base,
			&edge_height,
			pill_base,
			pill_height,
			pill_width))
		{
			return (boolean)TRUE;
		}
		result = FALSE;
	}
	if (pill_base->y < bounds->y0)
	{
		edge_base.x = bounds->x0;
		edge_base.y = bounds->y0;
		edge_height.i = bounds->x1 - bounds->x0;
		edge_height.j = 0.0f;
		if (vector_intersects_pill2d(
			&edge_base,
			&edge_height,
			pill_base,
			pill_height,
			pill_width))
		{
			return TRUE;
		}
		result = FALSE;
	}
	if (pill_base->x > bounds->x1)
	{
		set_real_point2d(&edge_base, bounds->x1, bounds->y0);
		set_real_vector2d(&edge_height, 0.0f, bounds->y1 - bounds->y0);
		if (vector_intersects_pill2d(
			&edge_base,
			&edge_height,
			pill_base,
			pill_height,
			pill_width))
		{
			return TRUE;
		}
		result = FALSE;
	}
	if (pill_base->y > bounds->y1)
	{
		edge_base.x = bounds->x0;
		edge_base.y = bounds->y1;
		edge_height.i = bounds->x1 - bounds->x0;
		edge_height.j = 0.0f;
		if (vector_intersects_pill2d(
			&edge_base,
			&edge_height,
			pill_base,
			pill_height,
			pill_width))
		{
			return TRUE;
		}
		result = FALSE;
	}

	return result;
}


boolean sphere_intersects_rectangle3d(
	real_point3d const *center,
	real radius,
	real_rectangle3d const *bounds)
{
	real dx;
	real dy;
	real dz;

	if (center->x <= bounds->x1)
	{
		if (center->x < bounds->x0)
		{
			dx = bounds->x0 - center->x;
		}
		else
		{
			dx = 0.f;
		}
	}
	else
	{
		dx = center->x - bounds->x1;
	}

	if (center->y <= bounds->y1)
	{
		if (center->y < bounds->y0)
		{
			dy = bounds->y0 - center->y;
		}
		else
		{
			dy = 0.f;
		}
	}
	else
	{
		dy = center->y - bounds->y1;
	}

	if (center->z <= bounds->z1)
	{
		if (center->z < bounds->z0)
		{
			dz = bounds->z0 - center->z;
		}
		else
		{
			dz = 0.f;
		}
	}
	else
	{
		dz = center->z - bounds->z1;
	}

	return dx * dx + dy * dy + dz * dz < radius * radius;
}

boolean sphere_intersects_triangle3d(
	real_point3d const *center,
	real radius,
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2)
{
	real_vector3d v;
	real_vector3d e0;
	real_vector3d e1;
	real_vector3d e2;
	real_vector3d n;
	real plane_dot;
	real normal_magnitude_squared;
	boolean result;

	result = TRUE;
	vector_from_points3d(p0, center, &v);
	vector_from_points3d(p0, p1, &e0);
	vector_from_points3d(p1, p2, &e1);
	cross_product3d(&e0, &e1, &n);
	plane_dot = dot_product3d(&n, &v);
	normal_magnitude_squared = magnitude_squared3d(&n);
	if (plane_dot * plane_dot > normal_magnitude_squared * radius * radius)
	{
		return FALSE;
	}

	if (triple_product3d(&v, &e0, &n) > 0.0f)
	{
		if (fast_vector_intersects_sphere(p0, &e0, center, radius))
		{
			return TRUE;
		}
		result = FALSE;
	}

	vector_from_points3d(p1, center, &v);
	if (triple_product3d(&v, &e1, &n) > 0.0f)
	{
		if (fast_vector_intersects_sphere(p1, &e1, center, radius))
		{
			return TRUE;
		}
		result = FALSE;
	}

	vector_from_points3d(p2, center, &v);
	vector_from_points3d(p2, p0, &e2);
	if (triple_product3d(&v, &e2, &n) < 0.0f)
	{
		if (fast_vector_intersects_sphere(p2, &e2, center, radius))
		{
			return TRUE;
		}
		result = FALSE;
	}

	return result;
}

boolean pill_intersects_triangle3d(
	real_point3d const *base,
	real_vector3d const *height,
	real width,
	real_point3d const *p0,
	real_point3d const *p1,
	real_point3d const *p2)
{
	real_vector3d e0;
	real_vector3d e1;
	real_vector3d e2;
	real_vector3d v;
	real_vector3d n;
	real_point3d p;
	real t;
	real closest_t;
	real plane_distance;
	boolean outside = FALSE;

	vector_from_points3d(p0, p1, &e0);
	vector_from_points3d(p1, p2, &e1);
	vector_from_points3d(base, p0, &v);
	cross_product3d(&e0, &e1, &n);
	t = dot_product3d(&n, &v) / dot_product3d(&n, height);

	if (t < 0.0f)
	{
		closest_t = 0.0f;
	}
	else if (t > 1.0f)
	{
		closest_t = 1.0f;
	}
	else
	{
		closest_t = t;
	}
	point_from_line3d(base, height, closest_t, &p);
	vector_from_points3d(p0, &p, &v);

	if (triple_product3d(&e0, &v, &n) < 0.0f)
	{
		if ((boolean)vector_intersects_pill3d(
			p0,
			&e0,
			base,
			height,
			width))
		{
			return TRUE;
		}
		outside = TRUE;
	}

	vector_from_points3d(p1, &p, &v);
	if (triple_product3d(&e1, &v, &n) < 0.0f)
	{
		if ((boolean)vector_intersects_pill3d(
			p1,
			&e1,
			base,
			height,
			width))
		{
			return TRUE;
		}
		outside = TRUE;
	}

	vector_from_points3d(p2, p0, &e2);
	vector_from_points3d(p2, &p, &v);
	if (triple_product3d(&e2, &v, &n) < 0.0f)
	{
		if ((boolean)vector_intersects_pill3d(
			p2,
			&e2,
			base,
			height,
			width))
		{
			return TRUE;
		}
		outside = TRUE;
	}

	if (outside)
	{
		return FALSE;
	}
	if (t > 0.0f && t < 1.0f)
	{
		return TRUE;
	}
	plane_distance = dot_product3d(&n, &v);
	return plane_distance * plane_distance <=
		magnitude_squared3d(&n) * width * width;
}


boolean vector_intersects_rectangle2d(
	real_point2d const *point,
	real_vector2d const *vector,
	real_rectangle2d const *bounds)
{
	real t_in = -FLT_MAX;
	real t_out = FLT_MAX;

	if (!(fabs(vector->i) < _real_epsilon))
	{
		real ooi = 1.f / vector->i;
		real t0 = (bounds->x0-point->x) * ooi;
		real t1 = (bounds->x1-point->x) * ooi;

		if (vector->i > 0.f)
		{
			if (t_in < t0) t_in = t0;
			if (t_out > t1) t_out = t1;
		}
		else
		{
			if (t_in < t1) t_in = t1;
			if (t_out > t0) t_out = t0;
		}

		if (t_in > t_out)
		{
			return FALSE;
		}
	}
	else
	{
		if (point->x < bounds->x0 || point->x > bounds->x1)
		{
			return FALSE;
		}
	}

	if (!(fabs(vector->j) < _real_epsilon))
	{
		real ooj = 1.f  / vector->j;
		real t0 = (bounds->y0-point->y) * ooj;
		real t1 = (bounds->y1-point->y) * ooj;

		if (vector->j > 0.f)
		{
			if (t_in < t0) t_in = t0;
			if (t_out > t1) t_out = t1;
		}
		else
		{
			if (t_in < t1) t_in = t1;
			if (t_out > t0) t_out = t0;
		}

		if (t_in > t_out)
		{
			return FALSE;
		}
	}
	else
	{
		if (point->y < bounds->y0 || point->y > bounds->y1)
		{
			return FALSE;
		}
	}

	return t_out >= 0.f && t_in <= 1.f;
}

boolean vector_intersects_pill2d(
	real_point2d const *point,
	real_vector2d const *vector,
	real_point2d const *pill_base,
	real_vector2d const *pill_height,
	real pill_width)
{
	real_vector2d offset;
	real cross;
	real dot;
	real vector_magnitude_squared;
	real pill_magnitude_squared;
	real inverse_cross;
	real inverse_vector_magnitude_squared;
	real inverse_pill_magnitude_squared;
	real t0;
	real t1;
	real t0_start;
	real t0_end;
	real t1_start;
	real t1_end;
	real clamped_t0;
	real clamped_t1;
	real_point2d closest_point0;
	real_point2d closest_point1;
	real_vector2d closest_offset;
	boolean t0_out_of_range;
	boolean t1_out_of_range;

	vector_from_points2d(point, pill_base, &offset);
	cross = cross_product2d(vector, pill_height);
	if (!(fabs(cross) < _real_epsilon))
	{
		inverse_cross = 1.0f / cross;
		t0 = cross_product2d(&offset, pill_height) * inverse_cross;
		t1 = cross_product2d(&offset, vector) * inverse_cross;
		t0_out_of_range = t0 < 0.0f || t0 > 1.0f;
		t1_out_of_range = t1 < 0.0f || t1 > 1.0f;
		if (!t0_out_of_range && !t1_out_of_range)
		{
			return TRUE;
		}

		if (t0_out_of_range)
		{
			if (t0 < 0.0f)
				clamped_t0 = 0.0f;
			else
				clamped_t0 = 1.0f;
			point_from_line2d(point, vector, clamped_t0, &closest_point0);
		}

		if (t1_out_of_range)
		{
			if (t1 < 0.0f)
				clamped_t1 = 0.0f;
			else
				clamped_t1 = 1.0f;
			point_from_line2d(pill_base, pill_height, clamped_t1, &closest_point1);
		}

		return !((!t0_out_of_range ||
				point_in_pill2d(
					&closest_point0,
					pill_base,
					pill_height,
					pill_width) == FALSE) &&
			(!t1_out_of_range ||
				point_in_pill2d(
					&closest_point1,
					point,
					vector,
					pill_width) == FALSE));
	}

	dot = dot_product2d(vector, pill_height);
	vector_magnitude_squared = magnitude_squared2d(vector);
	if (vector_magnitude_squared > _real_epsilon)
	{
		inverse_vector_magnitude_squared = 1.0f / vector_magnitude_squared;
		t0_start =
			dot_product2d(&offset, vector) *
			inverse_vector_magnitude_squared;
		t0_end =
			inverse_vector_magnitude_squared * dot +
			t0_start;
		t0 =
			(PIN(t0_start, 0.0f, 1.0f) +
				PIN(t0_end, 0.0f, 1.0f)) *
			0.5f;
	}
	else
	{
		t0 = 0.0f;
	}

	pill_magnitude_squared = magnitude_squared2d(pill_height);
	if (pill_magnitude_squared > _real_epsilon)
	{
		inverse_pill_magnitude_squared = 1.0f / pill_magnitude_squared;
		t1_start =
			-dot_product2d(&offset, pill_height) *
			inverse_pill_magnitude_squared;
		t1_end =
			inverse_pill_magnitude_squared * dot +
			t1_start;
		t1 =
			(PIN(t1_start, 0.0f, 1.0f) +
				PIN(t1_end, 0.0f, 1.0f)) *
			0.5f;
	}
	else
	{
		t1 = 0.0f;
	}

	point_from_line2d(point, vector, t0, &closest_point0);
	point_from_line2d(pill_base, pill_height, t1, &closest_point1);
	vector_from_points2d(&closest_point0, &closest_point1, &closest_offset);
	return magnitude_squared2d(&closest_offset) <= pill_width * pill_width;
}

long vector_intersects_pill3d(
	real_point3d const *point,
	real_vector3d const *vector,
	real_point3d const *pill_base,
	real_vector3d const *pill_height,
	real pill_width)
{
	real_vector3d offset;
	real_vector3d cross;
	real_vector3d numerator_cross;
	real denominator;
	real inverse_denominator;
	real dot;
	real vector_magnitude_squared;
	real pill_magnitude_squared;
	real inverse_vector_magnitude_squared;
	real inverse_pill_magnitude_squared;
	real t0;
	real t1;
	real t0_start;
	real t0_end;
	real t1_start;
	real t1_end;
	real clamped_t0;
	real clamped_t1;
	real_point3d closest_point0;
	real_point3d closest_point1;
	real_vector3d closest_offset;
	boolean t0_out_of_range;
	boolean t1_out_of_range;

	vector_from_points3d(point, pill_base, &offset);
	cross_product3d(vector, pill_height, &cross);
	denominator = magnitude_squared3d_right_associated(&cross);
	if (!(fabs(denominator) < _real_epsilon))
	{
		inverse_denominator = 1.0f / denominator;
		scale_vector3d(&cross, inverse_denominator, &cross);
		cross_product3d(&offset, pill_height, &numerator_cross);
		t0 = dot_product3d(&numerator_cross, &cross);
		cross_product3d(&offset, vector, &numerator_cross);
		t1 = dot_product3d(&numerator_cross, &cross);
		t0_out_of_range = t0 < 0.0f || t0 > 1.0f;
		t1_out_of_range = t1 < 0.0f || t1 > 1.0f;
		if (!t0_out_of_range && !t1_out_of_range)
			goto closest_distance;

		if (t0_out_of_range)
		{
			clamped_t0 = t0 < 0.0f ? 0.0f : 1.0f;
			point_from_line3d(point, vector, clamped_t0, &closest_point0);
		}
		if (t1_out_of_range)
		{
			clamped_t1 = t1 < 0.0f ? 0.0f : 1.0f;
			point_from_line3d(pill_base, pill_height, clamped_t1, &closest_point1);
		}
		if (t0_out_of_range &&
			fast_vector_intersects_sphere(
				pill_base,
				pill_height,
				&closest_point0,
				pill_width))
		{
			goto intersection;
		}
		if (t1_out_of_range &&
			fast_vector_intersects_sphere(
				point,
				vector,
				&closest_point1,
				pill_width))
		{
			goto intersection;
		}
		return FALSE;
	}

	dot = dot_product3d(vector, pill_height);
	vector_magnitude_squared = magnitude_squared3d(vector);
	if (vector_magnitude_squared > _real_epsilon)
	{
		inverse_vector_magnitude_squared = 1.0f / vector_magnitude_squared;
		t0_start = offset.k * vector->k;
		t0_start += offset.j * vector->j;
		t0_start += offset.i * vector->i;
		t0_start *= inverse_vector_magnitude_squared;
		t0_end =
			inverse_vector_magnitude_squared * dot +
			t0_start;
		t0 =
			(PIN(t0_start, 0.0f, 1.0f) +
				PIN(t0_end, 0.0f, 1.0f)) *
			0.5f;
	}
	else
	{
		t0 = 0.0f;
	}

	pill_magnitude_squared = magnitude_squared3d(pill_height);
	if (pill_magnitude_squared > _real_epsilon)
	{
		inverse_pill_magnitude_squared = 1.0f / pill_magnitude_squared;
		t1_start =
			-dot_product3d(&offset, pill_height) *
			inverse_pill_magnitude_squared;
		t1_end =
			inverse_pill_magnitude_squared * dot +
			t1_start;
		t1 =
			(PIN(t1_start, 0.0f, 1.0f) +
				PIN(t1_end, 0.0f, 1.0f)) *
			0.5f;
	}
	else
	{
		t1 = 0.0f;
	}

closest_distance:
	point_from_line3d(point, vector, t0, &closest_point0);
	point_from_line3d(pill_base, pill_height, t1, &closest_point1);
	vector_from_points3d(&closest_point0, &closest_point1, &closest_offset);
	if (!(magnitude_squared3d(&closest_offset) <= pill_width * pill_width))
		return FALSE;

intersection:
	return TRUE;
}


#define VECTOR_INTERSECTS_LINE2D(point, vector, line_point, line_endpoint, t_in, t_out) \
	do \
	{ \
		real_vector2d offset; \
		real_vector2d line_vector; \
		real numerator; \
		real divisor; \
		real t; \
		vector_from_points2d((line_point), (point), &offset); \
		vector_from_points2d((line_point), (line_endpoint), &line_vector); \
		numerator = offset.i * line_vector.j - offset.j * line_vector.i; \
		divisor = line_vector.i * (vector)->j - line_vector.j * (vector)->i; \
		if (!(fabs(divisor) < _real_epsilon)) \
		{ \
			t = numerator / divisor; \
			if (divisor > 0.0f) \
			{ \
				if (*(t_in) < t) \
					*(t_in) = t; \
			} \
			else \
			{ \
				if (*(t_out) > t) \
					*(t_out) = t; \
			} \
			if (*(t_in) > *(t_out)) \
				return FALSE; \
		} \
		else if (numerator > 0.0f) \
		{ \
			return FALSE; \
		} \
	} while (0)

boolean vector_intersects_triangle2d(
	real_point2d const *point,
	real_vector2d const *vector,
	real_point2d const *triangle0,
	real_point2d const *triangle1,
	real_point2d const *triangle2)
{
	real t_in = 0.0f;
	real t_out = 1.0f;

	VECTOR_INTERSECTS_LINE2D(
		point,
		vector,
		triangle0,
		triangle1,
		&t_in,
		&t_out);

	VECTOR_INTERSECTS_LINE2D(
		point,
		vector,
		triangle1,
		triangle2,
		&t_in,
		&t_out);

	VECTOR_INTERSECTS_LINE2D(
		point,
		vector,
		triangle2,
		triangle0,
		&t_in,
		&t_out);

	return TRUE;
}

#undef VECTOR_INTERSECTS_LINE2D

boolean vector_intersects_triangle3d(
	real_point3d const *point,
	real_vector3d const *vector,
	real_point3d const *triangle0,
	real_point3d const *triangle1,
	real_point3d const *triangle2,
	real *result)
{
	real_vector3d edge0;
	real_vector3d edge1;
	real_vector3d normal;
	real_vector3d offset;
	real_vector3d cross;
	real denominator;
	real inverse_denominator;
	real t;
	real t0;
	real t1;
	boolean intersects = FALSE;

	vector_from_points3d(triangle0, triangle1, &edge0);
	vector_from_points3d(triangle0, triangle2, &edge1);
	cross_product3d(&edge0, &edge1, &normal);
	denominator = dot_product3d(&normal, vector);
	if (!(fabs(denominator) < _real_epsilon))
	{
		inverse_denominator = 1.0f / denominator;
		vector_from_points3d(point, triangle0, &offset);
		t = dot_product3d(&offset, &normal) * inverse_denominator;
		if (t >= 0.0f && t <= 1.0f)
		{
			cross_product3d(&offset, vector, &cross);
			t0 = dot_product3d(&cross, &edge1) * inverse_denominator;
			if (t0 >= 0.0f && t0 <= 1.0f)
			{
				t1 = -dot_product3d(&cross, &edge0) * inverse_denominator;
				if (t1 >= 0.0f && t0 + t1 <= 1.0f)
				{
					*result = t;
					intersects = TRUE;
				}
			}
		}
	}

	return intersects;
}

boolean vector_intersects_rectangle3d(
	real_point3d const *point,
	real_vector3d const *vector,
	real_rectangle3d const *bounds)
{
	real t_in = -FLT_MAX;
	real t_out = FLT_MAX;

	if (!(fabs(vector->i) < _real_epsilon))
	{
		real ooi = 1.f / vector->i;
		real t0 = (bounds->x0-point->x) * ooi;
		real t1 = (bounds->x1-point->x) * ooi;

		if (vector->i > 0.f)
		{
			if (t_in < t0) t_in = t0;
			if (t_out > t1) t_out = t1;
		}
		else
		{
			if (t_in < t1) t_in = t1;
			if (t_out > t0) t_out = t0;
		}

		if (t_in > t_out)
		{
			return FALSE;
		}
	}
	else
	{
		if (point->x < bounds->x0 || point->x > bounds->x1)
			return FALSE;
	}

	if (!(fabs(vector->j) < _real_epsilon))
	{
		real ooj = 1.f / vector->j;
		real t0 = (bounds->y0-point->y) * ooj;
		real t1 = (bounds->y1-point->y) * ooj;

		if (vector->j > 0.f)
		{
			if (t_in < t0) t_in = t0;
			if (t_out > t1) t_out = t1;
		}
		else
		{
			if (t_in < t1) t_in = t1;
			if (t_out > t0) t_out = t0;
		}

		if (t_in > t_out)
		{
			return FALSE;
		}
	}
	else
	{
		if (point->y < bounds->y0 || point->y > bounds->y1)
		{
			return FALSE;
		}
	}

	if (!(fabs(vector->k) < _real_epsilon))
	{
		real ook = 1 / vector->k;
		real t0 = (bounds->z0-point->z) * ook;
		real t1 = (bounds->z1-point->z) * ook;

		if (vector->k > 0.f)
		{
			if (t_in < t0) t_in = t0;
			if (t_out > t1) t_out = t1;
		}
		else
		{
			if (t_in < t1) t_in = t1;
			if (t_out > t0) t_out = t0;
		}

		if (t_in > t_out)
		{
			return FALSE;
		}
	}
	else
	{
		if (point->z < bounds->z0 || point->z > bounds->z1)
		{
			return FALSE;
		}
	}

	return t_out >= 0.f && t_in <= 1.f;
}

boolean pill_intersects_rectangle3d(
	real_point3d const *base,
	real_vector3d const *height,
	real width,
	real_rectangle3d const *bounds)
{
	/* originally stubbed */
	return FALSE;
}

real_vector2d *fast_normalize2d(
	real_vector2d *v)
{
	real magnitude_squared = magnitude_squared2d(v);
	
	if (magnitude_squared != 0.f)
	{
		scale_vector2d(v, 1.f / square_root(magnitude_squared), v);
	}

	return v;
}

real_vector3d *fast_normalize3d(
	real_vector3d *v)
{
	real magnitude_squared = magnitude_squared3d(v);
	
	if (magnitude_squared != 0.f)
	{
		scale_vector3d(v, 1.f / square_root(magnitude_squared), v);
	}

	return v;
}

real cross_product_magnitude3d(
	real_vector3d const *a,
	real_vector3d const *b)
{
	real_vector3d c;
	cross_product3d(a, b, &c);
	return magnitude3d(&c);
}

real dequantize_byte_to_real(
	real min,
	real max,
	byte value)
{
	if (value == UNSIGNED_CHAR_MAX)
	{
		return max;
	}
	
	return (max-min) * ((real)value / 255.f) + min;
}

byte quantize_real_to_byte_lower_bound(
	real min,
	real max,
	real value)
{
	real range;
	byte test;

	range = max - min;
	test = (byte)(long)(((value - min) / range) * 255.f);

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2803,
		min - _real_epsilon <= value &&
			value <= max + _real_epsilon,
		csprintf(
			temporary,
			"%lf is not between %lf and %lf",
			value,
			min,
			max));

	while (test > 0 &&
		value < dequantize_byte_to_real(min, max, test))
	{
		test--;
	}

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2809,
		dequantize_byte_to_real(min, max, test) <= value ||
			(test == 0 &&
				dequantize_byte_to_real(min, max, test) <= value + _real_epsilon),
		"dequantize_byte_to_real(min, max, test)<=value || (test==0 && dequantize_byte_to_real(min, max, test)<=value+_real_epsilon)");

	return test;
}

byte quantize_real_to_byte_upper_bound(
	real min,
	real max,
	real value)
{
	real range;
	byte test;

	range = max - min;
	test = (byte)(long)(((value - min) / range) * 255.f);

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2823,
		min - _real_epsilon <= value &&
			value <= max + _real_epsilon,
		csprintf(
			temporary,
			"%lf is not between %lf and %lf",
			value,
			min,
			max));

	while (test < UNSIGNED_CHAR_MAX &&
		value > dequantize_byte_to_real(min, max, test))
	{
		test++;
	}

	match_vassert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		2829,
		dequantize_byte_to_real(min, max, test) >= value ||
			(test == UNSIGNED_CHAR_MAX &&
				dequantize_byte_to_real(min, max, test) >= value - _real_epsilon),
		"dequantize_byte_to_real(min, max, test)>=value || (test==UNSIGNED_CHAR_MAX && dequantize_byte_to_real(min, max, test)>=value-_real_epsilon)");

	return test;
}

byte_rectangle3d *quantize_real_to_byte_rectangle3d(
	real_rectangle3d const *parent,
	real_rectangle3d const *rectangle,
	byte_rectangle3d *result)
{
	if (rectangle->x0 == REAL_MAX)
	{
		match_assert(
			"c:\\halo\\SOURCE\\math\\real_math.c",
			2843,
			rectangle->x1==REAL_MIN && rectangle->y0==REAL_MAX && rectangle->y1==REAL_MIN && rectangle->z0==REAL_MAX && rectangle->z1==REAL_MIN);

		csmemset(result, 0, sizeof(*result));
	}
	else
	{
		result->x0 = quantize_real_to_byte_lower_bound(parent->x0, parent->x1, rectangle->x0);
		result->x1 = quantize_real_to_byte_upper_bound(parent->x0, parent->x1, rectangle->x1);
		result->y0 = quantize_real_to_byte_lower_bound(parent->y0, parent->y1, rectangle->y0);
		result->y1 = quantize_real_to_byte_upper_bound(parent->y0, parent->y1, rectangle->y1);
		result->z0 = quantize_real_to_byte_lower_bound(parent->z0, parent->z1, rectangle->z0);
		result->z1 = quantize_real_to_byte_upper_bound(parent->z0, parent->z1, rectangle->z1);
	}

	return result;
}

real signed_angle_between_vectors2d(
	real_vector2d const *a,
	real_vector2d const *b)
{
	real dot_product = dot_product2d(a, b);
	real result = arccosine(PIN(dot_product, -1.f, 1.f));

	if ((a->i*b->j) - (a->j*b->i) < 0.f)
	{
		result = -result;
	}

	return result;
}

real angle_between_vectors2d(
	real_vector2d const *a,
	real_vector2d const *b)
{
	real angle = 0.f;
	real aa_bb = (a->i*a->i + a->j*a->j) * (b->i*b->i + b->j*b->j);
	
	if (aa_bb != 0.f)
	{
		real ab = dot_product2d(a, b);
		real c = PIN((ab/aa_bb*ab) + (ab/aa_bb*ab) - 1.f, -1.f, 1.f);

		angle = arccosine(c) * 0.5f;
		
		if (ab < 0.f)
		{
			angle = _pi - angle;
		}
	}

	return angle;
}

real angle_between_vectors3d(
	real_vector3d const *a,
	real_vector3d const *b)
{

	real angle = 0.f;
	real aa_bb = (a->i*a->i + a->j*a->j + a->k*a->k) * (b->i*b->i + b->j*b->j + b->k*b->k);

	if (aa_bb != 0.f)
	{
		real ab = dot_product3d(a, b);
		real c = PIN((ab/aa_bb*ab) + (ab/aa_bb*ab) - 1.f, -1.f, 1.f);

		angle = arccosine(c) * 0.5f;

		if (ab < 0.f)
		{
			angle = _pi - angle;
		}
	}

	return angle;
}

real angle_between_normals3d(
	real_vector3d const *a,
	real_vector3d const *b)
{
	real result = 0.f;

	if ((*(long *)&a->i) == (*(long *)&b->i)
		&& (*(long *)&a->j) == (*(long *)&b->j)
		&& (*(long *)&a->k) == (*(long *)&b->k))
	{
		return result;
	}

	result = PIN(dot_product3d(a, b), -1.f, 1.f);
	return arccosine(result);
}

void yaw_vectors(
	real_vector3d *forward,
	real_vector3d const  *up,
	real sine,
	real cosine)
{
	real_vector3d cross;
	cross_product3d(up, forward, &cross);

	forward->i = forward->i*cosine + cross.i*sine;
	forward->j = forward->j*cosine + cross.j*sine;
	forward->k = forward->k*cosine + cross.k*sine;
	return;
}

void pitch_vectors(
	real_vector3d *forward,
	real_vector3d *up,
	real sine,
	real cosine)
{
	real_vector3d backward;
	negate_vector3d(forward, &backward);

	forward->i = forward->i*cosine + up->i*sine;
	forward->j = forward->j*cosine + up->j*sine;
	forward->k = forward->k*cosine + up->k*sine;

	up->i = up->i*cosine + backward.i*sine;
	up->j = up->j*cosine + backward.j*sine;
	up->k = up->k*cosine + backward.k*sine;
	return;
}

void fast_normals_interpolate(
	real_vector3d const *a,
	real_vector3d const *b,
	real t,
	real_vector3d *result)
{
	real one_minus_t = 1-t;
	result->i = t*b->i + one_minus_t * a->i;
	result->j = t*b->j + one_minus_t * a->j;
	result->k = t*b->k + one_minus_t * a->k;
	fast_normalize3d(result);
	return;
}

void normals_interpolate(
	real_vector3d const *a,
	real_vector3d const *b,
	real t,
	real_vector3d *result)
{
	real angle;
	real_vector3d c;

	angle = angle_between_vectors3d(a, b);

	if (t <= 0.5f)
	{
		angle *= t;
		cross_product3d(a, b, &c);
		*result = *a;
	}
	else
	{
		angle *= (1-t);
		cross_product3d(b, a, &c);
		*result = *b;
	}

	if (normalize3d(&c) != 0.0f)
	{
		rotate_vector_about_axis(result, &c, sine(angle), cosine(angle));
	}

	return;
}

boolean pin_normal_to_cone3d(
	real_vector3d const *normal,
	real_vector3d const *direction,
	real sine,
	real cosine,
	real_vector3d *result)
{
	real_vector3d axis;
	boolean pinned;

	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\math\\real_math.c", 515, normal);
	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\math\\real_math.c", 516, direction);

	if (dot_product3d(normal, direction) >= cosine)
	{
		*result = *normal;
		pinned = FALSE;
	}
	else
	{
		cross_product3d(direction, normal, &axis);
		if (normalize3d(&axis) == 0.f)
		{
			perpendicular3d(direction, &axis);
			normalize3d(&axis);
		}

		*result = *direction;
		rotate_vector_about_axis(result, &axis, sine, cosine);
		pinned = TRUE;
	}

	match_assert_valid_real_normal3d("c:\\halo\\SOURCE\\math\\real_math.c", 540, result);
	return pinned;
}

real_vector3d *reflect_vector3d(
	real_vector3d const *incident,
	real_vector3d const *normal,
	real_vector3d *reflection)
{
	real incident_dot_normal = dot_product3d(incident, normal);
	real twice_incident_dot_normal = incident_dot_normal + incident_dot_normal;
	reflection->i = incident->i - normal->i*twice_incident_dot_normal;
	reflection->j = incident->j - normal->j*twice_incident_dot_normal;
	reflection->k = incident->k - normal->k*twice_incident_dot_normal;
	return reflection;
}

real_vector3d *refract_vector3d(
	real_vector3d const *incident,
	real_vector3d const *normal,
	real coefficient_of_refraction,
	real_vector3d *refraction)
{
	real refraction_scale;

	real incident_magnitude = magnitude3d(incident);
	real one_over_incident_magnitude = 1.f / incident_magnitude;

	real sine_incident = cross_product_magnitude3d(incident, normal) * one_over_incident_magnitude;
	real cosine_incident = dot_product3d(incident, normal) * one_over_incident_magnitude;
	real sine_refraction = coefficient_of_refraction * sine_incident;

	real cosine_refraction = square_root(1.f - sine_refraction * sine_refraction);
	
	real delta = (sine_incident * cosine_refraction + cosine_incident * sine_refraction) / sine_refraction;

	refraction->i = delta * normal->i + incident->i;
	refraction->j = delta * normal->j + incident->j;
	refraction->k = delta * normal->k + incident->k;

	refraction_scale = incident_magnitude / magnitude3d(refraction);
	
	refraction->i *= refraction_scale;
	refraction->j *= refraction_scale;
	refraction->k *= refraction_scale;

	return refraction;
}

void quaternion_normalize(
	real_quaternion *q)
{
	real magnitude_squared = q->v.i*q->v.i + q->v.j*q->v.j + q->v.k*q->v.k + q->w*q->w;
	if (magnitude_squared > 0.f)
	{
		real one_over_magnitude = reciprocal_square_root(magnitude_squared);
		set_real_quaternion(q, one_over_magnitude*q->v.i, one_over_magnitude*q->v.j, one_over_magnitude*q->v.k, one_over_magnitude*q->w);
	}
	else
	{
		set_real_quaternion(q, 0.f, 0.f, 0.f, 1.f);
	}
	return;
}

void quaternion_from_angle_and_vector(
	real_quaternion *q,
	real a,
	real_vector3d const *v)
{
	real half_angle = a * 0.5f;
	real sine_half_angle = sine(half_angle);

	q->w = cosine(half_angle);
	q->v.i = sine_half_angle*v->i;
	q->v.j = sine_half_angle*v->j;
	q->v.k = sine_half_angle*v->k;

	return;
}

void quaternion_to_angle_and_vector(
	real_quaternion const *q,
	real *a,
	real_vector3d *v)
{
	*v = q->v;
	*a = 2 * arctangent(normalize3d(v), q->w) ;
	
	if (*a > _pi)
	{
		negate_vector3d(v, v);
		*a = _pi * 2.0f - *a;
	}

	return;
}

void quaternions_interpolate_and_normalize(
	real_quaternion const *q0,
	real_quaternion const *q1,
	real t,
	real_quaternion *result)
{
	quaternions_interpolate(q0, q1, t, result);
	quaternion_normalize(result);
	return;
}

void orientations_interpolate(
	real_orientation const *a,
	real_orientation const *b,
	real t,
	real_orientation *result)
{
	quaternions_interpolate(&a->rotation, &b->rotation, t, &result->rotation);
	quaternion_normalize(&result->rotation);

	result->translation.x = a->translation.x*(1.f - t) + b->translation.x*t;
	result->translation.y = a->translation.y*(1.f - t) + b->translation.y*t;
	result->translation.z = a->translation.z*(1.f - t) + b->translation.z*t;
	result->scale = a->scale*(1.f - t) + b->scale*t;
	return;
}

real_euler_angles2d *euler_angles2d_from_vector3d(
	real_euler_angles2d *angles,
	real_vector3d const *vector)
{
	angles->yaw = arctangent(vector->j, vector->i);
	angles->pitch = arctangent(vector->k, square_root(vector->i*vector->i+vector->j*vector->j));
	return angles;
}

real_vector3d *vector3d_from_angle(
	real_vector3d *vector,
	real angle)
{
	vector->k = 0.f;
	vector->i = cosine(angle);
	vector->j = sine(angle);
	return vector;
}

real_vector3d *vector3d_from_euler_angles2d(
	real_vector3d *vector,
	real_euler_angles2d const *angles)
{
	real cosine_pitch = cosine(angles->pitch);
	vector->i = cosine(angles->yaw)* cosine_pitch;
	vector->j = sine(angles->yaw)* cosine_pitch;
	vector->k = sine(angles->pitch);
	return vector;
}

boolean point_in_pill2d(
	real_point2d const *point,
	real_point2d const *base,
	real_vector2d const *height,
	real width)
{
	real_vector2d v;
	real height_magnitude_squared;
	real t;

	vector_from_points2d(base, point, &v);
	height_magnitude_squared = dot_product2d(&v, height) / magnitude_squared2d(height);
	t = -PIN(height_magnitude_squared, 0.f, 1.f);
	point_from_line2d((real_point2d *)&v, height, t, (real_point2d *)&v);

	return magnitude_squared2d(&v)<=width*width;
}

real point_to_line_distance_squared3d(
	real_point3d const *point,
	real_point3d const *base,
	real_vector3d const *height)
{
	real t;
	real distance_squared;
	real_vector3d v;
	
	vector_from_points3d(base, point, &v);
	t = PIN(dot_product3d(&v, height) / magnitude_squared3d(height), 0.f, 1.f);
	point_from_line3d((real_point3d *)&v, height, -t, (real_point3d *)&v);
	distance_squared = magnitude_squared3d(&v);

	return distance_squared;
}

real vector_to_line_distance_squared3d(
	real_point3d const *point,
	real_vector3d const *vector,
	real_point3d const *line_point,
	real_vector3d const *line_vector)
{
	real_vector3d offset;
	real_vector3d cross;
	real_vector3d numerator_cross0;
	real_vector3d numerator_cross1;
	real denominator;
	real inverse_denominator;
	real dot;
	real vector_magnitude_squared;
	real line_magnitude_squared;
	real inverse_vector_magnitude_squared;
	real inverse_line_magnitude_squared;
	real t0;
	real t1;
	real t0_start;
	real t0_end;
	real t1_start;
	real t1_end;
	real clamped_t0;
	real clamped_t1;
	real d1;
	real d0;
	real_point3d closest_point0;
	real_point3d closest_point1;
	real_vector3d closest_offset;
	boolean t0_out_of_range;
	boolean t1_out_of_range;

	vector_from_points3d(point, line_point, &offset);
	cross_product3d(vector, line_vector, &cross);
	denominator = magnitude_squared3d_right_associated(&cross);
	if (!(fabs(denominator) < _real_epsilon))
	{
		inverse_denominator = 1.0f / denominator;
		scale_vector3d(&cross, inverse_denominator, &cross);
		cross_product3d(&offset, line_vector, &numerator_cross0);
		t0 = dot_product3d(&numerator_cross0, &cross);
		cross_product3d(&offset, vector, &numerator_cross1);
		t1 = dot_product3d(&numerator_cross1, &cross);
		t0_out_of_range = t0 < 0.0f || t0 > 1.0f;
		t1_out_of_range = t1 < 0.0f || t1 > 1.0f;
		if (t0_out_of_range || t1_out_of_range)
		{
			d0 = REAL_MAX;
			d1 = REAL_MAX;
			if (t0_out_of_range)
			{
				clamped_t0 = t0 < 0.0f ? 0.0f : 1.0f;
				point_from_line3d(point, vector, clamped_t0, &closest_point0);
				d0 = point_to_line_distance_squared3d(
					&closest_point0,
					line_point,
					line_vector);
			}
			if (t1_out_of_range)
			{
				clamped_t1 = t1 < 0.0f ? 0.0f : 1.0f;
				point_from_line3d(line_point, line_vector, clamped_t1, &closest_point1);
				d1 = point_to_line_distance_squared3d(
					&closest_point1,
					point,
					vector);
			}
			match_assert(
				"c:\\halo\\SOURCE\\math\\real_math.c",
				942,
				(d0 < REAL_MAX) || (d1 < REAL_MAX));
			return d0 > d1 ? d1 : d0;
		}
	}
	else
	{
		dot = dot_product3d(vector, line_vector);
		vector_magnitude_squared = magnitude_squared3d(vector);
		if (vector_magnitude_squared > _real_epsilon)
		{
			inverse_vector_magnitude_squared = 1.0f / vector_magnitude_squared;
			t0_start = offset.k * vector->k;
			t0_start += offset.j * vector->j;
			t0_start += offset.i * vector->i;
			t0_start *= inverse_vector_magnitude_squared;
			t0_end =
				inverse_vector_magnitude_squared * dot +
				t0_start;
			t0 =
				(PIN(t0_start, 0.0f, 1.0f) +
					PIN(t0_end, 0.0f, 1.0f)) *
				0.5f;
		}
		else
		{
			t0 = 0.0f;
		}

		line_magnitude_squared = magnitude_squared3d(line_vector);
		if (line_magnitude_squared > _real_epsilon)
		{
			inverse_line_magnitude_squared = 1.0f / line_magnitude_squared;
			t1_start =
				-dot_product3d(&offset, line_vector) *
				inverse_line_magnitude_squared;
			t1_end =
				inverse_line_magnitude_squared * dot +
				t1_start;
			t1 =
				(PIN(t1_start, 0.0f, 1.0f) +
					PIN(t1_end, 0.0f, 1.0f)) *
				0.5f;
		}
		else
		{
			t1 = 0.0f;
		}
	}

	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		986,
		(t0 >= 0.0f) && (t0 <= 1.0f));
	match_assert(
		"c:\\halo\\SOURCE\\math\\real_math.c",
		987,
		(t1 >= 0.0f) && (t1 <= 1.0f));
	point_from_line3d(point, vector, t0, &closest_point0);
	point_from_line3d(line_point, line_vector, t1, &closest_point1);
	vector_from_points3d(&closest_point0, &closest_point1, &closest_offset);
	return magnitude_squared3d(&closest_offset);
}


boolean sphere_test_vector3d(
	real_point3d const *center,
	real radius,
	real_point3d const *point,
	real_vector3d const *vector,
	real *t,
	real_vector3d *normal)
{
	real_vector3d x;
	real xv;

	vector_from_points3d(center, point, &x);
	xv = dot_product3d(&x, vector);

	if (xv < 0)
	{
		real xx = magnitude_squared3d(&x);
		real xx_rr = xx - radius*radius;

		if (xx_rr <= 0)
		{
			*t = 0;
			scale_vector3d(&x, reciprocal_square_root(xx), normal);
			return TRUE;
		}
		else
		{
			real d = vector->i*vector->i + vector->j*vector->j + vector->k*vector->k;
			real vv = xv*xv - d*xx_rr;

			if (vv >= 0)
			{
				real n = -(square_root(vv) + xv);

				if (n <= d)
				{
					*t = n/d;
					fast_normalize3d((real_vector3d *)point_from_line3d((real_point3d *)&x, vector, *t, (real_point3d *)normal));
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

boolean pill_test_vector3d(
	real_point3d const *base,
	real height,
	real radius,
	real_point3d const *point,
	real_vector3d const *vector,
	real *t,
	real_vector3d *normal)
{
	real_vector2d x;
	real xv;
	real xx_rr;
	real d;
	real vv;
	real n;
	real height_fraction;
	real top_z;
	real_point3d top;
	boolean result = FALSE;

	x.i = point->x - base->x;
	x.j = point->y - base->y;
	xv = x.i * vector->i + x.j * vector->j;
	xx_rr = x.i * x.i + x.j * x.j - radius * radius;
	if (xx_rr <= 0.0f)
	{
		n = 0.0f;
	}
	else
	{
		d = vector->i * vector->i + vector->j * vector->j;
		vv = xv * xv - d * xx_rr;
		if (vv >= 0.0f)
		{
			n = -(square_root(vv) + xv);
			if (n <= d)
			{
				n = n / d;
			}
			else
			{
				goto done;
			}
		}
		else
		{
			goto done;
		}
	}

	result = TRUE;
	height_fraction =
		(point->z - base->z + n * vector->k) / (height * height);
	if (height_fraction < 0.0f)
	{
		result = sphere_test_vector3d(base, radius, point, vector, t, normal);
	}
	else if (height_fraction > 1.0f)
	{
		top_z = base->z + height;
		top.x = base->x;
		top.y = base->y;
		top.z = top_z;
		result = sphere_test_vector3d(&top, radius, point, vector, t, normal);
	}
	else if (xv < 0.0f)
	{
		*t = n;
		normal->i = x.i + n * vector->i;
		normal->j = x.j + n * vector->j;
		fast_normalize2d((real_vector2d *)normal);
		normal->k = 0.0f;
	}
	else
	{
		result = FALSE;
	}

	if (result && dot_product3d(normal, vector) > 0.0f)
	{
		result = FALSE;
	}

done:
	return result;
}

boolean point_from_planes3d(
	real_plane3d const *plane0,
	real_plane3d const *plane1,
	real_plane3d const *plane2,
	real_point3d *point)
{
	real_vector3d cross;
	real determinant;
	real distance;

	cross_product3d(&plane0->n, &plane1->n, &cross);
	determinant = dot_product3d(&cross, &plane2->n);
	if (!(fabs(determinant) < _real_epsilon))
	{
		cross_product3d(&plane1->n, &plane2->n, &cross);
		distance = plane0->d;
		point->x = cross.i * distance;
		point->y = cross.j * distance;
		point->z = cross.k * distance;

		distance =
			(cross_product3d(&plane2->n, &plane0->n, &cross),
			plane1->d);
		point->x = cross.i * distance + point->x;
		point->y = cross.j * distance + point->y;
		point->z = cross.k * distance + point->z;

		distance =
			(cross_product3d(&plane0->n, &plane1->n, &cross),
			plane2->d);
		determinant = 1.0f / determinant;
		point->x = (cross.i * distance + point->x) * determinant;
		point->y = (cross.j * distance + point->y) * determinant;
		point->z = (cross.k * distance + point->z) * determinant;
		return TRUE;
	}

	return FALSE;
}

boolean line_from_planes3d(
	real_plane3d const *plane0,
	real_plane3d const *plane1,
	real_point3d *point,
	real_vector3d *direction)
{
	real_vector3d local_direction;
	real determinant;
	real inverse_determinant;
	real distance;

	cross_product3d(&plane0->n, &plane1->n, direction);
	determinant = magnitude_squared3d(direction);
	if (!(fabs(determinant) < _real_epsilon))
	{
		cross_product3d(&plane1->n, direction, &local_direction);
		distance = plane0->d;
		point->x = local_direction.i * distance;
		point->y = local_direction.j * distance;
		point->z = local_direction.k * distance;

		distance =
			(cross_product3d(direction, &plane0->n, &local_direction),
			 plane1->d);
		inverse_determinant = 1.0f / determinant;
		point->x =
			(local_direction.i * distance + point->x) *
			inverse_determinant;
		point->y =
			(local_direction.j * distance + point->y) *
			inverse_determinant;
		point->z =
			(local_direction.k * distance + point->z) *
			inverse_determinant;
		return TRUE;
	}

	return FALSE;
}

void angular_accelerate_to_position(
	real_vector3d *position,
	real_vector3d const *position_desired,
	real_vector3d *angular_velocity,
	real angular_velocity_magnitude_maximum,
	real angular_acceleration_magnitude_maximum)
{
	real cosine_value;
	real speed;
	real speed_squared;
	real_vector3d axis;
	real_vector3d delta;
	real delta_magnitude_squared;
	real angle;

	if (angular_acceleration_magnitude_maximum <= 0.0f &&
		angular_velocity_magnitude_maximum <= 0.0f)
	{
		*angular_velocity = *global_zero_vector3d;
		*position = *position_desired;
		return;
	}

	cosine_value = dot_product3d(position, position_desired);
	if (cosine_value < -1.0f)
	{
		cosine_value = -1.0f;
	}
	else if (cosine_value > 1.0f)
	{
		cosine_value = 1.0f;
	}

	speed_squared = arccosine(cosine_value);
	speed_squared =
		speed_squared * angular_acceleration_magnitude_maximum +
		speed_squared * angular_acceleration_magnitude_maximum;
	if (speed_squared >=
		angular_velocity_magnitude_maximum *
		angular_velocity_magnitude_maximum)
	{
		speed = angular_velocity_magnitude_maximum;
	}
	else
	{
		speed = square_root(speed_squared);
	}

	cross_product3d(position, position_desired, &axis);
	normalize3d(&axis);

	scale_vector3d(&axis, speed, &axis);
	subtract_vectors3d(&axis, angular_velocity, &delta);
	delta_magnitude_squared = magnitude_squared3d(&delta);
	if (delta_magnitude_squared <
		angular_acceleration_magnitude_maximum *
		angular_acceleration_magnitude_maximum)
	{
		if (speed < 1.0000001e-6f)
		{
			*angular_velocity = *global_zero_vector3d;
			*position = *position_desired;
			return;
		}
		*angular_velocity = axis;
	}
	else
	{
		angular_acceleration_magnitude_maximum =
			angular_acceleration_magnitude_maximum /
			square_root(delta_magnitude_squared);
		angular_velocity->i =
			delta.i * angular_acceleration_magnitude_maximum +
			angular_velocity->i;
		angular_velocity->j =
			delta.j * angular_acceleration_magnitude_maximum +
			angular_velocity->j;
		angular_velocity->k =
			delta.k * angular_acceleration_magnitude_maximum +
			angular_velocity->k;
	}

	axis = *angular_velocity;
	angle = normalize3d(&axis);
	if (angle != 0.0f)
	{
		rotate_vector_about_axis(position, &axis, sine(angle), cosine(angle));
		normalize3d(position);
	}
	return;
}


boolean accelerate_to_velocity3d(
	real_vector3d *velocity,
	real_vector3d const *target_velocity,
	real acceleration)
{
	real_vector3d delta;
	boolean result;

	subtract_vectors3d(target_velocity, velocity, &delta);
	if (limit3d(&delta, acceleration))
	{
		add_vectors3d(velocity, &delta, velocity);
		result = FALSE;
	}
	else
	{
		*velocity = *target_velocity;
		result = TRUE;
	}

	return result;
}

boolean accerate_to_position3d(
	real_point3d *position,
	real_vector3d *velocity,
	real_point3d const *target_position,
	long unused,
	real maximum_length)
{
	volatile real delta[3];
	real magnitude;
	real scale;

	delta[0] = target_position->x - position->x;
	delta[1] = target_position->y - position->y;
	delta[2] = target_position->z - position->z;
	magnitude = normalize3d((real_vector3d *)(real *)delta);
	if (magnitude == 0.0f)
	{
		delta[0] = 0.0f;
		delta[1] = 0.0f;
		delta[2] = 0.0f;
	}
	else
	{
		scale = square_root(
			magnitude * maximum_length +
			magnitude * maximum_length);
		delta[0] = delta[0] * scale;
		delta[1] = delta[1] * scale;
		delta[2] = delta[2] * scale;
	}

	if (accelerate_to_velocity3d(
			velocity,
			(real_vector3d *)(real *)delta,
			maximum_length) &&
		magnitude == 0.0f)
	{
		*position = *target_position;
		return TRUE;
	}

	position->x = position->x + velocity->i;
	position->y = velocity->j + position->y;
	position->z = velocity->k + position->z;
	return FALSE;
}

boolean accelerate_to_position(
	real *position,
	real *velocity,
	real target_position,
	real maximum_velocity,
	real acceleration,
	real minimum_position,
	real maximum_position,
	boolean periodic)
{
	real current_position;
	real current_velocity;
	real delta;
	real half_range;
	real limit;
	real speed;
	real braking_distance;
	real step_clamped;
	real new_position;
	real output_position;
	real acceleration_squared;
	boolean result;

	current_position = *position;
	current_velocity = *velocity;
	acceleration_squared = acceleration * acceleration;
	result = FALSE;
	delta = target_position - current_position;

	if (periodic)
	{
		half_range = (maximum_position - minimum_position) * 0.5f;
		if (delta > half_range)
		{
			delta = delta - (half_range + half_range);
		}
		else if (delta < -half_range)
		{
			delta = half_range + half_range + delta;
		}
	}

	limit = MIN(maximum_velocity, acceleration);

	if (fabs(delta - current_velocity) <= limit)
	{
		current_velocity = 0.0f;
		output_position = target_position;
		if (target_position < minimum_position)
			output_position = minimum_position;
		else if (target_position > maximum_position)
			output_position = maximum_position;
		result = TRUE;
	}
	else
	{
		braking_distance = (maximum_velocity + maximum_velocity) * fabs(delta);
		if (braking_distance >= acceleration_squared)
			speed = acceleration;
		else
			speed = square_root(braking_distance);
		if (delta < 0.0f)
			speed = -speed;
		step_clamped = speed - current_velocity;
		if (fabs(step_clamped) > maximum_velocity)
		{
			if (step_clamped < 0.0f)
				step_clamped = -maximum_velocity;
			else
				step_clamped = maximum_velocity;
		}
		current_velocity = current_velocity + step_clamped;
		new_position = step_clamped * 0.5f + current_velocity + current_position;
		if (periodic)
		{
			if (new_position < minimum_position)
				new_position = maximum_position - minimum_position + new_position;
			else if (new_position > maximum_position)
				new_position = new_position - (maximum_position - minimum_position);
		}
		if (new_position < minimum_position)
			new_position = minimum_position;
		else if (new_position > maximum_position)
			new_position = maximum_position;
		output_position = new_position;
	}

	*velocity = current_velocity;
	*position = output_position;
	return result;
}

void accelerate_to_velocity(
	real *position,
	real *velocity,
	real acceleration,
	real target_velocity,
	real minimum_position,
	real maximum_position,
	boolean periodic)
{
	if (periodic)
	{
		real new_position = *position;
		real new_velocity = *velocity;
		real delta = target_velocity - new_velocity;

		if (fabs(delta) > acceleration)
		{
			if (delta < 0.0f)
				delta = -acceleration;
			else
				delta = acceleration;
		}

		new_velocity += delta;
		new_position += new_velocity + delta * 0.5f;
		if (new_position < minimum_position || new_position > maximum_position)
		{
			new_position = fmod(
				new_position - minimum_position,
				maximum_position - minimum_position) - minimum_position;
		}

		*position = new_position;
		*velocity = new_velocity;
	}
	else
	{
		accelerate_to_position(
			position,
			velocity,
			target_velocity < 0.0f ? minimum_position : maximum_position,
			acceleration,
			fabs(target_velocity),
			minimum_position,
			maximum_position,
			FALSE);
	}

	return;
}

boolean valid_real_sine_cosine(
	real sine, 
	real cosine)
{
	return valid_realcmp(sine * sine + cosine * cosine, 1.0f);
}

/* ---------- private code */

#ifdef HALO_N64
#include <math.h>

/* The N64's trigonometry: Cephes' single precision routines (S. Moshier),
inline. newlib's sinf and cosf took every angle past pi/4 through
__ieee754_rem_pio2f, and its atanf/acosf through divisions and long
polynomials: a few per cent of a frame in the HUD and the units' aiming
(aiming_screen_apply). Within a few units in the last place; angles
beyond +-8192 (or out of acos's range) go to newlib. */
#define N64_PI 3.14159265358979f
#define N64_PI_2 1.57079632679490f
#define N64_PI_4 0.78539816339745f

/* an angle's octant and its remainder there: sine of the remainder when
the octant asks (*use_sine), cosine else */
static __inline real n64_trig_reduce(
	real x,
	int *octant)
{
	int j = (int)(x * 1.27323954473516f);
	real y = (real)j;

	if (j & 1)
	{
		j += 1;
		y += 1.f;
	}
	*octant = j & 7;
	return ((x - y * 0.78515625f) - y * 2.4187564849853515625e-4f) - y * 3.77489497744594108e-8f;
}

static __inline real n64_sine_polynomial(
	real x)
{
	real z = x * x;

	return ((-1.9515295891e-4f * z + 8.3321608736e-3f) * z - 1.6666654611e-1f) * z * x + x;
}

static __inline real n64_cosine_polynomial(
	real x)
{
	real z = x * x;

	return ((2.443315711809948e-5f * z - 1.388731625493765e-3f) * z + 4.166664568298827e-2f) * z * z - 0.5f * z + 1.f;
}

real n64_sinf(
	real angle)
{
	real x = angle < 0.f ? -angle : angle, y;
	int negative = angle < 0.f, octant;

	if (!(x < 8192.f))
		return sinf(angle);
	x = n64_trig_reduce(x, &octant);
	if (octant > 3)
	{
		negative = !negative;
		octant -= 4;
	}
	y = octant == 1 || octant == 2 ? n64_cosine_polynomial(x) : n64_sine_polynomial(x);
	return negative ? -y : y;
}

real n64_cosf(
	real angle)
{
	real x = angle < 0.f ? -angle : angle, y;
	int negative = 0, octant;

	if (!(x < 8192.f))
		return cosf(angle);
	x = n64_trig_reduce(x, &octant);
	if (octant > 3)
	{
		negative = !negative;
		octant -= 4;
	}
	if (octant > 1)
		negative = !negative;
	y = octant == 1 || octant == 2 ? n64_sine_polynomial(x) : n64_cosine_polynomial(x);
	return negative ? -y : y;
}

static __inline real n64_atanf(
	real value)
{
	real x = value < 0.f ? -value : value, y, z;

	if (x > 2.414213562373095f)
	{
		y = N64_PI_2;
		x = -1.f / x;
	}
	else if (x > 0.4142135623730950f)
	{
		y = N64_PI_4;
		x = (x - 1.f) / (x + 1.f);
	}
	else
		y = 0.f;
	z = x * x;
	y += (((8.05374449538e-2f * z - 1.38776856032e-1f) * z + 1.99777106478e-1f) * z - 3.33329491539e-1f) * z * x + x;
	return value < 0.f ? -y : y;
}

real n64_atan2f(
	real y,
	real x)
{
	real angle;

	if (y != y || x != x)
		return atan2f(y, x);
	if (x == 0.f)
		return y > 0.f ? N64_PI_2 : y < 0.f ? -N64_PI_2 : 0.f;
	if (y == 0.f)
		return x < 0.f ? N64_PI : 0.f;
	angle = n64_atanf(y / x);
	if (x < 0.f)
		angle += y < 0.f ? -N64_PI : N64_PI;
	return angle;
}

/* asin for |x| <= 1 */
static __inline real n64_asinf(
	real value)
{
	real a = value < 0.f ? -value : value, x, z;
	int large = a > 0.5f;

	if (a < 1.0e-4f)
		return value;
	if (large)
	{
		z = 0.5f * (1.f - a);
		x = sqrtf(z);
	}
	else
	{
		x = a;
		z = x * x;
	}
	z = ((((4.2163199048e-2f * z + 2.4181311049e-2f) * z + 4.5470025998e-2f) * z + 7.4953002686e-2f) * z +
		1.6666752422e-1f) * z * x + x;
	if (large)
		z = N64_PI_2 - (z + z);
	return value < 0.f ? -z : z;
}

real n64_acosf(
	real x)
{
	if (!(x >= -1.f && x <= 1.f))
		return acosf(x);
	if (x < -0.5f)
		return N64_PI - 2.f * n64_asinf(sqrtf(0.5f * (1.f + x)));
	if (x > 0.5f)
		return 2.f * n64_asinf(sqrtf(0.5f * (1.f - x)));
	return N64_PI_2 - n64_asinf(x);
}

/* pow and exp in single precision (port/include/halo_math.h sends the
game's calls here): the motion sensor's blips, the HUD's fades, light
volumes and spawn ratings went through musl's double pow. Integer powers
are multiplied out (exact where the double ones were: pow(10, 4)); the rest
is 2 to the power y log2 x, Cephes' polynomials, within a few units in the
last place; zero, negative, huge or tiny x go to newlib */
static __inline real n64_exp2f(
	real x)
{
	union { real f; unsigned long u; } scale;
	long whole;
	real fraction;

	if (!(x > -126.f))
		return x != x ? x : 0.f;
	if (!(x < 127.f))
		return powf(2.f, x);
	whole = (long)x;
	if ((real)whole > x)
		whole--;
	fraction = x - (real)whole;
	if (fraction > 0.5f)
	{
		fraction -= 1.f;
		whole++;
	}
	scale.u = (unsigned long)(whole + 127) << 23;
	return (fraction * (((((1.535336188319500e-4f * fraction + 1.339887440266574e-3f) * fraction +
		9.618437357674640e-3f) * fraction + 5.550332471162809e-2f) * fraction + 2.402264791363012e-1f) * fraction +
		6.931472028550421e-1f) + 1.f) * scale.f;
}

/* log2 of a positive, normal, finite x */
static __inline real n64_log2f(
	real x)
{
	union { real f; unsigned long u; } bits;
	long exponent;
	real mantissa, z, z2;

	bits.f = x;
	exponent = (long)((bits.u >> 23) & 0xFF) - 127;
	bits.u = (bits.u & 0x007FFFFF) | 0x3F800000;
	mantissa = bits.f;
	if (mantissa > 1.41421356f)
	{
		mantissa *= 0.5f;
		exponent++;
	}
	/* ln m = 2 atanh z, z = (m - 1) / (m + 1), |z| < 0.172 */
	z = (mantissa - 1.f) / (mantissa + 1.f);
	z2 = z * z;
	return (real)exponent + 2.885390082f * z *
		((((z2 * (1.f / 9.f) + (1.f / 7.f)) * z2 + (1.f / 5.f)) * z2 + (1.f / 3.f)) * z2 + 1.f);
}

real n64_powf(
	real x,
	real y)
{
	long whole = (long)y;

	if ((real)whole == y && whole >= -32 && whole <= 32)
	{
		unsigned long count = whole < 0 ? -whole : whole;
		real result = 1.f, base = x;

		while (count)
		{
			if (count & 1)
				result *= base;
			count >>= 1;
			if (count)
				base *= base;
		}
		return whole < 0 ? 1.f / result : result;
	}
	if (y == 0.5f && x >= 0.f)
		return sqrtf(x);
	if (!(x >= 1.17549435e-38f && x <= 3.4e38f))
		return powf(x, y);
	return n64_exp2f(y * n64_log2f(x));
}

real n64_expf(
	real x)
{
	return n64_exp2f(x * 1.44269504088896f);
}
#endif

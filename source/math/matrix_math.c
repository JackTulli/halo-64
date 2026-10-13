/*
MATRIX_MATH.C

symbols in this file:
000F8900 0040:
	_matrix4x3_identity (0000)
000F8940 0030:
	_matrix4x3_transpose (0000)
000F8970 00f0:
	_matrix4x3_inverse (0000)
000F8A60 0040:
	_matrix4x3_scale (0000)
000F8AA0 0050:
	_matrix4x3_translation (0000)
000F8AF0 00e0:
	_matrix4x3_rotation_from_axis_and_angle (0000)
000F8BD0 0120:
	_matrix4x3_rotation_from_quaternion (0000)
000F8CF0 0030:
	_matrix4x3_from_point_and_quaternion (0000)
000F8D20 0040:
	_matrix4x3_from_orientation (0000)
000F8D60 0050:
	_matrix4x3_to_point_and_vectors (0000)
000F8DB0 0080:
	_matrix4x3_transform_point (0000)
000F8E30 0070:
	_matrix4x3_transform_vector (0000)
000F8EA0 0060:
	_matrix4x3_transform_normal (0000)
000F8F00 00a0:
	_matrix4x3_inverse_transform_point (0000)
000F8FA0 0070:
	_matrix4x3_inverse_transform_vector (0000)
000F9010 0060:
	_matrix4x3_inverse_transform_normal (0000)
000F9070 0150:
	_matrix4x3_multiply (0000)
000F91C0 0050:
	_matrix3x3_determinant (0000)
000F9210 0070:
	_matrix3x3_transpose (0000)
000F9280 0140:
	_matrix3x3_inverse (0000)
000F93C0 00d0:
	_matrix3x3_from_axis_and_angle (0000)
000F9490 0120:
	_matrix3x3_multiply (0000)
000F95B0 0080:
	_matrix3x3_transform_vector (0000)
000F9630 0080:
	_matrix4x3_rotation_from_vectors (0000)
000F96B0 00b0:
	_matrix4x3_rotation_from_angles (0000)
000F9760 0080:
	_matrix4x3_rotation_to_angles (0000)
000F97E0 0150:
	_matrix4x3_rotation_to_quaternion (0000)
000F9930 0040:
	_matrix4x3_from_point_and_vectors (0000)
000F9970 0070:
	_vector_from_matrices4x3 (0000)
000F99E0 0080:
	_matrix4x3_transform_plane (0000)
000F9A60 0080:
	_matrix4x3_inverse_transform_plane (0000)
000F9AE0 0070:
	_matrix3x3_from_forward_and_up (0000)
000F9B50 0150:
	_matrix3x3_rotation_to_quaternion (0000)
000F9CA0 0040:
	_valid_real_plane3d (0000)
000F9CE0 00b0:
	_matrix4x3_from_plane (0000)
0027AC18 001c:
	??_C@_0BM@OIPNJJOM@?$CBrealcmp?$CIdeterminant?0?50?40f?$CJ?$AA@ (0000)
0027AC34 0022:
	??_C@_0CC@FNOHDKIK@c?3?2halo?2SOURCE?2math?2matrix_math?4@ (0000)
0027AC58 0008:
	__real@3f1a36e2eb1c432d (0000)
0027AC60 001a:
	??_C@_0BK@BHMHOMPE@valid_real_plane3d?$CIplane?$CJ?$AA@ (0000)
0030790C 000e:
	_data_0030790c (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "real_math.h"

/* ---------- constants */

/* ---------- macros */



/* ---------- structures */

/* ---------- prototypes */

/* ---------- globals */

/* ---------- public code */

void
matrix4x3_identity(
	real_matrix4x3 *matrix)
{
	matrix->scale = 1.f;
	matrix->forward.i = 1.f;
	matrix->forward.j = 0.f;
	matrix->forward.k = 0.f;
	matrix->left.i = 0.f;
	matrix->left.j = 1.f;
	matrix->left.k = 0.f;
	matrix->up.i = 0.f;
	matrix->up.j = 0.f;
	matrix->up.k = 1.f;
	matrix->position.x = 0.f;
	matrix->position.y = 0.f;
	matrix->position.z = 0.f;

	return;
}

void
matrix4x3_transpose(
	real_matrix4x3 *matrix)
{
	real temporary;

	temporary = matrix->left.i;
	matrix->left.i = matrix->forward.j;
	matrix->forward.j = temporary;

	temporary = matrix->up.i;
	matrix->up.i = matrix->forward.k;
	matrix->forward.k = temporary;

	temporary = matrix->up.j;
	matrix->up.j = matrix->left.k;
	matrix->left.k = temporary;

	return;
}

void
matrix4x3_scale(
	real_matrix4x3 *matrix,
	real scale)
{
	matrix->scale = scale;
	matrix->forward.i = 1.f;
	matrix->forward.j = 0.f;
	matrix->forward.k = 0.f;
	matrix->left.i = 0.f;
	matrix->left.j = 1.f;
	matrix->left.k = 0.f;
	matrix->up.i = 0.f;
	matrix->up.j = 0.f;
	matrix->up.k = 1.f;
	matrix->position.x = 0.f;
	matrix->position.y = 0.f;
	matrix->position.z = 0.f;

	return;
}

void matrix4x3_translation(
	real_matrix4x3 *matrix,
	real_point3d const *point)
{
	matrix->scale = 1.f;
	matrix->forward.i = 1.f;
	matrix->forward.j = 0.f;
	matrix->forward.k = 0.f;
	matrix->left.i = 0.f;
	matrix->left.j = 1.f;
	matrix->left.k = 0.f;
	matrix->up.i = 0.f;
	matrix->up.j = 0.f;
	matrix->up.k = 1.f;
	matrix->position = *point;

	return;
}

void matrix4x3_rotation_from_quaternion(
	real_matrix4x3 *matrix,
	real_quaternion const *quaternion)
{
	real scale =
		quaternion->v.i * quaternion->v.i +
		quaternion->v.j * quaternion->v.j +
		quaternion->v.k * quaternion->v.k +
		quaternion->w * quaternion->w;
	real x;
	real y;
	real z;
	real wx;
	real wy;
	real wz;
	real xx;
	real xy;
	real xz;
	real yy;
	real yz;
	real zz;

	if (scale != 0.f)
		scale = 2.f / scale;
	else
		scale = 0.f;

	x = scale * quaternion->v.i;
	y = scale * quaternion->v.j;
	z = scale * quaternion->v.k;
	wx = quaternion->w * x;
	wy = quaternion->w * y;
	wz = quaternion->w * z;
	xx = quaternion->v.i * x;
	xy = quaternion->v.i * y;
	xz = quaternion->v.i * z;
	yy = quaternion->v.j * y;
	yz = quaternion->v.j * z;
	zz = quaternion->v.k * z;

	matrix->scale = 1.f;
	matrix->forward.i = 1.f - (yy + zz);
	matrix->forward.j = xy - wz;
	matrix->forward.k = xz + wy;
	matrix->left.i = xy + wz;
	matrix->left.j = 1.f - (xx + zz);
	matrix->left.k = yz - wx;
	matrix->up.i = xz - wy;
	matrix->up.j = yz + wx;
	matrix->up.k = 1.f - (xx + yy);
	matrix->position.x = 0.f;
	matrix->position.y = 0.f;
	matrix->position.z = 0.f;

	return;
}

void matrix4x3_from_point_and_quaternion(
	real_matrix4x3 *matrix,
	real_point3d const *point,
	real_quaternion const *quaternion)
{
	matrix4x3_rotation_from_quaternion(matrix, quaternion);
	matrix->position = *point;

	return;
}

void matrix4x3_from_orientation(
	real_matrix4x3 *matrix,
	real_orientation const *orientation)
{
	matrix4x3_rotation_from_quaternion(matrix, &orientation->rotation);
	matrix->scale = orientation->scale;
	matrix->position = orientation->translation;

	return;
}

real_matrix3x3 *matrix3x3_transpose(
	real_matrix3x3 const *matrix,
	real_matrix3x3 *result)
{
	if (matrix == result)
	{
		real temporary;

		temporary = matrix->forward.j;
		result->forward.j = matrix->left.i;
		result->left.i = temporary;

		temporary = matrix->forward.k;
		result->forward.k = matrix->up.i;
		result->up.i = temporary;

		temporary = matrix->left.k;
		result->left.k = matrix->up.j;
		result->up.j = temporary;
	}
	else
	{
		result->forward.i = matrix->forward.i;
		result->forward.j = matrix->left.i;
		result->forward.k = matrix->up.i;
		result->left.i = matrix->forward.j;
		result->left.j = matrix->left.j;
		result->left.k = matrix->up.j;
		result->up.i = matrix->forward.k;
		result->up.j = matrix->left.k;
		result->up.k = matrix->up.k;
	}

	return result;
}

real_matrix3x3 *matrix3x3_multiply(
	real_matrix3x3 const *a,
	real_matrix3x3 const *b,
	real_matrix3x3 *result)
{
	real_matrix3x3 temporary;

	if (a == result)
	{
		temporary = *a;
		a = &temporary;
	}

	if (b == result)
	{
		temporary = *b;
		b = &temporary;
	}

	result->forward.i = b->forward.i * a->forward.i + b->forward.j * a->left.i + a->up.i * b->forward.k;
	result->forward.j = b->forward.j * a->left.j + b->forward.i * a->forward.j + b->forward.k * a->up.j;
	result->forward.k = b->forward.j * a->left.k + b->forward.i * a->forward.k + b->forward.k * a->up.k;

	result->left.i = b->left.j * a->left.i + a->forward.i * b->left.i + a->up.i * b->left.k;
	result->left.j = b->left.k * a->up.j + b->left.j * a->left.j + b->left.i * a->forward.j;
	result->left.k = b->left.k * a->up.k + b->left.j * a->left.k + b->left.i * a->forward.k;

	result->up.i = b->up.i * a->forward.i + b->up.j * a->left.i + b->up.k * a->up.i;
	result->up.j = b->up.k * a->up.j + b->up.j * a->left.j + b->up.i * a->forward.j;
	result->up.k = b->up.k * a->up.k + b->up.j * a->left.k + b->up.i * a->forward.k;

	return result;
}

real_matrix3x3 *matrix3x3_from_forward_and_up(
	real_matrix3x3 *matrix,
	real_vector3d const *forward,
	real_vector3d const *up)
{
	matrix->forward = *forward;
	cross_product3d(up, forward, &matrix->left);
	matrix->up = *up;

	return matrix;
}

/* January keeps both next-index tables in one 14-byte .data blob. The 4x3
table starts at offset 0 and the 3x3 table starts at offset 8. */
static short data_0030790c[7] = { 1, 2, 0, 0, 1, 2, 0 };

#define matrix4x3_next (data_0030790c)
#define matrix3x3_next (data_0030790c+4)

void matrix4x3_rotation_from_axis_and_angle(
	real_matrix4x3 *matrix,
	real_vector3d const *axis,
	real sine,
	real cosine)
{
	real_vector3d squared;
	real_vector3d scaled;
	real omc;

	squared.i = axis->i * axis->i;
	squared.j = axis->j * axis->j;
	squared.k = axis->k * axis->k;

	scaled.i = sine * axis->i;
	scaled.j = sine * axis->j;
	scaled.k = sine * axis->k;

	matrix->scale = 1.f;

	matrix->n[0][0] = (1.f - squared.i) * cosine + squared.i;

	omc = 1.f - cosine;

	matrix->n[0][1] = axis->j * axis->i * omc;
	matrix->n[1][0] = matrix->n[0][1] - scaled.k;
	matrix->n[0][1] = scaled.k + matrix->n[0][1];

	matrix->n[1][1] = (1.f - squared.j) * cosine + squared.j;

	matrix->n[0][2] = axis->k * axis->i * omc;
	matrix->n[2][0] = matrix->n[0][2] + scaled.j;
	matrix->n[0][2] = matrix->n[0][2] - scaled.j;

	matrix->n[2][2] = (1.f - squared.k) * cosine + squared.k;

	matrix->n[1][2] = axis->k * axis->j * omc;
	matrix->n[2][1] = matrix->n[1][2] - scaled.i;
	matrix->n[1][2] = scaled.i + matrix->n[1][2];

	matrix->position.z = 0.f;
	matrix->position.y = 0.f;
	matrix->position.x = 0.f;

	return;
}

void matrix3x3_from_axis_and_angle(
	real_matrix3x3 *matrix,
	real_vector3d const *axis,
	real sine,
	real cosine)
{
	real_vector3d squared;
	real_vector3d scaled;
	real omc;

	squared.i = axis->i * axis->i;
	squared.j = axis->j * axis->j;
	squared.k = axis->k * axis->k;

	scaled.i = sine * axis->i;
	scaled.j = sine * axis->j;
	scaled.k = sine * axis->k;

	matrix->n[0][0] = (1.f - squared.i) * cosine + squared.i;

	omc = 1.f - cosine;

	matrix->n[0][1] = axis->j * axis->i * omc;
	matrix->n[1][0] = matrix->n[0][1] - scaled.k;
	matrix->n[0][1] = scaled.k + matrix->n[0][1];

	matrix->n[1][1] = (1.f - squared.j) * cosine + squared.j;

	matrix->n[0][2] = axis->k * axis->i * omc;
	matrix->n[2][0] = matrix->n[0][2] + scaled.j;
	matrix->n[0][2] = matrix->n[0][2] - scaled.j;

	matrix->n[2][2] = (1.f - squared.k) * cosine + squared.k;

	matrix->n[1][2] = axis->k * axis->j * omc;
	matrix->n[2][1] = matrix->n[1][2] - scaled.i;
	matrix->n[1][2] = scaled.i + matrix->n[1][2];

	return;
}

void matrix4x3_rotation_to_quaternion(
	real_matrix4x3 const *matrix,
	real_quaternion *quaternion)
{
	real trace = matrix->n[0][0] + matrix->n[1][1] + matrix->n[2][2];

	if (trace > 0.f)
	{
		real s = square_root(trace + 1.f);

		quaternion->w = 0.5f * s;
		s = 0.5f / s;

		quaternion->v.i = (matrix->n[2][1] - matrix->n[1][2]) * s;
		quaternion->v.j = (matrix->n[0][2] - matrix->n[2][0]) * s;
		quaternion->v.k = (matrix->n[1][0] - matrix->n[0][1]) * s;
	}
	else
	{
		short i = 0;
		short j, k;
		real q[3];
		real s;

		if (matrix->n[1][1] > matrix->n[0][0])
			i = 1;

		if (matrix->n[2][2] > matrix->n[i][i])
			i = 2;

		j = matrix4x3_next[i];
		k = matrix4x3_next[j];

		s = square_root(matrix->n[i][i] - (matrix->n[k][k] + matrix->n[j][j]) + 1.f);

		q[i] = s * 0.5f;

		if (s != 0.f)
			s = 0.5f / s;

		q[j] = (matrix->n[i][j] + matrix->n[j][i]) * s;
		q[k] = (matrix->n[i][k] + matrix->n[k][i]) * s;

		quaternion->w = (matrix->n[k][j] - matrix->n[j][k]) * s;

		quaternion->v.i = q[0];
		quaternion->v.j = q[1];
		quaternion->v.k = q[2];
	}

	return;
}

void matrix3x3_rotation_to_quaternion(
	real_matrix3x3 const *matrix,
	real_quaternion *quaternion)
{
	real trace = matrix->n[1][1] + matrix->n[0][0] + matrix->n[2][2];

	if (trace > 0.f)
	{
		real s = square_root(trace + 1.f);

		quaternion->w = 0.5f * s;
		s = 0.5f / s;

		quaternion->v.i = (matrix->n[2][1] - matrix->n[1][2]) * s;
		quaternion->v.j = (matrix->n[0][2] - matrix->n[2][0]) * s;
		quaternion->v.k = (matrix->n[1][0] - matrix->n[0][1]) * s;
	}
	else
	{
		short i = 0;
		short j, k;
		real q[3];
		real s;

		if (matrix->n[1][1] > matrix->n[0][0])
			i = 1;

		if (matrix->n[2][2] > matrix->n[i][i])
			i = 2;

		j = matrix3x3_next[i];
		k = matrix3x3_next[j];

		s = square_root(matrix->n[i][i] - (matrix->n[k][k] + matrix->n[j][j]) + 1.f);

		q[i] = s * 0.5f;

		if (s != 0.f)
			s = 0.5f / s;

		q[j] = (matrix->n[i][j] + matrix->n[j][i]) * s;
		q[k] = (matrix->n[i][k] + matrix->n[k][i]) * s;

		quaternion->w = (matrix->n[k][j] - matrix->n[j][k]) * s;

		quaternion->v.i = q[0];
		quaternion->v.j = q[1];
		quaternion->v.k = q[2];
	}

	return;
}

real_matrix3x3 *matrix3x3_inverse(
	real_matrix3x3 const *matrix,
	real determinant,
	real_matrix3x3 *result)
{
	real_matrix3x3 temporary;
	real inverse;
	short i, j;

	if (matrix == result)
	{
		temporary = *matrix;
		matrix = &temporary;
	}

	match_assert("c:\\halo\\SOURCE\\math\\matrix_math.c", 787, !realcmp(determinant, 0.0f));

	inverse = 1.f / determinant;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			short next_i = i < 2 ? i + 1 : 0;
			short previous_i = i > 0 ? i - 1 : 2;
			short next_j = j < 2 ? j + 1 : 0;
			short previous_j = j > 0 ? j - 1 : 2;

			result->n[j][i] = (matrix->n[next_i][next_j] * matrix->n[previous_i][previous_j]
				- matrix->n[next_i][previous_j] * matrix->n[previous_i][next_j]) * inverse;
		}
	}

	return result;
}

void matrix4x3_inverse(
	real_matrix4x3 const *matrix,
	real_matrix4x3 *result)
{
	if (matrix->scale!=0.f)
	{
		real temp;

		real x = -matrix->position.x;
		real y = -matrix->position.y;
		real z = -matrix->position.z;

		if (matrix->scale!=1.f)
		{
			real scale = 1.f / matrix->scale;
			result->scale = scale;
			x *= scale;
			y *= scale;
			z *= scale;
		}
		else
		{
			result->scale = 1.f;
		}

		result->n[0][0] = matrix->n[0][0];
		result->n[1][1] = matrix->n[1][1];
		result->n[2][2] = matrix->n[2][2];

		temp = matrix->n[1][0];
		result->n[1][0] = matrix->n[0][1];
		result->n[0][1] = temp;

		temp = matrix->n[2][0];
		result->n[2][0] = matrix->n[0][2];
		result->n[0][2] = temp;

		temp = matrix->n[2][1];
		result->n[2][1] = matrix->n[1][2];
		result->n[1][2] = temp;

		result->n[3][0] = (x*result->n[0][0]) + y*result->n[1][0] + (z*result->n[2][0]);
		result->n[3][1] = (x*result->n[0][1]) + y*result->n[1][1] + (z*result->n[2][1]);
		result->n[3][2] = (x*result->n[0][2]) + y*result->n[1][2] + (z*result->n[2][2]);
	}
	else
	{
		memset(result, 0, sizeof(*result));
	}

	return;
}

void matrix4x3_rotation_from_vectors(
	real_matrix4x3 *matrix,
	real_vector3d const *forward,
	real_vector3d const *up)

{
	matrix->scale = 1.f;
	matrix->forward = *forward;
	cross_product3d(up, forward, &matrix->left);

	matrix->up = *up;
	set_real_point3d(&matrix->position, 0.f, 0.f, 0.f);

	return;
}

void matrix4x3_rotation_from_angles(
	real_matrix4x3 *matrix,
	real yaw,
	real pitch,
	real roll)
{
	real cosine_roll = cosine(roll);
	real sine_roll = sine(roll);
	real cosine_pitch = cosine(pitch);
	real sine_pitch = sine(pitch);
	real cosine_yaw = cosine(yaw);
	real sine_yaw = sine(yaw);
	real sine_pitch_cosine_roll = sine_pitch * cosine_roll;
	real sine_pitch_sine_roll = sine_pitch * sine_roll;

	matrix->scale = 1.f;
	set_real_point3d(&matrix->position, 0.f, 0.f, 0.f);

	matrix->n[0][0] = cosine_yaw * cosine_pitch;
	matrix->n[0][1] = sine_yaw * cosine_roll - cosine_yaw * sine_pitch_sine_roll;
	matrix->n[0][2] = cosine_yaw * sine_pitch_cosine_roll + sine_yaw * sine_roll;

	matrix->n[1][0] = -(sine_yaw * cosine_pitch);
	matrix->n[1][1] = sine_yaw * sine_pitch_sine_roll + cosine_yaw * cosine_roll;
	matrix->n[1][2] = cosine_yaw * sine_roll - sine_yaw * sine_pitch_cosine_roll;

	matrix->n[2][0] = -sine_pitch;
	matrix->n[2][1] = -(cosine_pitch * sine_roll);
	matrix->n[2][2] = cosine_pitch * cosine_roll;

	return;
}

void matrix4x3_rotation_to_angles(
	real_matrix4x3 *matrix,
	real_euler_angles3d *angles)
{
	real cosine_pitch;

	angles->pitch = -arcsine(matrix->n[2][0]);
	cosine_pitch = cosine(angles->pitch);

	if (cosine_pitch > 0.0001)
	{
		angles->roll = arctangent(-matrix->n[2][1] / cosine_pitch,
			matrix->n[2][2] / cosine_pitch);
		angles->yaw = arctangent(-matrix->n[1][0] / cosine_pitch,
			matrix->n[0][0] / cosine_pitch);
	}
	else
	{
		angles->roll = 0.f;
		angles->yaw = arctangent(matrix->n[0][1], matrix->n[1][1]);
	}

	return;
}

void matrix4x3_from_point_and_vectors(
	real_matrix4x3 *matrix,
	real_point3d const *point,
	real_vector3d const *forward,
	real_vector3d const *up)
{
	matrix4x3_rotation_from_vectors(matrix, forward, up);
	matrix->position = *point;
	
	return;
}

void
matrix4x3_to_point_and_vectors(
	real_matrix4x3 const *matrix,
	real_point3d *point,
	real_vector3d *forward,
	real_vector3d *up)
{
	*forward = matrix->forward;
	*up = matrix->up;
	*point = matrix->position;

	return;
}

real_vector3d *vector_from_matrices4x3(
	real_matrix4x3 const *a,
	real_matrix4x3 const *b,
	real_vector3d *rotation)
{
	real_matrix4x3 inverse_b;
	real_matrix4x3 relative;
	real_quaternion quaternion;
	real angle;

	matrix4x3_inverse(b, &inverse_b);
	matrix4x3_multiply(a, &inverse_b, &relative);
	matrix4x3_rotation_to_quaternion(&relative, &quaternion);
	quaternion_to_angle_and_vector(&quaternion, &angle, rotation);

	rotation->i *= angle;
	rotation->j *= angle;
	rotation->k *= angle;

	return rotation;
}

real_point3d *matrix4x3_transform_point(
	real_matrix4x3 const *matrix,
	real_point3d const *point,
	real_point3d *result)
{
	real x = point->x;
	real y = point->y;
	real z = point->z;

	if (matrix->scale != 1.f)
	{
		x *= matrix->scale;
		y *= matrix->scale;
		z *= matrix->scale;
	}

	result->x = matrix->up.i*z + matrix->left.i*y + matrix->forward.i*x + matrix->position.x;
	result->y = matrix->up.j*z + matrix->left.j*y + matrix->forward.j*x + matrix->position.y;
	result->z = matrix->up.k*z + matrix->left.k*y + matrix->forward.k*x + matrix->position.z;
	return result;
}

real_vector3d *matrix4x3_transform_vector(
	real_matrix4x3 const *matrix,
	real_vector3d const *vector,
	real_vector3d *result)
{
	real i = vector->i;
	real j = vector->j;
	real k = vector->k;

	if (matrix->scale != 1.f)
	{
		i *= matrix->scale;
		j *= matrix->scale;
		k *= matrix->scale;
	}

	result->i = i*matrix->forward.i + j*matrix->left.i + k*matrix->up.i;
	result->j = i*matrix->forward.j + j*matrix->left.j + k*matrix->up.j;
	result->k = i*matrix->forward.k + j*matrix->left.k + k*matrix->up.k;

	return result;
}

real_point3d *matrix4x3_inverse_transform_point(
	real_matrix4x3 const *matrix,
	real_point3d const *point,
	real_point3d *result)
{
	if (matrix->scale != 0.f)
	{
		real x = point->x - matrix->position.x;
		real y = point->y - matrix->position.y;
		real z = point->z - matrix->position.z;

		if (matrix->scale != 1.f)
		{
			real scale = 1.f / matrix->scale;
			x *= scale;
			y *= scale;
			z *= scale;
		}

		result->x = x * matrix->forward.i + y * matrix->forward.j + z * matrix->forward.k;
		result->y = x * matrix->left.i + y * matrix->left.j + z * matrix->left.k;
		result->z = x * matrix->up.i + y * matrix->up.j + z * matrix->up.k;
	}
	else
	{
		result->x = 0.f;
		result->y = 0.f;
		result->z = 0.f;
	}

	return result;
}

real_vector3d *matrix4x3_transform_normal(
	real_matrix4x3 const *matrix,
	real_vector3d const *normal,
	real_vector3d *result)
{
	real i = normal->i;
	real j = normal->j;
	real k = normal->k;

	result->i = i*matrix->forward.i + j*matrix->left.i + k*matrix->up.i;
	result->j = i*matrix->forward.j + j*matrix->left.j + k*matrix->up.j;
	result->k = i*matrix->forward.k + j*matrix->left.k + k*matrix->up.k;
	return result;
}

real_vector3d *
matrix4x3_inverse_transform_normal(
	real_matrix4x3 const *matrix,
	real_vector3d const *normal,
	real_vector3d *result)
{
	real i = normal->i;
	real j = normal->j;
	real k = normal->k;

	result->i = i * matrix->forward.i + j * matrix->forward.j + k * matrix->forward.k;
	result->j = i * matrix->left.i + j * matrix->left.j + k * matrix->left.k;
	result->k = i * matrix->up.i + j * matrix->up.j + k * matrix->up.k;

	return result;
}

real_vector3d *matrix4x3_inverse_transform_vector(
	real_matrix4x3 const *matrix,
	real_vector3d const *vector,
	real_vector3d *result)
{
	real i = vector->i;
	real j = vector->j;
	real k = vector->k;

	if (matrix->scale != 1.f)
	{
		real scale = 1.f / matrix->scale;
		i *= scale;
		j *= scale;
		k *= scale;
	}

	result->i = i * matrix->forward.i + j * matrix->forward.j + k * matrix->forward.k;
	result->j = i * matrix->left.i + j * matrix->left.j + k * matrix->left.k;
	result->k = i * matrix->up.i + j * matrix->up.j + k * matrix->up.k;

	return result;
}

real_plane3d *matrix4x3_transform_plane(
	real_matrix4x3 const *matrix,
	real_plane3d const *plane,
	real_plane3d *result)
{
	matrix4x3_transform_normal(matrix, &plane->n, &result->n);
	result->d = plane->d * matrix->scale + dot_product3d((real_vector3d const *)&matrix->position, &result->n);

	return result;
}

real_plane3d *matrix4x3_inverse_transform_plane(
	real_matrix4x3 const *matrix,
	real_plane3d const *plane,
	real_plane3d *result)
{
	if (matrix->scale != 0.f)
	{
		result->d = plane->d - dot_product3d((real_vector3d const *)&matrix->position, &plane->n);

		if (matrix->scale != 1.f)
			result->d /= matrix->scale;
	}
	else
	{
		result->d = 0.f;
	}

	matrix4x3_inverse_transform_vector(matrix, &plane->n, &result->n);

	return result;
}

void matrix4x3_multiply(
	real_matrix4x3 const *a,
	real_matrix4x3 const *b,
	real_matrix4x3 *result)
{
	real const *a_elements = a->n[0];
	real const *b_elements = b->n[0];
	real *result_elements = result->n[0];
	real const *a_scale = &a->scale;

#if defined(HALO_N64)
	{
		/* the C below written out (the VR4300: no loop, no copy; the same
		products and sums in the same order) */
		real a0 = a_elements[0], a1 = a_elements[1], a2 = a_elements[2];
		real a3 = a_elements[3], a4 = a_elements[4], a5 = a_elements[5];
		real a6 = a_elements[6], a7 = a_elements[7], a8 = a_elements[8];
		real b0 = b_elements[0], b1 = b_elements[1], b2 = b_elements[2];
		real b3 = b_elements[3], b4 = b_elements[4], b5 = b_elements[5];
		real b6 = b_elements[6], b7 = b_elements[7], b8 = b_elements[8];
		real b9 = b_elements[9], b10 = b_elements[10], b11 = b_elements[11];
		real scale = *a_scale;
		real p9 = (b9 * a0 + b10 * a3 + b11 * a6) * scale + a_elements[9];
		real p10 = (b9 * a1 + b10 * a4 + b11 * a7) * scale + a_elements[10];
		real p11 = (b9 * a2 + b10 * a5 + b11 * a8) * scale + a_elements[11];

		result_elements[0] = b0 * a0 + b1 * a3 + b2 * a6;
		result_elements[1] = b0 * a1 + b1 * a4 + b2 * a7;
		result_elements[2] = b0 * a2 + b1 * a5 + b2 * a8;
		result_elements[3] = b3 * a0 + b4 * a3 + b5 * a6;
		result_elements[4] = b3 * a1 + b4 * a4 + b5 * a7;
		result_elements[5] = b3 * a2 + b4 * a5 + b5 * a8;
		result_elements[6] = b6 * a0 + b7 * a3 + b8 * a6;
		result_elements[7] = b6 * a1 + b7 * a4 + b8 * a7;
		result_elements[8] = b6 * a2 + b7 * a5 + b8 * a8;
		result_elements[9] = p9;
		result_elements[10] = p10;
		result_elements[11] = p11;
	}
#elif defined(HALO_LINUX)
	{
		/* the SSE block below: rows of b combine the rows of a, and the
		position is scaled by a's scale before a's own is added */
		real product[12];
		long row, column;

		for (row = 0; row < 4; row++)
		{
			for (column = 0; column < 3; column++)
			{
				real value = b_elements[row * 3 + 0] * a_elements[column] +
					b_elements[row * 3 + 1] * a_elements[3 + column] +
					b_elements[row * 3 + 2] * a_elements[6 + column];

				if (row == 3)
					value = value * *a_scale + a_elements[9 + column];
				product[row * 3 + column] = value;
			}
		}
		for (row = 0; row < 12; row++)
			result_elements[row] = product[row];
	}
#else
	__asm
	{
		mov ecx, a_elements
		mov edx, b_elements
		mov eax, result_elements

		movss xmm2, dword ptr [ecx + 0x20]
		movhps xmm2, qword ptr [ecx + 0x18]
		movss xmm3, dword ptr [edx]
		movss xmm4, dword ptr [edx + 0x4]
		movss xmm0, dword ptr [ecx]
		movhps xmm0, qword ptr [ecx + 0x4]
		shufps xmm2, xmm2, 0x36
		shufps xmm3, xmm3, 0x0
		movss xmm1, dword ptr [ecx + 0xC]
		movhps xmm1, qword ptr [ecx + 0x10]
		shufps xmm4, xmm4, 0x0
		mulps xmm3, xmm0
		movss xmm5, dword ptr [edx + 0x8]
		movss xmm6, dword ptr [edx + 0xC]
		mulps xmm4, xmm1
		shufps xmm5, xmm5, 0x0
		mulps xmm5, xmm2
		shufps xmm6, xmm6, 0x0
		mulps xmm6, xmm0
		addps xmm3, xmm4
		movss xmm7, dword ptr [edx + 0x10]
		movss xmm4, dword ptr [edx + 0x1C]
		shufps xmm7, xmm7, 0x0
		addps xmm3, xmm5
		mulps xmm7, xmm1
		shufps xmm4, xmm4, 0x0
		movss xmm5, dword ptr [edx + 0x14]
		shufps xmm5, xmm5, 0x0
		mulps xmm4, xmm1
		mulps xmm5, xmm2
		addps xmm6, xmm7
		movss xmm7, dword ptr [edx + 0x18]
		movss dword ptr [eax], xmm3
		movhps qword ptr [eax + 0x4], xmm3
		addps xmm6, xmm5
		shufps xmm7, xmm7, 0x0
		movss xmm5, dword ptr [edx + 0x20]
		mulps xmm7, xmm0
		shufps xmm5, xmm5, 0x0
		movss dword ptr [eax + 0xC], xmm6
		mulps xmm5, xmm2
		addps xmm7, xmm4
		movhps qword ptr [eax + 0x10], xmm6
		addps xmm7, xmm5
		shufps xmm7, xmm7, 0x8F
		movhps qword ptr [eax + 0x18], xmm7
		movss dword ptr [eax + 0x20], xmm7
		movss xmm3, dword ptr [edx + 0x24]
		movss xmm4, dword ptr [edx + 0x28]
		movss xmm5, dword ptr [edx + 0x2C]
		shufps xmm3, xmm3, 0x0
		shufps xmm4, xmm4, 0x0
		shufps xmm5, xmm5, 0x0
		mulps xmm3, xmm0
		mulps xmm4, xmm1
		mulps xmm5, xmm2
		movss xmm6, dword ptr [ecx + 0x24]
		addps xmm3, xmm4
		movhps xmm6, qword ptr [ecx + 0x28]
		addps xmm3, xmm5
		mov ecx, a_scale
		movss xmm5, dword ptr [ecx]
		shufps xmm5, xmm5, 0x0
		mulps xmm3, xmm5
		addps xmm3, xmm6
		movss dword ptr [eax + 0x24], xmm3
		movhps qword ptr [eax + 0x28], xmm3
	}
#endif

	result->scale = a->scale * b->scale;

	return;
}

real
matrix3x3_determinant(
	real_matrix3x3 const *matrix)
{
	return matrix->up.i * matrix->forward.j * matrix->left.k
		+ matrix->forward.k * matrix->left.i * matrix->up.j
		+ matrix->up.k * matrix->forward.i * matrix->left.j
		- matrix->forward.i * matrix->up.j * matrix->left.k
		- matrix->up.k * matrix->forward.j * matrix->left.i
		- matrix->up.i * matrix->forward.k * matrix->left.j;
}

real_vector3d *matrix3x3_transform_vector(
	real_matrix3x3 const *matrix,
	real_vector3d const *vector,
	real_vector3d *result)
{
	real_vector3d temporary;

	if (vector == result)
	{
		temporary = *vector;
		vector = &temporary;
	}

	result->i = matrix->up.i * vector->k + matrix->left.i * vector->j + matrix->forward.i * vector->i;
	result->j = matrix->up.j * vector->k + matrix->forward.j * vector->i + matrix->left.j * vector->j;
	result->k = matrix->up.k * vector->k + matrix->forward.k * vector->i + matrix->left.k * vector->j;

	return result;
}

void matrix4x3_from_plane(
	real_matrix4x3 *matrix,
	real_plane3d const *plane)
{
	real_vector3d forward;
	real_point3d point;

	match_assert("c:\\halo\\SOURCE\\math\\matrix_math.c", 370, valid_real_plane3d(plane));

	perpendicular3d(&plane->n, &forward);
	normalize3d(&forward);

	point.x = plane->n.i * plane->d;
	point.y = plane->n.j * plane->d;
	point.z = plane->n.k * plane->d;

	matrix4x3_rotation_from_vectors(matrix, &forward, &plane->n);
	matrix->position = point;

	return;
}

/* ---------- private code */

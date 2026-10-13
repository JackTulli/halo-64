/*
RENDER_CAMERAS.C

symbols in this file:
00174FC0 00a0:
	_render_camera_warn_once (0000)
00175060 0020:
	_render_camera_new (0000)
00175080 0120:
	_render_camera_hack_frustum_z (0000)
001751A0 0120:
	_render_camera_build_frustum_bounds (0000)
001752C0 0060:
	_render_frustum_sphere_diameter_in_pixels (0000)
00175320 04b0:
	_render_frustum_cube_view_fraction (0000)
001757D0 00d0:
	_render_frustum_get_projection_bounds (0000)
001758A0 01e0:
	_render_camera_view_to_screen (0000)
00175A80 0100:
	_render_camera_screen_to_view (0000)
00175B80 0130:
	_render_camera_screen_to_world (0000)
00175CB0 0020:
	_render_camera_get_adjusted_field_of_view_tangent (0000)
00175CD0 0160:
	_render_camera_build_clipped_frustum_bounds (0000)
00175E30 00b0:
	_render_camera_triangle_frontfacing (0000)
00175EE0 0100:
	_render_frustum_build_point_flags (0000)
00175FE0 0060:
	_render_frustum_triangle_visible (0000)
00176040 02d0:
	_render_frustum_cube_visible (0000)
00176310 01b0:
	_render_frustum_sphere_visible (0000)
001764C0 00d0:
	_render_camera_world_to_screen (0000)
00176590 01b0:
	_render_camera_debug_frustum (0000)
00176740 0360:
	_render_camera_mirror (0000)
00176AA0 0d30:
	_render_camera_build_frustum (0000)
0029F768 0042:
	??_C@_0EC@HLNCAGFG@?$CD?$CD?$CD?5ERROR?5cameras?3?5frustum?9integ@ (0000)
0029F7AC 0035:
	??_C@_0DF@KOAPDDIF@id?$DO?$DN0?5?$CG?$CG?5id?$DMMAXIMUM_RENDER_CAMER@ (0000)
0029F7E4 0027:
	??_C@_0CH@JIJIIAGP@c?3?2halo?2SOURCE?2render?2render_cam@ (0000)
0029F80C 0025:
	??_C@_0CF@NAAMLNBF@frustum?5?$CG?$CG?5frustum?9?$DOprojection_v@ (0000)
0029F834 000f:
	??_C@_0P@IMNLCIED@frustum_bounds?$AA@ (0000)
0029F844 0027:
	??_C@_0CH@MLEDHCGL@frustum?9?$DOprojection_matrix?$FL3?$FN?$FL3?$FN@ (0000)
0029F86C 0028:
	??_C@_0CI@LBJFINLO@frustum?9?$DOprojection_matrix?$FL2?$FN?$FL3?$FN@ (0000)
0029F894 0027:
	??_C@_0CH@CHHIOMPE@frustum?9?$DOprojection_matrix?$FL1?$FN?$FL3?$FN@ (0000)
0029F8BC 0027:
	??_C@_0CH@LMNNKAJL@frustum?9?$DOprojection_matrix?$FL0?$FN?$FL3?$FN@ (0000)
0029F8E4 0027:
	??_C@_0CH@OFLFFKON@frustum?9?$DOprojection_matrix?$FL3?$FN?$FL1?$FN@ (0000)
0029F90C 0027:
	??_C@_0CH@JCCLIIBN@frustum?9?$DOprojection_matrix?$FL0?$FN?$FL1?$FN@ (0000)
0029F934 0027:
	??_C@_0CH@PCMOEOKO@frustum?9?$DOprojection_matrix?$FL3?$FN?$FL0?$FN@ (0000)
0029F95C 0027:
	??_C@_0CH@BOPFNADB@frustum?9?$DOprojection_matrix?$FL1?$FN?$FL0?$FN@ (0000)
0029F984 001a:
	??_C@_0BK@KOEOFHLL@frustum?9?$DOprojection_valid?$AA@ (0000)
0029F9A0 0017:
	??_C@_0BH@OICCFDNN@bounds?9?$DOz0?$DM?$DNbounds?9?$DOz1?$AA@ (0000)
0029F9B8 0017:
	??_C@_0BH@PLBJIHPN@bounds?9?$DOy0?$DM?$DNbounds?9?$DOy1?$AA@ (0000)
0029F9D0 0017:
	??_C@_0BH@EDCADGCC@bounds?9?$DOx0?$DM?$DNbounds?9?$DOx1?$AA@ (0000)
0029F9E8 0008:
	??_C@_07HEAHMGFP@frustum?$AA@ (0000)
0029F9F0 000d:
	??_C@_0N@GAIFNDPJ@screen_point?$AA@ (0000)
0029FA00 000b:
	??_C@_0L@JFJFDDCJ@view_point?$AA@ (0000)
0029FA0C 000c:
	??_C@_0M@PIACODLM@view_vector?$AA@ (0000)
0029FA18 000d:
	??_C@_0N@DOBKNIBB@world_vector?$AA@ (0000)
0029FA28 000c:
	??_C@_0M@IBHHCMBD@world_point?$AA@ (0000)
0029FA34 0004:
	__real@3f59999a (0000)
0029FA38 0021:
	??_C@_0CB@HKCALOIB@valid_real_vector3d?$CI?$CGresult?9?$DOup?$CJ@ (0000)
0029FA5C 0026:
	??_C@_0CG@PFGJGAON@valid_real_vector3d?$CI?$CGresult?9?$DOfor@ (0000)
0029FA84 0024:
	??_C@_0CE@KJKFBGON@valid_real_plane3d?$CI?$CGadjusted_pla@ (0000)
0029FAA8 0004:
	__real@3bc00000 (0000)
0029FAB0 0008:
	__real@3f899999a0000000 (0000)
0029FAB8 002e:
	??_C@_0CO@BEMDFKEN@valid_real_matrix4x3?$CI?$CGfrustum?9?$DOv@ (0000)
0029FAE8 002e:
	??_C@_0CO@FKHJEIFF@valid_real_matrix4x3?$CI?$CGfrustum?9?$DOw@ (0000)
0029FB18 0036:
	??_C@_0DG@PFOKCMGI@camera?9?$DOviewport_bounds?4y0?$DMcamer@ (0000)
0029FB50 0036:
	??_C@_0DG@MIEIKFFH@camera?9?$DOviewport_bounds?4x0?$DMcamer@ (0000)
0029FB88 001d:
	??_C@_0BN@MDHKMJIG@camera?9?$DOz_far?$DOcamera?9?$DOz_near?$AA@ (0000)
0029FBA8 0015:
	??_C@_0BF@GIODIKMM@camera?9?$DOz_near?$DO?$DN0?40f?$AA@ (0000)
0029FBC0 0030:
	??_C@_0DA@DCMPADCP@?$CD?$CD?$CD?5FATAL?5ERROR?3?5field?5of?5view?5s@ (0000)
0029FBF0 0033:
	??_C@_0DD@EPCFLHOD@camera?9?$DOvertical_field_of_view?$DM_@ (0000)
0029FC24 0004:
	__real@40490e38 (0000)
004B8B28 0113:
	_previous_projection_coefficients (0000)
	_render_camera_warning_values (0010)
	_render_camera_warnings_initialized (0110)
	_debug_no_frustum_clip (0111)
	_render_camera_debug_this_fucking_frustum (0112)
*/

/* ---------- headers */

#include "cseries.h"
#include "cseries/errors.h"
#include "render/render_cameras.h"
#include "render/render_camera_projection.h"
#include "render/render_cameras_internal.h"
#include "render/render_debug.h"
#include "structures/structure_visibility.h"

/* ---------- constants */

enum render_frustum_point_flag_bits
{
	_render_frustum_point_flags_left_bit,
	_render_frustum_point_flags_right_bit,
	_render_frustum_point_flags_top_bit,
	_render_frustum_point_flags_bottom_bit,
	_render_frustum_point_flags_near_bit,
	_render_frustum_point_flags_far_bit,
	NUMBER_OF_RENDER_FRUSTUM_POINT_FLAGS,

	RENDER_FRUSTUM_POINT_FLAGS_PLANE_MASK = FLAG(NUMBER_OF_RENDER_FRUSTUM_POINT_FLAGS) - 1,
};

enum render_frustum_plane
{
	_render_frustum_plane_left,
	_render_frustum_plane_right,
	_render_frustum_plane_bottom,
	_render_frustum_plane_top,
	_render_frustum_plane_near,
	_render_frustum_plane_far,
	NUMBER_OF_RENDER_FRUSTUM_PLANES,
};

enum render_camera_warning_condition
{
	_render_camera_warning_left_bottom_left,
	_render_camera_warning_left_top_left,
	_render_camera_warning_left_apex,
	_render_camera_warning_right_bottom_right,
	_render_camera_warning_right_top_right,
	_render_camera_warning_right_apex,
	_render_camera_warning_bottom_bottom_left,
	_render_camera_warning_bottom_bottom_right,
	_render_camera_warning_bottom_apex,
	_render_camera_warning_top_top_left,
	_render_camera_warning_top_top_right,
	_render_camera_warning_top_apex,
	_render_camera_warning_far_bottom_left,
	_render_camera_warning_far_bottom_right,
	_render_camera_warning_far_top_left,
	_render_camera_warning_far_top_right,
	_render_camera_warning_midpoint_left,
	_render_camera_warning_midpoint_right,
	_render_camera_warning_midpoint_bottom,
	_render_camera_warning_midpoint_top,
	_render_camera_warning_midpoint_near,
	_render_camera_warning_midpoint_far,
};

enum
{
	MAXIMUM_RENDER_CAMERA_WARNING_CONDITIONS = 64,
};

/* ---------- macros */

/* ---------- structures */

/* ---------- globals */

real previous_projection_coefficients[4]= {0};
static real render_camera_warning_values[MAXIMUM_RENDER_CAMERA_WARNING_CONDITIONS]= {0};
static boolean render_camera_warnings_initialized= {0};
boolean debug_no_frustum_clip= {0};
boolean render_camera_debug_this_fucking_frustum= {0};

/* ---------- private prototypes */

static void render_camera_warn_once(
	short id,
	real value);

/* ---------- public code */

void render_camera_new(
	struct render_camera *camera)
{
	csmemset(camera, 0, sizeof(*camera));
	return;
}

void render_camera_hack_frustum_z(
	struct render_frustum *frustum,
	real z_near,
	real z_far)
{
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		271,
		frustum && frustum->projection_valid);

	if (z_near == -1.0f && z_far == -1.0f)
	{
		previous_projection_coefficients[0] = frustum->projection_matrix[0][2];
		previous_projection_coefficients[1] = frustum->projection_matrix[1][2];
		previous_projection_coefficients[2] = frustum->projection_matrix[2][2];
		previous_projection_coefficients[3] = frustum->projection_matrix[3][2];
	}
	else if (z_near == 0.0f && z_far == 0.0f)
	{
		frustum->projection_matrix[0][2] = previous_projection_coefficients[0];
		frustum->projection_matrix[1][2] = previous_projection_coefficients[1];
		frustum->projection_matrix[2][2] = previous_projection_coefficients[2];
		frustum->projection_matrix[3][2] = previous_projection_coefficients[3];
	}
	else
	{
		frustum->projection_matrix[0][2] = 0.0f;
		frustum->projection_matrix[1][2] = 0.0f;
		frustum->projection_matrix[2][2] = -((z_near + z_far) / (z_far - z_near));
		frustum->projection_matrix[3][2] =
			(z_near * z_far * -2.0f) / (z_far - z_near);
	}

	return;
}

void render_camera_screen_to_view(
	const struct render_camera *camera,
	const struct render_frustum *frustum,
	const real_point2d *screen_point,
	real_vector3d *view_vector)
{
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1018,
		camera);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1019,
		frustum);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1020,
		screen_point);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1021,
		view_vector);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1028,
		frustum->projection_valid);

	view_vector->n[0] =
		(screen_point->n[0] - frustum->projection_matrix[2][0]) /
		frustum->projection_matrix[0][0];
	view_vector->n[1] =
		-((frustum->projection_matrix[2][1] + screen_point->n[1]) /
		frustum->projection_matrix[1][1]);
	view_vector->n[2] = -1.0f;
	return;
}

real render_camera_get_adjusted_field_of_view_tangent(
	real field_of_view)
{
	return tangent(field_of_view * 0.5f) * 0.85f;
}

boolean render_camera_world_to_screen(
	const struct render_camera *camera,
	const struct render_frustum *frustum,
	const real_point3d *world_point,
	real_point2d *screen_point)
{
	real_point3d view_point;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		961,
		camera);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		962,
		frustum);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		963,
		world_point);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		964,
		screen_point);

	matrix4x3_transform_point(
		&frustum->world_to_view,
		world_point,
		&view_point);
	return render_camera_view_to_screen(
		camera,
		frustum,
		&view_point,
		screen_point);
}

real render_frustum_sphere_diameter_in_pixels(
	const struct render_frustum *frustum,
	const real_point3d *point,
	real radius)
{
	real depth =
		frustum->world_to_view.up.k * point->z +
		frustum->world_to_view.left.k * point->y +
		frustum->world_to_view.forward.k * point->x +
		frustum->world_to_view.position.z;
	real absolute_depth = ABS(depth);
	real clamped_depth = MAX(absolute_depth, 0.1f);

	return (frustum->projection_world_to_screen.j / clamped_depth) * radius * 2.0f;
}

real render_frustum_cube_view_fraction(
	struct render_frustum const *frustum,
	real_rectangle3d const *bounds)
{
	real fraction;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		876,
		frustum);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		877,
		bounds);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		878,
		bounds->x0<=bounds->x1);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		879,
		bounds->y0<=bounds->y1);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		880,
		bounds->z0<=bounds->z1);

	if (bounds->z0 >= 0.0f)
	{
		fraction = 0.0f;
	}
	else if (bounds->z1 >= 0.0f)
	{
		fraction = 1.0f;
	}
	else
	{
		real inverse_z0 = 1.0f / bounds->z0;
		real projection_x = frustum->projection_matrix[0][0];
		real projection_offset_x = frustum->projection_matrix[2][0];
		real projection_y = frustum->projection_matrix[1][1];
		real projection_offset_y = frustum->projection_matrix[2][1];
		real inverse_z1 = 1.0f / bounds->z1;
		real left;
		real bottom;
		real right;
		real top;

		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			900,
			frustum->projection_valid);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			903,
			frustum->projection_matrix[1][0]==0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			904,
			frustum->projection_matrix[3][0]==0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			905,
			frustum->projection_matrix[0][1]==0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			906,
			frustum->projection_matrix[3][1]==0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			907,
			frustum->projection_matrix[0][3]==0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			908,
			frustum->projection_matrix[1][3]==0.0f);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			909,
			frustum->projection_matrix[2][3]==-1.0f);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			910,
			frustum->projection_matrix[3][3]==0.0f);

		left = MAX(
			MIN(
				(bounds->z0 * projection_offset_x + bounds->x0 * projection_x) *
					(-inverse_z0),
				(bounds->z1 * projection_offset_x + bounds->x0 * projection_x) *
					(-inverse_z1)),
			-1.0f);
		bottom = MAX(
			MIN(
				(bounds->z0 * projection_offset_y + bounds->y0 * projection_y) *
					(-inverse_z0),
				(bounds->z1 * projection_offset_y + bounds->y0 * projection_y) *
					(-inverse_z1)),
			-1.0f);
		right = MIN(
			MAX(
				(bounds->z0 * projection_offset_x + bounds->x1 * projection_x) *
					(-inverse_z0),
				(bounds->z1 * projection_offset_x + bounds->x1 * projection_x) *
					(-inverse_z1)),
			1.0f);
		top = MIN(
			MAX(
				(bounds->z0 * projection_offset_y + bounds->y1 * projection_y) *
					(-inverse_z0),
				(bounds->z1 * projection_offset_y + bounds->y1 * projection_y) *
					(-inverse_z1)),
			1.0f);
		fraction = (right - left) * (top - bottom) * 0.25f;
		if (!(fraction > 0.0f))
		{
			fraction = 0.0f;
		}
	}

	return fraction;
}

word render_frustum_build_point_flags(
	const struct render_frustum *frustum,
	const real_point3d *point)
{
	word flags = plane3d_distance_to_point(&frustum->world_planes[0], point) > 0.0f ?
		FLAG(_render_frustum_point_flags_left_bit) : 0;

	flags |= plane3d_distance_to_point(&frustum->world_planes[1], point) > 0.0f ?
		FLAG(_render_frustum_point_flags_right_bit) : 0;
	flags |= plane3d_distance_to_point(&frustum->world_planes[2], point) > 0.0f ?
		FLAG(_render_frustum_point_flags_bottom_bit) : 0;
	flags |= plane3d_distance_to_point(&frustum->world_planes[3], point) > 0.0f ?
		FLAG(_render_frustum_point_flags_top_bit) : 0;

	return flags;
}

boolean render_frustum_triangle_visible(
	struct render_frustum const *frustum,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2)
{
	word flags = RENDER_FRUSTUM_POINT_FLAGS_PLANE_MASK;
	word point_flags;

	point_flags = render_frustum_build_point_flags(frustum, point0);
	if (!point_flags)
		return TRUE;
	flags &= point_flags;
	point_flags = render_frustum_build_point_flags(frustum, point1);
	if (!point_flags)
		return TRUE;
	flags &= point_flags;
	point_flags = render_frustum_build_point_flags(frustum, point2);
	if (!point_flags)
		return TRUE;
	flags &= point_flags;

	return flags == 0;
}

short render_frustum_cube_visible(
	struct render_frustum const *frustum,
	real_rectangle3d const *bounds,
	boolean test_frustum_against_cube)
{
	if (frustum->world_bounds.x1 < bounds->x0)
	{
		return 0;
	}
	if (frustum->world_bounds.y1 < bounds->y0)
	{
		return 0;
	}
	if (frustum->world_bounds.z1 < bounds->z0)
	{
		return 0;
	}
	if (frustum->world_bounds.x0 > bounds->x1)
	{
		return 0;
	}
	if (frustum->world_bounds.y0 > bounds->y1)
	{
		return 0;
	}
	if (frustum->world_bounds.z0 > bounds->z1)
	{
		return 0;
	}

	{
		real_point3d cube_vertices[8];
		word intersection_flags = RENDER_FRUSTUM_POINT_FLAGS_PLANE_MASK;
		word union_flags = 0;
		short vertex_index;

		cube_vertices[0].x = cube_vertices[2].x = cube_vertices[4].x = cube_vertices[6].x = bounds->x0;
		cube_vertices[1].x = cube_vertices[3].x = cube_vertices[5].x = cube_vertices[7].x = bounds->x1;
		cube_vertices[0].y = cube_vertices[1].y = cube_vertices[4].y = cube_vertices[5].y = bounds->y0;
		cube_vertices[2].y = cube_vertices[3].y = cube_vertices[6].y = cube_vertices[7].y = bounds->y1;
		cube_vertices[0].z = cube_vertices[1].z = cube_vertices[2].z = cube_vertices[3].z = bounds->z0;
		cube_vertices[4].z = cube_vertices[5].z = cube_vertices[6].z = cube_vertices[7].z = bounds->z1;

		for (vertex_index = 0; vertex_index < NUMBEROF(cube_vertices); vertex_index++)
		{
			real_point3d const *vertex = &cube_vertices[vertex_index];
			word flags = dot_product3d((real_vector3d *)vertex, &frustum->world_planes[0].n) -
				frustum->world_planes[0].d > 0.0f ?
				FLAG(_render_frustum_point_flags_left_bit) : 0;

			flags |= dot_product3d((real_vector3d *)vertex, &frustum->world_planes[1].n) -
				frustum->world_planes[1].d > 0.0f ?
				FLAG(_render_frustum_point_flags_right_bit) : 0;
			flags |= dot_product3d((real_vector3d *)vertex, &frustum->world_planes[2].n) -
				frustum->world_planes[2].d > 0.0f ?
				FLAG(_render_frustum_point_flags_bottom_bit) : 0;
			flags |= dot_product3d((real_vector3d *)vertex, &frustum->world_planes[3].n) -
				frustum->world_planes[3].d > 0.0f ?
				FLAG(_render_frustum_point_flags_top_bit) : 0;

			intersection_flags &= flags;
			union_flags |= flags;
		}

		if (!union_flags)
		{
			return 2;
		}

		if (intersection_flags)
		{
			return 0;
		}

		if (test_frustum_against_cube)
		{
			intersection_flags = RENDER_FRUSTUM_POINT_FLAGS_PLANE_MASK;
			for (vertex_index = 0; vertex_index < NUMBEROF(frustum->world_vertices); vertex_index++)
			{
				real_point3d const *vertex = &frustum->world_vertices[vertex_index];
				word right_bit;
				word bottom_bit;
				word top_bit;
				word near_bit;
				word flags = vertex->x <= bounds->x0 ? FLAG(_render_frustum_point_flags_left_bit) : 0;

				right_bit = vertex->x >= bounds->x1 ? FLAG(_render_frustum_point_flags_right_bit) : 0;
				flags |= right_bit;
				bottom_bit = vertex->y <= bounds->y0 ? FLAG(_render_frustum_point_flags_bottom_bit) : 0;
				flags |= bottom_bit;
				top_bit = vertex->y >= bounds->y1 ? FLAG(_render_frustum_point_flags_top_bit) : 0;
				flags |= top_bit;
				near_bit = vertex->z <= bounds->z0 ? FLAG(_render_frustum_point_flags_near_bit) : 0;
				flags |= near_bit;
				flags |= vertex->z >= bounds->z1 ? FLAG(_render_frustum_point_flags_far_bit) : 0;
				intersection_flags &= flags;
			}

			if (intersection_flags)
			{
				return 0;
			}
		}

		return 1;
	}
}

boolean render_camera_triangle_frontfacing(
	struct render_camera const *camera,
	real_point3d const *point0,
	real_point3d const *point1,
	real_point3d const *point2)
{
	real_vector3d camera_to_point;
	real_vector3d edge0;
	real_vector3d edge1;
	real_vector3d normal;

	vector_from_points3d(&camera->position, point0, &camera_to_point);
	vector_from_points3d(point0, point1, &edge0);
	vector_from_points3d(point1, point2, &edge1);
	cross_product3d(&edge0, &edge1, &normal);

	return dot_product3d(&normal, &camera_to_point) > -_real_epsilon;
}

void render_frustum_get_projection_bounds(
	struct render_frustum const *frustum,
	real_rectangle2d *bounds)
{
	real x;
	real y;
	real inverse_x_scale;
	real inverse_y_scale;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		935,
		frustum && frustum->projection_valid);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		936,
		bounds);

	x = -frustum->projection_matrix[2][0];
	y = -frustum->projection_matrix[2][1];
	inverse_x_scale = -1.0f / frustum->projection_matrix[0][0];
	inverse_y_scale = -1.0f / frustum->projection_matrix[1][1];

	bounds->x0 = (x - 1.0f) * inverse_x_scale;
	bounds->x1 = (x + 1.0f) * inverse_x_scale;
	bounds->y0 = (y - 1.0f) * inverse_y_scale;
	bounds->y1 = (y + 1.0f) * inverse_y_scale;
	return;
}

void render_camera_screen_to_world(
	const struct render_camera *camera,
	const struct render_frustum *frustum,
	const real_point2d *screen_point,
	real_point3d *world_point,
	real_vector3d *world_vector)
{
	real_vector3d view_vector;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1052,
		camera);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1053,
		frustum);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1054,
		screen_point);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1055,
		world_point);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1056,
		world_vector);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		1058,
		frustum->projection_valid);

	render_camera_screen_to_view(camera, frustum, screen_point, &view_vector);
	*world_point = camera->position;
	matrix4x3_transform_vector(&frustum->view_to_world, &view_vector, world_vector);

	return;
}

void render_camera_build_frustum_bounds(
	struct render_camera const *camera,
	real_rectangle2d *frustum_bounds)
{
	rectangle2d const *viewport_bounds;
	rectangle2d const *window_bounds;
	real aspect_ratio;
	real inverse_window_height;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		305,
		camera);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		306,
		frustum_bounds);

	viewport_bounds = &camera->viewport_bounds;
	window_bounds = &camera->window_bounds;
	aspect_ratio = (real)(viewport_bounds->y1 - viewport_bounds->y0) /
		(viewport_bounds->x1 - viewport_bounds->x0);
	inverse_window_height = 1.0f /
		(window_bounds->y1 - window_bounds->y0);

	frustum_bounds->x0 =
		(2 * viewport_bounds->x0 -
		window_bounds->x0 - window_bounds->x1) *
		inverse_window_height;
	frustum_bounds->x1 =
		(2 * viewport_bounds->x1 -
		window_bounds->x0 - window_bounds->x1) *
		inverse_window_height;
	frustum_bounds->y0 =
		(2 * viewport_bounds->y0 -
		window_bounds->y0 - window_bounds->y1) *
		inverse_window_height;
	frustum_bounds->y1 =
		(2 * viewport_bounds->y1 -
		window_bounds->y0 - window_bounds->y1) *
		inverse_window_height;

	frustum_bounds->x0 *= aspect_ratio;
	frustum_bounds->x1 *= aspect_ratio;

	{
		real temporary_y0 = frustum_bounds->y0;

		frustum_bounds->y0 = -frustum_bounds->y1;
		frustum_bounds->y1 = -temporary_y0;
	}
	return;
}

boolean render_camera_build_clipped_frustum_bounds(
	struct render_camera const *camera,
	real_rectangle2d const *clip,
	real_rectangle2d *frustum_bounds)
{
	boolean use_full_bounds = TRUE;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		342,
		camera);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		343,
		clip);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		344,
		frustum_bounds);

	if (!debug_no_frustum_clip &&
		clip->x0 < clip->x1 &&
		clip->y0 < clip->y1)
	{
		long viewport_height = camera->viewport_bounds.y1 - camera->viewport_bounds.y0;
		long viewport_width = camera->viewport_bounds.x1 - camera->viewport_bounds.x0;
#ifdef HALO_LINUX
		/* as the projection's horizontal scale (render_camera_build_frustum) */
		real aspect_ratio = (real)viewport_height / ((real)viewport_width * halo_pixel_aspect());
#else
		real aspect_ratio = (real)viewport_height / (real)viewport_width;
#endif
		real inverse_tangent = 1.0f /
			tangent(camera->vertical_field_of_view * 0.5f);
		real horizontal_scale = inverse_tangent * aspect_ratio;

		frustum_bounds->x0 = clip->x0 * horizontal_scale;
		frustum_bounds->x1 = clip->x1 * horizontal_scale;
		frustum_bounds->y0 = inverse_tangent * clip->y0;
		frustum_bounds->y1 = clip->y1 * inverse_tangent;
		use_full_bounds =
			frustum_bounds->x0 >= frustum_bounds->x1 ||
			frustum_bounds->y0 >= frustum_bounds->y1;
	}

	if (use_full_bounds)
	{
		frustum_bounds->y1 = 1.0f;
		frustum_bounds->x1 = 1.0f;
		frustum_bounds->y0 = -1.0f;
		frustum_bounds->x0 = -1.0f;
	}

	return !use_full_bounds;
}

boolean render_camera_view_to_screen(
	struct render_camera const *camera,
	struct render_frustum const *frustum,
	real_point3d const *view_point,
	real_point2d *screen_point)
{
	boolean result = FALSE;

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		978,
		camera);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		979,
		frustum);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		980,
		view_point);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		981,
		screen_point);

	if (view_point->z < 0.0f)
	{
		real inverse_depth = -1.0f / view_point->z;

		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			988,
			frustum->projection_valid);

		screen_point->x =
			(frustum->projection_matrix[0][0] * view_point->x +
			frustum->projection_matrix[2][0] * view_point->z) * inverse_depth;
		screen_point->y =
			-(frustum->projection_matrix[1][1] * view_point->y +
			frustum->projection_matrix[2][1] * view_point->z) * inverse_depth;
		if (screen_point->x >= -1.0f && screen_point->x <= 1.0f &&
			screen_point->y >= -1.0f && screen_point->y <= 1.0f)
		{
			real viewport_width = (real)(
				camera->viewport_bounds.x1 - camera->viewport_bounds.x0);
			real viewport_height = (real)(
				camera->viewport_bounds.y1 - camera->viewport_bounds.y0);

			screen_point->x =
				viewport_width * ((screen_point->x + 1.0f) * 0.5f) +
				camera->viewport_bounds.x0;
			screen_point->y =
				((screen_point->y + 1.0f) * 0.5f) * viewport_height +
				camera->viewport_bounds.y0;
			result = TRUE;
		}
	}

	return result;
}

short render_frustum_sphere_visible(
	struct render_frustum const *frustum,
	real_point3d const *point,
	real radius)
{
	real distance0;
	real distance1;
	real distance2;
	real distance3;
	real distance5;
	real negative_radius;

	if (frustum->world_bounds.x1 < point->x - radius)
		return 0;
	if (frustum->world_bounds.y1 < point->y - radius)
		return 0;
	if (frustum->world_bounds.z1 < point->z - radius)
		return 0;
	if (frustum->world_bounds.x0 > point->x + radius)
		return 0;
	if (frustum->world_bounds.y0 > point->y + radius)
		return 0;
	if (frustum->world_bounds.z0 > point->z + radius)
		return 0;

	distance0 = plane3d_distance_to_point(&frustum->world_planes[0], point);
	if (distance0 > radius)
		return 0;
	distance1 = plane3d_distance_to_point(&frustum->world_planes[1], point);
	if (distance1 > radius)
		return 0;
	distance2 = plane3d_distance_to_point(&frustum->world_planes[2], point);
	if (distance2 > radius)
		return 0;
	distance3 = plane3d_distance_to_point(&frustum->world_planes[3], point);
	if (distance3 > radius)
		return 0;
	if (plane3d_distance_to_point(&frustum->world_planes[4], point) > radius)
		return 0;
	distance5 = plane3d_distance_to_point(&frustum->world_planes[5], point);
	if (distance5 > radius)
		return 0;

	negative_radius = -radius;
	if (distance0 < negative_radius &&
		distance1 < negative_radius &&
		distance2 < negative_radius &&
		distance3 < negative_radius &&
		distance5 < negative_radius)
	{
		return 2;
	}

	return 1;
}

void render_camera_debug_frustum(
	struct render_camera const *camera,
	struct render_frustum const *frustum)
{
	real_point3d points[3][3];
	short i;
	short j;

	if (render_camera_debug_this_fucking_frustum)
	{
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			1083,
			camera);
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			1084,
			frustum);

		for (i = -1; i < 2; i++)
		{
			for (j = -1; j < 2; j++)
			{
				points[j + 1][i + 1].x =
					tangent(camera->vertical_field_of_view * 0.5f) * j *
					(camera->window_bounds.x1 - camera->window_bounds.x0) /
					(camera->window_bounds.y1 - camera->window_bounds.y0);
				points[j + 1][i + 1].y =
					tangent(camera->vertical_field_of_view * 0.5f) * i;
				points[j + 1][i + 1].z = -1.0f;
				matrix4x3_transform_point(
					&frustum->view_to_world,
					&points[j + 1][i + 1],
					&points[j + 1][i + 1]);
			}
		}

		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				render_debug_line(
					TRUE,
					&points[j][0],
					&points[i][2],
					global_real_argb_red);
				render_debug_line(
					TRUE,
					&points[0][j],
					&points[2][i],
					global_real_argb_red);
			}
		}
	}

	return;
}

void render_camera_mirror(
	struct render_camera const *camera,
	struct render_mirror const *mirror,
	struct render_camera *result)
{
	real_plane3d plane = mirror->plane;
	real_plane3d adjusted_plane;
	real_vector3d adjusted_normal;

	*result = *camera;

	if (mirror->index_of_refraction == 0.0f)
	{
		if (fabs(dot_product3d(&plane.n, &camera->forward)) < 0.0125f)
		{
			real_point3d point_on_plane;
			real_vector3d const *normal = &plane.n;
			real_vector3d const *forward = &camera->forward;
			real distance_to_plane =
				-plane3d_distance_to_point(&plane, &camera->position);

			point_on_plane.x = normal->i * distance_to_plane + camera->position.x;
			point_on_plane.y = normal->j * distance_to_plane + camera->position.y;
			point_on_plane.z = normal->k * distance_to_plane + camera->position.z;
			adjusted_normal.i = normal->i + forward->i * 0.005859375f;
			adjusted_normal.j = normal->j + forward->j * 0.005859375f;
			adjusted_normal.k = normal->k + forward->k * 0.005859375f;
			normalize3d(&adjusted_normal);
			plane3d_from_point_and_normal(
				&adjusted_plane,
				&point_on_plane,
				&adjusted_normal);

			match_assert(
				"c:\\halo\\SOURCE\\render\\render_cameras.c",
				207,
				valid_real_plane3d(&adjusted_plane));
		}
		else
		{
			adjusted_plane = plane;
		}

		reflect_vector3d(
			&camera->forward,
			&adjusted_plane.n,
			&result->forward);
		reflect_vector3d(
			&camera->up,
			&adjusted_plane.n,
			&result->up);

		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			217,
			valid_real_vector3d(&result->forward));
		match_assert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			218,
			valid_real_vector3d(&result->up));

		{
			real mirror_scale =
				-2.0f * plane3d_distance_to_point(&adjusted_plane, &camera->position);

			result->position.x = adjusted_plane.n.i * mirror_scale + camera->position.x;
			result->position.y = adjusted_plane.n.j * mirror_scale + camera->position.y;
			result->position.z = adjusted_plane.n.k * mirror_scale + camera->position.z;
		}
		result->mirrored = !camera->mirrored;
		negate_vector3d(&result->up, &result->up);
	}
	else
	{
		real_vector3d cross_product;
		real inverse_forward_magnitude = 1.0f / magnitude3d(&camera->forward);
		real sine_of_incidence = magnitude3d(cross_product3d(
			&camera->forward,
			&plane.n,
			&cross_product)) * inverse_forward_magnitude;
		real refracted_sine = mirror->index_of_refraction * sine_of_incidence;
		real refraction_offset;

		if (sine_of_incidence != 0.0f)
		{
			real cosine_of_incidence =
				dot_product3d(&plane.n, &camera->forward) * inverse_forward_magnitude;
			real refraction_numerator = cosine_of_incidence * refracted_sine;

			refraction_offset =
				-(refraction_numerator * mirror->depth /
				(square_root(1.0f - refracted_sine * refracted_sine) * sine_of_incidence));
		}
		else
		{
			refraction_offset = 0.0f;
		}

		result->position.x = plane.n.i * refraction_offset + camera->position.x;
		result->position.y = plane.n.j * refraction_offset + camera->position.y;
		result->position.z = plane.n.k * refraction_offset + camera->position.z;
	}

	result->z_near = 0.0f;
	result->mirror_plane = plane;
	return;
}

void render_camera_build_frustum(
	const struct render_camera *camera,
	const real_rectangle2d *frustum_bounds,
	struct render_frustum *frustum,
	boolean build_projection)
{
	long viewport_width_integer =
		camera->viewport_bounds.x1 - camera->viewport_bounds.x0;
	long viewport_height_integer =
		camera->viewport_bounds.y1 - camera->viewport_bounds.y0;
	real viewport_width = (real)viewport_width_integer;
	real viewport_height = (real)viewport_height_integer;
	real half_bounds_width;
	real half_bounds_height;
	real bounds_center_x;
	real bounds_center_y;
	real field_of_view_tangent;
	real projection_x_scale;
	real projection_y_scale;
	real_vector3d view_left;
	real_vector3d view_up;
	real_vector3d view_backward;
	real_vector3d plane_normal;
	real_plane3d view_plane;
	real left_plane_z;
	real bottom_plane_z;
	real inverse_projection_x_scale;
	real inverse_projection_y_scale;
	real half_z;
	real far_left;
	real far_right;
	real far_bottom;
	real far_top;
	real_point3d view_point;
	long vertex_index;

	if (frustum_bounds)
	{
		frustum->frustum_bounds = *frustum_bounds;
	}
	else
	{
		frustum->frustum_bounds.y0 = -1.0f;
		frustum->frustum_bounds.x0 = -1.0f;
		frustum->frustum_bounds.y1 = 1.0f;
		frustum->frustum_bounds.x1 = 1.0f;
	}

	half_bounds_width =
		(frustum->frustum_bounds.x1 - frustum->frustum_bounds.x0) * 0.5f;
	half_bounds_height =
		(frustum->frustum_bounds.y1 - frustum->frustum_bounds.y0) * 0.5f;
	bounds_center_x =
		(frustum->frustum_bounds.x0 + frustum->frustum_bounds.x1) /
		half_bounds_width * -0.5f;
	bounds_center_y =
		(frustum->frustum_bounds.y0 + frustum->frustum_bounds.y1) /
		half_bounds_height * -0.5f;
	field_of_view_tangent = tangent(camera->vertical_field_of_view * 0.5f);
#ifdef HALO_LINUX
	/* a display that shows each pixel wider than it is (the console's
	anamorphic widescreen mode) shows that much more of the world: the
	viewport is that much wider to the projection */
	projection_x_scale = 1.0f /
		(half_bounds_width / viewport_height * viewport_width * halo_pixel_aspect() *
		field_of_view_tangent);
#else
	projection_x_scale = 1.0f /
		(half_bounds_width / viewport_height * viewport_width *
		field_of_view_tangent);
#endif
	projection_y_scale = 1.0f /
		(field_of_view_tangent * half_bounds_height);

	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		430,
		camera->vertical_field_of_view<_pi - _real_epsilon);
	if (camera->vertical_field_of_view <= _real_epsilon)
	{
		union
		{
			real value;
			unsigned long bits;
		} field_of_view;

		field_of_view.value = camera->vertical_field_of_view;
		match_vassert(
			"c:\\halo\\SOURCE\\render\\render_cameras.c",
			432,
			FALSE,
			csprintf(
				temporary,
				"### FATAL ERROR: field of view set to %f (0x%x)",
				(double)field_of_view.value,
				field_of_view.bits));
	}
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		433,
		camera->z_near>=0.0f);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		434,
		camera->z_far>camera->z_near);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		435,
		camera->viewport_bounds.x0<camera->viewport_bounds.x1);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		436,
		camera->viewport_bounds.y0<camera->viewport_bounds.y1);

	cross_product3d(&camera->forward, &camera->up, &view_left);
	cross_product3d(&view_left, &camera->forward, &view_up);
	negate_vector3d(&camera->forward, &view_backward);
	normalize3d(&view_left);
	normalize3d(&view_up);
	normalize3d(&view_backward);
	frustum->view_to_world.forward = view_left;
	frustum->view_to_world.left = view_up;
	frustum->view_to_world.up = view_backward;
	frustum->view_to_world.position = camera->position;
	frustum->view_to_world.scale = 1.0f;
	matrix4x3_inverse(&frustum->view_to_world, &frustum->world_to_view);
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		458,
		valid_real_matrix4x3(&frustum->world_to_view));
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		459,
		valid_real_matrix4x3(&frustum->view_to_world));

	plane_normal.i = -projection_x_scale;
	plane_normal.j = 0.0f;
	left_plane_z = bounds_center_x + 1.0f;
	plane_normal.k = left_plane_z;
	normalize3d(&plane_normal);
	plane3d_from_point_and_normal(&view_plane, global_origin3d, &plane_normal);
	matrix4x3_transform_plane(
		&frustum->view_to_world,
		&view_plane,
		&frustum->world_planes[_render_frustum_plane_left]);

	plane_normal.i = projection_x_scale;
	plane_normal.j = 0.0f;
	plane_normal.k = 1.0f - bounds_center_x;
	normalize3d(&plane_normal);
	plane3d_from_point_and_normal(&view_plane, global_origin3d, &plane_normal);
	matrix4x3_transform_plane(
		&frustum->view_to_world,
		&view_plane,
		&frustum->world_planes[_render_frustum_plane_right]);

	plane_normal.i = 0.0f;
	plane_normal.j = -projection_y_scale;
	bottom_plane_z = bounds_center_y + 1.0f;
	plane_normal.k = bottom_plane_z;
	normalize3d(&plane_normal);
	plane3d_from_point_and_normal(&view_plane, global_origin3d, &plane_normal);
	matrix4x3_transform_plane(
		&frustum->view_to_world,
		&view_plane,
		&frustum->world_planes[_render_frustum_plane_bottom]);

	plane_normal.i = 0.0f;
	plane_normal.j = projection_y_scale;
	plane_normal.k = 1.0f - bounds_center_y;
	normalize3d(&plane_normal);
	plane3d_from_point_and_normal(&view_plane, global_origin3d, &plane_normal);
	matrix4x3_transform_plane(
		&frustum->view_to_world,
		&view_plane,
		&frustum->world_planes[_render_frustum_plane_top]);

	view_plane.n.i = 0.0f;
	view_plane.n.j = 0.0f;
	view_plane.n.k = 1.0f;
	view_plane.d = -camera->z_near;
	matrix4x3_transform_plane(
		&frustum->view_to_world,
		&view_plane,
		&frustum->world_planes[_render_frustum_plane_near]);

	view_plane.n.i = 0.0f;
	view_plane.n.j = 0.0f;
	view_plane.n.k = -1.0f;
	view_plane.d = camera->z_far;
	matrix4x3_transform_plane(
		&frustum->view_to_world,
		&view_plane,
		&frustum->world_planes[_render_frustum_plane_far]);

	frustum->z_near = camera->z_near;
	frustum->z_far = camera->z_far;
	inverse_projection_x_scale = 1.0f / projection_x_scale;
	inverse_projection_y_scale = 1.0f / projection_y_scale;
	half_z = (camera->z_far + camera->z_near) * 0.5f;
	far_left = left_plane_z * -(inverse_projection_x_scale * camera->z_far);
	far_right =
		(bounds_center_x - 1.0f) * -(inverse_projection_x_scale * camera->z_far);
	far_bottom =
		bottom_plane_z * -(inverse_projection_y_scale * camera->z_far);
	far_top =
		(bounds_center_y - 1.0f) * -(inverse_projection_y_scale * camera->z_far);

	view_point.x = far_left;
	view_point.y = far_bottom;
	view_point.z = -camera->z_far;
	matrix4x3_transform_point(
		&frustum->view_to_world,
		&view_point,
		&frustum->world_vertices[0]);
	view_point.x = far_right;
	view_point.y = far_bottom;
	view_point.z = -camera->z_far;
	matrix4x3_transform_point(
		&frustum->view_to_world,
		&view_point,
		&frustum->world_vertices[1]);
	view_point.x = far_left;
	view_point.y = far_top;
	view_point.z = -camera->z_far;
	matrix4x3_transform_point(
		&frustum->view_to_world,
		&view_point,
		&frustum->world_vertices[2]);
	view_point.x = far_right;
	view_point.y = far_top;
	view_point.z = -camera->z_far;
	matrix4x3_transform_point(
		&frustum->view_to_world,
		&view_point,
		&frustum->world_vertices[3]);

	frustum->world_vertices[4] = camera->position;
	view_point.x = -(inverse_projection_x_scale * half_z * bounds_center_x);
	view_point.y = -(inverse_projection_y_scale * half_z * bounds_center_y);
	view_point.z = -half_z;
	matrix4x3_transform_point(
		&frustum->view_to_world,
		&view_point,
		&frustum->world_midpoint);

	frustum->world_bounds.x1 = frustum->world_vertices[0].x;
	frustum->world_bounds.x0 = frustum->world_vertices[0].x;
	frustum->world_bounds.y1 = frustum->world_vertices[0].y;
	frustum->world_bounds.y0 = frustum->world_vertices[0].y;
	frustum->world_bounds.z1 = frustum->world_vertices[0].z;
	frustum->world_bounds.z0 = frustum->world_vertices[0].z;
	for (vertex_index = 1;
		vertex_index < NUMBEROF(frustum->world_vertices);
		vertex_index++)
	{
		const real_point3d *vertex = &frustum->world_vertices[vertex_index];

		frustum->world_bounds.x0 = MIN(frustum->world_bounds.x0, vertex->x);
		frustum->world_bounds.y0 = MIN(frustum->world_bounds.y0, vertex->y);
		frustum->world_bounds.z0 = MIN(frustum->world_bounds.z0, vertex->z);
		frustum->world_bounds.x1 = MAX(frustum->world_bounds.x1, vertex->x);
		frustum->world_bounds.y1 = MAX(frustum->world_bounds.y1, vertex->y);
		frustum->world_bounds.z1 = MAX(frustum->world_bounds.z1, vertex->z);
	}

	if (build_projection)
	{
		real inverse_plane_z;
		real clip_offset;
		real projection_scale;

		if (camera->z_near == 0.0f)
		{
			matrix4x3_transform_plane(
				&frustum->world_to_view,
				&camera->mirror_plane,
				&view_plane);
		}
		else
		{
			view_plane.n.i = 0.0f;
			view_plane.n.j = 0.0f;
			view_plane.n.k = 1.0f;
			view_plane.d = -camera->z_near;
		}

		inverse_plane_z = 1.0f / view_plane.n.k;
		clip_offset = -(view_plane.d * inverse_plane_z);
		projection_scale = (real)(camera->z_far /
			((camera->z_far - clip_offset) *
			(fabs(inverse_plane_z * view_plane.n.i) +
			fabs(inverse_plane_z * view_plane.n.j) + 1.0)));
		view_plane.n.i *= inverse_plane_z * projection_scale;
		view_plane.n.j *= inverse_plane_z * projection_scale;
		view_plane.n.k = projection_scale;
		view_plane.d = -(projection_scale * clip_offset);
		if (view_plane.d > 0.0f && camera->z_near == 0.0f)
		{
			plane3d_negate(&view_plane, &view_plane);
		}

		csmemset(
			frustum->projection_matrix,
			0,
			sizeof(frustum->projection_matrix));
		frustum->projection_matrix[0][0] = projection_x_scale;
		frustum->projection_matrix[0][2] = -view_plane.n.i;
		frustum->projection_matrix[1][1] = projection_y_scale;
		frustum->projection_matrix[1][2] = -view_plane.n.j;
		frustum->projection_matrix[2][0] = -bounds_center_x;
		frustum->projection_matrix[2][1] = -bounds_center_y;
		frustum->projection_matrix[2][2] = -view_plane.n.k;
		frustum->projection_matrix[2][3] = -1.0f;
		frustum->projection_matrix[3][2] = view_plane.d;
		frustum->projection_valid = TRUE;
		frustum->projection_world_to_screen.i =
			projection_x_scale * viewport_width * 0.5f;
		frustum->projection_world_to_screen.j =
			projection_y_scale * viewport_height * 0.5f;
	}
	else
	{
		csmemset(
			frustum->projection_matrix,
			0,
			sizeof(frustum->projection_matrix));
		csmemset(
			&frustum->projection_world_to_screen,
			0,
			sizeof(frustum->projection_world_to_screen));
		frustum->projection_valid = FALSE;
	}

	render_camera_warn_once(
		_render_camera_warning_left_bottom_left,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_left],
			&frustum->world_vertices[0])));
	render_camera_warn_once(
		_render_camera_warning_left_top_left,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_left],
			&frustum->world_vertices[2])));
	render_camera_warn_once(
		_render_camera_warning_left_apex,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_left],
			&frustum->world_vertices[4])));
	render_camera_warn_once(
		_render_camera_warning_right_bottom_right,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_right],
			&frustum->world_vertices[1])));
	render_camera_warn_once(
		_render_camera_warning_right_top_right,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_right],
			&frustum->world_vertices[3])));
	render_camera_warn_once(
		_render_camera_warning_right_apex,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_right],
			&frustum->world_vertices[4])));
	render_camera_warn_once(
		_render_camera_warning_bottom_bottom_left,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_bottom],
			&frustum->world_vertices[0])));
	render_camera_warn_once(
		_render_camera_warning_bottom_bottom_right,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_bottom],
			&frustum->world_vertices[1])));
	render_camera_warn_once(
		_render_camera_warning_bottom_apex,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_bottom],
			&frustum->world_vertices[4])));
	render_camera_warn_once(
		_render_camera_warning_top_top_left,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_top],
			&frustum->world_vertices[2])));
	render_camera_warn_once(
		_render_camera_warning_top_top_right,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_top],
			&frustum->world_vertices[3])));
	render_camera_warn_once(
		_render_camera_warning_top_apex,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_top],
			&frustum->world_vertices[4])));
	render_camera_warn_once(
		_render_camera_warning_far_bottom_left,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_far],
			&frustum->world_vertices[0])));
	render_camera_warn_once(
		_render_camera_warning_far_bottom_right,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_far],
			&frustum->world_vertices[1])));
	render_camera_warn_once(
		_render_camera_warning_far_top_left,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_far],
			&frustum->world_vertices[2])));
	render_camera_warn_once(
		_render_camera_warning_far_top_right,
		(real)fabs(plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_far],
			&frustum->world_vertices[3])));
	render_camera_warn_once(
		_render_camera_warning_midpoint_left,
		plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_left],
			&frustum->world_midpoint));
	render_camera_warn_once(
		_render_camera_warning_midpoint_right,
		plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_right],
			&frustum->world_midpoint));
	render_camera_warn_once(
		_render_camera_warning_midpoint_bottom,
		plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_bottom],
			&frustum->world_midpoint));
	render_camera_warn_once(
		_render_camera_warning_midpoint_top,
		plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_top],
			&frustum->world_midpoint));
	render_camera_warn_once(
		_render_camera_warning_midpoint_near,
		plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_near],
			&frustum->world_midpoint));
	render_camera_warn_once(
		_render_camera_warning_midpoint_far,
		plane3d_distance_to_point(
			&frustum->world_planes[_render_frustum_plane_far],
			&frustum->world_midpoint));

	return;
}

/* ---------- private code */

static void render_camera_warn_once(
	short id,
	real value)
{
	match_assert(
		"c:\\halo\\SOURCE\\render\\render_cameras.c",
		135,
		id>=0 && id<MAXIMUM_RENDER_CAMERA_WARNING_CONDITIONS);

	if (!render_camera_warnings_initialized)
	{
		csmemset(
			render_camera_warning_values,
			0,
			sizeof(render_camera_warning_values));
		render_camera_warnings_initialized = TRUE;
	}

	if (value >= 0.05f && value > render_camera_warning_values[id])
	{
		error(
			_error_silent,
			"### ERROR cameras: frustum-integrity condition #%d violated by %f",
			(long)id,
			(double)value);
		render_camera_warning_values[id] = value;
	}

	return;
}

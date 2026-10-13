/*
CONTRAILS.H

header included in hcex build.
*/

#ifndef __CONTRAILS_H
#define __CONTRAILS_H
#pragma once

/* ---------- headers */

#include "cseries/cseries.h"
#include "math/real_math.h"
#include "memory/data.h"
#include "objects/objects.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct contrail_datum
{
	short identifier;
	word flags;
	long definition_index;
	long object_index;
	short attachment_index;
	short density_function_index;
	real density;
	short sequence_index;
	short frame_index;
	real texture_offset_u;
	real texture_offset_v;
	real time_until_point;
	real frame_time;
	real expired_dt;
	short contrail_point_counts[4];
	long first_contrail_point_indices[4];
};

struct contrail_point_datum
{
	short identifier;
	byte flags;
	char state_index;
	real time;
	real delta;
	real density;
	real width;
	struct location location;
	real_point3d position;
	real_vector3d velocity;
	long next_contrail_point_index;
};

typedef char verify_contrail_datum_size[
	sizeof(struct contrail_datum) == 0x44 ? 1 : -1];
typedef char verify_contrail_point_datum_size[
	sizeof(struct contrail_point_datum) == 0x38 ? 1 : -1];

/* ---------- prototypes/CONTRAILS.C */

real contrail_scale_value(
	real value,
	real scale,
	unsigned long flags,
	short flag_bit);
real contrail_scale_random_value(
	real value,
	real lower_bound,
	real upper_bound,
	unsigned long flags,
	short flag_bit);
void contrails_initialize(
	void);
void contrails_initialize_for_new_map(
	void);
void contrails_dispose_from_old_map(
	void);
void contrails_dispose(
	void);
void contrails_disconnect_from_structure_bsp(
	void);
void contrails_reconnect_to_structure_bsp(
	void);
void contrail_delete(
	long contrail_index);
long contrail_new(
	long definition_index,
	long object_index,
	short attachment_index);
void contrail_owner_collision(
	long contrail_index,
	unsigned char object_dying,
	real dt);
void contrails_update(
	real dt);

/* ---------- globals */

extern struct data_array *contrail_data;
extern struct data_array *contrail_point_data;

/* ---------- public code */

#endif // __CONTRAILS_H

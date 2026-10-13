/*
PROJECTILES_CALLBACKS.H

Projectile object-type callback declarations.
*/

#ifndef __PROJECTILES_CALLBACKS_H
#define __PROJECTILES_CALLBACKS_H
#pragma once

#include "cseries.h"

/* ---------- prototypes/PROJECTILES.C */

boolean projectile_new(
	long projectile_index);
boolean projectile_update(
	long projectile_index);
void projectile_export_function_values(
	long projectile_index);
void projectile_handle_deleted_object(
	long projectile_index,
	long deleted_object_index);
boolean projectile_handle_parent_destroyed(
	long projectile_index);

#endif // __PROJECTILES_CALLBACKS_H

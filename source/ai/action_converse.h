/*
ACTION_CONVERSE.H

Public interface owned by ACTION_CONVERSE.C.
*/

#ifndef __ACTION_CONVERSE_H
#define __ACTION_CONVERSE_H
#pragma once

/* ---------- headers */

#include "cseries.h"

/* ---------- structures */

struct converse_state_data;

/* ---------- prototypes/ACTION_CONVERSE.C */

boolean action_converse_setup(
	long actor_index,
	long conversation_index,
	struct converse_state_data *state_data);

#endif /* __ACTION_CONVERSE_H */

#ifndef __ACTOR_LOOKING_H
#define __ACTOR_LOOKING_H

union real_vector2d;

void actor_look_affect_movement(
	long actor_index);
void actor_look_update(
	long actor_index);
boolean valid_real_normal2d(
	union real_vector2d const *normal);
void actor_look_secondary_stop(
	long actor_index);

#endif // __ACTOR_LOOKING_H

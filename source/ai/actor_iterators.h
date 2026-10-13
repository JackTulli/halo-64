#ifndef __ACTOR_ITERATORS_H
#define __ACTOR_ITERATORS_H

struct actor_datum;
struct actor_iterator;

void actor_iterator_new(
	struct actor_iterator *iterator,
	boolean active_only);
struct actor_datum *actor_iterator_next(
	struct actor_iterator *iterator);

#endif // __ACTOR_ITERATORS_H

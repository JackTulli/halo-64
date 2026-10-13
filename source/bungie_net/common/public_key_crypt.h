/*
PUBLIC_KEY_CRYPT.H

header included in hcex build.
*/

#ifndef __PUBLIC_KEY_CRYPT_H
#define __PUBLIC_KEY_CRYPT_H
#pragma once

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct public_key
{
	unsigned long dwords[2];
};

/* ---------- prototypes/EXAMPLE.C */

/* ---------- globals */

/* ---------- public code */

void generate_key_parameters(
	struct public_key *p,
	struct public_key *x,
	struct public_key *g);
void generate_public_key(
	struct public_key const *p,
	struct public_key const *x,
	struct public_key const *g,
	struct public_key *public_key);
void generate_private_key(
	struct public_key const *public_key,
	struct public_key const *p,
	struct public_key const *x,
	struct public_key *private_key);

#endif // __PUBLIC_KEY_CRYPT_H

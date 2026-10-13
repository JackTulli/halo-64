/*
PUBLIC_KEY_CRYPT.C

symbols in this file:
0006F630 00d0:
	_code_0006f630 (0000)
0006F700 0080:
	_code_0006f700 (0000)
0006F780 0060:
	_code_0006f780 (0000)
0006F7E0 00e0:
	_generate_key_parameters (0000)
0006F8C0 00b0:
	_generate_public_key (0000)
0006F970 0110:
	_generate_private_key (0000)
00255938 0016:
	??_C@_0BG@IOLIBAAP@s?4qword?5?$DM?$DN?50xFFFFFFFF?$AA@ (0000)
00255950 0034:
	??_C@_0DE@OPHDMGPK@c?3?2halo?2SOURCE?2bungie_net?2common@ (0000)
00255984 0004:
	??_C@_03HKAEIKFL@g?$DMp?$AA@ (0000)
00255988 0008:
	??_C@_07KLDBGFKO@x?$DM?$CIp?91?$CJ?$AA@ (0000)
00255990 0004:
	??_C@_03HGABBMJE@p?$DO2?$AA@ (0000)
00255994 0022:
	??_C@_0CC@GBEDMBBG@g?9?$DOdwords?$FLi?$FN?5?$DM?5?$CIp?9?$DOdwords?$FLi?$FN?5?9?51@ (0000)
002559B8 0022:
	??_C@_0CC@EGFPFOJP@x?9?$DOdwords?$FLi?$FN?5?$DM?5?$CIp?9?$DOdwords?$FLi?$FN?5?9?52@ (0000)
002559DC 003b:
	??_C@_0DL@LPJFFING@p?$DN?5?$CF8lX?$CF8lX?6x?$DN?5?$CF8lX?$CF8lX?6g?$DN?5?$CF8lX?$CF@ (0000)
00255A18 0045:
	??_C@_0EF@COOLEFMM@public_key?$DN?5?$CF8lX?$CF8lX?6p?$DN?5?$CF8lX?$CF8lX@ (0000)
*/

/* ---------- headers */

#include "cseries.h"
#include "bungie_net/common/64bit_math.h"
#include "bungie_net/common/public_key_crypt.h"
#include "bungie_net/common/random_numbers.h"
#include "memory/byte_swapping.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

/* ---------- prototypes */

unsigned long randomprime(
	unsigned long maximum);

static unsigned long x_exp_y_mod_n(
	unsigned long base,
	unsigned long exponent,
	unsigned long modulus);
static unsigned long generate_diffie_hellman_public_key(
	unsigned long p,
	unsigned long x,
	unsigned long g);
static unsigned long generate_diffie_hellman_private_key(
	unsigned long public_key,
	unsigned long p,
	unsigned long x);

/* ---------- globals */

/* ---------- public code */

void generate_key_parameters(
	struct public_key *p,
	struct public_key *x,
	struct public_key *g)
{
	long i = 0;
	long count = 2;

	while (count)
	{
		unsigned long prime0;
		unsigned long prime1;

		do
		{
			prime0 = randomprime(0xFFFF);
			prime1 = randomprime(0xFFFF);
			p->dwords[i] = prime0 * prime1 + 2;
		}
		while (p->dwords[i] < 0xFFFFFF);

		x->dwords[i] = randomrange(0xFF, p->dwords[i] - 2);
		g->dwords[i] = randomrange(0xFF, p->dwords[i] - 1);
		match_assert(
			"c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c",
			162,
			x->dwords[i] < (p->dwords[i] - 2));
		match_assert(
			"c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c",
			163,
			g->dwords[i] < (p->dwords[i] - 1));

		i++;
		count--;
	}

	return;
}

void generate_public_key(
	struct public_key const *p,
	struct public_key const *x,
	struct public_key const *g,
	struct public_key *public_key)
{
	long i;

	for (i = 0; i < 2; i++)
		public_key->dwords[i] = generate_diffie_hellman_public_key(p->dwords[i], x->dwords[i], g->dwords[i]);

	error(
		2,
		"p= %8lX%8lX\nx= %8lX%8lX\ng= %8lX%8lX\npublic key= %8lX%8lX\n\n",
		p->dwords[0], p->dwords[1],
		x->dwords[0], x->dwords[1],
		g->dwords[0], g->dwords[1],
		public_key->dwords[0], public_key->dwords[1]);

	return;
}

void generate_private_key(
	struct public_key const *public_key,
	struct public_key const *p,
	struct public_key const *x,
	struct public_key *private_key)
{
	long i;

	for (i = 0; i < 2; i++)
	{
		private_key->dwords[i] = generate_diffie_hellman_private_key(
			public_key->dwords[i],
			p->dwords[i],
			x->dwords[i]);

		private_key->dwords[i] = SWAP4(private_key->dwords[i]);
	}

	error(
		2,
		"public_key= %8lX%8lX\np= %8lX%8lX\nx= %8lX%8lX\nprivate key= %8lX%8lX\n\n",
		public_key->dwords[0], public_key->dwords[1],
		p->dwords[0], p->dwords[1],
		x->dwords[0], x->dwords[1],
		private_key->dwords[0], private_key->dwords[1]);

	return;
}

/* ---------- private code */

static unsigned long x_exp_y_mod_n(
	unsigned long base,
	unsigned long exponent,
	unsigned long modulus)
{
	struct qword_value s;
	struct qword_value base_value;
	struct qword_value modulus_value;
	struct qword_value product;

	s.qword = 1;
	base_value.qword = base;
	modulus_value.qword = modulus;

	while (exponent)
	{
		if (exponent & 1)
		{
			multiply64(&s, &base_value, &product);
			divide64(&product, &modulus_value, NULL, &s);
		}

		exponent >>= 1;
		multiply64(&base_value, &base_value, &product);
		divide64(&product, &modulus_value, NULL, &base_value);
	}

	match_assert(
		"c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c",
		95,
		s.qword <= 0xFFFFFFFF);

	return (unsigned long)s.qword;
}

static unsigned long generate_diffie_hellman_public_key(
	unsigned long p,
	unsigned long x,
	unsigned long g)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c", 112, p>2);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c", 113, x<(p-1));
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c", 114, g<p);

	return x_exp_y_mod_n(g, x, p);
}

static unsigned long generate_diffie_hellman_private_key(
	unsigned long public_key,
	unsigned long p,
	unsigned long x)
{
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c", 133, p>2);
	match_assert("c:\\halo\\SOURCE\\bungie_net\\common\\public_key_crypt.c", 134, x<(p-1));

	return x_exp_y_mod_n(public_key, x, p);
}

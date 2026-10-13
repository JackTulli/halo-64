/*
GAME_ENGINE_STUB.C

symbols in this file:
000A4710 0010:
	_code_000a4710 (0000)
000A4720 0010:
	_code_000a4720 (0000)
000A4730 0010:
	_code_000a4730 (0000)
000A4740 0010:
	_code_000a4740 (0000)
000A4750 0010:
	_code_000a4750 (0000)
000A4760 0010:
	_code_000a4760 (0000)
000A4770 0010:
	_code_000a4770 (0000)
000A4780 0010:
	_code_000a4780 (0000)
000A4790 0010:
	_code_000a4790 (0000)
000A47A0 0010:
	_code_000a47a0 (0000)
000A47B0 0010:
	_code_000a47b0 (0000)
000A47C0 0010:
	_code_000a47c0 (0000)
000A47D0 0010:
	_code_000a47d0 (0000)
000A47E0 0010:
	_code_000a47e0 (0000)
000A47F0 0010:
	_code_000a47f0 (0000)
0025C210 0005:
	??_C@_04GGADAGKI@stub?$AA@ (0000)
002DE6F8 0088:
	_stub_engine (0000)
*/

/* ---------- headers */

#include "cseries/cseries.h"

/* ---------- constants */

enum
{
	_game_engine_type_stub = 7,
	NUMBER_OF_STUB_GAME_ENGINE_CALLBACKS = 32,
};

/* ---------- macros */

/* ---------- structures */

typedef void (*stub_game_engine_callback)(void);

struct stub_game_engine
{
	char const *name;
	long type;
	stub_game_engine_callback callbacks[NUMBER_OF_STUB_GAME_ENGINE_CALLBACKS];
};

typedef char verify_stub_game_engine_size[sizeof(struct stub_game_engine) == 0x88 ? 1 : -1];

/* ---------- prototypes */

void code_000a4710(void);
boolean code_000a4720(void);
void code_000a4730(void);
void code_000a4740(void);
void code_000a4750(void);
void code_000a4760(void);
void code_000a4770(void);
void code_000a4780(void);
void code_000a4790(void);
void code_000a47a0(void);
void code_000a47b0(void);
void code_000a47c0(void);
boolean code_000a47d0(void);
void code_000a47e0(void);
void code_000a47f0(void);

/* ---------- globals */

struct stub_game_engine stub_engine =
{
	"stub",
	_game_engine_type_stub,
	{
		code_000a4710,
		(stub_game_engine_callback) code_000a4720,
		code_000a4730,
		code_000a4740,
		code_000a4750,
		code_000a4760,
		code_000a4770,
		code_000a4780,
		code_000a4790,
		code_000a47a0,
		code_000a47b0,
		NULL,
		NULL,
		NULL,
		NULL,
		code_000a47c0,
		NULL,
		NULL,
		NULL,
		NULL,
		(stub_game_engine_callback) code_000a47d0,
		code_000a47e0,
		code_000a47f0,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	},
};

/* ---------- public code */

void code_000a4710(void)
{
}

boolean code_000a4720(void)
{
	return TRUE;
}

void code_000a4730(void)
{
}

void code_000a4740(void)
{
}

void code_000a4750(void)
{
}

void code_000a4760(void)
{
}

void code_000a4770(void)
{
}

void code_000a4780(void)
{
}

void code_000a4790(void)
{
}

void code_000a47a0(void)
{
}

void code_000a47b0(void)
{
}

void code_000a47c0(void)
{
}

boolean code_000a47d0(void)
{
	return TRUE;
}

void code_000a47e0(void)
{
}

void code_000a47f0(void)
{
}

/* ---------- private code */

/* N64_LINK.H

Halo 64 Online's cable to Halo 64 Link, the program on the computer at the
other end of the flashcart's USB port (port/n64/docs/online/README.md):
libdragon's usb.c (UNFLoader's USB library) carries the link's packets, one
USB packet of datatype N64_LINK_DATATYPE each. In an emulator the cart's
USB port is an emulated one, bridged to a socket on the computer.

A link packet: its header (N64_LINK_HEADER_SIZE bytes, big-endian),

	0  2  'H' '6'
	2  1  the protocol's version (N64_LINK_VERSION)
	3  1  its type (enum n64_link_type)
	4  2  its payload's length
	6  2  its sequence number (the sender's, one more each packet)

then its payload. */

#ifndef __N64_LINK_H
#define __N64_LINK_H

#include <stdint.h>

#define N64_LINK_DATATYPE 0x48
/* 2: CONTROL, the player's own state in FRAME, LOAD's teams */
#define N64_LINK_VERSION 2
#define N64_LINK_HEADER_SIZE 8
/* the largest payload either side sends */
#define N64_LINK_PAYLOAD_MAXIMUM 4096

enum n64_link_type
{
	/* the N64's */
	N64_LINK_HELLO = 1,   /* version (1), build (string): until WELCOME */
	N64_LINK_INPUT = 2,   /* the frame (4), controllers (1), each one's state */
	N64_LINK_LIST = 3,    /* the games, please: GAMES */
	N64_LINK_JOIN = 4,    /* a listed game's id (4) */
	N64_LINK_HOST = 5,    /* map (string), game type (string) */
	N64_LINK_LEAVE = 6,   /* back to the menu */
	/* a tick of the N64's game: the control its player's unit played, for the
	Link's to play too (its player_control.c): the tick (4: the N64's game
	time), the local player (1), the control's flags (4), its facing's yaw
	and pitch, the throttle's two parts and the trigger (5 x f32), the
	weapon, grenade and zoom it wants (3 x s8, -1 none) */
	N64_LINK_CONTROL = 7,
	/* the player's name (string, up to 11 characters): the Link plays under
	it from the next game it joins (sent after each WELCOME, and when it is
	changed) */
	N64_LINK_NAME = 8,
	/* the Link's */
	N64_LINK_WELCOME = 64, /* version (1), the Link's build (string) */
	N64_LINK_STATUS = 65,  /* a line for the menu (string) */
	N64_LINK_GAMES = 66,   /* count (1), each: id (4), players (1), most (1), name, map, game type (strings) */
	N64_LINK_LOAD = 67,    /* map (string), game type (string), teams (1), the player's team (1): the game begins */
	N64_LINK_FRAME = 68,   /* the tick's views and the players' own units (n64_online.c) */
	N64_LINK_ENDED = 69,   /* the game is over, or left */
	N64_LINK_OBJECTS = 70, /* the tick's objects, as many packets as they fill */
	N64_LINK_FRAME_END = 71, /* the tick's frame is whole */
	/* a message the game printed on a local player's HUD (who killed whom,
	who joined, the scores): the local player (1), its length (1), its UTF-16
	code units (2 each) */
	N64_LINK_MESSAGE = 72,
	/* the game joined, about once a second while joined (and once when it is
	left, flag 4 clear): flags (1: 1 teams, 2 the game is on -- else its
	lobby --, 4 joined), the red and blue teams' scores (2 + 2), the game's
	name, map and game type (strings), how many players in all (1) and in
	this list (1), then each, the best first: name, score (strings), kills
	and deaths (2 + 2), team (1: -1 none), flags (1: 1 this machine's) */
	N64_LINK_SCORES = 73,
	/* what happens in the Link's game, for the N64's own to make again
	(the layouts: the Link's link_export.c) -- the effects begun (weapons
	fired, impacts, explosions: their sounds with them), the sounds that
	are no effect's and the announcer's, a hit the player takes, and each
	frame the HUD's view of the game (the crosshair's target, the state
	message, the goals, the motion sensor's blips) */
	N64_LINK_EFFECTS = 74,
	N64_LINK_SOUNDS = 75,
	N64_LINK_DAMAGE = 76,
	N64_LINK_HUD = 77
};

/* the link's state, for the menu */
enum n64_link_state
{
	N64_LINK_NO_CART,     /* no flashcart USB port (nor an emulated one) */
	N64_LINK_WAITING,     /* a port, no Link answering yet */
	N64_LINK_CONNECTED    /* the Link has said WELCOME */
};

#define N64_LINK_GAMES_MAXIMUM 32

struct n64_link_game
{
	uint32_t id;
	uint8_t players, most;
	char name[32], map[32], game_type[24];
};

/* the USB port found (once) and the link's state; call each frame */
void n64_link_update(void);
enum n64_link_state n64_link_state(void);
/* the cart usb.c found (its CART_ names), the Link's build */
int n64_link_cart(void);
const char *n64_link_build(void);
/* the latest STATUS line ("" none) */
const char *n64_link_status(void);
/* the latest game list, and how many times one has come */
int n64_link_games(const struct n64_link_game **games);
unsigned long n64_link_games_serial(void);

/* a controller as the game's input layer makes it (xdk_input.c: the Xbox
pad's buttons, analog buttons and sticks) */
struct n64_link_pad
{
	uint8_t port;
	uint16_t buttons;
	uint8_t analog[8];
	int16_t sticks[4]; /* left x, left y, right x, right y */
};

/* the frame's controllers, to the Link (INPUT) */
void n64_link_send_input(uint32_t frame, const struct n64_link_pad *pads, int count);
/* a tick's control, to the Link (CONTROL) */
struct n64_link_control
{
	uint32_t tick;
	uint8_t local_player;
	uint32_t flags;
	float yaw, pitch, throttle[2], trigger;
	int8_t weapon, grenade, zoom;
};
void n64_link_send_control(const struct n64_link_control *control);

/* a LOAD the Link sent and the menu has not yet taken: its map (1), else 0;
whether the game has teams, and the player's */
int n64_link_take_load(char *map, int size, int *teams, int *team);

/* the online games (n64_online.c): the Link's frames handed over */
void n64_online_packet(int type, const uint8_t *payload, int length);
/* a controller on an N64 port for the Link (xdk_input.c: the player's
scheme and settings applied), 1 if there is one */
int n64_input_link_pad(int port, struct n64_link_pad *pad);

/* the player's name, to the Link now and after each WELCOME ("": the
Link's own) */
void n64_link_set_name(const char *name);

/* requests */
void n64_link_list(void);
void n64_link_join(uint32_t id);
void n64_link_leave(void);

#endif

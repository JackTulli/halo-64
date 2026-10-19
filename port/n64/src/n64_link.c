/* N64_LINK.C

Halo 64 Online's cable to Halo 64 Link (n64_link.h; port/n64/docs/online):
the flashcart's USB port, found once by libdragon's usb.c, then each frame
the packets that came read and handled, and HELLO said once a second until
the Link answers. A Link silent for LINK_SILENCE_MS is gone: the menu says
so, and HELLO goes out again. */

#ifdef HALO_N64_ONLINE

#include <libdragon.h>
#include <usb.h>
#include <stdio.h>
#include <string.h>

#include "n64.h"
#include "n64_link.h"

/* a Link that has sent nothing this long is gone (it says STATUS at least
once a second, but a map loading holds it for seconds) */
#define LINK_SILENCE_MS 15000
#define HELLO_INTERVAL_MS 1000
/* the most the N64 stays quiet while the Link is there (it takes five
seconds of silence for a pulled cable) */
#define KEEPALIVE_MS 1000
/* packets handled a frame at most (a game's tick is four: FRAME, two
OBJECTS, FRAME_END; thirty a second, at a frame rate under that) */
#define PACKETS_PER_UPDATE 64

static struct
{
	int initialized, cart;
	enum n64_link_state state;
	uint16_t sequence;
	unsigned long last_hello, last_heard, last_sent;
	/* (the log's: packets each way, the last report, the state reported) */
	unsigned long packets_in, packets_out, last_report;
	int reported_state;
	char build[32], status[96];
	char load_map[32];
	int load_pending, load_teams, load_team;
	struct n64_link_game games[N64_LINK_GAMES_MAXIMUM];
	int game_count;
	unsigned long games_serial;
	/* the game joined's players (SCORES) */
	struct n64_online_scores scores;
	/* the player's name (NAME) */
	char name[16];
	uint8_t packet[N64_LINK_HEADER_SIZE + N64_LINK_PAYLOAD_MAXIMUM] __attribute__((aligned(8)));
	uint8_t out[N64_LINK_HEADER_SIZE + N64_LINK_PAYLOAD_MAXIMUM] __attribute__((aligned(8)));
} cable;

/* ---------- packets out */

static void send_packet(int type, const void *payload, int length)
{
	uint8_t *p = cable.out;

	if (cable.cart == CART_NONE || length < 0 || length > N64_LINK_PAYLOAD_MAXIMUM)
		return;
	p[0] = 'H';
	p[1] = '6';
	p[2] = N64_LINK_VERSION;
	p[3] = (uint8_t)type;
	p[4] = (uint8_t)(length >> 8);
	p[5] = (uint8_t)length;
	p[6] = (uint8_t)(cable.sequence >> 8);
	p[7] = (uint8_t)cable.sequence;
	cable.sequence++;
	if (length)
		memcpy(p + N64_LINK_HEADER_SIZE, payload, length);
	usb_write(N64_LINK_DATATYPE, p, N64_LINK_HEADER_SIZE + length);
	cable.last_sent = get_ticks_ms();
	cable.packets_out++;
}

/* a string: its length (one byte), its bytes */
static int put_string(uint8_t *at, int room, const char *text)
{
	int length = (int)strlen(text);

	if (length > 255)
		length = 255;
	if (length + 1 > room)
		return 0;
	at[0] = (uint8_t)length;
	memcpy(at + 1, text, length);
	return length + 1;
}

static void send_hello(void)
{
	uint8_t payload[64];
	int length = 0;

	payload[length++] = N64_LINK_VERSION;
	length += put_string(payload + length, sizeof(payload) - length, "halo-online " __DATE__);
	send_packet(N64_LINK_HELLO, payload, length);
}

/* ---------- packets in */

struct reader
{
	const uint8_t *at, *end;
	int bad;
};

static unsigned read_u8(struct reader *r)
{
	if (r->at + 1 > r->end)
	{
		r->bad = 1;
		return 0;
	}
	return *r->at++;
}

static unsigned read_u16(struct reader *r)
{
	unsigned value;

	if (r->at + 2 > r->end)
	{
		r->bad = 1;
		return 0;
	}
	value = (unsigned)r->at[0] << 8 | r->at[1];
	r->at += 2;
	return value;
}

static uint32_t read_u32(struct reader *r)
{
	uint32_t value;

	if (r->at + 4 > r->end)
	{
		r->bad = 1;
		return 0;
	}
	value = (uint32_t)r->at[0] << 24 | (uint32_t)r->at[1] << 16 | (uint32_t)r->at[2] << 8 | r->at[3];
	r->at += 4;
	return value;
}

/* a string into out (size bytes with its end), upper case: the menu's
fonts have the capitals (what doesn't fit is cut) */
static void read_string(struct reader *r, char *out, int size)
{
	unsigned length = read_u8(r), i, kept = 0;

	if (r->at + length > r->end)
	{
		r->bad = 1;
		length = 0;
	}
	for (i = 0; i < length; i++)
	{
		char c = (char)r->at[i];

		if (c >= 'a' && c <= 'z')
			c = (char)(c - 'a' + 'A');
		if (c < ' ' || c > '~')
			c = '?';
		if ((int)kept < size - 1)
			out[kept++] = c;
	}
	out[kept] = 0;
	r->at += length;
}

static void handle(const uint8_t *packet, int size)
{
	struct reader r = {packet + N64_LINK_HEADER_SIZE, packet + size, 0};
	int length = packet[4] << 8 | packet[5];

	if (packet[0] != 'H' || packet[1] != '6' || N64_LINK_HEADER_SIZE + length > size)
		return;
	r.end = packet + N64_LINK_HEADER_SIZE + length;
	cable.last_heard = get_ticks_ms();
	if (packet[2] != N64_LINK_VERSION)
	{
		snprintf(cable.status, sizeof(cable.status), "THE LINK SPEAKS VERSION %d, THIS CARTRIDGE %d: UPDATE ONE",
			packet[2], N64_LINK_VERSION);
		cable.state = N64_LINK_WAITING;
		return;
	}
	switch (packet[3])
	{
	case N64_LINK_WELCOME:
		read_u8(&r);
		read_string(&r, cable.build, sizeof(cable.build));
		/* (the player's name, each time: a Link started again forgets it) */
		if (cable.state != N64_LINK_CONNECTED && cable.name[0])
		{
			cable.state = N64_LINK_CONNECTED;
			n64_link_set_name(cable.name);
		}
		cable.state = N64_LINK_CONNECTED;
		break;
	case N64_LINK_STATUS:
		read_string(&r, cable.status, sizeof(cable.status));
		break;
	case N64_LINK_GAMES:
	{
		int count = (int)read_u8(&r), kept = 0;

		for (int i = 0; i < count && !r.bad; i++)
		{
			struct n64_link_game game;

			game.id = read_u32(&r);
			game.players = (uint8_t)read_u8(&r);
			game.most = (uint8_t)read_u8(&r);
			read_string(&r, game.name, sizeof(game.name));
			read_string(&r, game.map, sizeof(game.map));
			read_string(&r, game.game_type, sizeof(game.game_type));
			if (!r.bad && kept < N64_LINK_GAMES_MAXIMUM)
				cable.games[kept++] = game;
		}
		cable.game_count = kept;
		cable.games_serial++;
		break;
	}
	case N64_LINK_LOAD:
	{
		char map[32], game_type[24];

		read_string(&r, map, sizeof(map));
		read_string(&r, game_type, sizeof(game_type));
		cable.load_teams = (int)read_u8(&r);
		cable.load_team = (int)read_u8(&r);
		if (r.bad)
			cable.load_teams = cable.load_team = 0;
		/* (the map's folder name: lower case, as the cartridge has it) */
		for (char *c = map; *c; c++)
			if (*c >= 'A' && *c <= 'Z')
				*c = (char)(*c - 'A' + 'a');
		snprintf(cable.load_map, sizeof(cable.load_map), "%s", map);
		cable.load_pending = 1;
		snprintf(cable.status, sizeof(cable.status), "LOADING %s", map);
		break;
	}
	case N64_LINK_ENDED:
		snprintf(cable.status, sizeof(cable.status), "THE GAME IS OVER");
		cable.load_pending = 0;
		n64_online_packet(packet[3], r.at, (int)(r.end - r.at));
		break;
	case N64_LINK_FRAME:
	case N64_LINK_OBJECTS:
	case N64_LINK_FRAME_END:
	case N64_LINK_MESSAGE:
	case N64_LINK_EFFECTS:
	case N64_LINK_SOUNDS:
	case N64_LINK_DAMAGE:
	case N64_LINK_HUD:
		n64_online_packet(packet[3], r.at, (int)(r.end - r.at));
		break;
	case N64_LINK_SCORES:
	{
		struct n64_online_scores *s = &cable.scores;
		unsigned flags = read_u8(&r);
		int count;

		s->teams = flags & 1;
		s->in_game = (flags >> 1) & 1;
		s->joined = (flags >> 2) & 1;
		s->red = (int16_t)read_u16(&r);
		s->blue = (int16_t)read_u16(&r);
		read_string(&r, s->game, sizeof(s->game));
		read_string(&r, s->map, sizeof(s->map));
		read_string(&r, s->game_type, sizeof(s->game_type));
		s->total = (int)read_u8(&r);
		count = (int)read_u8(&r);
		s->count = 0;
		for (int i = 0; i < count && !r.bad; i++)
		{
			struct n64_score_row row;

			memset(&row, 0, sizeof(row));
			read_string(&r, row.name, sizeof(row.name));
			read_string(&r, row.score, sizeof(row.score));
			row.kills = (short)(int16_t)read_u16(&r);
			row.deaths = (short)(int16_t)read_u16(&r);
			row.team = (signed char)(int8_t)read_u8(&r);
			row.human = (unsigned char)(read_u8(&r) & 1);
			if (!r.bad && s->count < N64_SCORE_ROWS)
				s->rows[s->count++] = row;
		}
		s->valid = !r.bad && s->joined;
		s->serial++;
		break;
	}
	default:
		break;
	}
}

/* ---------- each frame */

void n64_link_update(void)
{
	unsigned long now;

	if (!cable.initialized)
	{
		cable.initialized = 1;
		cable.cart = usb_initialize() ? usb_getcart() : CART_NONE;
		cable.state = cable.cart == CART_NONE ? N64_LINK_NO_CART : N64_LINK_WAITING;
		debugf("n64: online: flashcart USB %d\n", cable.cart);
	}
	if (cable.cart == CART_NONE)
		return;
	for (int i = 0; i < PACKETS_PER_UPDATE; i++)
	{
		unsigned long header = usb_poll();
		int type, size;

		if (!header)
			break;
		type = USBHEADER_GETTYPE(header);
		size = USBHEADER_GETSIZE(header);
		if (type != N64_LINK_DATATYPE || size < N64_LINK_HEADER_SIZE || size > (int)sizeof(cable.packet))
		{
			/* (another program's, or too long: passed over) */
			usb_skip(size);
			continue;
		}
		usb_read(cable.packet, size);
		cable.packets_in++;
		handle(cable.packet, size);
	}
	/* (after the packets: one just heard is never in the future) */
	now = get_ticks_ms();
	if (cable.state == N64_LINK_CONNECTED && now - cable.last_heard > LINK_SILENCE_MS)
	{
		cable.state = N64_LINK_WAITING;
		cable.game_count = 0;
		cable.scores.valid = 0;
		snprintf(cable.status, sizeof(cable.status), "THE LINK STOPPED ANSWERING");
	}
	if (cable.state == N64_LINK_WAITING && now - cable.last_hello >= HELLO_INTERVAL_MS)
	{
		cable.last_hello = now;
		send_hello();
	}
	if (cable.state == N64_LINK_CONNECTED && now - cable.last_sent >= KEEPALIVE_MS)
		n64_link_send_input(0, NULL, 0);
	if ((int)cable.state != cable.reported_state || now - cable.last_report >= 2000)
	{
		debugf("n64: link: state %d, %lu packets in, %lu out, last heard %lu ms ago, %s\n", cable.state,
			cable.packets_in, cable.packets_out, now - cable.last_heard, cable.status);
		cable.reported_state = (int)cable.state;
		cable.last_report = now;
	}
}

int n64_link_take_load(char *map, int size, int *teams, int *team)
{
	if (!cable.load_pending)
		return 0;
	cable.load_pending = 0;
	snprintf(map, size, "%s", cable.load_map);
	*teams = cable.load_teams;
	*team = cable.load_team;
	return 1;
}

enum n64_link_state n64_link_state(void)
{
	return cable.state;
}

int n64_link_cart(void)
{
	return cable.cart;
}

const char *n64_link_build(void)
{
	return cable.build;
}

const char *n64_link_status(void)
{
	return cable.status;
}

const struct n64_online_scores *n64_online_scores(void)
{
	return &cable.scores;
}

int n64_link_games(const struct n64_link_game **games)
{
	*games = cable.games;
	return cable.state == N64_LINK_CONNECTED ? cable.game_count : 0;
}

unsigned long n64_link_games_serial(void)
{
	return cable.games_serial;
}

/* ---------- requests */

void n64_link_send_input(uint32_t frame, const struct n64_link_pad *pads, int count)
{
	uint8_t payload[5 + 4 * 19];
	int length = 0;

	if (count < 0)
		count = 0;
	if (count > 4)
		count = 4;
	payload[length++] = (uint8_t)(frame >> 24);
	payload[length++] = (uint8_t)(frame >> 16);
	payload[length++] = (uint8_t)(frame >> 8);
	payload[length++] = (uint8_t)frame;
	payload[length++] = (uint8_t)count;
	for (int i = 0; i < count; i++)
	{
		payload[length++] = pads[i].port;
		payload[length++] = (uint8_t)(pads[i].buttons >> 8);
		payload[length++] = (uint8_t)pads[i].buttons;
		memcpy(payload + length, pads[i].analog, 8);
		length += 8;
		for (int k = 0; k < 4; k++)
		{
			payload[length++] = (uint8_t)((uint16_t)pads[i].sticks[k] >> 8);
			payload[length++] = (uint8_t)pads[i].sticks[k];
		}
	}
	send_packet(N64_LINK_INPUT, payload, length);
}

static int put_u32(uint8_t *at, uint32_t value)
{
	at[0] = (uint8_t)(value >> 24);
	at[1] = (uint8_t)(value >> 16);
	at[2] = (uint8_t)(value >> 8);
	at[3] = (uint8_t)value;
	return 4;
}

static int put_f32(uint8_t *at, float value)
{
	uint32_t bits;

	memcpy(&bits, &value, sizeof(bits));
	return put_u32(at, bits);
}

void n64_link_send_control(const struct n64_link_control *control)
{
	uint8_t payload[4 + 1 + 4 + 5 * 4 + 3];
	int length = 0;

	length += put_u32(payload + length, control->tick);
	payload[length++] = control->local_player;
	length += put_u32(payload + length, control->flags);
	length += put_f32(payload + length, control->yaw);
	length += put_f32(payload + length, control->pitch);
	length += put_f32(payload + length, control->throttle[0]);
	length += put_f32(payload + length, control->throttle[1]);
	length += put_f32(payload + length, control->trigger);
	payload[length++] = (uint8_t)control->weapon;
	payload[length++] = (uint8_t)control->grenade;
	payload[length++] = (uint8_t)control->zoom;
	send_packet(N64_LINK_CONTROL, payload, length);
}

void n64_link_set_name(const char *name)
{
	uint8_t payload[16];
	int length;

	snprintf(cable.name, sizeof(cable.name), "%s", name);
	if (cable.state != N64_LINK_CONNECTED)
		return;
	length = put_string(payload, sizeof(payload), cable.name);
	send_packet(N64_LINK_NAME, payload, length);
}

void n64_link_list(void)
{
	send_packet(N64_LINK_LIST, NULL, 0);
}

void n64_link_join(uint32_t id)
{
	uint8_t payload[4] = {(uint8_t)(id >> 24), (uint8_t)(id >> 16), (uint8_t)(id >> 8), (uint8_t)id};

	send_packet(N64_LINK_JOIN, payload, sizeof(payload));
}

void n64_link_leave(void)
{
	send_packet(N64_LINK_LEAVE, NULL, 0);
}

#endif /* HALO_N64_ONLINE */

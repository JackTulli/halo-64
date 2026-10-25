/* N64_ONLINE.C

Halo 64 Online's games (port/n64/docs/online), the platform's half: the
game itself is a local game of the Link's map, the player's own unit played
on the N64 at once (port/n64/game/n64_online_game.c); this file keeps the
Link's latest whole frame -- the views, the state the Link has of each
player's own unit, and every other object to draw -- and draws those objects
into the game's views (n64_online_draw_objects), the Link's camera for a
player in a vehicle (n64_online_view). Each tick's control goes to the Link
(n64_online_send_control); each drawn frame an INPUT tells it one was drawn,
the pace of its frames.

The Link's frames (its port/linux/game/link_export.c says the layout): a
FRAME packet (the views, each with its player's HUD and own unit), OBJECTS
packets (each object's model, tint, regions, bounding sphere and nodes: a
quaternion and an offset each), then FRAME_END; a frame is used once its
FRAME_END has come, the one before until then (two buffers). */

#ifdef HALO_N64_ONLINE

#include <libdragon.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "n64.h"
#include "n64_link.h"

#define ONLINE_VIEWS 4
#define ONLINE_OBJECTS 160
/* node matrices a frame holds (13 floats each: Halo's real_matrix4x3) */
#define ONLINE_NODES 1536
#define ONLINE_REGIONS 8
#define CLUSTER_WORDS 32

struct online_view
{
	int player;
	float position[3], forward[3], up[3];
	float field_of_view;
	short cluster_count;
	unsigned long clusters[CLUSTER_WORDS];
	struct n64_online_self self;
};

struct online_object
{
	unsigned short id;
	struct n64_object n64;
	unsigned char regions[ONLINE_REGIONS];
};

struct online_frame
{
	uint32_t tick;
	int view_count;
	struct online_view views[ONLINE_VIEWS];
	int object_count, node_count;
	struct online_object objects[ONLINE_OBJECTS];
	float nodes[ONLINE_NODES * 13];
	/* the first local player's HUD (HUD), if it came with the frame */
	int hud_valid;
	struct n64_online_hud hud;
};

static struct
{
	int playing, ended, port;
	/* building: the frame the packets fill; shown: the last whole one */
	struct online_frame *building, *shown;
	int shown_valid;
	unsigned long serial;
	uint32_t frame;
	/* (the log's: whole frames come, drawn, and the last report's time) */
	unsigned long frames_in, frames_drawn, controls_sent, reported;
} online;

static struct online_frame frames[2];

/* the Link's HUD messages not yet shown (MESSAGE) */
#define MESSAGES 8
#define MESSAGE_LENGTH 64

static struct
{
	int first, count;
	struct
	{
		int local_player_index;
		unsigned short text[MESSAGE_LENGTH];
	} queue[MESSAGES];
} messages;

/* what happens in the Link's game, not yet made again (EFFECTS, SOUNDS,
DAMAGE): queues, the oldest dropped when full */
#define QUEUED_EFFECTS 48
#define QUEUED_SOUNDS 32
#define QUEUED_DAMAGE 4

static struct
{
	int effect_first, effect_count, sound_first, sound_count, damage_first, damage_count;
	struct n64_online_effect effects[QUEUED_EFFECTS];
	struct n64_online_sound sounds[QUEUED_SOUNDS];
	struct n64_online_damage damage[QUEUED_DAMAGE];
} events;

/* a queue's next slot (the oldest dropped if full) */
#define QUEUE_PUSH(first, count, size) \
	((count) == (size) ? ((first) = ((first) + 1) % (size), (first + count - 1) % (size)) : \
		((count)++, ((first) + (count) - 1) % (size)))

/* ---------- reading the Link's packets (n64_link.c hands them over) */

struct online_reader
{
	const uint8_t *at, *end;
	int bad;
};

static uint32_t read_u32(struct online_reader *r)
{
	uint32_t v;

	if (r->at + 4 > r->end)
	{
		r->bad = 1;
		return 0;
	}
	v = (uint32_t)r->at[0] << 24 | (uint32_t)r->at[1] << 16 | (uint32_t)r->at[2] << 8 | r->at[3];
	r->at += 4;
	return v;
}

static unsigned read_u16(struct online_reader *r)
{
	unsigned v;

	if (r->at + 2 > r->end)
	{
		r->bad = 1;
		return 0;
	}
	v = (unsigned)r->at[0] << 8 | r->at[1];
	r->at += 2;
	return v;
}

static unsigned read_u8(struct online_reader *r)
{
	if (r->at + 1 > r->end)
	{
		r->bad = 1;
		return 0;
	}
	return *r->at++;
}

static float read_f32(struct online_reader *r)
{
	uint32_t bits = read_u32(r);
	float value;

	memcpy(&value, &bits, sizeof(value));
	return isfinite(value) ? value : 0.f;
}

static float read_unit(struct online_reader *r)
{
	return (float)(int16_t)read_u16(r) * (1.f / 32767.f);
}

static void skip_string(struct online_reader *r)
{
	unsigned length = read_u8(r);

	if (r->at + length > r->end)
		r->bad = 1;
	else
		r->at += length;
}

/* a view's player's own unit (link_export.c's put_self) */
static void read_self(struct online_reader *r, struct n64_online_self *s)
{
	unsigned flags;

	s->applied_tick = read_u32(r);
	flags = read_u8(r);
	s->alive = flags & 1;
	s->in_vehicle = (flags >> 1) & 1;
	s->action_target = (flags >> 2) & 1;
	for (int k = 0; k < 3; k++)
		s->position[k] = read_f32(r);
	for (int k = 0; k < 3; k++)
		s->velocity[k] = read_f32(r);
	s->yaw = read_f32(r);
	s->pitch = read_f32(r);
	s->respawn_ticks = (int)read_u16(r);
	s->team = (int)(int8_t)read_u8(r);
	s->current_weapon = (int)(int8_t)read_u8(r);
	for (int w = 0; w < 4; w++)
	{
		unsigned definition = read_u16(r);

		s->weapons[w].definition = definition == 0xFFFF ? -1 : (int)definition;
		s->weapons[w].loaded = (int)(int16_t)read_u16(r);
		s->weapons[w].total = (int)(int16_t)read_u16(r);
		s->weapons[w].age = (float)read_u8(r) * (1.f / 255.f);
	}
}

static void frame_begin(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	struct online_frame *f = online.building;
	int views;

	f->tick = read_u32(&r);
	f->object_count = f->node_count = 0;
	f->hud_valid = 0;
	views = (int)read_u8(&r);
	f->view_count = 0;
	for (int i = 0; i < views && !r.bad; i++)
	{
		struct online_view *v = &f->views[f->view_count < ONLINE_VIEWS ? f->view_count : ONLINE_VIEWS - 1];
		struct n64_online_self *s = &v->self;
		int words;

		v->player = (int)read_u8(&r);
		for (int k = 0; k < 3; k++)
			v->position[k] = read_f32(&r);
		for (int k = 0; k < 3; k++)
			v->forward[k] = read_unit(&r);
		for (int k = 0; k < 3; k++)
			v->up[k] = read_unit(&r);
		v->field_of_view = read_f32(&r);
		v->cluster_count = (short)read_u16(&r);
		words = (v->cluster_count + 31) / 32;
		for (int w = 0; w < words; w++)
		{
			uint32_t bits = read_u32(&r);

			if (w < CLUSTER_WORDS)
				v->clusters[w] = bits;
		}
		if (words > CLUSTER_WORDS)
			v->cluster_count = 0;
		/* the player's HUD: alive, health, shields, the weapon's name,
		rounds, heat, grenades (the weapon's own are in the unit's state) */
		read_u8(&r);
		s->health = (float)read_u8(&r) * (1.f / 255.f);
		s->shields = (float)read_u16(&r) * (1.f / 1000.f);
		skip_string(&r);
		read_u16(&r);
		read_u16(&r);
		read_u8(&r);
		s->frags = (int)read_u8(&r);
		s->plasmas = (int)read_u8(&r);
		read_self(&r, s);
		if (f->view_count < ONLINE_VIEWS)
			f->view_count++;
	}
	if (r.bad)
		f->view_count = 0;
}

/* a node's matrix (Halo's real_matrix4x3: scale, forward, left, up,
position) from its quaternion, offset and scale */
static void node_matrix(float *m, const float q[4], const float position[3], float scale)
{
	float x = q[0], y = q[1], z = q[2], w = q[3];
	float length = x * x + y * y + z * z + w * w;

	if (length > 0.f && fabsf(length - 1.f) > 0.001f)
	{
		float inverse = 1.f / sqrtf(length);

		x *= inverse;
		y *= inverse;
		z *= inverse;
		w *= inverse;
	}
	m[0] = scale;
	/* forward: the first column, left the second, up the third */
	m[1] = 1.f - 2.f * (y * y + z * z);
	m[2] = 2.f * (x * y + w * z);
	m[3] = 2.f * (x * z - w * y);
	m[4] = 2.f * (x * y - w * z);
	m[5] = 1.f - 2.f * (x * x + z * z);
	m[6] = 2.f * (y * z + w * x);
	m[7] = 2.f * (x * z + w * y);
	m[8] = 2.f * (y * z - w * x);
	m[9] = 1.f - 2.f * (x * x + y * y);
	m[10] = position[0];
	m[11] = position[1];
	m[12] = position[2];
}

static void frame_objects(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	struct online_frame *f = online.building;
	int count;

	if (read_u32(&r) != f->tick)
		return;
	count = (int)read_u8(&r);
	for (int i = 0; i < count && !r.bad; i++)
	{
		struct online_object *o = f->object_count < ONLINE_OBJECTS ? &f->objects[f->object_count] : NULL;
		unsigned regions, nodes, flags;
		float origin[3], center[3], radius;
		float *matrices = f->nodes + f->node_count * 13;
		unsigned char tint[4];

		unsigned id = read_u16(&r);
		unsigned model = read_u16(&r);
		for (int k = 0; k < 4; k++)
			tint[k] = (unsigned char)read_u8(&r);
		regions = read_u8(&r);
		unsigned char region_bytes[32];
		for (unsigned k = 0; k < regions; k++)
		{
			unsigned value = read_u8(&r);

			if (k < sizeof(region_bytes))
				region_bytes[k] = (unsigned char)value;
		}
		for (int k = 0; k < 3; k++)
			center[k] = read_f32(&r);
		radius = read_f32(&r);
		nodes = read_u8(&r);
		flags = read_u8(&r);
		for (int k = 0; k < 3; k++)
			origin[k] = read_f32(&r);
		int fits = o && f->node_count + (int)nodes <= ONLINE_NODES;
		for (unsigned n = 0; n < nodes && !r.bad; n++)
		{
			float q[4], position[3], scale = 1.f;

			for (int k = 0; k < 4; k++)
				q[k] = read_unit(&r);
			for (int k = 0; k < 3; k++)
				position[k] = origin[k] + (float)(int16_t)read_u16(&r) * (1.f / 256.f);
			if (flags & 1)
				scale = (float)read_u16(&r) * (1.f / 4096.f);
			if (fits)
				node_matrix(matrices + n * 13, q, position, scale);
		}
		if (r.bad || !fits)
			continue;
		o->id = (unsigned short)id;
		memset(&o->n64, 0, sizeof(o->n64));
		o->n64.model_tag_index = (short)model;
		o->n64.node_count = (short)nodes;
		o->n64.node_matrices = matrices;
		memset(o->regions, 0, sizeof(o->regions));
		for (unsigned k = 0; k < regions && k < ONLINE_REGIONS && k < sizeof(region_bytes); k++)
			o->regions[k] = region_bytes[k];
		o->n64.region_permutations = o->regions;
		o->n64.region_count = ONLINE_REGIONS;
		o->n64.head_node = -1;
		memcpy(o->n64.center, center, sizeof(center));
		o->n64.radius = radius;
		memcpy(o->n64.tint, tint, sizeof(tint));
		/* (the renderer's key: none of the local game's objects') */
		o->n64.key = 0x40000000UL + id;
		f->node_count += (int)nodes;
		f->object_count++;
	}
}

static void frame_end(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	struct online_frame *done;

	if (read_u32(&r) != online.building->tick || r.bad)
		return;
	done = online.building;
	online.building = online.shown;
	online.shown = done;
	online.shown_valid = 1;
	online.serial++;
	done->hud.serial = online.serial;
	for (int v = 0; v < done->view_count; v++)
		done->views[v].self.serial = online.serial;
	online.frames_in++;
}

static void message(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	int local_player_index = (int)read_u8(&r), units = (int)read_u8(&r), kept = 0, slot;

	if (r.bad)
		return;
	/* (full: the oldest dropped) */
	if (messages.count == MESSAGES)
	{
		messages.first = (messages.first + 1) % MESSAGES;
		messages.count--;
	}
	slot = (messages.first + messages.count) % MESSAGES;
	for (int i = 0; i < units && !r.bad; i++)
	{
		unsigned unit = read_u16(&r);

		if (kept < MESSAGE_LENGTH - 1 && !r.bad)
			messages.queue[slot].text[kept++] = (unsigned short)unit;
	}
	messages.queue[slot].text[kept] = 0;
	messages.queue[slot].local_player_index = local_player_index;
	messages.count++;
}

static void effects_in(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	int count = (int)read_u8(&r);

	for (int i = 0; i < count && !r.bad; i++)
	{
		struct n64_online_effect e;

		e.definition = (int)read_u16(&r);
		for (int k = 0; k < 3; k++)
			e.point[k] = read_f32(&r);
		for (int k = 0; k < 3; k++)
			e.forward[k] = read_unit(&r);
		e.scale_a = (float)read_u8(&r) * (1.f / 255.f);
		e.scale_b = (float)read_u8(&r) * (1.f / 255.f);
		if (r.bad)
			break;
		events.effects[QUEUE_PUSH(events.effect_first, events.effect_count, QUEUED_EFFECTS)] = e;
	}
}

static void sounds_in(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	int count = (int)read_u8(&r);

	for (int i = 0; i < count && !r.bad; i++)
	{
		struct n64_online_sound sound;
		unsigned flags;

		sound.definition = (int)read_u16(&r);
		for (int k = 0; k < 3; k++)
			sound.point[k] = read_f32(&r);
		sound.scale = (float)read_u8(&r) * (1.f / 255.f);
		flags = read_u8(&r);
		sound.announcer = (flags & 2) != 0;
		if (r.bad)
			break;
		events.sounds[QUEUE_PUSH(events.sound_first, events.sound_count, QUEUED_SOUNDS)] = sound;
	}
}

static void damage_in(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	struct n64_online_damage d;

	d.local_player_index = (int)read_u8(&r);
	d.definition = (int)read_u16(&r);
	for (int k = 0; k < 3; k++)
		d.direction[k] = read_unit(&r);
	d.scale = read_f32(&r);
	d.total = read_f32(&r);
	d.has_attacker = (int)(read_u8(&r) & 1);
	for (int k = 0; k < 3; k++)
		d.attacker[k] = read_f32(&r);
	if (r.bad)
		return;
	events.damage[QUEUE_PUSH(events.damage_first, events.damage_count, QUEUED_DAMAGE)] = d;
}

static void hud_in(const uint8_t *payload, int length)
{
	struct online_reader r = {payload, payload + length, 0};
	struct online_frame *f = online.building;
	struct n64_online_hud *h = &f->hud;
	unsigned count;

	/* (the first local player's: the only one yet) */
	if (read_u8(&r) != 0)
		return;
	h->autoaim = (float)read_u8(&r) * (1.f / 255.f);
	count = read_u8(&r);
	for (unsigned i = 0; i < count; i++)
	{
		char c = (char)read_u8(&r);

		if (i < sizeof(h->target) - 1)
			h->target[i] = c >= 'a' && c <= 'z' ? (char)(c - 'a' + 'A') : c;
	}
	h->target[count < sizeof(h->target) - 1 ? count : sizeof(h->target) - 1] = 0;
	h->target_enemy = (int)read_u8(&r);
	count = read_u8(&r);
	for (unsigned i = 0; i < count; i++)
	{
		unsigned unit = read_u16(&r);

		if (i < 63)
			h->state[i] = (unsigned short)unit;
	}
	h->state[count < 63 ? count : 63] = 0;
	h->goal_radar = (int)read_u8(&r);
	count = read_u8(&r);
	h->goal_count = 0;
	for (unsigned i = 0; i < count && !r.bad; i++)
	{
		float position[3];
		short nav_point;

		for (int k = 0; k < 3; k++)
			position[k] = read_f32(&r);
		nav_point = (short)read_u16(&r);
		if (h->goal_count < N64_ONLINE_GOALS)
		{
			memcpy(h->goals[h->goal_count].position, position, sizeof(position));
			h->goals[h->goal_count].nav_point = nav_point;
			h->goal_count++;
		}
	}
	count = read_u8(&r);
	h->blip_count = 0;
	for (unsigned i = 0; i < count && !r.bad; i++)
	{
		float position[3];
		signed char type, size;

		for (int k = 0; k < 3; k++)
			position[k] = read_f32(&r);
		type = (signed char)read_u8(&r);
		size = (signed char)read_u8(&r);
		if (h->blip_count < N64_ONLINE_BLIPS)
		{
			memcpy(h->blips[h->blip_count].position, position, sizeof(position));
			h->blips[h->blip_count].type = type;
			h->blips[h->blip_count].size = size;
			h->blip_count++;
		}
	}
	f->hud_valid = !r.bad;
	/* the aim's magnetism and its target's motion across the view (a Link
	before v0.6.2 ends the message before them: none) */
	h->magnetism = 0.f;
	h->target_velocity[0] = h->target_velocity[1] = 0.f;
	if (f->hud_valid && r.end - r.at >= 5)
	{
		h->magnetism = (float)read_u8(&r) * (1.f / 255.f);
		h->target_velocity[0] = read_unit(&r) * N64_ONLINE_ANGULAR_RANGE;
		h->target_velocity[1] = read_unit(&r) * N64_ONLINE_ANGULAR_RANGE;
	}
}

int n64_online_take_effect(struct n64_online_effect *effect)
{
	if (!events.effect_count)
		return 0;
	*effect = events.effects[events.effect_first];
	events.effect_first = (events.effect_first + 1) % QUEUED_EFFECTS;
	events.effect_count--;
	return 1;
}

int n64_online_take_sound(struct n64_online_sound *sound)
{
	if (!events.sound_count)
		return 0;
	*sound = events.sounds[events.sound_first];
	events.sound_first = (events.sound_first + 1) % QUEUED_SOUNDS;
	events.sound_count--;
	return 1;
}

int n64_online_take_damage(struct n64_online_damage *damage)
{
	if (!events.damage_count)
		return 0;
	*damage = events.damage[events.damage_first];
	events.damage_first = (events.damage_first + 1) % QUEUED_DAMAGE;
	events.damage_count--;
	return 1;
}

const struct n64_online_hud *n64_online_hud(int local_player_index)
{
	if (local_player_index != 0 || !online.playing || !online.shown_valid || !online.shown->hud_valid)
		return NULL;
	return &online.shown->hud;
}

int n64_online_take_message(int *local_player_index, unsigned short *text, int size)
{
	int i;

	if (!messages.count || size < 1)
		return 0;
	*local_player_index = messages.queue[messages.first].local_player_index;
	for (i = 0; i < size - 1 && messages.queue[messages.first].text[i]; i++)
		text[i] = messages.queue[messages.first].text[i];
	text[i] = 0;
	messages.first = (messages.first + 1) % MESSAGES;
	messages.count--;
	return 1;
}

void n64_online_packet(int type, const uint8_t *payload, int length)
{
	if (!online.playing || !online.building)
		return;
	if (type == N64_LINK_MESSAGE)
		message(payload, length);
	else if (type == N64_LINK_EFFECTS)
		effects_in(payload, length);
	else if (type == N64_LINK_SOUNDS)
		sounds_in(payload, length);
	else if (type == N64_LINK_DAMAGE)
		damage_in(payload, length);
	else if (type == N64_LINK_HUD)
		hud_in(payload, length);
	else if (type == N64_LINK_FRAME)
		frame_begin(payload, length);
	else if (type == N64_LINK_OBJECTS)
		frame_objects(payload, length);
	else if (type == N64_LINK_FRAME_END)
		frame_end(payload, length);
	else if (type == N64_LINK_ENDED)
		online.ended = 1;
}

/* ---------- the game's (n64_online_game.c, n64_render_game.c) */

void n64_online_begin(int port)
{
	debugf("n64: online: a game begins (controller %d)\n", port + 1);
	online.building = &frames[0];
	online.shown = &frames[1];
	online.shown_valid = 0;
	online.ended = 0;
	online.port = port;
	online.playing = 1;
	messages.first = messages.count = 0;
	memset(&events, 0, sizeof(events));
	online.frames_in = online.frames_drawn = online.controls_sent = 0;
	online.reported = get_ticks_ms();
}

void n64_online_end(void)
{
	if (online.playing)
		debugf("n64: online: the game is over, back to the menu\n");
	online.playing = 0;
	online.shown_valid = 0;
}

int n64_online_active(void)
{
	return online.playing;
}

int n64_online_update(void)
{
	struct n64_pad pad;

	if (!online.playing)
		return 0;
	n64_link_update();
	if (n64_link_state() != N64_LINK_CONNECTED)
	{
		debugf("n64: online: the Link is gone\n");
		return 0;
	}
	/* (Start and Z together: leave) */
	n64_input_get(online.port, &pad);
	if (pad.connected && (pad.buttons & N64_BUTTON_START) && (pad.buttons & N64_BUTTON_Z))
	{
		debugf("n64: online: leaving (Start and Z)\n");
		n64_link_leave();
		return 0;
	}
	if (online.ended)
	{
		debugf("n64: online: the Link says the game is over\n");
		return 0;
	}
	/* (a frame drawn: the Link's next one, please -- no controllers: the
	game's control goes each tick, CONTROL) */
	n64_link_send_input(online.frame++, NULL, 0);
	online.frames_drawn++;
	if (get_ticks_ms() - online.reported >= 5000)
	{
		struct online_frame *f = online.shown;

		online.reported = get_ticks_ms();
		debugf("n64: online: %lu frames from the Link, %lu drawn, %lu controls sent; tick %lu, %d objects, %d nodes\n",
			online.frames_in, online.frames_drawn, online.controls_sent, (unsigned long)f->tick, f->object_count,
			f->node_count);
	}
	return 1;
}

void n64_online_send_control(const struct n64_online_control *control)
{
	struct n64_link_control c;

	if (!online.playing)
		return;
	c.tick = (uint32_t)control->tick;
	c.local_player = (uint8_t)control->local_player;
	c.flags = (uint32_t)control->flags;
	c.yaw = control->yaw;
	c.pitch = control->pitch;
	c.throttle[0] = control->throttle[0];
	c.throttle[1] = control->throttle[1];
	c.trigger = control->trigger;
	c.weapon = (int8_t)control->weapon;
	c.grenade = (int8_t)control->grenade;
	c.zoom = (int8_t)control->zoom;
	n64_link_send_control(&c);
	online.controls_sent++;
}

static struct online_view *view_of(int local_player_index)
{
	struct online_frame *f = online.shown;

	if (!online.playing || !online.shown_valid)
		return NULL;
	for (int v = 0; v < f->view_count; v++)
	{
		if (f->views[v].player == local_player_index)
			return &f->views[v];
	}
	return NULL;
}

const struct n64_online_self *n64_online_self(int local_player_index)
{
	struct online_view *v = view_of(local_player_index);

	return v ? &v->self : NULL;
}

int n64_online_view(int local_player_index, struct n64_view *view)
{
	struct online_view *v = view_of(local_player_index);

	if (!v || !v->self.in_vehicle)
		return 0;
	memcpy(view->position, v->position, sizeof(view->position));
	memcpy(view->forward, v->forward, sizeof(view->forward));
	memcpy(view->up, v->up, sizeof(view->up));
	/* (Halo's field of view is the horizontal one; the view's vertical one at
	the screen's 4:3, the view's own shape) */
	if (view->y1 > view->y0)
	{
		float aspect = (float)(view->x1 - view->x0) / (float)(view->y1 - view->y0) * 0.75f;
		float half = tanf(v->field_of_view * 0.5f) * 0.75f / (aspect > 0.f ? aspect : 1.f);

		view->vertical_field_of_view = 2.f * atanf(half);
	}
	return 1;
}

void n64_online_draw_objects(void)
{
	struct online_frame *f = online.shown;

	if (!online.playing || !online.shown_valid)
		return;
	for (int i = 0; i < f->object_count; i++)
		n64_render_object(&f->objects[i].n64);
}

#endif /* HALO_N64_ONLINE */

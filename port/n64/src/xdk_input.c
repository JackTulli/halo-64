/*
XDK_INPUT.C

The XInput gamepads the game reads (input/input_xbox.c), from the N64's
four controller ports (n64_input.c).

The game is played with the Xbox's two sticks; the N64 controller has one,
so the default layout is the one N64 shooters used (Turok, Perfect Dark's
1.2): the stick turns and looks, the C buttons walk and strafe.

    stick       look (Xbox right stick)
    C buttons   move and strafe (Xbox left stick)
    Z           fire (right trigger)
    R           throw a grenade (left trigger)
    A           jump (A)
    B           reload / action (X)
    L           melee (B)
    D-pad up    switch weapon (Y)
    D-pad down  crouch (left stick click)
    D-pad left  zoom (right stick click)
    D-pad right switch grenades (black); held, the flashlight (white)
    Start       start

The game's rumble (XInputSetState, once a frame) goes to the controller's
Rumble Pak (n64_input_rumble: its one motor switched on for a share of the
frames as strong as Halo's two motors).

Each controller can choose another scheme on the menu's CONTROLS page
(n64_controls, n64.h): bumper jumper (R jumps, A throws a grenade),
GoldenEye's (the stick turns and walks, C left and right strafe, C up and
down look), north (the stick walks and strafes, the C buttons look) or
D-pad move (the standard one with the C buttons and the D-pad swapped: the
D-pad walks and strafes, C up switches weapons, C down crouches, C left
zooms, C right switches grenades -- issue 5, GoldenEye's and Perfect Dark's
way of holding the controller);
crouch held or toggled (a jump stands back up), look inverted or not, fire
and grenades swapped or not, and the look speeds of the stick and of the C
buttons (n64_look_speed: Halo's turning rates times them).

Or two controllers, one in each hand, held by their middle grips, as
GoldenEye's and Perfect Dark's 2.x styles have it (dual_pads): the second
is the player's choice, else Perfect Dark's pairs, sockets 1 and 3, 2 and 4
(n64_input_pairs, found again at each poll), and the lower-numbered socket
is the left hand. DUAL MODERN: the left stick walks and strafes, the right
one turns and looks up and down; DUAL CLASSIC: the left stick walks and
turns, the right one strafes and looks up and down (GoldenEye's way of
turning); their LEFT versions swap the hands. The aiming hand's Z fires,
the other's throws a grenade (the two swapped by the fire setting); every
other button does what it does in the standard scheme on either controller,
but the C buttons, which no longer walk: C up switches weapons, C down
crouches, C left is melee, C right zooms. Without a second controller the
first plays the standard way.

Test mode (a file "autotest" on the cartridge): the first two ports are
driven by a script instead, so an unattended run exercises the game
(n64_test_input).
*/

#include "platform.h"
#include "n64_debug_flags.h"

#include <string.h>

#define PORT_COUNT 4

XPP_DEVICE_TYPE XDEVICE_TYPE_GAMEPAD_TABLE;
XPP_DEVICE_TYPE XDEVICE_TYPE_MEMORY_UNIT_TABLE;
XPP_DEVICE_TYPE XDEVICE_TYPE_DEBUG_KEYBOARD_TABLE;

struct controller
{
	BOOL open;
	DWORD packet_number;
	XINPUT_GAMEPAD previous;
};

static struct controller controllers[PORT_COUNT];
static DWORD reported_gamepads;
static struct n64_pad golden_pads[4];

/* D-pad right, tapped or held: a tap switches grenades (Halo's Black
button), a hold of D_RIGHT_HOLD polls the flashlight (its White) -- it
pressed White alone, so no layout could switch grenades (issue 4); on the
campaign cartridge the hold steps the players' speed (n64_campaign.c) */
#define D_RIGHT_HOLD 10
static unsigned char d_right_held[PORT_COUNT];
static unsigned long last_poll_tick;

int n64_test_input = 0;
/* the pause menu is up (port/n64/game/n64_game.c): every gamepad reads as
untouched, so that no player turns, fires or jumps meanwhile */
int n64_input_blocked = 0;
/* the N64 controller port each of the game's gamepads reads: the menu's
players are the game's gamepads 0 to n-1 (port/n64/game/n64_game.c) */
int n64_gamepad_ports[4] = {0, 1, 2, 3};
/* how many of them are players' (0: not set, every controller one) */
int n64_gamepad_players = 0;
/* each port's second controller (a two-controller player's), found at each
poll: its port, or -1 */
signed char n64_input_partners[4] = {-1, -1, -1, -1};

int n64_input_partner_of(int port)
{
	for (int owner = 0; owner < 4; owner++)
		if (n64_input_partners[owner] == port)
			return owner;
	return -1;
}

static void find_partners(void)
{
	int players = 0;

	for (int i = 0; i < n64_gamepad_players && i < 4; i++)
		players |= 1 << (n64_gamepad_ports[i] & 3);
	n64_input_pairs(n64_gamepad_players > 0 ? players : -1, n64_input_partners);
}

static void poll(void)
{
	/* once per game frame is plenty: the game reads every port in turn */
	unsigned long now = n64_ticks();

	if (now - last_poll_tick > N64_TICKS_PER_SECOND / 240)
	{
		n64_input_poll();
		find_partners();
		last_poll_tick = now;
	}
}

static DWORD connected_gamepads(void)
{
	DWORD mask = 0;
	int port;

	if (n64_test_input)
		return 0xF;
	poll();
	for (port = 0; port < PORT_COUNT; port++)
	{
		struct n64_pad pad;

		n64_input_get(n64_gamepad_ports[port], &pad);
		if (pad.connected)
			mask |= 1 << port;
	}
	return mask;
}

VOID WINAPI XInitDevices(DWORD preallocation_type_count, PXDEVICE_PREALLOC_TYPE preallocation_types)
{
	(void)preallocation_type_count;
	(void)preallocation_types;
}

BOOL WINAPI XGetDeviceChanges(PXPP_DEVICE_TYPE device_type, PDWORD insertions, PDWORD removals)
{
	*insertions = 0;
	*removals = 0;
	if (device_type == XDEVICE_TYPE_GAMEPAD)
	{
		DWORD connected = connected_gamepads();

		*insertions = connected & ~reported_gamepads;
		*removals = reported_gamepads & ~connected;
		reported_gamepads = connected;
	}
	return *insertions || *removals;
}

HANDLE WINAPI XInputOpen(PXPP_DEVICE_TYPE device_type, DWORD port, DWORD slot,
	PXINPUT_POLLING_PARAMETERS polling_parameters)
{
	(void)slot;
	(void)polling_parameters;
	if (device_type == XDEVICE_TYPE_GAMEPAD && port < PORT_COUNT)
	{
		memset(&controllers[port], 0, sizeof(controllers[port]));
		controllers[port].open = TRUE;
		return (HANDLE)&controllers[port];
	}
	SetLastError(ERROR_DEVICE_NOT_CONNECTED);
	return NULL;
}

VOID WINAPI XInputClose(HANDLE device)
{
	struct controller *controller = (struct controller *)device;

	if (controller >= controllers && controller < controllers + PORT_COUNT)
		controller->open = FALSE;
}

static int controller_port(HANDLE device)
{
	struct controller *controller = (struct controller *)device;

	if (controller < controllers || controller >= controllers + PORT_COUNT || !controller->open)
		return -1;
	return (int)(controller - controllers);
}

/* the N64 stick reads about +-85 at full tilt */
static SHORT stick(int value)
{
	long scaled = (long)value * 32767 / 80;

	if (scaled > 32767)
		scaled = 32767;
	if (scaled < -32768)
		scaled = -32768;
	return (SHORT)scaled;
}

/* a C button pair as one axis of an Xbox stick: -1, 0 or 1 times full (a C
button looks with the stick all the way over -- Halo turns faster still
when the stick is held there -- times the C buttons' own look speed,
n64_look_speed) */
static SHORT c_axis(unsigned short buttons, unsigned short plus, unsigned short minus)
{
	return (SHORT)((buttons & plus) ? 32767 : (buttons & minus) ? -32767 : 0);
}

/* the C buttons' look speed at the menu's 5: half again the stick's (with
no in-between, the slow end of the stick's range is no use to them) */
#define C_LOOK_SPEED 1.5f

/* a look speed of the menu's (n64_controls: 1-10 less 5): 1.25 times
faster a step */
static float look_speed(signed char setting)
{
	float speed = 1.f;

	for (int i = 0; i < setting; i++)
		speed *= 1.25f;
	for (int i = 0; i > setting; i--)
		speed *= 0.8f;
	return speed;
}

void n64_look_speed(int local_player_index, float *yaw, float *pitch)
{
	const struct n64_controls *controls = &n64_controls[n64_gamepad_ports[local_player_index & 3] & 3];
	float stick_speed = look_speed(controls->stick_speed);
	float c_speed = C_LOOK_SPEED * look_speed(controls->c_speed);

	*yaw = controls->scheme == N64_SCHEME_NORTH ? c_speed : stick_speed;
	*pitch = controls->scheme == N64_SCHEME_NORTH || controls->scheme == N64_SCHEME_GOLDENEYE ? c_speed : controls->scheme == N64_SCHEME_GOLDEN_STYLE ? c_speed / C_LOOK_SPEED : stick_speed;
}

int n64_golden_selected(int local)
{
	return local >= 0 && local < 4 && n64_controls[n64_gamepad_ports[local] & 3].scheme == N64_SCHEME_GOLDEN_STYLE;
}

void n64_golden_move(int local, float *forward, float *strafe)
{
	const struct n64_pad *pad = &golden_pads[n64_gamepad_ports[local & 3] & 3];
	*forward = pad->stick_y >= -5 && pad->stick_y <= 5 ? 0.f :
		pad->stick_y > 0 ? n64_min_f(1.f, (pad->stick_y - 5.f) / 70.f) : n64_max_f(-1.f, (pad->stick_y + 5.f) / 70.f);
	*strafe = (pad->buttons & N64_BUTTON_C_RIGHT) ? -1.f : (pad->buttons & N64_BUTTON_C_LEFT) ? 1.f : 0.f;
}

unsigned short n64_golden_buttons(int local)
{
	return golden_pads[n64_gamepad_ports[local & 3] & 3].buttons;
}

int n64_golden_raw(int local, float *x, float *y, int *aim)
{
	int port;
	const struct n64_pad *pad;
	const struct n64_controls *controls;
	if (!n64_golden_selected(local) || n64_input_blocked) return 0;
	port = n64_gamepad_ports[local] & 3;
	pad = &golden_pads[port];
	controls = &n64_controls[port];
	if (!pad->connected) return 0;
	*aim = !!(pad->buttons & N64_BUTTON_R);
	*x = pad->stick_x;
	*y = *aim ? pad->stick_y : (pad->buttons & N64_BUTTON_C_UP) ? 80.f : (pad->buttons & N64_BUTTON_C_DOWN) ? -80.f : 0.f;
	if (controls->inverted) *y = -*y;
	return 1;
}

/* the C buttons and the D-pad swapped (N64_SCHEME_DPAD) */
static unsigned short swap_c_and_d(unsigned short b)
{
	static const unsigned short c[4] = {N64_BUTTON_C_UP, N64_BUTTON_C_DOWN, N64_BUTTON_C_LEFT, N64_BUTTON_C_RIGHT};
	static const unsigned short d[4] = {N64_BUTTON_D_UP, N64_BUTTON_D_DOWN, N64_BUTTON_D_LEFT, N64_BUTTON_D_RIGHT};
	unsigned short out = b;

	for (int i = 0; i < 4; i++)
	{
		out &= (unsigned short)~(c[i] | d[i]);
		if (b & c[i])
			out |= d[i];
		if (b & d[i])
			out |= c[i];
	}
	return out;
}

/* a two-controller player's controllers as one (N64_SCHEME_DUAL_*): the
sticks as the style has them, into the gamepad; the buttons as one
controller's of the standard scheme (b), the aiming hand's Z as Z (fire),
the other hand's Z as R (grenade), and the C buttons as what they do here
-- C up the D-pad's up (switch weapons), C down its down (crouch), C left L
(melee), C right the D-pad's left (zoom); the left hand the lower port */
static unsigned short dual_pads(const struct n64_controls *controls, int n64_port, const struct n64_pad *pad,
	int partner_port, const struct n64_pad *partner, XINPUT_GAMEPAD *gamepad, SHORT *look_y)
{
	const struct n64_pad *left = partner_port < (n64_port & 3) ? partner : pad;
	const struct n64_pad *right = left == pad ? partner : pad;
	const struct n64_pad *aim = N64_SCHEME_DUAL_LEFT_AIMS(controls->scheme) ? left : right;
	const struct n64_pad *move = aim == left ? right : left;
	unsigned short both = left->buttons | right->buttons;
	unsigned short b = both & (unsigned short)~(N64_BUTTON_Z | N64_BUTTON_R |
		N64_BUTTON_C_UP | N64_BUTTON_C_DOWN | N64_BUTTON_C_LEFT | N64_BUTTON_C_RIGHT);

	if (both & N64_BUTTON_C_UP) b |= N64_BUTTON_D_UP;
	if (both & N64_BUTTON_C_DOWN) b |= N64_BUTTON_D_DOWN;
	if (both & N64_BUTTON_C_LEFT) b |= N64_BUTTON_L;
	if (both & N64_BUTTON_C_RIGHT) b |= N64_BUTTON_D_LEFT;
	/* (fire and grenades swapped: the moving hand fires) */
	if ((controls->swap_fire ? move : aim)->buttons & N64_BUTTON_Z) b |= N64_BUTTON_Z;
	if (((controls->swap_fire ? aim : move)->buttons & N64_BUTTON_Z) || (both & N64_BUTTON_R)) b |= N64_BUTTON_R;
	if (N64_SCHEME_DUAL_TURNS(controls->scheme))
	{
		gamepad->sThumbLY = stick(move->stick_y);
		gamepad->sThumbRX = stick(move->stick_x);
		gamepad->sThumbLX = stick(aim->stick_x);
	}
	else
	{
		gamepad->sThumbLY = stick(move->stick_y);
		gamepad->sThumbLX = stick(move->stick_x);
		gamepad->sThumbRX = stick(aim->stick_x);
	}
	*look_y = stick(aim->stick_y);
	return b;
}

/* partner_port: the second controller's port (a two-controller player's,
n64_input_partners), or -1 */
static void pad_to_gamepad(int n64_port, const struct n64_pad *pad, int partner_port, XINPUT_GAMEPAD *gamepad)
{
	struct n64_controls *controls = &n64_controls[n64_port & 3];
	int dual = N64_SCHEME_IS_DUAL(controls->scheme) && partner_port >= 0;
	unsigned short b = controls->scheme == N64_SCHEME_DPAD ? swap_c_and_d(pad->buttons) : pad->buttons;
	unsigned short pressed;
	unsigned short jump = N64_BUTTON_A, grenade = N64_BUTTON_R, fire = N64_BUTTON_Z;
	SHORT look_y;
	int crouch;

	golden_pads[n64_port & 3] = *pad;
	if (dual)
	{
		struct n64_pad partner;

		n64_input_get(partner_port & 3, &partner);
		b = dual_pads(controls, n64_port, pad, partner_port & 3, &partner, gamepad, &look_y);
	}
	pressed = b & ~controls->last_buttons;
	if (controls->scheme == N64_SCHEME_GOLDEN_STYLE) grenade = N64_BUTTON_D_LEFT;
	/* (a two-controller scheme without its second controller: the standard
	one) */
	switch (dual ? -1 : controls->scheme)
	{
	case -1:
		break;
	case N64_SCHEME_GOLDEN_STYLE:
	case N64_SCHEME_GOLDENEYE:
		gamepad->sThumbRX = stick(pad->stick_x);
		gamepad->sThumbLY = stick(pad->stick_y);
		gamepad->sThumbLX = c_axis(b, N64_BUTTON_C_RIGHT, N64_BUTTON_C_LEFT);
		look_y = c_axis(b, N64_BUTTON_C_UP, N64_BUTTON_C_DOWN);
		break;
	case N64_SCHEME_NORTH:
		gamepad->sThumbLX = stick(pad->stick_x);
		gamepad->sThumbLY = stick(pad->stick_y);
		gamepad->sThumbRX = c_axis(b, N64_BUTTON_C_RIGHT, N64_BUTTON_C_LEFT);
		look_y = c_axis(b, N64_BUTTON_C_UP, N64_BUTTON_C_DOWN);
		break;
	default:
		gamepad->sThumbRX = stick(pad->stick_x);
		gamepad->sThumbLY = c_axis(b, N64_BUTTON_C_UP, N64_BUTTON_C_DOWN);
		gamepad->sThumbLX = c_axis(b, N64_BUTTON_C_RIGHT, N64_BUTTON_C_LEFT);
		look_y = stick(pad->stick_y);
		break;
	}
	gamepad->sThumbRY = !controls->inverted ? look_y : look_y == -32768 ? 32767 : (SHORT)-look_y;
	if (controls->scheme == N64_SCHEME_BUMPER_JUMPER)
	{
		jump = N64_BUTTON_R;
		grenade = N64_BUTTON_A;
	}
	/* fire and grenades swapped: fire on the grenade's button (two
	controllers: their Zs swapped already, dual_pads) */
	if (controls->swap_fire && !dual)
	{
		fire = grenade;
		grenade = N64_BUTTON_Z;
	}
	/* crouch: held, or toggled by each press (a jump stands back up) */
	if (controls->toggle_crouch)
	{
		if (pressed & N64_BUTTON_D_DOWN)
			controls->crouched = !controls->crouched;
		if (pressed & jump)
			controls->crouched = 0;
		crouch = controls->crouched;
	}
	else
		crouch = (b & N64_BUTTON_D_DOWN) != 0;
	controls->last_buttons = b;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] = (b & fire) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] = (b & grenade) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_A] = (b & jump) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_X] = (b & N64_BUTTON_B) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_B] = (b & N64_BUTTON_L) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_Y] = (b & N64_BUTTON_D_UP) ? 255 : 0;
	{
		unsigned char *held = &d_right_held[n64_port & 3];

		if (b & N64_BUTTON_D_RIGHT)
		{
			if (*held < 255)
				(*held)++;
			if (*held == D_RIGHT_HOLD)
			{
#ifdef HALO_N64_CAMPAIGN
				extern int n64_campaign_speed_requests;

				n64_campaign_speed_requests++;
#else
				gamepad->bAnalogButtons[XINPUT_GAMEPAD_WHITE] = 255;
#endif
			}
		}
		else
		{
			/* (let go before the hold: a tap) */
			if (*held && *held < D_RIGHT_HOLD)
				gamepad->bAnalogButtons[XINPUT_GAMEPAD_BLACK] = 255;
			*held = 0;
		}
	}
	if (crouch)
		gamepad->wButtons |= XINPUT_GAMEPAD_LEFT_THUMB;
	if (controls->scheme != N64_SCHEME_GOLDEN_STYLE && (b & N64_BUTTON_D_LEFT))
		gamepad->wButtons |= XINPUT_GAMEPAD_RIGHT_THUMB;
	if (b & N64_BUTTON_START)
		gamepad->wButtons |= XINPUT_GAMEPAD_START;
}

/* the test script: walk forward, turn now and then, fire in bursts, jump
and throw a grenade occasionally; each port differs */
static void test_gamepad(int port, XINPUT_GAMEPAD *gamepad)
{
	unsigned long seconds_x8 = (unsigned long)(n64_microseconds() / 125000);
	unsigned long phase = seconds_x8 + port * 37;
	if (n64_debug_flag("debug_weapon_showcase"))
	{
		/* Fixed pose, no grenades: inspect the held artwork and a clean burst. */
		gamepad->bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] = ((phase % 64) >= 32 && (phase % 64) < 48) ? 255 : 0;
		gamepad->bAnalogButtons[XINPUT_GAMEPAD_B] = (phase % 64) == 16 ? 255 : 0;
		gamepad->bAnalogButtons[XINPUT_GAMEPAD_X] = (phase % 64) == 56 ? 255 : 0;
		return;
	}

	gamepad->sThumbLY = 26000;
	gamepad->sThumbLX = ((phase / 24) & 1) ? 12000 : -12000;
	gamepad->sThumbRX = ((phase / 16) % 3 == 0) ? 20000 : ((phase / 16) % 3 == 1) ? -8000 : 0;
	gamepad->sThumbRY = ((phase / 40) & 1) ? 3000 : -3000;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] = ((phase % 12) < 5) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_A] = ((phase % 50) == 7) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] = ((phase % 120) == 30) ? 255 : 0;
	gamepad->bAnalogButtons[XINPUT_GAMEPAD_X] = ((phase % 64) == 9) ? 255 : 0;
}

#ifdef HALO_N64_ONLINE
#include "n64_link.h"

/* the online cartridge's controllers, for Halo 64 Link (n64_online.c): an
N64 port's pad as the game's input makes it (its scheme and settings) */
int n64_input_link_pad(int port, struct n64_link_pad *out)
{
	struct n64_pad pad;
	XINPUT_GAMEPAD gamepad;

	poll();
	n64_input_get(port & 3, &pad);
	if (!pad.connected)
		return 0;
	memset(&gamepad, 0, sizeof(gamepad));
	pad_to_gamepad(port & 3, &pad, n64_input_partners[port & 3], &gamepad);
	out->port = (uint8_t)port;
	out->buttons = gamepad.wButtons;
	memcpy(out->analog, gamepad.bAnalogButtons, sizeof(out->analog));
	out->sticks[0] = gamepad.sThumbLX;
	out->sticks[1] = gamepad.sThumbLY;
	out->sticks[2] = gamepad.sThumbRX;
	out->sticks[3] = gamepad.sThumbRY;
	return 1;
}
#endif

DWORD WINAPI XInputGetState(HANDLE device, PXINPUT_STATE state)
{
	int port = controller_port(device);

	memset(state, 0, sizeof(*state));
	if (port < 0)
		return ERROR_DEVICE_NOT_CONNECTED;
	golden_pads[n64_gamepad_ports[port] & 3].connected = 0;
	if (n64_test_input)
	{
		if (port < 4 && !n64_input_blocked)
		{
			if (n64_golden_selected(port))
			{
				unsigned long phase = (unsigned long)(n64_microseconds() / 125000) + port * 7;
				struct n64_pad pad = {0};
				pad.connected = 1;
				pad.stick_x = phase % 64 < 32 ? 35 : 70;
				pad.stick_y = phase % 64 < 32 ? 20 : 0;
				pad.buttons = phase % 64 < 32 ? N64_BUTTON_R : 0;
				if (phase % 16 < 4) pad.buttons |= N64_BUTTON_Z;
				pad_to_gamepad(n64_gamepad_ports[port], &pad, -1, &state->Gamepad);
			}
			else test_gamepad(port, &state->Gamepad);
		}
	}
	else
	{
		struct n64_pad pad;
		int n64_port = n64_gamepad_ports[port] & 3;

		poll();
		n64_input_get(n64_port, &pad);
		if (!pad.connected)
			return ERROR_DEVICE_NOT_CONNECTED;
		/* (a player's second controller reads as untouched: its player's
		gamepad has it) */
		if (!n64_input_blocked && n64_input_partner_of(n64_port) < 0)
			pad_to_gamepad(n64_port, &pad, n64_input_partners[n64_port], &state->Gamepad);
	}
	if (memcmp(&state->Gamepad, &controllers[port].previous, sizeof(state->Gamepad)))
	{
		controllers[port].packet_number++;
		controllers[port].previous = state->Gamepad;
	}
	state->dwPacketNumber = controllers[port].packet_number;
	return ERROR_SUCCESS;
}

DWORD WINAPI XInputSetState(HANDLE device, PXINPUT_FEEDBACK feedback)
{
	int port = controller_port(device);

	if (feedback)
	{
		/* (the scripted players have no motors; none while paused) */
		if (port >= 0 && !n64_test_input)
		{
			int n64_port = n64_gamepad_ports[port] & 3, partner = n64_input_partners[n64_port];
			float left = n64_input_blocked ? 0.f : feedback->Rumble.wLeftMotorSpeed * (1.f / 65535.f);
			float right = n64_input_blocked ? 0.f : feedback->Rumble.wRightMotorSpeed * (1.f / 65535.f);

			/* (a two-controller player's Rumble Paks both, as the player has
			it; a second controller's own gamepad leaves its motor alone) */
			if (n64_input_partner_of(n64_port) < 0)
			{
				n64_input_rumble(n64_port, left, right);
				if (partner >= 0)
					n64_input_rumble(partner, n64_controls[n64_port].no_rumble ? 0.f : left,
						n64_controls[n64_port].no_rumble ? 0.f : right);
			}
		}
		feedback->Header.dwStatus = ERROR_SUCCESS;
	}
	return ERROR_SUCCESS;
}

DWORD WINAPI XInputDebugInitKeyboardQueue(PXINPUT_DEBUG_KEYQUEUE_PARAMETERS parameters)
{
	(void)parameters;
	return ERROR_SUCCESS;
}

DWORD WINAPI XInputDebugGetKeystroke(PXINPUT_DEBUG_KEYSTROKE keystroke)
{
	(void)keystroke;
	return ERROR_HANDLE_EOF;
}

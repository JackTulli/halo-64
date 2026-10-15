/*
N64_INPUT.C

The four controller ports (libdragon's joypad module) for the XDK-facing
gamepad layer (xdk_input.c), and their Rumble Paks.

Halo drives two motors at any speed (its rumble: player_rumble.c, sent
through XInputSetState once a frame); the Rumble Pak's one motor is on or
off. A strength is made of the two speeds (the heavy motor's whole, the
light one's three quarters) and the motor switched on for that share of
the frames, spread evenly: each frame the strength is added up, and the
motor is on in a frame where the sum passes a whole frame's worth -- on
throughout at full strength, a buzz every few frames for a weak one, off
below a floor. Libdragon finds the Rumble Pak itself
(joypad_get_rumble_supported); a player can turn it off on the CONTROLS
page (n64_controls.no_rumble).
*/

#include <libdragon.h>
#include <string.h>

#include "n64.h"

struct n64_controls n64_controls[4];

/* START, A, B and D-pad pressed since n64_input_take_pressed last asked, each port
(the pause menu's: a frame at 10 fps is longer than a quick press) */
static unsigned short pressed_latch[4];

void n64_input_poll(void)
{
	joypad_poll();
	for (int port = 0; port < 4; port++)
	{
		joypad_buttons_t pressed;

		if (!joypad_is_connected(port))
			continue;
		pressed = joypad_get_buttons_pressed(port);
		if (pressed.start) pressed_latch[port] |= N64_BUTTON_START;
		if (pressed.a) pressed_latch[port] |= N64_BUTTON_A;
		if (pressed.b) pressed_latch[port] |= N64_BUTTON_B;
		if (pressed.d_up) pressed_latch[port] |= N64_BUTTON_D_UP;
		if (pressed.d_down) pressed_latch[port] |= N64_BUTTON_D_DOWN;
		if (pressed.d_left) pressed_latch[port] |= N64_BUTTON_D_LEFT;
		if (pressed.d_right) pressed_latch[port] |= N64_BUTTON_D_RIGHT;
	}
}

unsigned short n64_input_take_pressed(int port)
{
	unsigned short pressed = pressed_latch[port & 3];

	pressed_latch[port & 3] = 0;
	return pressed;
}

void n64_input_clear_pressed(void)
{
	memset(pressed_latch, 0, sizeof(pressed_latch));
}

void n64_input_get(int port, struct n64_pad *pad)
{
	joypad_inputs_t inputs;
	unsigned short buttons = 0;

	memset(pad, 0, sizeof(*pad));
	if (!joypad_is_connected(port))
		return;
	inputs = joypad_get_inputs(port);
	pad->connected = 1;
	pad->stick_x = inputs.stick_x;
	pad->stick_y = inputs.stick_y;
	if (inputs.btn.a) buttons |= N64_BUTTON_A;
	if (inputs.btn.b) buttons |= N64_BUTTON_B;
	if (inputs.btn.z) buttons |= N64_BUTTON_Z;
	if (inputs.btn.start) buttons |= N64_BUTTON_START;
	if (inputs.btn.d_up) buttons |= N64_BUTTON_D_UP;
	if (inputs.btn.d_down) buttons |= N64_BUTTON_D_DOWN;
	if (inputs.btn.d_left) buttons |= N64_BUTTON_D_LEFT;
	if (inputs.btn.d_right) buttons |= N64_BUTTON_D_RIGHT;
	if (inputs.btn.l) buttons |= N64_BUTTON_L;
	if (inputs.btn.r) buttons |= N64_BUTTON_R;
	if (inputs.btn.c_up) buttons |= N64_BUTTON_C_UP;
	if (inputs.btn.c_down) buttons |= N64_BUTTON_C_DOWN;
	if (inputs.btn.c_left) buttons |= N64_BUTTON_C_LEFT;
	if (inputs.btn.c_right) buttons |= N64_BUTTON_C_RIGHT;
	pad->buttons = buttons;
}

/* ---------- the Rumble Paks */

/* a strength below this: off (a motor barely turning is felt as nothing) */
#define RUMBLE_FLOOR 0.08f

static struct
{
	float sum;   /* the strength added up, frame by frame */
	int on;      /* as last set */
	int pulse;   /* frames of the menu's buzz still to come */
} rumble[4];

static void rumble_switch(int port, int on)
{
	if (on == rumble[port].on)
		return;
	rumble[port].on = on;
	if (joypad_is_connected(port) && joypad_get_rumble_supported(port))
		joypad_set_rumble_active(port, on != 0);
}

void n64_input_rumble(int port, float left, float right)
{
	float strength;

	if (port < 0 || port >= 4)
		return;
	strength = left > right * 0.75f ? left : right * 0.75f;
	if (n64_controls[port].no_rumble || strength < RUMBLE_FLOOR)
	{
		rumble[port].sum = 0.f;
		rumble_switch(port, 0);
		return;
	}
	rumble[port].sum += strength > 1.f ? 1.f : strength;
	if (rumble[port].sum >= 1.f)
	{
		rumble[port].sum -= 1.f;
		rumble_switch(port, 1);
	}
	else
		rumble_switch(port, 0);
}

int n64_input_rumble_present(int port)
{
	return port >= 0 && port < 4 && joypad_is_connected(port) && joypad_get_rumble_supported(port);
}

void n64_input_rumble_pulse(int port, int frames)
{
	if (port >= 0 && port < 4)
		rumble[port].pulse = frames;
}

/* once a frame outside a game (the menu): every motor off, but for the buzz
a player gets turning it on */
void n64_input_rumble_menu_frame(void)
{
	for (int port = 0; port < 4; port++)
	{
		rumble[port].sum = 0.f;
		rumble_switch(port, rumble[port].pulse > 0);
		if (rumble[port].pulse > 0)
			rumble[port].pulse--;
	}
}

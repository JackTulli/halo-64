/*
N64_INPUT.C

The four controller ports (libdragon's joypad module) for the XDK-facing
gamepad layer (xdk_input.c).
*/

#include <libdragon.h>
#include <string.h>

#include "n64.h"

struct n64_controls n64_controls[4];

void n64_input_poll(void)
{
	joypad_poll();
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

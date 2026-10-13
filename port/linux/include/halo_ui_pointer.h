/*
HALO_UI_POINTER.H

The mouse in the menus of the desktop builds. While a menu is up, the mouse
is released and its pointer shows (port/linux/src/sdl_platform.c); each frame
the menus (source/interface/ui_widget.c) ask where it is, in their own 640x480
coordinates (port/linux/src/d3d8_gl.c undoes the letterbox, the scale and the
widescreen centering), and what it did since.
*/

#ifndef HALO_UI_POINTER_H
#define HALO_UI_POINTER_H

struct halo_ui_pointer
{
	short x, y;
	/* where the latest left click was */
	short click_x, click_y;
	unsigned char moved;
	unsigned char left_clicks;
	unsigned char right_clicks;
	/* whole wheel notches, away from the user positive */
	signed char wheel_steps;
};

/* frees the mouse for the menus while menus_active, and captures it again
for aiming when not; while menus are active returns nonzero and what the
pointer did since the last call. Always 0 on Android. */
int halo_ui_pointer_update(int menus_active, struct halo_ui_pointer *pointer);

#endif

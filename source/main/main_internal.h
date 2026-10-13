/*
MAIN_INTERNAL.H

Narrow cross-translation-unit interface owned by MAIN.C.
*/

#ifndef __MAIN_INTERNAL_H
#define __MAIN_INTERNAL_H
#pragma once

short main_get_window_count(
	void);

/* Engine-internal frame pacing control; defined by MAIN.C. */
extern boolean global_frame_rate_throttle;

#endif /* __MAIN_INTERNAL_H */

/*
SHELL.C
*/

/* ---------- headers */

#include "cseries.h"
#include "shell.h"
#include "errors.h"
#include "tag_files.h"
#include "real_math.h"
#include "game_state.h"
#include "rasterizer.h"
#include "integer_math.h"
#include "input.h"
#include "sound_manager.h"

/* ---------- globals */

static boolean application_paused;

/* ---------- public code */

boolean shell_initialize(
	void)
{
	boolean success = FALSE;
	boolean platform_initialized = FALSE;
	
	cseries_initialize();
	platform_initialized = shell_platform_initialize();
	
	if (platform_initialized)
	{
		boolean rasterizer_initialized;
		
		errors_initialize();
		tag_files_open();
		real_math_initialize();
		game_state_initialize();
		
		rasterizer_initialized = rasterizer_initialize();
		
		if (rasterizer_initialized)
		{
			input_initialize();
			sound_initialize();
			
			success = TRUE;
		}
			
		shell_platform_verify();
	}
	
	return success;
}

void shell_dispose(
	void)
{
	sound_dispose();
	input_dispose();
	rasterizer_dispose();
	real_math_dispose();
	tag_files_close();
	errors_dispose();
	shell_platform_dispose();
	cseries_dispose();

	return;
}

boolean shell_application_is_paused(
	void)
{
	return application_paused;
}

void shell_application_pause(
	boolean paused)
{
	if (application_paused!=paused)
	{
		application_paused = paused;
		shell_screen_pause(paused);
	}

	return;
}

boolean shell_running_import_tool(void)
{
	return FALSE;
}

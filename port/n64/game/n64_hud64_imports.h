/*
N64_HUD64_IMPORTS.H

HUD STYLE 64's module (N64_DSO, tools/n64_build.py) calls these functions
of the game under other names, each a small function of the program's
(n64_hud64_stub.c) that calls the game's own. Each function a module
imports is kept in the program as the module links to it (the externs
linker script), and the link-time optimiser then leaves it, and every call
of the game's to it, as written: no copy specialised for its callers. The
game's object and tag lookups, called all over the game, had such copies;
imported, they cost the program about 2.5 KB with either style. Through
these names the game's own calls stay as they were. (The module's other
imports cost nothing more imported as they are: measured.) Included
first, before cseries.h.
*/

#ifndef N64_HUD64_IMPORTS_H
#define N64_HUD64_IMPORTS_H

#ifdef N64_DSO
#define object_get_and_verify_type n64_hud64_import_object
#define tag_get n64_hud64_import_tag
#endif

#endif

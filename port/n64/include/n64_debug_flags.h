/* The test flags: a file rom:/<name> on the cartridge's file system (port.json
"flags") switches a test feature on, and its first line is its value
(port/n64/src/n64_files.c; the list is in README.md, "Debugging").

A release program (tools/n64_build.py --release, N64_STRIP_TEST_FLAGS) has
none: each use is the value it has when the cartridge holds no such file --
n64_debug_flag 0, n64_debug_text 0 with its buffer left as it was -- so a
release program does what a developer program does on a cartridge without
flag files, and the code a flag guards is left out of it. The functions are
not built into a release program, so a use that this header does not reach
cannot link into the program (in a module it would be an import, which the
module's load refuses: port/n64/src/n64_module.c). */
#ifndef N64_DEBUG_FLAGS_H
#define N64_DEBUG_FLAGS_H

#ifdef N64_STRIP_TEST_FLAGS
#define n64_debug_flag(name) ((void)(name), 0)
#define n64_debug_text(name, buffer, size) ((void)(name), (void)(buffer), (void)(size), 0)
#else
/* TRUE when the cartridge carries the file rom:/<name> (debug switches,
port.json "flags"); looked up once per name */
int n64_debug_flag(const char *name);
/* a debug setting's value (port.json flag "name=value"): 0 when unset */
int n64_debug_text(const char *name, char *buffer, int size);
#endif

#endif

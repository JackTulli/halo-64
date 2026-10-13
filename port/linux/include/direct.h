/*
DIRECT.H

MSVC directory functions.
*/

#ifndef __HALO_LINUX_DIRECT_H
#define __HALO_LINUX_DIRECT_H

int _mkdir(const char *path);
int _rmdir(const char *path);
int _chdir(const char *path);
char *_getcwd(char *buffer, int maximum_length);

#define mkdir(path) _mkdir(path)
#define rmdir _rmdir
#define chdir _chdir
#define getcwd _getcwd

#endif

/*
SYS/STAT.H

MSVC <sys/stat.h> for game code. glibc's struct stat cannot be used here:
the game is built with -malign-double, which changes the layout of its
64-bit members. This is the MSVC structure; posix_files.c fills it in.
*/

#ifndef __HALO_LINUX_SYS_STAT_H
#define __HALO_LINUX_SYS_STAT_H

#include <sys/types.h>
#include <time.h>

/* glibc's <fcntl.h> can define these as struct timespec member paths */
#undef st_atime
#undef st_mtime
#undef st_ctime

struct _stat
{
	unsigned int st_dev;
	unsigned short st_ino;
	unsigned short st_mode;
	short st_nlink;
	short st_uid;
	short st_gid;
	unsigned int st_rdev;
	long st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
};

#define _S_IFMT 0170000
#define _S_IFDIR 0040000
#define _S_IFCHR 0020000
#define _S_IFIFO 0010000
#define _S_IFREG 0100000
#define _S_IREAD 0000400
#define _S_IWRITE 0000200
#define _S_IEXEC 0000100

#ifndef S_IFMT
#define S_IFMT _S_IFMT
#define S_IFDIR _S_IFDIR
#define S_IFCHR _S_IFCHR
#define S_IFREG _S_IFREG
#define S_IREAD _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC _S_IEXEC
#endif

int _stat(const char *path, struct _stat *buffer);
int _fstat(int handle, struct _stat *buffer);

#endif

/*
EXCPT.H

MSVC structured exception handling support for the Linux build.
The __try/__except keywords are handled in halo_linux_prefix.h.
*/

#ifndef __HALO_LINUX_EXCPT_H
#define __HALO_LINUX_EXCPT_H

typedef enum _EXCEPTION_DISPOSITION
{
	ExceptionContinueExecution,
	ExceptionContinueSearch,
	ExceptionNestedException,
	ExceptionCollidedUnwind
} EXCEPTION_DISPOSITION;

#define EXCEPTION_EXECUTE_HANDLER 1
#define EXCEPTION_CONTINUE_SEARCH 0
#define EXCEPTION_CONTINUE_EXECUTION -1

#define GetExceptionCode() 0UL
#define GetExceptionInformation() ((struct _EXCEPTION_POINTERS *)0)
#define AbnormalTermination() 0

#endif

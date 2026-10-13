/*
FEATURES.H

Stands in for musl's internal <features.h>, which the data headers in
../src include for its "hidden" (defined in libm.h). Being first on the
include path, it also stands in front of the C library's own <features.h>
(glibc's headers include it), which it passes on to.
*/

#ifndef __HALO_MUSL_MATH_FEATURES_H
#define __HALO_MUSL_MATH_FEATURES_H

#if defined(__has_include_next)
#if __has_include_next(<features.h>)
#include_next <features.h>
#endif
#endif

#include "libm.h"

#endif

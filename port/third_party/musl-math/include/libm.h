/*
LIBM.H

Stands in for musl's internal src/internal/libm.h for the files in ../src,
which are musl's unchanged; it is force-included ahead of each of them
(-include), so the names below apply to the data files too. The helpers
are musl's (see ../COPYRIGHT); what depends on the platform (long double,
endianness, FMA) is left out, so every port builds the same code.

musl's internal names get a halo_musl_ prefix: the Android port links
musl itself as its C library, whose copies of them must not clash.
*/

#ifndef _LIBM_H
#define _LIBM_H

#include <stdint.h>
#include <float.h>
#include "math.h"

#define __rem_pio2_large halo_musl_rem_pio2_large
#define __rem_pio2 halo_musl_rem_pio2
#define __sin halo_musl_sin
#define __cos halo_musl_cos
#define __tan halo_musl_tan
#define __math_xflow halo_musl_math_xflow
#define __math_uflow halo_musl_math_uflow
#define __math_oflow halo_musl_math_oflow
#define __math_divzero halo_musl_math_divzero
#define __math_invalid halo_musl_math_invalid
#define __exp_data halo_musl_exp_data
#define __log_data halo_musl_log_data
#define __log2_data halo_musl_log2_data
#define __pow_log_data halo_musl_pow_log_data

/* (musl's hidden visibility: nothing here is exported anyway) */
#define hidden

/* Support non-nearest rounding mode.  */
#define WANT_ROUNDING 1
/* Support signaling NaNs.  */
#define WANT_SNAN 0

#define issignalingf_inline(x) 0
#define issignaling_inline(x) 0

#define TOINT_INTRINSICS 0

/* Helps static branch prediction so hot path can be better optimized.  */
#define predict_true(x) __builtin_expect(!!(x), 1)
#define predict_false(x) __builtin_expect(x, 0)

/* Evaluate an expression as the specified type. */
static inline float eval_as_float(float x)
{
	float y = x;
	return y;
}

static inline double eval_as_double(double x)
{
	double y = x;
	return y;
}

/* fp_barrier returns its input, but limits code transformations
   as if it had a side-effect (e.g. observable io) and returned
   an arbitrary value.  */
static inline float fp_barrierf(float x)
{
	volatile float y = x;
	return y;
}

static inline double fp_barrier(double x)
{
	volatile double y = x;
	return y;
}

/* fp_force_eval ensures that the input value is computed when that's
   otherwise unused.  */
static inline void fp_force_evalf(float x)
{
	volatile float y;
	y = x;
}

static inline void fp_force_eval(double x)
{
	volatile double y;
	y = x;
}

#define FORCE_EVAL(x) do {                        \
	if (sizeof(x) == sizeof(float)) {         \
		fp_force_evalf(x);                \
	} else {                                  \
		fp_force_eval(x);                 \
	}                                         \
} while(0)

#define asuint(f) ((union{float _f; uint32_t _i;}){f})._i
#define asfloat(i) ((union{uint32_t _i; float _f;}){i})._f
#define asuint64(f) ((union{double _f; uint64_t _i;}){f})._i
#define asdouble(i) ((union{uint64_t _i; double _f;}){i})._f

#define EXTRACT_WORDS(hi,lo,d)                    \
do {                                              \
  uint64_t __u = asuint64(d);                     \
  (hi) = __u >> 32;                               \
  (lo) = (uint32_t)__u;                           \
} while (0)

#define GET_HIGH_WORD(hi,d)                       \
do {                                              \
  (hi) = asuint64(d) >> 32;                       \
} while (0)

#define GET_LOW_WORD(lo,d)                        \
do {                                              \
  (lo) = (uint32_t)asuint64(d);                   \
} while (0)

#define INSERT_WORDS(d,hi,lo)                     \
do {                                              \
  (d) = asdouble(((uint64_t)(hi)<<32) | (uint32_t)(lo)); \
} while (0)

#define SET_HIGH_WORD(d,hi)                       \
  INSERT_WORDS(d, hi, (uint32_t)asuint64(d))

#define SET_LOW_WORD(d,lo)                        \
  INSERT_WORDS(d, asuint64(d)>>32, lo)

#define GET_FLOAT_WORD(w,d)                       \
do {                                              \
  (w) = asuint(d);                                \
} while (0)

#define SET_FLOAT_WORD(d,w)                       \
do {                                              \
  (d) = asfloat(w);                               \
} while (0)

int __rem_pio2_large(double*,double*,int,int,int);
int __rem_pio2(double,double*);
double __sin(double,double,int);
double __cos(double,double);
double __tan(double,double,int);

/* error handling functions */
double __math_xflow(uint32_t, double);
double __math_uflow(uint32_t);
double __math_oflow(uint32_t);
double __math_divzero(uint32_t);
double __math_invalid(double);

#endif

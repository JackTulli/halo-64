# musl-math

The maths functions the game calls (`sin`, `cos`, `tan`, `asin`, `acos`,
`atan`, `atan2`, `exp`, `log`, `log2`, `log10`, `pow`), from musl libc,
MIT licensed (see `COPYRIGHT`).

Upstream: https://musl.libc.org, release 1.2.5 (the release the Android
port builds its C library from). The files in `src/` are musl's
`src/math/` files, copied unchanged.

System link games run in lockstep, so every machine must compute the same
results to the last bit, and the C libraries' versions of these functions
differ (glibc, the Windows UCRT, Android's musl). Every native port builds
these instead, from the same source with the same flags, as `halo_sin` and
so on (`port/include/halo_math.h`, which the game's `<math.h>` includes).

`include/` holds this port's stand-ins for musl's internal headers
(`libm.h`, `features.h`) and for `<math.h>`: the same types on every port,
no FMA code path, and musl's internal names prefixed `halo_musl_` so they
cannot clash with the Android port's own musl.

Checked: a million inputs per function give bit-identical results built
for i686 with `-march=native` and `-march=x86-64`, x86-64 Linux, x86-64
Windows (UCRT) and ARM64 Android (a Pixel 9 Pro XL), within 1 ULP of glibc
(2 for `log10`).

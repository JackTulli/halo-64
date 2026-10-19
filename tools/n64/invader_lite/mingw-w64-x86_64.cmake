# CMake toolchain for cross-compiling invader-lite (and libsamplerate) to
# 64-bit Windows with Ubuntu's MinGW-w64 (apt install mingw-w64). The "posix"
# compilers use winpthreads for std::thread; everything is linked statically
# so the .exe files need no DLLs.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(INVADER_LITE_MINGW x86_64-w64-mingw32)
set(CMAKE_C_COMPILER ${INVADER_LITE_MINGW}-gcc-posix)
set(CMAKE_CXX_COMPILER ${INVADER_LITE_MINGW}-g++-posix)
set(CMAKE_RC_COMPILER ${INVADER_LITE_MINGW}-windres)
set(CMAKE_AR ${INVADER_LITE_MINGW}-ar CACHE FILEPATH "")
set(CMAKE_RANLIB ${INVADER_LITE_MINGW}-ranlib CACHE FILEPATH "")

# libraries and headers from the MinGW sysroot only (build.sh passes the
# full paths of zlib and libsamplerate); programs, i.e. python3 for Invader's
# code generators, from the build host
set(CMAKE_FIND_ROOT_PATH /usr/${INVADER_LITE_MINGW})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# no DLLs: static libgcc, libstdc++ and winpthread; stripped
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static -static-libgcc -static-libstdc++ -s")

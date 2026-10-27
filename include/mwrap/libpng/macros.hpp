// <mwrap/libpng/macros.hpp> — libpng's macros as macros. Every macro of
// <png.h> also has a mapping in mcpplibs.mwrap.libpng.macros; this header is
// for what a mapping cannot do: `#if` / `#ifdef` tests of the build options
// (PNG_READ_SUPPORTED …), code written against the macros themselves, and
// setjmp (pngconf.h includes <setjmp.h>) for png_jmpbuf callers. Include it
// before `import mcpplibs.mwrap.libpng;`: it is <png.h>, so every macro is
// defined, and while a macro is defined it expands even inside
// `mcpplibs::mwrap::libpng::macros::NAME`.
#pragma once
#include <png.h>

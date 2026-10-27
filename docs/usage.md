# Using mcpplibs.mwrap.libpng

`import mcpplibs.mwrap.libpng;` declares libpng 1.6.43's API as `<png.h>` does. Types, struct tags and functions are the upstream globals, the very same entities as the header's, so a translation unit may include the header too. The library is `compat.libpng` 1.6.43, built over `compat.zlib` 1.3.2 with upstream's prebuilt `pnglibconf.h` (no SIMD code).

## Modules

| Module | What it holds |
|---|---|
| `mcpplibs.mwrap.libpng` | every type (`png_structp`, `png_infop`, `png_image`, `png_color`, `png_text`, the callback types …, 104), struct tag (`png_struct_def`, `png_color_struct` …, 12) and function (`png_create_read_struct`, `png_read_png`, `png_image_finish_read` … 246) png.h declares in this configuration; re-exports the one below |
| `mcpplibs.mwrap.libpng.macros` | the macros, in `mcpplibs::mwrap::libpng::macros`; its partition `:options` holds the build options |

The macros module's functions take the root module's types (`png_image`, `png_uint_32` …): import the root, which re-exports it.

## Macros

A named module cannot export a macro. `mcpplibs::mwrap::libpng::macros` holds them under their original names:

- **Constants** as `inline constexpr`, each of its macro's type: `PNG_COLOR_TYPE_*`, `PNG_COLOR_MASK_*`, `PNG_INTERLACE_*`, `PNG_TRANSFORM_*`, `PNG_INFO_*`, `PNG_FREE_*`, `PNG_FILTER_*`, `PNG_CRC_*`, `PNG_HANDLE_CHUNK_*`, `PNG_ALPHA_*`, `PNG_GAMMA_*`, `PNG_sRGB_INTENT_*`, `PNG_FORMAT_*` and `PNG_FORMAT_FLAG_*`, `PNG_IMAGE_VERSION`, `PNG_IMAGE_FLAG_*`, `PNG_OPTION_*`, `PNG_IO_*`, `PNG_FP_1`, `PNG_UINT_31_MAX`, `PNG_LIBPNG_VER*`, `PNG_LIBPNG_BUILD_*`, and pnglibconf.h's settings (`PNG_ZBUF_SIZE`, `PNG_USER_WIDTH_MAX` …). `PNG_LIBPNG_VER_STRING` and `PNG_HEADER_VERSION_STRING` are `const char[]`.
- **Options**: each `PNG_<NAME>_SUPPORTED` of pnglibconf.h, on or off (177), plus `PNG_READ_16_TO_8_SUPPORTED` and `PNG_USE_READ_MACROS`, is an `inline constexpr bool`, true exactly when the macro is defined where the module is built. Use them with `if constexpr` or `static_assert`.
- **Computations**, `constexpr`, with the macros' results and types: the simplified API's sizes (`PNG_IMAGE_SIZE`, `PNG_IMAGE_ROW_STRIDE`, `PNG_IMAGE_BUFFER_SIZE`, `PNG_IMAGE_PIXEL_*`, `PNG_IMAGE_SAMPLE_*`, `PNG_IMAGE_COLORMAP_SIZE`, `PNG_IMAGE_PNG_SIZE_MAX`, `PNG_IMAGE_FAILED` …, taking a `png_image` or a `png_uint_32` format), the Adam7 helpers (`PNG_PASS_ROWS`, `PNG_ROW_FROM_PASS_ROW`, `PNG_ROW_IN_INTERLACE_PASS` …, templates over the argument types), `PNG_get_uint_32` / `PNG_get_uint_16` / `PNG_get_int_32`, `PNG_ZLIB_MAX_SIZE`, and `png_composite` / `png_composite_16`, which assign to their first argument (a reference here) with the formula png.h selects.
- **Calls**: `png_check_sig`, `png_image_write_get_memory_size` (both references, as the macro takes `&image` and `&size`), `png_jmpbuf` (below), and `png_libpng_ver`, a constant initialised by `png_get_header_ver(NULL)`.

A function evaluates each argument once; the macros evaluate some several times (`image` in `PNG_IMAGE_SIZE`, `alpha` in `png_composite`). The header-mix test compares every function-like macro with its mapping over a grid of arguments, values and types.

`png_get_uint_32`, `png_get_uint_16` and `png_get_int_32` are macros in png.h (`PNG_USE_READ_MACROS`) over functions of the same names. The module exports the library's functions, which give the same results.

### Errors: setjmp and png_jmpbuf

libpng reports an error of the low-level API by calling the error handler, which must not return: the default one, and `png_longjmp`, longjmp to the buffer `png_jmpbuf` registers. `setjmp` is the C library's macro, so include `<csetjmp>` (or the side header) before the imports:

```cpp
#include <csetjmp>
import std;
import mcpplibs.mwrap.libpng;
using namespace mcpplibs::mwrap::libpng::macros;

bool read_header(png_structp png, png_infop info) {
    if (setjmp(png_jmpbuf(png))) return false;  // png_error longjmped here
    png_read_info(png, info);
    return true;
}
```

`macros::png_jmpbuf(png)` does what the macro does: it registers the C library's `longjmp` with `png_set_longjmp_fn` and returns the `jmp_buf&` to pass to `setjmp`. Call `setjmp` in a frame that stays alive while libpng runs. Between the `setjmp` and the `longjmp`, that frame and the frames being unwound must not hold objects with non-trivial destructors, because a longjmp does not run destructors. Keep such frames C-shaped, as in [tests/api_test.cpp](../tests/api_test.cpp) and the default template, and destroy the read or write struct after the jump. The simplified API (`png_image_*`) needs none of this: it catches its own errors and reports them in `png_image::message`.

### The side header

`<mwrap/libpng/macros.hpp>` is `<png.h>`. It is needed for `#if` / `#ifdef` tests of the options, and for code that uses the macros themselves (`png_jmpbuf`, `setjmp`). Include it before the imports. While a macro is defined it expands even inside a qualified name, so `macros::PNG_COLOR_TYPE_RGB` is not reachable there. Use the plain macro, or set it aside with `#pragma push_macro` / `#undef`, as [tests/header_mix_test.cpp](../tests/header_mix_test.cpp) does.

### Not exported

| Name | Why |
|---|---|
| `png_err`, `png_set_strip_error_numbers` | declared by png.h only without `PNG_ERROR_TEXT_SUPPORTED` / with `PNG_ERROR_NUMBERS_SUPPORTED`, neither of which this build has |
| `png_get_io_chunk_name` | `PNG_REMOVED`: not declared at all |
| `PNG_EXPORT`, `PNGAPI`, `PNG_DEPRECATED`, `PNG_RESTRICT`, `PNG_CONST`, the include guards … (29) | declaration machinery and attributes, not API |
| `png_error`, `png_warning`, `png_benign_error`, `PNG_ARM_NEON`, `PNG_IGNORE_ADLER32` … (14) | defined by the headers only in other configurations; the functions are exported |

Deprecated upstream and exported as declared: `png_reset_zstream`, `png_info_init_3`, `png_convert_to_rfc1123`, `png_malloc_default`, `png_free_default`. [tools/check_exports.py](../tools/check_exports.py) accounts for all 492 macros the headers define here.

## Checks

- `mcpp test`: the API through the module (simplified read and write in memory and through stdio, formats, colour maps, negative strides; the low-level writer and reader with custom I/O, ancillary chunks, unknown chunks, transforms, Adam7 row by row, `png_read_png`; gamma and sRGB; progressive reading; user memory functions; the longjmp error path with libpng's and a user's handler), the constants at compile time, and the header mixed with the import with every function-like macro compared with its mapping.
- `python3 tools/check_exports.py`: `src/libpng.cppm` and `src/options.cppm` are what [tools/gen_exports.py](../tools/gen_exports.py) writes from the pinned headers; every declaration is exported and every macro mapped or listed with its reason.
- `python3 tools/run_conformance.py`: libpng's `pngtest`, ported to C++ against the module ([examples/pngtest](../examples/pngtest)), on `pngtest.png`, `contrib/pngsuite`, `contrib/testpngs` and its crashers and bad palettes, as upstream's `tests/pngtest-all` runs them. Every case's exit status and output must equal those of upstream's `pngtest.c`, built from the same tarball against png.h. 96 cases, 167 images passing, every crasher an expected failure.

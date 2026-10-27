# libpng

> libpng, the PNG reference library, through C++23 modules.

**English** · [简体中文](README.zh.md) · [Upstream](https://github.com/pnggroup/libpng) · [Releases](https://github.com/mcpplibs/libpng-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/libpng-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/libpng-m/actions/workflows/ci.yml)

The upstream C API is unchanged: `png_structp`, `png_create_read_struct`,
`png_image`, `png_image_finish_read` are the upstream globals of `<png.h>`.
The library is `compat.libpng` 1.6.43 over `compat.zlib`. This package maps
the header's macros to constants and constexpr functions, and its build
options to `constexpr bool`. libpng's own `pngtest` runs on every CI run, on
a port written against the module, with every outcome equal to upstream's.

## Modules

- `import mcpplibs.mwrap.libpng;`: the API of png.h, re-exporting the one below
- `import mcpplibs.mwrap.libpng.macros;`: `PNG_COLOR_TYPE_RGB`, `PNG_FORMAT_RGBA`, `PNG_IMAGE_SIZE`, `png_jmpbuf`, `PNG_READ_SUPPORTED` … in `mcpplibs::mwrap::libpng::macros`

> More Modules API -> [Usage and API](docs/usage.md)

## Getting started

```sh
mcpp new pictures --template mcpplibs.mwrap.libpng@1.6.43
cd pictures
mcpp run
```

The default template writes an RGBA gradient with the simplified API, reads
it back pixel for pixel, and describes it with the low-level reader;
`mcpp run -- photo.png` describes any PNG.

```toml
[dependencies]
"mcpplibs.mwrap.libpng" = "1.6.43"
```

```cpp
import std;
import mcpplibs.mwrap.libpng;
using namespace mcpplibs::mwrap::libpng::macros;  // PNG_IMAGE_VERSION, PNG_FORMAT_RGBA, PNG_IMAGE_SIZE …

std::vector<png_byte> decode(std::span<const png_byte> file) {
    png_image image{};
    image.version = PNG_IMAGE_VERSION;
    if (!png_image_begin_read_from_memory(&image, file.data(), file.size())) return {};
    image.format = PNG_FORMAT_RGBA;
    std::vector<png_byte> rgba(PNG_IMAGE_SIZE(image));
    if (!png_image_finish_read(&image, nullptr, rgba.data(), 0, nullptr)) return {};
    return rgba;
}
```

## Macros and errors

Constants keep their macros' types (`PNG_INFO_eXIf` is `unsigned`,
`PNG_FP_MAX` a `png_fixed_point`). Computations are constexpr functions
with the macros' results: `PNG_IMAGE_*` over a `png_image`, the Adam7
`PNG_PASS_*` helpers, `PNG_get_uint_32`, `png_composite`. Each
`PNG_<NAME>_SUPPORTED` option is a `constexpr bool` for `if constexpr`.

The low-level API reports errors by longjmp. `setjmp` is the C library's
macro, so include `<csetjmp>` before the imports, then write
`if (setjmp(png_jmpbuf(png))) { … }` as libpng documents; `png_jmpbuf` is
the module's function. The simplified API reports errors in
`png_image::message` instead. `<mwrap/libpng/macros.hpp>` (`<png.h>`) is
for `#if` tests of the options and for code using the macros themselves;
include it before the imports. Details: [docs/usage.md](docs/usage.md).

## Examples

- [examples/pngtest](examples/pngtest): libpng's `pngtest.c`, ported to C++ against the module;
  [tools/run_conformance.py](tools/run_conformance.py) runs it on the release's test images.

## Build and testing

Linux, macOS and Windows CI build the modules and run the tests, the export
audit ([tools/check_exports.py](tools/check_exports.py): the generated surface is
fresh, every declaration exported, every macro mapped or listed with its reason)
and the conformance run. That run uses `pngtest.png`, `contrib/pngsuite`,
`contrib/testpngs`, the crashers and the bad palettes, and checks each outcome
against upstream's `pngtest.c` built from the same tarball. A separate job
scaffolds the default template as an installed package and runs it.

## License

[Apache-2.0](LICENSE) for the wrapper, by mcpp-community ([NOTICE](NOTICE)). libpng
remains under the PNG Reference Library License version 2
([upstream/libpng/LICENSE](upstream/libpng/LICENSE), [README](upstream/libpng/README)).

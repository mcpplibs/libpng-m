# libpng

> 通过 C++23 模块使用 libpng（PNG 参考库）。

[English](README.md) · **简体中文** · [上游](https://github.com/pnggroup/libpng) · [发布](https://github.com/mcpplibs/libpng-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [索引](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/libpng-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/libpng-m/actions/workflows/ci.yml)

上游的 C API 保持不变：`png_structp`、`png_create_read_struct`、`png_image`、
`png_image_finish_read` 就是 `<png.h>` 里的上游全局名字。库本身是
`compat.libpng` 1.6.43（基于 `compat.zlib`）。本包把头文件里的宏映射成常量和
constexpr 函数，把构建选项映射成 `constexpr bool`。每次 CI 都会在一个只用本模块
改写的移植版上运行 libpng 自带的 `pngtest`，每个结果都必须与上游一致。

## 模块

- `import mcpplibs.mwrap.libpng;`：png.h 的 API，并重新导出下面这个模块
- `import mcpplibs.mwrap.libpng.macros;`：`PNG_COLOR_TYPE_RGB`、`PNG_FORMAT_RGBA`、`PNG_IMAGE_SIZE`、`png_jmpbuf`、`PNG_READ_SUPPORTED` 等，位于 `mcpplibs::mwrap::libpng::macros`

> 更多模块 API -> [用法和 API](docs/usage.md)

## 快速开始

```sh
mcpp new pictures --template mcpplibs.mwrap.libpng@1.6.43
cd pictures
mcpp run
```

默认模板用简化 API 写出一幅 RGBA 渐变图，再逐像素读回，并用底层读取器描述它；
`mcpp run -- photo.png` 可以描述任意 PNG。

```toml
[dependencies]
"mcpplibs.mwrap.libpng" = "1.6.43"
```

```cpp
import std;
import mcpplibs.mwrap.libpng;
using namespace mcpplibs::mwrap::libpng::macros;  // PNG_IMAGE_VERSION、PNG_FORMAT_RGBA、PNG_IMAGE_SIZE 等

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

## 宏与错误处理

常量保留宏原来的类型（`PNG_INFO_eXIf` 是 `unsigned`，`PNG_FP_MAX` 是
`png_fixed_point`）。计算类的宏是结果与宏一致的 constexpr 函数：作用于
`png_image` 的 `PNG_IMAGE_*`、Adam7 的 `PNG_PASS_*` 辅助宏、`PNG_get_uint_32`、
`png_composite`。每个 `PNG_<NAME>_SUPPORTED` 选项都是可用于 `if constexpr`
的 `constexpr bool`。

底层 API 通过 longjmp 报告错误。`setjmp` 是 C 库的宏，所以要在 import 之前
包含 `<csetjmp>`，然后按 libpng 文档写 `if (setjmp(png_jmpbuf(png))) { … }`；
其中 `png_jmpbuf` 是模块提供的函数。简化 API 则把错误写进
`png_image::message`。`<mwrap/libpng/macros.hpp>`（即 `<png.h>`）用于在
`#if` 中测试选项，以及直接使用宏的代码；请在 import 之前包含它。
详见 [docs/usage.md](docs/usage.md)。

## 示例

- [examples/pngtest](examples/pngtest)：把 libpng 的 `pngtest.c` 移植到 C++，只使用本模块；
  [tools/run_conformance.py](tools/run_conformance.py) 用发布包里的测试图片运行它。

## 构建与测试

Linux、macOS 和 Windows 的 CI 会构建模块，并运行测试、导出审计
（[tools/check_exports.py](tools/check_exports.py)：生成的接口是最新的，每个声明
都已导出，每个宏都已映射或注明原因）和一致性测试。一致性测试使用 `pngtest.png`、
`contrib/pngsuite`、`contrib/testpngs`、crashers 和 bad palette 图片，并把每个结果
与同一 tarball 构建出的上游 `pngtest.c` 逐一比较。另一个任务把默认模板当作已安装
的包生成出来并运行。

## 许可证

封装部分采用 [Apache-2.0](LICENSE)，作者为 mcpp-community（[NOTICE](NOTICE)）。
libpng 仍采用 PNG Reference Library License version 2
（[upstream/libpng/LICENSE](upstream/libpng/LICENSE)、[README](upstream/libpng/README)）。

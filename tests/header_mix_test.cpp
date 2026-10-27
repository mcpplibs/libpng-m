// The raw header and the import in one translation unit (the side header
// first, the canonical order): the module's names are the very same
// entities; png.h's macros work beside them, png_jmpbuf and setjmp in their
// macro form included; the header's own values equal the mapped constants.
// While a header's macro is defined it expands even inside a qualified name,
// so `macros::PNG_COLOR_TYPE_RGB` is only reachable with the macro set aside.
#include <mwrap/libpng/macros.hpp>
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.libpng;

namespace {
constexpr int raw_rgb = PNG_COLOR_TYPE_RGB;
constexpr auto raw_rgba_format = PNG_FORMAT_RGBA;
constexpr png_uint_32 raw_size = PNG_IMAGE_SIZE((png_image{nullptr, 1, 3, 2, PNG_FORMAT_RGBA, 0, 0, 0, {}}));
}  // namespace

#pragma push_macro("PNG_COLOR_TYPE_RGB")
#pragma push_macro("PNG_FORMAT_RGBA")
#pragma push_macro("PNG_IMAGE_SIZE")
#undef PNG_COLOR_TYPE_RGB
#undef PNG_FORMAT_RGBA
#undef PNG_IMAGE_SIZE
static_assert(mcpplibs::mwrap::libpng::macros::PNG_COLOR_TYPE_RGB == raw_rgb);
static_assert(mcpplibs::mwrap::libpng::macros::PNG_FORMAT_RGBA == raw_rgba_format);
static_assert(mcpplibs::mwrap::libpng::macros::PNG_IMAGE_SIZE(png_image{nullptr, 1, 3, 2, raw_rgba_format, 0, 0, 0, {}}) == raw_size);
#pragma pop_macro("PNG_IMAGE_SIZE")
#pragma pop_macro("PNG_FORMAT_RGBA")
#pragma pop_macro("PNG_COLOR_TYPE_RGB")

#ifndef PNG_SIMPLIFIED_READ_SUPPORTED  // the options as #if tests: the side header's purpose
#error "png.h's options are macros here"
#endif

// Every function-like macro of png.h against its mapping, over a grid of
// arguments: the header's expansions here, the module's functions below.
namespace by_macro {
auto w_PNG_IMAGE_SAMPLE_CHANNELS(png_uint_32 f) { return PNG_IMAGE_SAMPLE_CHANNELS(f); }
auto w_PNG_IMAGE_SAMPLE_COMPONENT_SIZE(png_uint_32 f) { return PNG_IMAGE_SAMPLE_COMPONENT_SIZE(f); }
auto w_PNG_IMAGE_SAMPLE_SIZE(png_uint_32 f) { return PNG_IMAGE_SAMPLE_SIZE(f); }
auto w_PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(png_uint_32 f) { return PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(f); }
auto w_PNG_IMAGE_PIXEL_CHANNELS(png_uint_32 f) { return PNG_IMAGE_PIXEL_CHANNELS(f); }
auto w_PNG_IMAGE_PIXEL_COMPONENT_SIZE(png_uint_32 f) { return PNG_IMAGE_PIXEL_COMPONENT_SIZE(f); }
auto w_PNG_IMAGE_PIXEL_SIZE(png_uint_32 f) { return PNG_IMAGE_PIXEL_SIZE(f); }
auto w_PNG_IMAGE_ROW_STRIDE(const png_image& i) { return PNG_IMAGE_ROW_STRIDE(i); }
auto w_PNG_IMAGE_SIZE(const png_image& i) { return PNG_IMAGE_SIZE(i); }
auto w_PNG_IMAGE_COLORMAP_SIZE(const png_image& i) { return PNG_IMAGE_COLORMAP_SIZE(i); }
auto w_PNG_IMAGE_DATA_SIZE(const png_image& i) { return PNG_IMAGE_DATA_SIZE(i); }
auto w_PNG_IMAGE_COMPRESSED_SIZE_MAX(const png_image& i) { return PNG_IMAGE_COMPRESSED_SIZE_MAX(i); }
auto w_PNG_IMAGE_PNG_SIZE_MAX(const png_image& i) { return PNG_IMAGE_PNG_SIZE_MAX(i); }
auto w_PNG_IMAGE_FAILED(const png_image& i) { return PNG_IMAGE_FAILED(i); }
auto w_PNG_PASS_START_ROW(int p) { return PNG_PASS_START_ROW(p); }
auto w_PNG_PASS_START_COL(int p) { return PNG_PASS_START_COL(p); }
auto w_PNG_PASS_ROW_OFFSET(int p) { return PNG_PASS_ROW_OFFSET(p); }
auto w_PNG_PASS_COL_OFFSET(int p) { return PNG_PASS_COL_OFFSET(p); }
auto w_PNG_PASS_ROW_SHIFT(int p) { return PNG_PASS_ROW_SHIFT(p); }
auto w_PNG_PASS_COL_SHIFT(int p) { return PNG_PASS_COL_SHIFT(p); }
auto w_PNG_PASS_ROWS(png_uint_32 a, int p) { return PNG_PASS_ROWS(a, p); }
auto w_PNG_PASS_COLS(png_uint_32 a, int p) { return PNG_PASS_COLS(a, p); }
auto w_PNG_ROW_FROM_PASS_ROW(png_uint_32 a, int p) { return PNG_ROW_FROM_PASS_ROW(a, p); }
auto w_PNG_COL_FROM_PASS_COL(png_uint_32 a, int p) { return PNG_COL_FROM_PASS_COL(a, p); }
auto w_PNG_ROW_IN_INTERLACE_PASS(png_uint_32 a, int p) { return PNG_ROW_IN_INTERLACE_PASS(a, p); }
auto w_PNG_COL_IN_INTERLACE_PASS(png_uint_32 a, int p) { return PNG_COL_IN_INTERLACE_PASS(a, p); }
auto w_PNG_PASS_MASK(png_uint_32 a, int p) { return PNG_PASS_MASK(a, p); }
auto w_PNG_IMAGE_BUFFER_SIZE(const png_image& i, png_uint_32 s) { return PNG_IMAGE_BUFFER_SIZE(i, s); }
auto w_PNG_ZLIB_MAX_SIZE(png_alloc_size_t b) { return PNG_ZLIB_MAX_SIZE(b); }
auto w_PNG_get_uint_32(png_const_bytep b) { return PNG_get_uint_32(b); }
auto w_PNG_get_uint_16(png_const_bytep b) { return PNG_get_uint_16(b); }
auto w_PNG_get_int_32(png_const_bytep b) { return PNG_get_int_32(b); }
png_byte w_png_composite(png_byte f, png_byte a, png_byte b) { png_byte c; png_composite(c, f, a, b); return c; }
png_uint_16 w_png_composite_16(png_uint_16 f, png_uint_16 a, png_uint_16 b) { png_uint_16 c; png_composite_16(c, f, a, b); return c; }
}  // namespace by_macro

#pragma push_macro("PNG_IMAGE_SAMPLE_CHANNELS")
#pragma push_macro("PNG_IMAGE_SAMPLE_COMPONENT_SIZE")
#pragma push_macro("PNG_IMAGE_SAMPLE_SIZE")
#pragma push_macro("PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS")
#pragma push_macro("PNG_IMAGE_PIXEL_CHANNELS")
#pragma push_macro("PNG_IMAGE_PIXEL_COMPONENT_SIZE")
#pragma push_macro("PNG_IMAGE_PIXEL_SIZE")
#pragma push_macro("PNG_IMAGE_ROW_STRIDE")
#pragma push_macro("PNG_IMAGE_SIZE")
#pragma push_macro("PNG_IMAGE_COLORMAP_SIZE")
#pragma push_macro("PNG_IMAGE_DATA_SIZE")
#pragma push_macro("PNG_IMAGE_COMPRESSED_SIZE_MAX")
#pragma push_macro("PNG_IMAGE_PNG_SIZE_MAX")
#pragma push_macro("PNG_IMAGE_FAILED")
#pragma push_macro("PNG_PASS_START_ROW")
#pragma push_macro("PNG_PASS_START_COL")
#pragma push_macro("PNG_PASS_ROW_OFFSET")
#pragma push_macro("PNG_PASS_COL_OFFSET")
#pragma push_macro("PNG_PASS_ROW_SHIFT")
#pragma push_macro("PNG_PASS_COL_SHIFT")
#pragma push_macro("PNG_PASS_ROWS")
#pragma push_macro("PNG_PASS_COLS")
#pragma push_macro("PNG_ROW_FROM_PASS_ROW")
#pragma push_macro("PNG_COL_FROM_PASS_COL")
#pragma push_macro("PNG_ROW_IN_INTERLACE_PASS")
#pragma push_macro("PNG_COL_IN_INTERLACE_PASS")
#pragma push_macro("PNG_PASS_MASK")
#pragma push_macro("PNG_IMAGE_BUFFER_SIZE")
#pragma push_macro("PNG_ZLIB_MAX_SIZE")
#pragma push_macro("PNG_get_uint_32")
#pragma push_macro("PNG_get_uint_16")
#pragma push_macro("PNG_get_int_32")
#pragma push_macro("png_composite")
#pragma push_macro("png_composite_16")
#undef PNG_IMAGE_SAMPLE_CHANNELS
#undef PNG_IMAGE_SAMPLE_COMPONENT_SIZE
#undef PNG_IMAGE_SAMPLE_SIZE
#undef PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS
#undef PNG_IMAGE_PIXEL_CHANNELS
#undef PNG_IMAGE_PIXEL_COMPONENT_SIZE
#undef PNG_IMAGE_PIXEL_SIZE
#undef PNG_IMAGE_ROW_STRIDE
#undef PNG_IMAGE_SIZE
#undef PNG_IMAGE_COLORMAP_SIZE
#undef PNG_IMAGE_DATA_SIZE
#undef PNG_IMAGE_COMPRESSED_SIZE_MAX
#undef PNG_IMAGE_PNG_SIZE_MAX
#undef PNG_IMAGE_FAILED
#undef PNG_PASS_START_ROW
#undef PNG_PASS_START_COL
#undef PNG_PASS_ROW_OFFSET
#undef PNG_PASS_COL_OFFSET
#undef PNG_PASS_ROW_SHIFT
#undef PNG_PASS_COL_SHIFT
#undef PNG_PASS_ROWS
#undef PNG_PASS_COLS
#undef PNG_ROW_FROM_PASS_ROW
#undef PNG_COL_FROM_PASS_COL
#undef PNG_ROW_IN_INTERLACE_PASS
#undef PNG_COL_IN_INTERLACE_PASS
#undef PNG_PASS_MASK
#undef PNG_IMAGE_BUFFER_SIZE
#undef PNG_ZLIB_MAX_SIZE
#undef PNG_get_uint_32
#undef PNG_get_uint_16
#undef PNG_get_int_32
#undef png_composite
#undef png_composite_16
namespace by_module {
using namespace mcpplibs::mwrap::libpng::macros;
auto w_PNG_IMAGE_SAMPLE_CHANNELS(png_uint_32 f) { return PNG_IMAGE_SAMPLE_CHANNELS(f); }
auto w_PNG_IMAGE_SAMPLE_COMPONENT_SIZE(png_uint_32 f) { return PNG_IMAGE_SAMPLE_COMPONENT_SIZE(f); }
auto w_PNG_IMAGE_SAMPLE_SIZE(png_uint_32 f) { return PNG_IMAGE_SAMPLE_SIZE(f); }
auto w_PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(png_uint_32 f) { return PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(f); }
auto w_PNG_IMAGE_PIXEL_CHANNELS(png_uint_32 f) { return PNG_IMAGE_PIXEL_CHANNELS(f); }
auto w_PNG_IMAGE_PIXEL_COMPONENT_SIZE(png_uint_32 f) { return PNG_IMAGE_PIXEL_COMPONENT_SIZE(f); }
auto w_PNG_IMAGE_PIXEL_SIZE(png_uint_32 f) { return PNG_IMAGE_PIXEL_SIZE(f); }
auto w_PNG_IMAGE_ROW_STRIDE(const png_image& i) { return PNG_IMAGE_ROW_STRIDE(i); }
auto w_PNG_IMAGE_SIZE(const png_image& i) { return PNG_IMAGE_SIZE(i); }
auto w_PNG_IMAGE_COLORMAP_SIZE(const png_image& i) { return PNG_IMAGE_COLORMAP_SIZE(i); }
auto w_PNG_IMAGE_DATA_SIZE(const png_image& i) { return PNG_IMAGE_DATA_SIZE(i); }
auto w_PNG_IMAGE_COMPRESSED_SIZE_MAX(const png_image& i) { return PNG_IMAGE_COMPRESSED_SIZE_MAX(i); }
auto w_PNG_IMAGE_PNG_SIZE_MAX(const png_image& i) { return PNG_IMAGE_PNG_SIZE_MAX(i); }
auto w_PNG_IMAGE_FAILED(const png_image& i) { return PNG_IMAGE_FAILED(i); }
auto w_PNG_PASS_START_ROW(int p) { return PNG_PASS_START_ROW(p); }
auto w_PNG_PASS_START_COL(int p) { return PNG_PASS_START_COL(p); }
auto w_PNG_PASS_ROW_OFFSET(int p) { return PNG_PASS_ROW_OFFSET(p); }
auto w_PNG_PASS_COL_OFFSET(int p) { return PNG_PASS_COL_OFFSET(p); }
auto w_PNG_PASS_ROW_SHIFT(int p) { return PNG_PASS_ROW_SHIFT(p); }
auto w_PNG_PASS_COL_SHIFT(int p) { return PNG_PASS_COL_SHIFT(p); }
auto w_PNG_PASS_ROWS(png_uint_32 a, int p) { return PNG_PASS_ROWS(a, p); }
auto w_PNG_PASS_COLS(png_uint_32 a, int p) { return PNG_PASS_COLS(a, p); }
auto w_PNG_ROW_FROM_PASS_ROW(png_uint_32 a, int p) { return PNG_ROW_FROM_PASS_ROW(a, p); }
auto w_PNG_COL_FROM_PASS_COL(png_uint_32 a, int p) { return PNG_COL_FROM_PASS_COL(a, p); }
auto w_PNG_ROW_IN_INTERLACE_PASS(png_uint_32 a, int p) { return PNG_ROW_IN_INTERLACE_PASS(a, p); }
auto w_PNG_COL_IN_INTERLACE_PASS(png_uint_32 a, int p) { return PNG_COL_IN_INTERLACE_PASS(a, p); }
auto w_PNG_PASS_MASK(png_uint_32 a, int p) { return PNG_PASS_MASK(a, p); }
auto w_PNG_IMAGE_BUFFER_SIZE(const png_image& i, png_uint_32 s) { return PNG_IMAGE_BUFFER_SIZE(i, s); }
auto w_PNG_ZLIB_MAX_SIZE(png_alloc_size_t b) { return PNG_ZLIB_MAX_SIZE(b); }
auto w_PNG_get_uint_32(png_const_bytep b) { return PNG_get_uint_32(b); }
auto w_PNG_get_uint_16(png_const_bytep b) { return PNG_get_uint_16(b); }
auto w_PNG_get_int_32(png_const_bytep b) { return PNG_get_int_32(b); }
png_byte w_png_composite(png_byte f, png_byte a, png_byte b) { png_byte c; png_composite(c, f, a, b); return c; }
png_uint_16 w_png_composite_16(png_uint_16 f, png_uint_16 a, png_uint_16 b) { png_uint_16 c; png_composite_16(c, f, a, b); return c; }
}  // namespace by_module
#pragma pop_macro("png_composite_16")
#pragma pop_macro("png_composite")
#pragma pop_macro("PNG_get_int_32")
#pragma pop_macro("PNG_get_uint_16")
#pragma pop_macro("PNG_get_uint_32")
#pragma pop_macro("PNG_ZLIB_MAX_SIZE")
#pragma pop_macro("PNG_IMAGE_BUFFER_SIZE")
#pragma pop_macro("PNG_PASS_MASK")
#pragma pop_macro("PNG_COL_IN_INTERLACE_PASS")
#pragma pop_macro("PNG_ROW_IN_INTERLACE_PASS")
#pragma pop_macro("PNG_COL_FROM_PASS_COL")
#pragma pop_macro("PNG_ROW_FROM_PASS_ROW")
#pragma pop_macro("PNG_PASS_COLS")
#pragma pop_macro("PNG_PASS_ROWS")
#pragma pop_macro("PNG_PASS_COL_SHIFT")
#pragma pop_macro("PNG_PASS_ROW_SHIFT")
#pragma pop_macro("PNG_PASS_COL_OFFSET")
#pragma pop_macro("PNG_PASS_ROW_OFFSET")
#pragma pop_macro("PNG_PASS_START_COL")
#pragma pop_macro("PNG_PASS_START_ROW")
#pragma pop_macro("PNG_IMAGE_FAILED")
#pragma pop_macro("PNG_IMAGE_PNG_SIZE_MAX")
#pragma pop_macro("PNG_IMAGE_COMPRESSED_SIZE_MAX")
#pragma pop_macro("PNG_IMAGE_DATA_SIZE")
#pragma pop_macro("PNG_IMAGE_COLORMAP_SIZE")
#pragma pop_macro("PNG_IMAGE_SIZE")
#pragma pop_macro("PNG_IMAGE_ROW_STRIDE")
#pragma pop_macro("PNG_IMAGE_PIXEL_SIZE")
#pragma pop_macro("PNG_IMAGE_PIXEL_COMPONENT_SIZE")
#pragma pop_macro("PNG_IMAGE_PIXEL_CHANNELS")
#pragma pop_macro("PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS")
#pragma pop_macro("PNG_IMAGE_SAMPLE_SIZE")
#pragma pop_macro("PNG_IMAGE_SAMPLE_COMPONENT_SIZE")
#pragma pop_macro("PNG_IMAGE_SAMPLE_CHANNELS")

namespace {
// The macro png_jmpbuf (png.h's) and the C library's setjmp, in their header
// form: libpng longjmps here from its error handler.
int read_header_or_fail(png_structp png, png_infop info) {
    if (setjmp(png_jmpbuf(png))) return 1;
    png_read_info(png, info);
    return 0;
}
}  // namespace

TEST(HeaderMix, SameEntitiesAndValues) {
    static_assert(std::is_same_v<decltype(&::png_create_read_struct), png_structp (*)(png_const_charp, png_voidp, png_error_ptr, png_error_ptr)>);
    static_assert(std::is_same_v<png_image, ::png_image> && std::is_same_v<png_struct, ::png_struct_def>);
    // The library's version, read through the header's macro and the module's constant.
    EXPECT_STREQ(png_libpng_ver, PNG_LIBPNG_VER_STRING);  // the header's: a call
    // The error path in its macro form: a truncated signature, png_error, longjmp.
    const png_byte not_png[] = {137, 'P', 'N', 'G', 13, 10, 26, 10, 0, 0, 0, 13, 'I', 'H', 'D', 'R', 0};
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    struct Source { const png_byte* p; std::size_t n; } source{not_png, sizeof not_png};
    png_set_read_fn(png, &source, [](png_structp p, png_bytep out, size_t n) {
        auto* s = static_cast<Source*>(png_get_io_ptr(p));
        if (n > s->n) png_error(p, "truncated");
        std::memcpy(out, s->p, n);
        s->p += n;
        s->n -= n;
    });
    EXPECT_EQ(read_header_or_fail(png, info), 1);
    png_destroy_read_struct(&png, &info, nullptr);
}

TEST(HeaderMix, EveryFunctionLikeMacroAgreesWithItsMapping) {
    std::mt19937 random(1643);
    const png_uint_32 formats[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0x10, 0x13, 0x20, 0x23, 0x33, 0x3b, 0x40, 0x41, 0x7f};
    for (png_uint_32 f : formats) {
        EXPECT_EQ(by_macro::w_PNG_IMAGE_SAMPLE_CHANNELS(f), by_module::w_PNG_IMAGE_SAMPLE_CHANNELS(f)) << "PNG_IMAGE_SAMPLE_CHANNELS " << f;
        static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_SAMPLE_CHANNELS(f)), decltype(by_module::w_PNG_IMAGE_SAMPLE_CHANNELS(f))>);
        EXPECT_EQ(by_macro::w_PNG_IMAGE_SAMPLE_COMPONENT_SIZE(f), by_module::w_PNG_IMAGE_SAMPLE_COMPONENT_SIZE(f)) << "PNG_IMAGE_SAMPLE_COMPONENT_SIZE " << f;
        static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_SAMPLE_COMPONENT_SIZE(f)), decltype(by_module::w_PNG_IMAGE_SAMPLE_COMPONENT_SIZE(f))>);
        EXPECT_EQ(by_macro::w_PNG_IMAGE_SAMPLE_SIZE(f), by_module::w_PNG_IMAGE_SAMPLE_SIZE(f)) << "PNG_IMAGE_SAMPLE_SIZE " << f;
        static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_SAMPLE_SIZE(f)), decltype(by_module::w_PNG_IMAGE_SAMPLE_SIZE(f))>);
        EXPECT_EQ(by_macro::w_PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(f), by_module::w_PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(f)) << "PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS " << f;
        static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(f)), decltype(by_module::w_PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(f))>);
        EXPECT_EQ(by_macro::w_PNG_IMAGE_PIXEL_CHANNELS(f), by_module::w_PNG_IMAGE_PIXEL_CHANNELS(f)) << "PNG_IMAGE_PIXEL_CHANNELS " << f;
        static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_PIXEL_CHANNELS(f)), decltype(by_module::w_PNG_IMAGE_PIXEL_CHANNELS(f))>);
        EXPECT_EQ(by_macro::w_PNG_IMAGE_PIXEL_COMPONENT_SIZE(f), by_module::w_PNG_IMAGE_PIXEL_COMPONENT_SIZE(f)) << "PNG_IMAGE_PIXEL_COMPONENT_SIZE " << f;
        static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_PIXEL_COMPONENT_SIZE(f)), decltype(by_module::w_PNG_IMAGE_PIXEL_COMPONENT_SIZE(f))>);
        EXPECT_EQ(by_macro::w_PNG_IMAGE_PIXEL_SIZE(f), by_module::w_PNG_IMAGE_PIXEL_SIZE(f)) << "PNG_IMAGE_PIXEL_SIZE " << f;
        static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_PIXEL_SIZE(f)), decltype(by_module::w_PNG_IMAGE_PIXEL_SIZE(f))>);
        for (int n = 0; n < 50; ++n) {
            png_image i{};
            i.format = f;
            i.width = random() % 5000;
            i.height = random() % 5000;
            i.colormap_entries = random() % 257;
            i.warning_or_error = random() % 4;
            EXPECT_EQ(by_macro::w_PNG_IMAGE_ROW_STRIDE(i), by_module::w_PNG_IMAGE_ROW_STRIDE(i)) << "PNG_IMAGE_ROW_STRIDE";
            static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_ROW_STRIDE(i)), decltype(by_module::w_PNG_IMAGE_ROW_STRIDE(i))>);
            EXPECT_EQ(by_macro::w_PNG_IMAGE_SIZE(i), by_module::w_PNG_IMAGE_SIZE(i)) << "PNG_IMAGE_SIZE";
            static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_SIZE(i)), decltype(by_module::w_PNG_IMAGE_SIZE(i))>);
            EXPECT_EQ(by_macro::w_PNG_IMAGE_COLORMAP_SIZE(i), by_module::w_PNG_IMAGE_COLORMAP_SIZE(i)) << "PNG_IMAGE_COLORMAP_SIZE";
            static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_COLORMAP_SIZE(i)), decltype(by_module::w_PNG_IMAGE_COLORMAP_SIZE(i))>);
            EXPECT_EQ(by_macro::w_PNG_IMAGE_DATA_SIZE(i), by_module::w_PNG_IMAGE_DATA_SIZE(i)) << "PNG_IMAGE_DATA_SIZE";
            static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_DATA_SIZE(i)), decltype(by_module::w_PNG_IMAGE_DATA_SIZE(i))>);
            EXPECT_EQ(by_macro::w_PNG_IMAGE_COMPRESSED_SIZE_MAX(i), by_module::w_PNG_IMAGE_COMPRESSED_SIZE_MAX(i)) << "PNG_IMAGE_COMPRESSED_SIZE_MAX";
            static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_COMPRESSED_SIZE_MAX(i)), decltype(by_module::w_PNG_IMAGE_COMPRESSED_SIZE_MAX(i))>);
            EXPECT_EQ(by_macro::w_PNG_IMAGE_PNG_SIZE_MAX(i), by_module::w_PNG_IMAGE_PNG_SIZE_MAX(i)) << "PNG_IMAGE_PNG_SIZE_MAX";
            static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_PNG_SIZE_MAX(i)), decltype(by_module::w_PNG_IMAGE_PNG_SIZE_MAX(i))>);
            EXPECT_EQ(by_macro::w_PNG_IMAGE_FAILED(i), by_module::w_PNG_IMAGE_FAILED(i)) << "PNG_IMAGE_FAILED";
            static_assert(std::is_same_v<decltype(by_macro::w_PNG_IMAGE_FAILED(i)), decltype(by_module::w_PNG_IMAGE_FAILED(i))>);
            const png_uint_32 stride = random() % 100000;
            EXPECT_EQ(by_macro::w_PNG_IMAGE_BUFFER_SIZE(i, stride), by_module::w_PNG_IMAGE_BUFFER_SIZE(i, stride));
            const png_alloc_size_t b = random();
            EXPECT_EQ(by_macro::w_PNG_ZLIB_MAX_SIZE(b), by_module::w_PNG_ZLIB_MAX_SIZE(b));
        }
    }
    for (int p = 0; p < PNG_INTERLACE_ADAM7_PASSES; ++p) {
        EXPECT_EQ(by_macro::w_PNG_PASS_START_ROW(p), by_module::w_PNG_PASS_START_ROW(p)) << "PNG_PASS_START_ROW " << p;
        EXPECT_EQ(by_macro::w_PNG_PASS_START_COL(p), by_module::w_PNG_PASS_START_COL(p)) << "PNG_PASS_START_COL " << p;
        EXPECT_EQ(by_macro::w_PNG_PASS_ROW_OFFSET(p), by_module::w_PNG_PASS_ROW_OFFSET(p)) << "PNG_PASS_ROW_OFFSET " << p;
        EXPECT_EQ(by_macro::w_PNG_PASS_COL_OFFSET(p), by_module::w_PNG_PASS_COL_OFFSET(p)) << "PNG_PASS_COL_OFFSET " << p;
        EXPECT_EQ(by_macro::w_PNG_PASS_ROW_SHIFT(p), by_module::w_PNG_PASS_ROW_SHIFT(p)) << "PNG_PASS_ROW_SHIFT " << p;
        EXPECT_EQ(by_macro::w_PNG_PASS_COL_SHIFT(p), by_module::w_PNG_PASS_COL_SHIFT(p)) << "PNG_PASS_COL_SHIFT " << p;
        for (png_uint_32 a = 0; a < 70; ++a) {
            EXPECT_EQ(by_macro::w_PNG_PASS_ROWS(a, p), by_module::w_PNG_PASS_ROWS(a, p)) << "PNG_PASS_ROWS " << a << " " << p;
            EXPECT_EQ(by_macro::w_PNG_PASS_COLS(a, p), by_module::w_PNG_PASS_COLS(a, p)) << "PNG_PASS_COLS " << a << " " << p;
            EXPECT_EQ(by_macro::w_PNG_ROW_FROM_PASS_ROW(a, p), by_module::w_PNG_ROW_FROM_PASS_ROW(a, p)) << "PNG_ROW_FROM_PASS_ROW " << a << " " << p;
            EXPECT_EQ(by_macro::w_PNG_COL_FROM_PASS_COL(a, p), by_module::w_PNG_COL_FROM_PASS_COL(a, p)) << "PNG_COL_FROM_PASS_COL " << a << " " << p;
            EXPECT_EQ(by_macro::w_PNG_ROW_IN_INTERLACE_PASS(a, p), by_module::w_PNG_ROW_IN_INTERLACE_PASS(a, p)) << "PNG_ROW_IN_INTERLACE_PASS " << a << " " << p;
            EXPECT_EQ(by_macro::w_PNG_COL_IN_INTERLACE_PASS(a, p), by_module::w_PNG_COL_IN_INTERLACE_PASS(a, p)) << "PNG_COL_IN_INTERLACE_PASS " << a << " " << p;
            EXPECT_EQ(by_macro::w_PNG_PASS_MASK(a, p), by_module::w_PNG_PASS_MASK(a, p)) << "PNG_PASS_MASK " << a << " " << p;
        }
    }
    for (int n = 0; n < 2000; ++n) {
        png_byte b[4];
        for (auto& x : b) x = static_cast<png_byte>(random());
        EXPECT_EQ(by_macro::w_PNG_get_uint_32(b), by_module::w_PNG_get_uint_32(b));
        EXPECT_EQ(by_macro::w_PNG_get_uint_16(b), by_module::w_PNG_get_uint_16(b));
        EXPECT_EQ(by_macro::w_PNG_get_int_32(b), by_module::w_PNG_get_int_32(b));
        EXPECT_EQ(by_macro::w_png_composite(b[0], b[1], b[2]), by_module::w_png_composite(b[0], b[1], b[2]));
        const auto f16 = static_cast<png_uint_16>(random()), a16 = static_cast<png_uint_16>(random()), g16 = static_cast<png_uint_16>(random());
        EXPECT_EQ(by_macro::w_png_composite_16(f16, a16, g16), by_module::w_png_composite_16(f16, a16, g16));
    }
}

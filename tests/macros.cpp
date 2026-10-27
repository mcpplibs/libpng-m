// The constants and constexpr functions of mcpplibs.mwrap.libpng.macros, at
// compile time, without png.h. The types the functions take (png_image,
// png_uint_32 …) are the root module's: each entity has one export owner.
import mcpplibs.mwrap.libpng.macros;
import mcpplibs.mwrap.libpng;
namespace macros = mcpplibs::mwrap::libpng::macros;
using namespace macros;

// Versions and the options of this build (compat.libpng's pnglibconf.h).
static_assert(PNG_LIBPNG_VER == 10643 && PNG_LIBPNG_VER_MAJOR == 1 && PNG_LIBPNG_VER_MINOR == 6 && PNG_LIBPNG_VER_RELEASE == 43);
static_assert(PNG_LIBPNG_VER_STRING[0] == '1' && sizeof PNG_LIBPNG_VER_STRING == 7);
static_assert(sizeof PNG_HEADER_VERSION_STRING == sizeof " libpng version 1.6.43\n");
static_assert(PNG_LIBPNG_BUILD_TYPE == PNG_LIBPNG_BUILD_STABLE && PNG_LIBPNG_VER_SONUM == 16);
static_assert(PNG_READ_SUPPORTED && PNG_WRITE_SUPPORTED && PNG_SIMPLIFIED_READ_SUPPORTED && PNG_SIMPLIFIED_WRITE_SUPPORTED);
static_assert(PNG_SETJMP_SUPPORTED && PNG_STDIO_SUPPORTED && PNG_PROGRESSIVE_READ_SUPPORTED && PNG_READ_16_TO_8_SUPPORTED);
static_assert(!PNG_ERROR_NUMBERS_SUPPORTED && !PNG_ARM_NEON_API_SUPPORTED && !PNG_BENIGN_WRITE_ERRORS_SUPPORTED);
static_assert(PNG_USE_READ_MACROS && PNG_DEFAULT_READ_MACROS == 1 && PNG_ZBUF_SIZE == 8192 && PNG_IDAT_READ_SIZE == PNG_ZBUF_SIZE);
static_assert(PNG_USER_WIDTH_MAX == 1000000 && PNG_Z_DEFAULT_COMPRESSION == -1 && PNG_sRGB_PROFILE_CHECKS == 2);

// Each constant has its macro's type.
template <class A, class B> constexpr bool same = false;
template <class A> constexpr bool same<A, A> = true;
static_assert(same<decltype(PNG_COLOR_TYPE_RGB_ALPHA), const int> && PNG_COLOR_TYPE_RGBA == 6 && PNG_COLOR_TYPE_GA == 4);
static_assert(same<decltype(PNG_INFO_eXIf), const unsigned> && PNG_INFO_eXIf == 0x10000U);
static_assert(same<decltype(PNG_FORMAT_RGBA), const unsigned> && same<decltype(PNG_FORMAT_GRAY), const int>);
static_assert(same<decltype(PNG_UINT_31_MAX), const png_uint_32> && PNG_UINT_32_MAX == 0xffffffffU);
static_assert(same<decltype(PNG_FP_MAX), const png_fixed_point> && PNG_FP_MIN == -PNG_FP_MAX && PNG_FP_1 == 100000);
static_assert(same<decltype(PNG_GAMMA_THRESHOLD), const double> && PNG_GAMMA_THRESHOLD > 0.0499 && PNG_GAMMA_THRESHOLD < 0.0501);
static_assert(PNG_SIZE_MAX == static_cast<decltype(sizeof 0)>(-1) && PNG_GAMMA_LINEAR == PNG_FP_1 && PNG_GAMMA_sRGB == 220000);
static_assert(PNG_TRANSFORM_SCALE_16 == 0x8000 && PNG_TRANSFORM_STRIP_FILLER_BEFORE == PNG_TRANSFORM_STRIP_FILLER);
static_assert(PNG_ALL_FILTERS == 0xf8 && PNG_FAST_FILTERS == 0x38 && PNG_FILTER_VALUE_LAST == 5);
static_assert(PNG_FREE_UNKN == 0x0200U && PNG_FREE_MUL == (PNG_FREE_SPLT | PNG_FREE_TEXT | PNG_FREE_UNKN));
static_assert(PNG_IMAGE_VERSION == 1 && PNG_IMAGE_ERROR == 2 && PNG_HANDLE_CHUNK_LAST == 4 && PNG_OPTION_NEXT == 14);
static_assert(PNG_FORMAT_ABGR_COLORMAP == (PNG_FORMAT_FLAG_ALPHA | PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_BGR | PNG_FORMAT_FLAG_AFIRST | PNG_FORMAT_FLAG_COLORMAP));

// The simplified API's computations, on constant images.
static_assert(PNG_IMAGE_SAMPLE_CHANNELS(PNG_FORMAT_GRAY) == 1 && PNG_IMAGE_SAMPLE_CHANNELS(PNG_FORMAT_ABGR) == 4);
static_assert(PNG_IMAGE_SAMPLE_COMPONENT_SIZE(PNG_FORMAT_LINEAR_RGB_ALPHA) == 2 && PNG_IMAGE_SAMPLE_SIZE(PNG_FORMAT_LINEAR_RGB_ALPHA) == 8);
static_assert(PNG_IMAGE_PIXEL_SIZE(PNG_FORMAT_RGBA_COLORMAP) == 1 && PNG_IMAGE_PIXEL_CHANNELS(PNG_FORMAT_BGR) == 3);
static_assert(PNG_IMAGE_PIXEL_(PNG_IMAGE_SAMPLE_SIZE, PNG_FORMAT_RGB) == 3 && PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(PNG_FORMAT_RGBA) == 1024);
constexpr png_image image_of(png_uint_32 w, png_uint_32 h, png_uint_32 format, png_uint_32 entries = 0) {
    png_image i{};
    i.version = PNG_IMAGE_VERSION;
    i.width = w;
    i.height = h;
    i.format = format;
    i.colormap_entries = entries;
    return i;
}
constexpr png_image rgba = image_of(640, 480, PNG_FORMAT_RGBA), deep = image_of(10, 3, PNG_FORMAT_LINEAR_RGB);
constexpr png_image mapped = image_of(16, 16, PNG_FORMAT_RGBA_COLORMAP, 200);
static_assert(PNG_IMAGE_ROW_STRIDE(rgba) == 2560 && PNG_IMAGE_SIZE(rgba) == 640u * 480 * 4);
static_assert(PNG_IMAGE_ROW_STRIDE(deep) == 30 && PNG_IMAGE_SIZE(deep) == 10u * 3 * 3 * 2 && PNG_IMAGE_BUFFER_SIZE(deep, 32) == 2u * 3 * 32);
static_assert(same<decltype(PNG_IMAGE_BUFFER_SIZE(rgba, 1ull)), unsigned long long>);  // the row stride's type, as the macro's
static_assert(PNG_IMAGE_SIZE(mapped) == 256 && PNG_IMAGE_COLORMAP_SIZE(mapped) == 800 && PNG_IMAGE_DATA_SIZE(mapped) == 272);
static_assert(PNG_ZLIB_MAX_SIZE(1000u) == 1000u + 125u + 16u + 11u && same<decltype(PNG_ZLIB_MAX_SIZE(1ull)), unsigned long long>);
static_assert(PNG_IMAGE_COMPRESSED_SIZE_MAX(mapped) == PNG_ZLIB_MAX_SIZE(png_alloc_size_t{272}));
static_assert(PNG_IMAGE_PNG_SIZE_MAX(mapped) == (8 + 25 + 16 + 44 + 12 + (12 + 3 * 200 + 12 + 200) + 12) + 12 * (PNG_IMAGE_COMPRESSED_SIZE_MAX(mapped) / 8192) + PNG_IMAGE_COMPRESSED_SIZE_MAX(mapped));
constexpr png_image failed = [] { png_image i{}; i.warning_or_error = PNG_IMAGE_ERROR | PNG_IMAGE_WARNING; return i; }();
constexpr png_image warned = [] { png_image i{}; i.warning_or_error = PNG_IMAGE_WARNING; return i; }();
static_assert(PNG_IMAGE_FAILED(failed) && !PNG_IMAGE_FAILED(warned));

// Adam7: the 8x8 tile, pass by pass, as the PNG specification draws it.
//   1 6 4 6 2 6 4 6 / 7 7 7 7 7 7 7 7 / 5 6 5 6 5 6 5 6 / 7 … / 3 6 4 6 3 6 4 6 …
constexpr int adam7[8][8] = {{1, 6, 4, 6, 2, 6, 4, 6}, {7, 7, 7, 7, 7, 7, 7, 7}, {5, 6, 5, 6, 5, 6, 5, 6}, {7, 7, 7, 7, 7, 7, 7, 7},
                             {3, 6, 4, 6, 3, 6, 4, 6}, {7, 7, 7, 7, 7, 7, 7, 7}, {5, 6, 5, 6, 5, 6, 5, 6}, {7, 7, 7, 7, 7, 7, 7, 7}};
constexpr bool tile_matches() {
    for (int pass = 0; pass < PNG_INTERLACE_ADAM7_PASSES; ++pass)
        for (int y = 0; y < 8; ++y)
            for (int x = 0; x < 8; ++x) {
                const bool in = PNG_ROW_IN_INTERLACE_PASS(y, pass) && PNG_COL_IN_INTERLACE_PASS(x, pass);
                if (in != (adam7[y][x] == pass + 1)) return false;
            }
    return true;
}
static_assert(tile_matches());
static_assert(PNG_PASS_START_ROW(0) == 0 && PNG_PASS_START_COL(1) == 4 && PNG_PASS_START_ROW(2) == 4 && PNG_PASS_START_ROW(6) == 1);
static_assert(PNG_PASS_ROW_OFFSET(6) == 2 && PNG_PASS_COL_OFFSET(5) == 2 && PNG_PASS_ROW_SHIFT(0) == 3 && PNG_PASS_COL_SHIFT(6) == 0);
static_assert(PNG_PASS_ROWS(11u, 0) == 2 && PNG_PASS_COLS(13u, 1) == 2 && PNG_PASS_COLS(3u, 1) == 0 && PNG_PASS_ROWS(11u, 6) == 5);
static_assert(PNG_ROW_FROM_PASS_ROW(2, 6) == 5 && PNG_COL_FROM_PASS_COL(1, 1) == 12);

// The read macros and compositing.
constexpr png_byte be[] = {0xde, 0xad, 0xbe, 0xef, 0xff, 0xfe, 0x1d, 0xc0};
static_assert(PNG_get_uint_32(be) == 0xdeadbeefU && PNG_get_uint_16(be + 2) == 0xbeef && PNG_get_int_32(be + 4) == -123456);
constexpr int composite(png_byte fg, png_byte alpha, png_byte bg) { png_byte c = 0; png_composite(c, fg, alpha, bg); return c; }
static_assert(composite(255, 255, 0) == 255 && composite(255, 0, 7) == 7 && composite(200, 128, 100) == 150);
constexpr int composite16(png_uint_16 fg, png_uint_16 alpha, png_uint_16 bg) { png_uint_16 c = 0; png_composite_16(c, fg, alpha, bg); return c; }
static_assert(composite16(65535, 65535, 0) == 65535 && composite16(1000, 0, 2000) == 2000);

int main() { return 0; }

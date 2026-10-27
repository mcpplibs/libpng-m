// mcpplibs.mwrap.libpng.macros — the macros of <png.h> and its pnglibconf.h
// (a named module cannot export a macro), under their original names in
// mcpplibs::mwrap::libpng::macros:
//
//   - the constants (PNG_COLOR_TYPE_*, PNG_TRANSFORM_*, PNG_INFO_*,
//     PNG_FILTER_*, PNG_FORMAT_*, PNG_LIBPNG_VER*, PNG_IMAGE_VERSION, the
//     pnglibconf.h settings …) as `inline constexpr`, each of its macro's
//     type; the version strings as `const char[]`;
//   - the options, PNG_<NAME>_SUPPORTED, as `inline constexpr bool` (the
//     :options partition, generated);
//   - the computations as constexpr functions with the macros' semantics:
//     the simplified API's PNG_IMAGE_* sizes, the Adam7 PNG_PASS_* helpers,
//     PNG_get_uint_32 / PNG_get_uint_16 / PNG_get_int_32, png_composite;
//     png_check_sig, png_image_write_get_memory_size and png_jmpbuf as
//     functions over the calls they expand to; png_libpng_ver as a constant
//     initialised by that call.
//
// Nothing is left that only a macro can be. <mwrap/libpng/macros.hpp>
// (= <png.h>) remains for `#if` tests of the options and for code written
// against the macros; setjmp, which a png_jmpbuf caller needs, is the C
// library's macro (<csetjmp>). tools/check_exports.py accounts for every
// macro.
module;
#include <png.h>

namespace mwrap_libpng_capture {
inline constexpr auto value_PNG_DEFAULT_READ_MACROS = PNG_DEFAULT_READ_MACROS;
inline constexpr auto value_PNG_QUANTIZE_BLUE_BITS = PNG_QUANTIZE_BLUE_BITS;
inline constexpr auto value_PNG_sCAL_PRECISION = PNG_sCAL_PRECISION;
inline constexpr auto value_PNG_API_RULE = PNG_API_RULE;
inline constexpr auto value_PNG_USER_WIDTH_MAX = PNG_USER_WIDTH_MAX;
inline constexpr auto value_PNG_INFLATE_BUF_SIZE = PNG_INFLATE_BUF_SIZE;
inline constexpr auto value_PNG_GAMMA_THRESHOLD_FIXED = PNG_GAMMA_THRESHOLD_FIXED;
inline constexpr auto value_PNG_USER_HEIGHT_MAX = PNG_USER_HEIGHT_MAX;
inline constexpr auto value_PNG_TEXT_Z_DEFAULT_COMPRESSION = PNG_TEXT_Z_DEFAULT_COMPRESSION;
inline constexpr auto value_PNG_ZLIB_VERNUM = PNG_ZLIB_VERNUM;
inline constexpr auto value_PNG_MAX_GAMMA_8 = PNG_MAX_GAMMA_8;
inline constexpr auto value_PNG_IDAT_READ_SIZE = PNG_IDAT_READ_SIZE;
inline constexpr auto value_PNG_ZBUF_SIZE = PNG_ZBUF_SIZE;
inline constexpr auto value_PNG_USER_CHUNK_MALLOC_MAX = PNG_USER_CHUNK_MALLOC_MAX;
inline constexpr auto value_PNG_Z_DEFAULT_STRATEGY = PNG_Z_DEFAULT_STRATEGY;
inline constexpr auto value_PNG_QUANTIZE_GREEN_BITS = PNG_QUANTIZE_GREEN_BITS;
inline constexpr auto value_PNG_Z_DEFAULT_COMPRESSION = PNG_Z_DEFAULT_COMPRESSION;
inline constexpr auto value_PNG_USER_CHUNK_CACHE_MAX = PNG_USER_CHUNK_CACHE_MAX;
inline constexpr auto value_PNG_Z_DEFAULT_NOFILTER_STRATEGY = PNG_Z_DEFAULT_NOFILTER_STRATEGY;
inline constexpr auto value_PNG_QUANTIZE_RED_BITS = PNG_QUANTIZE_RED_BITS;
inline constexpr auto value_PNG_TEXT_Z_DEFAULT_STRATEGY = PNG_TEXT_Z_DEFAULT_STRATEGY;
inline constexpr auto value_PNG_sRGB_PROFILE_CHECKS = PNG_sRGB_PROFILE_CHECKS;
inline constexpr auto value_PNG_TRANSFORM_GRAY_TO_RGB = PNG_TRANSFORM_GRAY_TO_RGB;
inline constexpr auto value_PNG_FREE_ALL = PNG_FREE_ALL;
inline constexpr auto value_PNG_HANDLE_CHUNK_AS_DEFAULT = PNG_HANDLE_CHUNK_AS_DEFAULT;
inline constexpr auto value_PNG_INFO_PLTE = PNG_INFO_PLTE;
inline constexpr auto value_PNG_TRANSFORM_SHIFT = PNG_TRANSFORM_SHIFT;
inline constexpr auto value_PNG_CRC_QUIET_USE = PNG_CRC_QUIET_USE;
inline constexpr auto value_PNG_CRC_DEFAULT = PNG_CRC_DEFAULT;
inline constexpr auto value_PNG_SIZE_MAX = PNG_SIZE_MAX;
inline constexpr auto value_PNG_HANDLE_CHUNK_LAST = PNG_HANDLE_CHUNK_LAST;
inline constexpr auto value_PNG_TRANSFORM_PACKSWAP = PNG_TRANSFORM_PACKSWAP;
inline constexpr auto value_PNG_FILTER_UP = PNG_FILTER_UP;
inline constexpr auto value_PNG_FILTER_VALUE_LAST = PNG_FILTER_VALUE_LAST;
inline constexpr auto value_PNG_sRGB_INTENT_PERCEPTUAL = PNG_sRGB_INTENT_PERCEPTUAL;
inline constexpr auto value_PNG_IO_MASK_LOC = PNG_IO_MASK_LOC;
inline constexpr auto value_PNG_INTERLACE_ADAM7_PASSES = PNG_INTERLACE_ADAM7_PASSES;
inline constexpr auto value_PNG_LIBPNG_BUILD_SPECIAL = PNG_LIBPNG_BUILD_SPECIAL;
inline constexpr auto value_PNG_COLOR_TYPE_RGB = PNG_COLOR_TYPE_RGB;
inline constexpr auto value_PNG_FREE_HIST = PNG_FREE_HIST;
inline constexpr auto value_PNG_IMAGE_WARNING = PNG_IMAGE_WARNING;
inline constexpr auto value_PNG_EQUATION_HYPERBOLIC = PNG_EQUATION_HYPERBOLIC;
inline constexpr auto value_PNG_BACKGROUND_GAMMA_FILE = PNG_BACKGROUND_GAMMA_FILE;
inline constexpr auto value_PNG_USER_WILL_FREE_DATA = PNG_USER_WILL_FREE_DATA;
inline constexpr auto value_PNG_FILLER_BEFORE = PNG_FILLER_BEFORE;
inline constexpr auto value_PNG_COMPRESSION_TYPE_DEFAULT = PNG_COMPRESSION_TYPE_DEFAULT;
inline constexpr auto value_PNG_FREE_TEXT = PNG_FREE_TEXT;
inline constexpr auto value_PNG_LIBPNG_VER_DLLNUM = PNG_LIBPNG_VER_DLLNUM;
inline constexpr auto value_PNG_NO_FILTERS = PNG_NO_FILTERS;
inline constexpr auto value_PNG_INFO_tRNS = PNG_INFO_tRNS;
inline constexpr auto value_PNG_FILTER_HEURISTIC_LAST = PNG_FILTER_HEURISTIC_LAST;
inline constexpr auto value_PNG_FP_MAX = PNG_FP_MAX;
inline constexpr auto value_PNG_LIBPNG_BUILD_RC = PNG_LIBPNG_BUILD_RC;
inline constexpr auto value_PNG_sRGB_INTENT_SATURATION = PNG_sRGB_INTENT_SATURATION;
inline constexpr auto value_PNG_TRANSFORM_SCALE_16 = PNG_TRANSFORM_SCALE_16;
inline constexpr auto value_PNG_IO_CHUNK_CRC = PNG_IO_CHUNK_CRC;
inline constexpr auto value_PNG_FORMAT_FLAG_BGR = PNG_FORMAT_FLAG_BGR;
inline constexpr auto value_PNG_COLOR_MASK_PALETTE = PNG_COLOR_MASK_PALETTE;
inline constexpr auto value_PNG_FILTER_HEURISTIC_DEFAULT = PNG_FILTER_HEURISTIC_DEFAULT;
inline constexpr auto value_PNG_BACKGROUND_GAMMA_UNKNOWN = PNG_BACKGROUND_GAMMA_UNKNOWN;
inline constexpr auto value_PNG_TEXT_COMPRESSION_LAST = PNG_TEXT_COMPRESSION_LAST;
inline constexpr auto value_PNG_TEXT_COMPRESSION_NONE = PNG_TEXT_COMPRESSION_NONE;
inline constexpr auto value_PNG_RESOLUTION_UNKNOWN = PNG_RESOLUTION_UNKNOWN;
inline constexpr auto value_PNG_FILTER_VALUE_AVG = PNG_FILTER_VALUE_AVG;
inline constexpr auto value_PNG_FORMAT_LINEAR_Y = PNG_FORMAT_LINEAR_Y;
inline constexpr auto value_PNG_FILTER_HEURISTIC_WEIGHTED = PNG_FILTER_HEURISTIC_WEIGHTED;
inline constexpr auto value_PNG_TRANSFORM_BGR = PNG_TRANSFORM_BGR;
inline constexpr auto value_PNG_FORMAT_AG = PNG_FORMAT_AG;
inline constexpr auto value_PNG_LIBPNG_VER_MINOR = PNG_LIBPNG_VER_MINOR;
inline constexpr auto value_PNG_ALPHA_PNG = PNG_ALPHA_PNG;
inline constexpr auto value_PNG_FILTER_PAETH = PNG_FILTER_PAETH;
inline constexpr auto value_PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB = PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB;
inline constexpr auto value_PNG_TRANSFORM_INVERT_MONO = PNG_TRANSFORM_INVERT_MONO;
inline constexpr auto value_PNG_FORMAT_LINEAR_Y_ALPHA = PNG_FORMAT_LINEAR_Y_ALPHA;
inline constexpr auto value_PNG_sRGB_INTENT_LAST = PNG_sRGB_INTENT_LAST;
inline constexpr auto value_PNG_FREE_TRNS = PNG_FREE_TRNS;
inline constexpr auto value_PNG_TRANSFORM_STRIP_FILLER = PNG_TRANSFORM_STRIP_FILLER;
inline constexpr auto value_PNG_ALL_MNG_FEATURES = PNG_ALL_MNG_FEATURES;
inline constexpr auto value_PNG_INTERLACE_ADAM7 = PNG_INTERLACE_ADAM7;
inline constexpr auto value_PNG_RGB_TO_GRAY_DEFAULT = PNG_RGB_TO_GRAY_DEFAULT;
inline constexpr auto value_PNG_ALPHA_BROKEN = PNG_ALPHA_BROKEN;
inline constexpr auto value_PNG_FORMAT_FLAG_ALPHA = PNG_FORMAT_FLAG_ALPHA;
inline constexpr auto value_PNG_OFFSET_MICROMETER = PNG_OFFSET_MICROMETER;
inline constexpr auto value_PNG_LIBPNG_BUILD_BETA = PNG_LIBPNG_BUILD_BETA;
inline constexpr auto value_PNG_FAST_FILTERS = PNG_FAST_FILTERS;
inline constexpr auto value_PNG_COLOR_TYPE_RGBA = PNG_COLOR_TYPE_RGBA;
inline constexpr auto value_PNG_INFO_eXIf = PNG_INFO_eXIf;
inline constexpr auto value_PNG_TEXT_COMPRESSION_NONE_WR = PNG_TEXT_COMPRESSION_NONE_WR;
inline constexpr auto value_PNG_FILTER_VALUE_UP = PNG_FILTER_VALUE_UP;
inline constexpr auto value_PNG_FILTER_VALUE_SUB = PNG_FILTER_VALUE_SUB;
inline constexpr auto value_PNG_IO_MASK_OP = PNG_IO_MASK_OP;
inline constexpr auto value_PNG_INFO_cHRM = PNG_INFO_cHRM;
inline constexpr auto value_PNG_ITXT_COMPRESSION_NONE = PNG_ITXT_COMPRESSION_NONE;
inline constexpr auto value_PNG_COLOR_TYPE_PALETTE = PNG_COLOR_TYPE_PALETTE;
inline constexpr auto value_PNG_COLOR_TYPE_GRAY = PNG_COLOR_TYPE_GRAY;
inline constexpr auto value_PNG_FORMAT_ABGR = PNG_FORMAT_ABGR;
inline constexpr auto value_PNG_FREE_MUL = PNG_FREE_MUL;
inline constexpr auto value_PNG_INFO_bKGD = PNG_INFO_bKGD;
inline constexpr auto value_PNG_FORMAT_BGR_COLORMAP = PNG_FORMAT_BGR_COLORMAP;
inline constexpr auto value_PNG_HAVE_IHDR = PNG_HAVE_IHDR;
inline constexpr auto value_PNG_CRC_ERROR_QUIT = PNG_CRC_ERROR_QUIT;
inline constexpr auto value_PNG_FREE_SPLT = PNG_FREE_SPLT;
inline constexpr auto value_PNG_INTERLACE_LAST = PNG_INTERLACE_LAST;
inline constexpr auto value_PNG_FORMAT_GRAY = PNG_FORMAT_GRAY;
inline constexpr auto value_PNG_LIBPNG_BUILD_RELEASE_STATUS_MASK = PNG_LIBPNG_BUILD_RELEASE_STATUS_MASK;
inline constexpr auto value_PNG_EQUATION_LAST = PNG_EQUATION_LAST;
inline constexpr auto value_PNG_FILTER_AVG = PNG_FILTER_AVG;
inline constexpr auto value_PNG_ITXT_COMPRESSION_zTXt = PNG_ITXT_COMPRESSION_zTXt;
inline constexpr auto value_PNG_FORMAT_LINEAR_RGB_ALPHA = PNG_FORMAT_LINEAR_RGB_ALPHA;
inline constexpr auto value_PNG_FREE_PCAL = PNG_FREE_PCAL;
inline constexpr auto value_PNG_HANDLE_CHUNK_NEVER = PNG_HANDLE_CHUNK_NEVER;
inline constexpr auto value_PNG_LIBPNG_BUILD_PRIVATE = PNG_LIBPNG_BUILD_PRIVATE;
inline constexpr auto value_PNG_TRANSFORM_STRIP_ALPHA = PNG_TRANSFORM_STRIP_ALPHA;
inline constexpr auto value_PNG_TRANSFORM_EXPAND_16 = PNG_TRANSFORM_EXPAND_16;
inline constexpr auto value_PNG_FREE_UNKN = PNG_FREE_UNKN;
inline constexpr auto value_PNG_COLOR_TYPE_RGB_ALPHA = PNG_COLOR_TYPE_RGB_ALPHA;
inline constexpr auto value_PNG_FORMAT_FLAG_COLORMAP = PNG_FORMAT_FLAG_COLORMAP;
inline constexpr auto value_PNG_TRANSFORM_PACKING = PNG_TRANSFORM_PACKING;
inline constexpr auto value_PNG_FP_HALF = PNG_FP_HALF;
inline constexpr auto value_PNG_INFO_sRGB = PNG_INFO_sRGB;
inline constexpr auto value_PNG_FORMAT_ARGB_COLORMAP = PNG_FORMAT_ARGB_COLORMAP;
inline constexpr auto value_PNG_LIBPNG_VER_SONUM = PNG_LIBPNG_VER_SONUM;
inline constexpr auto value_PNG_INTERLACE_NONE = PNG_INTERLACE_NONE;
inline constexpr auto value_PNG_OFFSET_LAST = PNG_OFFSET_LAST;
inline constexpr auto value_PNG_FORMAT_RGBA_COLORMAP = PNG_FORMAT_RGBA_COLORMAP;
inline constexpr auto value_PNG_LIBPNG_VER_BUILD = PNG_LIBPNG_VER_BUILD;
inline constexpr auto value_PNG_COMPRESSION_TYPE_BASE = PNG_COMPRESSION_TYPE_BASE;
inline constexpr auto value_PNG_FLAG_MNG_EMPTY_PLTE = PNG_FLAG_MNG_EMPTY_PLTE;
inline constexpr auto value_PNG_sRGB_INTENT_ABSOLUTE = PNG_sRGB_INTENT_ABSOLUTE;
inline constexpr auto value_PNG_INFO_sPLT = PNG_INFO_sPLT;
inline constexpr auto value_PNG_HANDLE_CHUNK_IF_SAFE = PNG_HANDLE_CHUNK_IF_SAFE;
inline constexpr auto value_PNG_ERROR_ACTION_WARN = PNG_ERROR_ACTION_WARN;
inline constexpr auto value_PNG_TRANSFORM_SWAP_ENDIAN = PNG_TRANSFORM_SWAP_ENDIAN;
inline constexpr auto value_PNG_BACKGROUND_GAMMA_UNIQUE = PNG_BACKGROUND_GAMMA_UNIQUE;
inline constexpr auto value_PNG_INFO_oFFs = PNG_INFO_oFFs;
inline constexpr auto value_PNG_COLOR_TYPE_GA = PNG_COLOR_TYPE_GA;
inline constexpr auto value_PNG_LIBPNG_VER_RELEASE = PNG_LIBPNG_VER_RELEASE;
inline constexpr auto value_PNG_OPTION_UNSET = PNG_OPTION_UNSET;
inline constexpr auto value_PNG_LIBPNG_VER_SHAREDLIB = PNG_LIBPNG_VER_SHAREDLIB;
inline constexpr auto value_PNG_HANDLE_CHUNK_ALWAYS = PNG_HANDLE_CHUNK_ALWAYS;
inline constexpr auto value_PNG_FORMAT_FLAG_AFIRST = PNG_FORMAT_FLAG_AFIRST;
inline constexpr auto value_PNG_FILTER_TYPE_DEFAULT = PNG_FILTER_TYPE_DEFAULT;
inline constexpr auto value_PNG_IMAGE_FLAG_16BIT_sRGB = PNG_IMAGE_FLAG_16BIT_sRGB;
inline constexpr auto value_PNG_TRANSFORM_SWAP_ALPHA = PNG_TRANSFORM_SWAP_ALPHA;
inline constexpr auto value_PNG_IMAGE_VERSION = PNG_IMAGE_VERSION;
inline constexpr auto value_PNG_IO_READING = PNG_IO_READING;
inline constexpr auto value_PNG_TEXT_COMPRESSION_zTXt = PNG_TEXT_COMPRESSION_zTXt;
inline constexpr auto value_PNG_FORMAT_FLAG_ASSOCIATED_ALPHA = PNG_FORMAT_FLAG_ASSOCIATED_ALPHA;
inline constexpr auto value_PNG_TRANSFORM_IDENTITY = PNG_TRANSFORM_IDENTITY;
inline constexpr auto value_PNG_FREE_ROWS = PNG_FREE_ROWS;
inline constexpr auto value_PNG_FLAG_MNG_FILTER_64 = PNG_FLAG_MNG_FILTER_64;
inline constexpr auto value_PNG_FILLER_AFTER = PNG_FILLER_AFTER;
inline constexpr auto value_PNG_DESTROY_WILL_FREE_DATA = PNG_DESTROY_WILL_FREE_DATA;
inline constexpr auto value_PNG_SET_WILL_FREE_DATA = PNG_SET_WILL_FREE_DATA;
inline constexpr auto value_PNG_ALPHA_STANDARD = PNG_ALPHA_STANDARD;
inline constexpr auto value_PNG_FORMAT_RGB = PNG_FORMAT_RGB;
inline constexpr auto value_PNG_INFO_pCAL = PNG_INFO_pCAL;
inline constexpr auto value_PNG_TRANSFORM_STRIP_16 = PNG_TRANSFORM_STRIP_16;
inline constexpr auto value_PNG_FORMAT_ARGB = PNG_FORMAT_ARGB;
inline constexpr auto value_PNG_TRANSFORM_STRIP_FILLER_AFTER = PNG_TRANSFORM_STRIP_FILLER_AFTER;
inline constexpr auto value_PNG_INFO_IDAT = PNG_INFO_IDAT;
inline constexpr auto value_PNG_IO_WRITING = PNG_IO_WRITING;
inline constexpr auto value_PNG_MAXIMUM_INFLATE_WINDOW = PNG_MAXIMUM_INFLATE_WINDOW;
inline constexpr auto value_PNG_FREE_EXIF = PNG_FREE_EXIF;
inline constexpr auto value_PNG_FREE_PLTE = PNG_FREE_PLTE;
inline constexpr auto value_PNG_OPTION_INVALID = PNG_OPTION_INVALID;
inline constexpr auto value_PNG_TRANSFORM_INVERT_ALPHA = PNG_TRANSFORM_INVERT_ALPHA;
inline constexpr auto value_PNG_COLOR_TYPE_GRAY_ALPHA = PNG_COLOR_TYPE_GRAY_ALPHA;
inline constexpr auto value_PNG_CRC_WARN_DISCARD = PNG_CRC_WARN_DISCARD;
inline constexpr auto value_PNG_FORMAT_RGB_COLORMAP = PNG_FORMAT_RGB_COLORMAP;
inline constexpr auto value_PNG_IMAGE_ERROR = PNG_IMAGE_ERROR;
inline constexpr auto value_PNG_IO_SIGNATURE = PNG_IO_SIGNATURE;
inline constexpr auto value_PNG_INFO_tIME = PNG_INFO_tIME;
inline constexpr auto value_PNG_SCALE_LAST = PNG_SCALE_LAST;
inline constexpr auto value_PNG_IO_CHUNK_DATA = PNG_IO_CHUNK_DATA;
inline constexpr auto value_PNG_COLOR_MASK_COLOR = PNG_COLOR_MASK_COLOR;
inline constexpr auto value_PNG_IMAGE_FLAG_FAST = PNG_IMAGE_FLAG_FAST;
inline constexpr auto value_PNG_OFFSET_PIXEL = PNG_OFFSET_PIXEL;
inline constexpr auto value_PNG_FILTER_TYPE_BASE = PNG_FILTER_TYPE_BASE;
inline constexpr auto value_PNG_FORMAT_GA = PNG_FORMAT_GA;
inline constexpr auto value_PNG_GAMMA_MAC_18 = PNG_GAMMA_MAC_18;
inline constexpr auto value_PNG_LIBPNG_BUILD_STABLE = PNG_LIBPNG_BUILD_STABLE;
inline constexpr auto value_PNG_FILTER_VALUE_PAETH = PNG_FILTER_VALUE_PAETH;
inline constexpr auto value_PNG_OPTION_NEXT = PNG_OPTION_NEXT;
inline constexpr auto value_PNG_COLOR_MASK_ALPHA = PNG_COLOR_MASK_ALPHA;
inline constexpr auto value_PNG_INFO_iCCP = PNG_INFO_iCCP;
inline constexpr auto value_PNG_INFO_sBIT = PNG_INFO_sBIT;
inline constexpr auto value_PNG_SCALE_RADIAN = PNG_SCALE_RADIAN;
inline constexpr auto value_PNG_RESOLUTION_METER = PNG_RESOLUTION_METER;
inline constexpr auto value_PNG_CRC_NO_CHANGE = PNG_CRC_NO_CHANGE;
inline constexpr auto value_PNG_FILTER_SUB = PNG_FILTER_SUB;
inline constexpr auto value_PNG_FORMAT_BGR = PNG_FORMAT_BGR;
inline constexpr char value_PNG_HEADER_VERSION_STRING[] = PNG_HEADER_VERSION_STRING;
inline constexpr auto value_PNG_INFO_sCAL = PNG_INFO_sCAL;
inline constexpr auto value_PNG_FORMAT_FLAG_COLOR = PNG_FORMAT_FLAG_COLOR;
inline constexpr auto value_PNG_ALPHA_ASSOCIATED = PNG_ALPHA_ASSOCIATED;
inline constexpr auto value_PNG_FP_1 = PNG_FP_1;
inline constexpr auto value_PNG_ERROR_ACTION_ERROR = PNG_ERROR_ACTION_ERROR;
inline constexpr auto value_PNG_EQUATION_ARBITRARY = PNG_EQUATION_ARBITRARY;
inline constexpr auto value_PNG_KEYWORD_MAX_LENGTH = PNG_KEYWORD_MAX_LENGTH;
inline constexpr auto value_PNG_OPTION_ON = PNG_OPTION_ON;
inline constexpr auto value_PNG_SKIP_sRGB_CHECK_PROFILE = PNG_SKIP_sRGB_CHECK_PROFILE;
inline constexpr auto value_PNG_IO_CHUNK_HDR = PNG_IO_CHUNK_HDR;
inline constexpr auto value_PNG_TEXT_COMPRESSION_zTXt_WR = PNG_TEXT_COMPRESSION_zTXt_WR;
inline constexpr auto value_PNG_SCALE_UNKNOWN = PNG_SCALE_UNKNOWN;
inline constexpr auto value_PNG_FORMAT_FLAG_LINEAR = PNG_FORMAT_FLAG_LINEAR;
inline constexpr auto value_PNG_FORMAT_BGRA_COLORMAP = PNG_FORMAT_BGRA_COLORMAP;
inline constexpr auto value_PNG_INTRAPIXEL_DIFFERENCING = PNG_INTRAPIXEL_DIFFERENCING;
inline constexpr auto value_PNG_LIBPNG_BUILD_TYPE = PNG_LIBPNG_BUILD_TYPE;
inline constexpr auto value_PNG_LIBPNG_BUILD_BASE_TYPE = PNG_LIBPNG_BUILD_BASE_TYPE;
inline constexpr auto value_PNG_INFO_pHYs = PNG_INFO_pHYs;
inline constexpr auto value_PNG_DEFAULT_sRGB = PNG_DEFAULT_sRGB;
inline constexpr auto value_PNG_EQUATION_BASE_E = PNG_EQUATION_BASE_E;
inline constexpr auto value_PNG_LIBPNG_BUILD_PATCH = PNG_LIBPNG_BUILD_PATCH;
inline constexpr auto value_PNG_IO_NONE = PNG_IO_NONE;
inline constexpr auto value_PNG_INFO_gAMA = PNG_INFO_gAMA;
inline constexpr auto value_PNG_GAMMA_LINEAR = PNG_GAMMA_LINEAR;
inline constexpr auto value_PNG_FILTER_NONE = PNG_FILTER_NONE;
inline constexpr auto value_PNG_OPTION_OFF = PNG_OPTION_OFF;
inline constexpr auto value_PNG_ALPHA_OPTIMIZED = PNG_ALPHA_OPTIMIZED;
inline constexpr auto value_PNG_GAMMA_sRGB = PNG_GAMMA_sRGB;
inline constexpr auto value_PNG_sRGB_INTENT_RELATIVE = PNG_sRGB_INTENT_RELATIVE;
inline constexpr auto value_PNG_FP_MIN = PNG_FP_MIN;
inline constexpr char value_PNG_LIBPNG_VER_STRING[] = PNG_LIBPNG_VER_STRING;
inline constexpr auto value_PNG_EQUATION_LINEAR = PNG_EQUATION_LINEAR;
inline constexpr auto value_PNG_HAVE_PLTE = PNG_HAVE_PLTE;
inline constexpr auto value_PNG_FORMAT_LINEAR_RGB = PNG_FORMAT_LINEAR_RGB;
inline constexpr auto value_PNG_SCALE_METER = PNG_SCALE_METER;
inline constexpr auto value_PNG_FORMAT_ABGR_COLORMAP = PNG_FORMAT_ABGR_COLORMAP;
inline constexpr auto value_PNG_FILTER_HEURISTIC_UNWEIGHTED = PNG_FILTER_HEURISTIC_UNWEIGHTED;
inline constexpr auto value_PNG_FORMAT_RGBA = PNG_FORMAT_RGBA;
inline constexpr auto value_PNG_UINT_31_MAX = PNG_UINT_31_MAX;
inline constexpr auto value_PNG_INFO_hIST = PNG_INFO_hIST;
inline constexpr auto value_PNG_MAX_PALETTE_LENGTH = PNG_MAX_PALETTE_LENGTH;
inline constexpr auto value_PNG_RESOLUTION_LAST = PNG_RESOLUTION_LAST;
inline constexpr auto value_PNG_TRANSFORM_EXPAND = PNG_TRANSFORM_EXPAND;
inline constexpr auto value_PNG_FREE_ICCP = PNG_FREE_ICCP;
inline constexpr auto value_PNG_ALL_FILTERS = PNG_ALL_FILTERS;
inline constexpr auto value_PNG_BACKGROUND_GAMMA_SCREEN = PNG_BACKGROUND_GAMMA_SCREEN;
inline constexpr auto value_PNG_FREE_SCAL = PNG_FREE_SCAL;
inline constexpr auto value_PNG_FILTER_VALUE_NONE = PNG_FILTER_VALUE_NONE;
inline constexpr auto value_PNG_UINT_32_MAX = PNG_UINT_32_MAX;
inline constexpr auto value_PNG_ALPHA_PREMULTIPLIED = PNG_ALPHA_PREMULTIPLIED;
inline constexpr auto value_PNG_LIBPNG_VER = PNG_LIBPNG_VER;
inline constexpr auto value_PNG_CRC_WARN_USE = PNG_CRC_WARN_USE;
inline constexpr auto value_PNG_AFTER_IDAT = PNG_AFTER_IDAT;
inline constexpr auto value_PNG_FORMAT_BGRA = PNG_FORMAT_BGRA;
inline constexpr auto value_PNG_ERROR_ACTION_NONE = PNG_ERROR_ACTION_NONE;
inline constexpr auto value_PNG_LIBPNG_BUILD_ALPHA = PNG_LIBPNG_BUILD_ALPHA;
inline constexpr auto value_PNG_LIBPNG_VER_MAJOR = PNG_LIBPNG_VER_MAJOR;
inline constexpr auto value_PNG_GAMMA_THRESHOLD = PNG_GAMMA_THRESHOLD;
inline constexpr auto value_PNG_TRANSFORM_STRIP_FILLER_BEFORE = PNG_TRANSFORM_STRIP_FILLER_BEFORE;
}  // namespace mwrap_libpng_capture

// Every mapped name, so the definitions below are not rewritten.
#undef PNG_DEFAULT_READ_MACROS
#undef PNG_QUANTIZE_BLUE_BITS
#undef PNG_sCAL_PRECISION
#undef PNG_API_RULE
#undef PNG_USER_WIDTH_MAX
#undef PNG_INFLATE_BUF_SIZE
#undef PNG_GAMMA_THRESHOLD_FIXED
#undef PNG_USER_HEIGHT_MAX
#undef PNG_TEXT_Z_DEFAULT_COMPRESSION
#undef PNG_ZLIB_VERNUM
#undef PNG_MAX_GAMMA_8
#undef PNG_IDAT_READ_SIZE
#undef PNG_ZBUF_SIZE
#undef PNG_USER_CHUNK_MALLOC_MAX
#undef PNG_Z_DEFAULT_STRATEGY
#undef PNG_QUANTIZE_GREEN_BITS
#undef PNG_Z_DEFAULT_COMPRESSION
#undef PNG_USER_CHUNK_CACHE_MAX
#undef PNG_Z_DEFAULT_NOFILTER_STRATEGY
#undef PNG_QUANTIZE_RED_BITS
#undef PNG_TEXT_Z_DEFAULT_STRATEGY
#undef PNG_sRGB_PROFILE_CHECKS
#undef PNG_TRANSFORM_GRAY_TO_RGB
#undef PNG_FREE_ALL
#undef PNG_HANDLE_CHUNK_AS_DEFAULT
#undef PNG_INFO_PLTE
#undef PNG_TRANSFORM_SHIFT
#undef PNG_CRC_QUIET_USE
#undef PNG_CRC_DEFAULT
#undef PNG_SIZE_MAX
#undef PNG_HANDLE_CHUNK_LAST
#undef PNG_TRANSFORM_PACKSWAP
#undef PNG_FILTER_UP
#undef PNG_FILTER_VALUE_LAST
#undef PNG_sRGB_INTENT_PERCEPTUAL
#undef PNG_IO_MASK_LOC
#undef PNG_INTERLACE_ADAM7_PASSES
#undef PNG_LIBPNG_BUILD_SPECIAL
#undef PNG_COLOR_TYPE_RGB
#undef PNG_FREE_HIST
#undef PNG_IMAGE_WARNING
#undef PNG_EQUATION_HYPERBOLIC
#undef PNG_BACKGROUND_GAMMA_FILE
#undef PNG_USER_WILL_FREE_DATA
#undef PNG_FILLER_BEFORE
#undef PNG_COMPRESSION_TYPE_DEFAULT
#undef PNG_FREE_TEXT
#undef PNG_LIBPNG_VER_DLLNUM
#undef PNG_NO_FILTERS
#undef PNG_INFO_tRNS
#undef PNG_FILTER_HEURISTIC_LAST
#undef PNG_FP_MAX
#undef PNG_LIBPNG_BUILD_RC
#undef PNG_sRGB_INTENT_SATURATION
#undef PNG_TRANSFORM_SCALE_16
#undef PNG_IO_CHUNK_CRC
#undef PNG_FORMAT_FLAG_BGR
#undef PNG_COLOR_MASK_PALETTE
#undef PNG_FILTER_HEURISTIC_DEFAULT
#undef PNG_BACKGROUND_GAMMA_UNKNOWN
#undef PNG_TEXT_COMPRESSION_LAST
#undef PNG_TEXT_COMPRESSION_NONE
#undef PNG_RESOLUTION_UNKNOWN
#undef PNG_FILTER_VALUE_AVG
#undef PNG_FORMAT_LINEAR_Y
#undef PNG_FILTER_HEURISTIC_WEIGHTED
#undef PNG_TRANSFORM_BGR
#undef PNG_FORMAT_AG
#undef PNG_LIBPNG_VER_MINOR
#undef PNG_ALPHA_PNG
#undef PNG_FILTER_PAETH
#undef PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB
#undef PNG_TRANSFORM_INVERT_MONO
#undef PNG_FORMAT_LINEAR_Y_ALPHA
#undef PNG_sRGB_INTENT_LAST
#undef PNG_FREE_TRNS
#undef PNG_TRANSFORM_STRIP_FILLER
#undef PNG_ALL_MNG_FEATURES
#undef PNG_INTERLACE_ADAM7
#undef PNG_RGB_TO_GRAY_DEFAULT
#undef PNG_ALPHA_BROKEN
#undef PNG_FORMAT_FLAG_ALPHA
#undef PNG_OFFSET_MICROMETER
#undef PNG_LIBPNG_BUILD_BETA
#undef PNG_FAST_FILTERS
#undef PNG_COLOR_TYPE_RGBA
#undef PNG_INFO_eXIf
#undef PNG_TEXT_COMPRESSION_NONE_WR
#undef PNG_FILTER_VALUE_UP
#undef PNG_FILTER_VALUE_SUB
#undef PNG_IO_MASK_OP
#undef PNG_INFO_cHRM
#undef PNG_ITXT_COMPRESSION_NONE
#undef PNG_COLOR_TYPE_PALETTE
#undef PNG_COLOR_TYPE_GRAY
#undef PNG_FORMAT_ABGR
#undef PNG_FREE_MUL
#undef PNG_INFO_bKGD
#undef PNG_FORMAT_BGR_COLORMAP
#undef PNG_HAVE_IHDR
#undef PNG_CRC_ERROR_QUIT
#undef PNG_FREE_SPLT
#undef PNG_INTERLACE_LAST
#undef PNG_FORMAT_GRAY
#undef PNG_LIBPNG_BUILD_RELEASE_STATUS_MASK
#undef PNG_EQUATION_LAST
#undef PNG_FILTER_AVG
#undef PNG_ITXT_COMPRESSION_zTXt
#undef PNG_FORMAT_LINEAR_RGB_ALPHA
#undef PNG_FREE_PCAL
#undef PNG_HANDLE_CHUNK_NEVER
#undef PNG_LIBPNG_BUILD_PRIVATE
#undef PNG_TRANSFORM_STRIP_ALPHA
#undef PNG_TRANSFORM_EXPAND_16
#undef PNG_FREE_UNKN
#undef PNG_COLOR_TYPE_RGB_ALPHA
#undef PNG_FORMAT_FLAG_COLORMAP
#undef PNG_TRANSFORM_PACKING
#undef PNG_FP_HALF
#undef PNG_INFO_sRGB
#undef PNG_FORMAT_ARGB_COLORMAP
#undef PNG_LIBPNG_VER_SONUM
#undef PNG_INTERLACE_NONE
#undef PNG_OFFSET_LAST
#undef PNG_FORMAT_RGBA_COLORMAP
#undef PNG_LIBPNG_VER_BUILD
#undef PNG_COMPRESSION_TYPE_BASE
#undef PNG_FLAG_MNG_EMPTY_PLTE
#undef PNG_sRGB_INTENT_ABSOLUTE
#undef PNG_INFO_sPLT
#undef PNG_HANDLE_CHUNK_IF_SAFE
#undef PNG_ERROR_ACTION_WARN
#undef PNG_TRANSFORM_SWAP_ENDIAN
#undef PNG_BACKGROUND_GAMMA_UNIQUE
#undef PNG_INFO_oFFs
#undef PNG_COLOR_TYPE_GA
#undef PNG_LIBPNG_VER_RELEASE
#undef PNG_OPTION_UNSET
#undef PNG_LIBPNG_VER_SHAREDLIB
#undef PNG_HANDLE_CHUNK_ALWAYS
#undef PNG_FORMAT_FLAG_AFIRST
#undef PNG_FILTER_TYPE_DEFAULT
#undef PNG_IMAGE_FLAG_16BIT_sRGB
#undef PNG_TRANSFORM_SWAP_ALPHA
#undef PNG_IMAGE_VERSION
#undef PNG_IO_READING
#undef PNG_TEXT_COMPRESSION_zTXt
#undef PNG_FORMAT_FLAG_ASSOCIATED_ALPHA
#undef PNG_TRANSFORM_IDENTITY
#undef PNG_FREE_ROWS
#undef PNG_FLAG_MNG_FILTER_64
#undef PNG_FILLER_AFTER
#undef PNG_DESTROY_WILL_FREE_DATA
#undef PNG_SET_WILL_FREE_DATA
#undef PNG_ALPHA_STANDARD
#undef PNG_FORMAT_RGB
#undef PNG_INFO_pCAL
#undef PNG_TRANSFORM_STRIP_16
#undef PNG_FORMAT_ARGB
#undef PNG_TRANSFORM_STRIP_FILLER_AFTER
#undef PNG_INFO_IDAT
#undef PNG_IO_WRITING
#undef PNG_MAXIMUM_INFLATE_WINDOW
#undef PNG_FREE_EXIF
#undef PNG_FREE_PLTE
#undef PNG_OPTION_INVALID
#undef PNG_TRANSFORM_INVERT_ALPHA
#undef PNG_COLOR_TYPE_GRAY_ALPHA
#undef PNG_CRC_WARN_DISCARD
#undef PNG_FORMAT_RGB_COLORMAP
#undef PNG_IMAGE_ERROR
#undef PNG_IO_SIGNATURE
#undef PNG_INFO_tIME
#undef PNG_SCALE_LAST
#undef PNG_IO_CHUNK_DATA
#undef PNG_COLOR_MASK_COLOR
#undef PNG_IMAGE_FLAG_FAST
#undef PNG_OFFSET_PIXEL
#undef PNG_FILTER_TYPE_BASE
#undef PNG_FORMAT_GA
#undef PNG_GAMMA_MAC_18
#undef PNG_LIBPNG_BUILD_STABLE
#undef PNG_FILTER_VALUE_PAETH
#undef PNG_OPTION_NEXT
#undef PNG_COLOR_MASK_ALPHA
#undef PNG_INFO_iCCP
#undef PNG_INFO_sBIT
#undef PNG_SCALE_RADIAN
#undef PNG_RESOLUTION_METER
#undef PNG_CRC_NO_CHANGE
#undef PNG_FILTER_SUB
#undef PNG_FORMAT_BGR
#undef PNG_HEADER_VERSION_STRING
#undef PNG_INFO_sCAL
#undef PNG_FORMAT_FLAG_COLOR
#undef PNG_ALPHA_ASSOCIATED
#undef PNG_FP_1
#undef PNG_ERROR_ACTION_ERROR
#undef PNG_EQUATION_ARBITRARY
#undef PNG_KEYWORD_MAX_LENGTH
#undef PNG_OPTION_ON
#undef PNG_SKIP_sRGB_CHECK_PROFILE
#undef PNG_IO_CHUNK_HDR
#undef PNG_TEXT_COMPRESSION_zTXt_WR
#undef PNG_SCALE_UNKNOWN
#undef PNG_FORMAT_FLAG_LINEAR
#undef PNG_FORMAT_BGRA_COLORMAP
#undef PNG_INTRAPIXEL_DIFFERENCING
#undef PNG_LIBPNG_BUILD_TYPE
#undef PNG_LIBPNG_BUILD_BASE_TYPE
#undef PNG_INFO_pHYs
#undef PNG_DEFAULT_sRGB
#undef PNG_EQUATION_BASE_E
#undef PNG_LIBPNG_BUILD_PATCH
#undef PNG_IO_NONE
#undef PNG_INFO_gAMA
#undef PNG_GAMMA_LINEAR
#undef PNG_FILTER_NONE
#undef PNG_OPTION_OFF
#undef PNG_ALPHA_OPTIMIZED
#undef PNG_GAMMA_sRGB
#undef PNG_sRGB_INTENT_RELATIVE
#undef PNG_FP_MIN
#undef PNG_LIBPNG_VER_STRING
#undef PNG_EQUATION_LINEAR
#undef PNG_HAVE_PLTE
#undef PNG_FORMAT_LINEAR_RGB
#undef PNG_SCALE_METER
#undef PNG_FORMAT_ABGR_COLORMAP
#undef PNG_FILTER_HEURISTIC_UNWEIGHTED
#undef PNG_FORMAT_RGBA
#undef PNG_UINT_31_MAX
#undef PNG_INFO_hIST
#undef PNG_MAX_PALETTE_LENGTH
#undef PNG_RESOLUTION_LAST
#undef PNG_TRANSFORM_EXPAND
#undef PNG_FREE_ICCP
#undef PNG_ALL_FILTERS
#undef PNG_BACKGROUND_GAMMA_SCREEN
#undef PNG_FREE_SCAL
#undef PNG_FILTER_VALUE_NONE
#undef PNG_UINT_32_MAX
#undef PNG_ALPHA_PREMULTIPLIED
#undef PNG_LIBPNG_VER
#undef PNG_CRC_WARN_USE
#undef PNG_AFTER_IDAT
#undef PNG_FORMAT_BGRA
#undef PNG_ERROR_ACTION_NONE
#undef PNG_LIBPNG_BUILD_ALPHA
#undef PNG_LIBPNG_VER_MAJOR
#undef PNG_GAMMA_THRESHOLD
#undef PNG_TRANSFORM_STRIP_FILLER_BEFORE
#undef PNG_IMAGE_SAMPLE_CHANNELS
#undef PNG_IMAGE_SAMPLE_COMPONENT_SIZE
#undef PNG_IMAGE_SAMPLE_SIZE
#undef PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS
#undef PNG_IMAGE_PIXEL_
#undef PNG_IMAGE_PIXEL_CHANNELS
#undef PNG_IMAGE_PIXEL_COMPONENT_SIZE
#undef PNG_IMAGE_PIXEL_SIZE
#undef PNG_IMAGE_ROW_STRIDE
#undef PNG_IMAGE_BUFFER_SIZE
#undef PNG_IMAGE_SIZE
#undef PNG_IMAGE_COLORMAP_SIZE
#undef PNG_IMAGE_FAILED
#undef PNG_IMAGE_DATA_SIZE
#undef PNG_ZLIB_MAX_SIZE
#undef PNG_IMAGE_COMPRESSED_SIZE_MAX
#undef PNG_IMAGE_PNG_SIZE_MAX_
#undef PNG_IMAGE_PNG_SIZE_MAX
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
#undef PNG_PASS_MASK
#undef PNG_ROW_IN_INTERLACE_PASS
#undef PNG_COL_IN_INTERLACE_PASS
#undef PNG_get_uint_32
#undef PNG_get_uint_16
#undef PNG_get_int_32
#undef png_composite
#undef png_composite_16
#undef png_check_sig
#undef png_image_write_get_memory_size
#undef png_jmpbuf
#undef png_libpng_ver
// The option png_composite reads, from the :options partition.
#undef PNG_READ_COMPOSITE_NODIV_SUPPORTED

export module mcpplibs.mwrap.libpng.macros;

export import :options;

namespace c = mwrap_libpng_capture;

export namespace mcpplibs::mwrap::libpng::macros {

// ── constants ────────────────────────────────────────────────────────────
inline constexpr auto PNG_DEFAULT_READ_MACROS = c::value_PNG_DEFAULT_READ_MACROS;
inline constexpr auto PNG_QUANTIZE_BLUE_BITS = c::value_PNG_QUANTIZE_BLUE_BITS;
inline constexpr auto PNG_sCAL_PRECISION = c::value_PNG_sCAL_PRECISION;
inline constexpr auto PNG_API_RULE = c::value_PNG_API_RULE;
inline constexpr auto PNG_USER_WIDTH_MAX = c::value_PNG_USER_WIDTH_MAX;
inline constexpr auto PNG_INFLATE_BUF_SIZE = c::value_PNG_INFLATE_BUF_SIZE;
inline constexpr auto PNG_GAMMA_THRESHOLD_FIXED = c::value_PNG_GAMMA_THRESHOLD_FIXED;
inline constexpr auto PNG_USER_HEIGHT_MAX = c::value_PNG_USER_HEIGHT_MAX;
inline constexpr auto PNG_TEXT_Z_DEFAULT_COMPRESSION = c::value_PNG_TEXT_Z_DEFAULT_COMPRESSION;
inline constexpr auto PNG_ZLIB_VERNUM = c::value_PNG_ZLIB_VERNUM;
inline constexpr auto PNG_MAX_GAMMA_8 = c::value_PNG_MAX_GAMMA_8;
inline constexpr auto PNG_IDAT_READ_SIZE = c::value_PNG_IDAT_READ_SIZE;
inline constexpr auto PNG_ZBUF_SIZE = c::value_PNG_ZBUF_SIZE;
inline constexpr auto PNG_USER_CHUNK_MALLOC_MAX = c::value_PNG_USER_CHUNK_MALLOC_MAX;
inline constexpr auto PNG_Z_DEFAULT_STRATEGY = c::value_PNG_Z_DEFAULT_STRATEGY;
inline constexpr auto PNG_QUANTIZE_GREEN_BITS = c::value_PNG_QUANTIZE_GREEN_BITS;
inline constexpr auto PNG_Z_DEFAULT_COMPRESSION = c::value_PNG_Z_DEFAULT_COMPRESSION;
inline constexpr auto PNG_USER_CHUNK_CACHE_MAX = c::value_PNG_USER_CHUNK_CACHE_MAX;
inline constexpr auto PNG_Z_DEFAULT_NOFILTER_STRATEGY = c::value_PNG_Z_DEFAULT_NOFILTER_STRATEGY;
inline constexpr auto PNG_QUANTIZE_RED_BITS = c::value_PNG_QUANTIZE_RED_BITS;
inline constexpr auto PNG_TEXT_Z_DEFAULT_STRATEGY = c::value_PNG_TEXT_Z_DEFAULT_STRATEGY;
inline constexpr auto PNG_sRGB_PROFILE_CHECKS = c::value_PNG_sRGB_PROFILE_CHECKS;
inline constexpr auto PNG_TRANSFORM_GRAY_TO_RGB = c::value_PNG_TRANSFORM_GRAY_TO_RGB;
inline constexpr auto PNG_FREE_ALL = c::value_PNG_FREE_ALL;
inline constexpr auto PNG_HANDLE_CHUNK_AS_DEFAULT = c::value_PNG_HANDLE_CHUNK_AS_DEFAULT;
inline constexpr auto PNG_INFO_PLTE = c::value_PNG_INFO_PLTE;
inline constexpr auto PNG_TRANSFORM_SHIFT = c::value_PNG_TRANSFORM_SHIFT;
inline constexpr auto PNG_CRC_QUIET_USE = c::value_PNG_CRC_QUIET_USE;
inline constexpr auto PNG_CRC_DEFAULT = c::value_PNG_CRC_DEFAULT;
inline constexpr auto PNG_SIZE_MAX = c::value_PNG_SIZE_MAX;
inline constexpr auto PNG_HANDLE_CHUNK_LAST = c::value_PNG_HANDLE_CHUNK_LAST;
inline constexpr auto PNG_TRANSFORM_PACKSWAP = c::value_PNG_TRANSFORM_PACKSWAP;
inline constexpr auto PNG_FILTER_UP = c::value_PNG_FILTER_UP;
inline constexpr auto PNG_FILTER_VALUE_LAST = c::value_PNG_FILTER_VALUE_LAST;
inline constexpr auto PNG_sRGB_INTENT_PERCEPTUAL = c::value_PNG_sRGB_INTENT_PERCEPTUAL;
inline constexpr auto PNG_IO_MASK_LOC = c::value_PNG_IO_MASK_LOC;
inline constexpr auto PNG_INTERLACE_ADAM7_PASSES = c::value_PNG_INTERLACE_ADAM7_PASSES;
inline constexpr auto PNG_LIBPNG_BUILD_SPECIAL = c::value_PNG_LIBPNG_BUILD_SPECIAL;
inline constexpr auto PNG_COLOR_TYPE_RGB = c::value_PNG_COLOR_TYPE_RGB;
inline constexpr auto PNG_FREE_HIST = c::value_PNG_FREE_HIST;
inline constexpr auto PNG_IMAGE_WARNING = c::value_PNG_IMAGE_WARNING;
inline constexpr auto PNG_EQUATION_HYPERBOLIC = c::value_PNG_EQUATION_HYPERBOLIC;
inline constexpr auto PNG_BACKGROUND_GAMMA_FILE = c::value_PNG_BACKGROUND_GAMMA_FILE;
inline constexpr auto PNG_USER_WILL_FREE_DATA = c::value_PNG_USER_WILL_FREE_DATA;
inline constexpr auto PNG_FILLER_BEFORE = c::value_PNG_FILLER_BEFORE;
inline constexpr auto PNG_COMPRESSION_TYPE_DEFAULT = c::value_PNG_COMPRESSION_TYPE_DEFAULT;
inline constexpr auto PNG_FREE_TEXT = c::value_PNG_FREE_TEXT;
inline constexpr auto PNG_LIBPNG_VER_DLLNUM = c::value_PNG_LIBPNG_VER_DLLNUM;
inline constexpr auto PNG_NO_FILTERS = c::value_PNG_NO_FILTERS;
inline constexpr auto PNG_INFO_tRNS = c::value_PNG_INFO_tRNS;
inline constexpr auto PNG_FILTER_HEURISTIC_LAST = c::value_PNG_FILTER_HEURISTIC_LAST;
inline constexpr auto PNG_FP_MAX = c::value_PNG_FP_MAX;
inline constexpr auto PNG_LIBPNG_BUILD_RC = c::value_PNG_LIBPNG_BUILD_RC;
inline constexpr auto PNG_sRGB_INTENT_SATURATION = c::value_PNG_sRGB_INTENT_SATURATION;
inline constexpr auto PNG_TRANSFORM_SCALE_16 = c::value_PNG_TRANSFORM_SCALE_16;
inline constexpr auto PNG_IO_CHUNK_CRC = c::value_PNG_IO_CHUNK_CRC;
inline constexpr auto PNG_FORMAT_FLAG_BGR = c::value_PNG_FORMAT_FLAG_BGR;
inline constexpr auto PNG_COLOR_MASK_PALETTE = c::value_PNG_COLOR_MASK_PALETTE;
inline constexpr auto PNG_FILTER_HEURISTIC_DEFAULT = c::value_PNG_FILTER_HEURISTIC_DEFAULT;
inline constexpr auto PNG_BACKGROUND_GAMMA_UNKNOWN = c::value_PNG_BACKGROUND_GAMMA_UNKNOWN;
inline constexpr auto PNG_TEXT_COMPRESSION_LAST = c::value_PNG_TEXT_COMPRESSION_LAST;
inline constexpr auto PNG_TEXT_COMPRESSION_NONE = c::value_PNG_TEXT_COMPRESSION_NONE;
inline constexpr auto PNG_RESOLUTION_UNKNOWN = c::value_PNG_RESOLUTION_UNKNOWN;
inline constexpr auto PNG_FILTER_VALUE_AVG = c::value_PNG_FILTER_VALUE_AVG;
inline constexpr auto PNG_FORMAT_LINEAR_Y = c::value_PNG_FORMAT_LINEAR_Y;
inline constexpr auto PNG_FILTER_HEURISTIC_WEIGHTED = c::value_PNG_FILTER_HEURISTIC_WEIGHTED;
inline constexpr auto PNG_TRANSFORM_BGR = c::value_PNG_TRANSFORM_BGR;
inline constexpr auto PNG_FORMAT_AG = c::value_PNG_FORMAT_AG;
inline constexpr auto PNG_LIBPNG_VER_MINOR = c::value_PNG_LIBPNG_VER_MINOR;
inline constexpr auto PNG_ALPHA_PNG = c::value_PNG_ALPHA_PNG;
inline constexpr auto PNG_FILTER_PAETH = c::value_PNG_FILTER_PAETH;
inline constexpr auto PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB = c::value_PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB;
inline constexpr auto PNG_TRANSFORM_INVERT_MONO = c::value_PNG_TRANSFORM_INVERT_MONO;
inline constexpr auto PNG_FORMAT_LINEAR_Y_ALPHA = c::value_PNG_FORMAT_LINEAR_Y_ALPHA;
inline constexpr auto PNG_sRGB_INTENT_LAST = c::value_PNG_sRGB_INTENT_LAST;
inline constexpr auto PNG_FREE_TRNS = c::value_PNG_FREE_TRNS;
inline constexpr auto PNG_TRANSFORM_STRIP_FILLER = c::value_PNG_TRANSFORM_STRIP_FILLER;
inline constexpr auto PNG_ALL_MNG_FEATURES = c::value_PNG_ALL_MNG_FEATURES;
inline constexpr auto PNG_INTERLACE_ADAM7 = c::value_PNG_INTERLACE_ADAM7;
inline constexpr auto PNG_RGB_TO_GRAY_DEFAULT = c::value_PNG_RGB_TO_GRAY_DEFAULT;
inline constexpr auto PNG_ALPHA_BROKEN = c::value_PNG_ALPHA_BROKEN;
inline constexpr auto PNG_FORMAT_FLAG_ALPHA = c::value_PNG_FORMAT_FLAG_ALPHA;
inline constexpr auto PNG_OFFSET_MICROMETER = c::value_PNG_OFFSET_MICROMETER;
inline constexpr auto PNG_LIBPNG_BUILD_BETA = c::value_PNG_LIBPNG_BUILD_BETA;
inline constexpr auto PNG_FAST_FILTERS = c::value_PNG_FAST_FILTERS;
inline constexpr auto PNG_COLOR_TYPE_RGBA = c::value_PNG_COLOR_TYPE_RGBA;
inline constexpr auto PNG_INFO_eXIf = c::value_PNG_INFO_eXIf;
inline constexpr auto PNG_TEXT_COMPRESSION_NONE_WR = c::value_PNG_TEXT_COMPRESSION_NONE_WR;
inline constexpr auto PNG_FILTER_VALUE_UP = c::value_PNG_FILTER_VALUE_UP;
inline constexpr auto PNG_FILTER_VALUE_SUB = c::value_PNG_FILTER_VALUE_SUB;
inline constexpr auto PNG_IO_MASK_OP = c::value_PNG_IO_MASK_OP;
inline constexpr auto PNG_INFO_cHRM = c::value_PNG_INFO_cHRM;
inline constexpr auto PNG_ITXT_COMPRESSION_NONE = c::value_PNG_ITXT_COMPRESSION_NONE;
inline constexpr auto PNG_COLOR_TYPE_PALETTE = c::value_PNG_COLOR_TYPE_PALETTE;
inline constexpr auto PNG_COLOR_TYPE_GRAY = c::value_PNG_COLOR_TYPE_GRAY;
inline constexpr auto PNG_FORMAT_ABGR = c::value_PNG_FORMAT_ABGR;
inline constexpr auto PNG_FREE_MUL = c::value_PNG_FREE_MUL;
inline constexpr auto PNG_INFO_bKGD = c::value_PNG_INFO_bKGD;
inline constexpr auto PNG_FORMAT_BGR_COLORMAP = c::value_PNG_FORMAT_BGR_COLORMAP;
inline constexpr auto PNG_HAVE_IHDR = c::value_PNG_HAVE_IHDR;
inline constexpr auto PNG_CRC_ERROR_QUIT = c::value_PNG_CRC_ERROR_QUIT;
inline constexpr auto PNG_FREE_SPLT = c::value_PNG_FREE_SPLT;
inline constexpr auto PNG_INTERLACE_LAST = c::value_PNG_INTERLACE_LAST;
inline constexpr auto PNG_FORMAT_GRAY = c::value_PNG_FORMAT_GRAY;
inline constexpr auto PNG_LIBPNG_BUILD_RELEASE_STATUS_MASK = c::value_PNG_LIBPNG_BUILD_RELEASE_STATUS_MASK;
inline constexpr auto PNG_EQUATION_LAST = c::value_PNG_EQUATION_LAST;
inline constexpr auto PNG_FILTER_AVG = c::value_PNG_FILTER_AVG;
inline constexpr auto PNG_ITXT_COMPRESSION_zTXt = c::value_PNG_ITXT_COMPRESSION_zTXt;
inline constexpr auto PNG_FORMAT_LINEAR_RGB_ALPHA = c::value_PNG_FORMAT_LINEAR_RGB_ALPHA;
inline constexpr auto PNG_FREE_PCAL = c::value_PNG_FREE_PCAL;
inline constexpr auto PNG_HANDLE_CHUNK_NEVER = c::value_PNG_HANDLE_CHUNK_NEVER;
inline constexpr auto PNG_LIBPNG_BUILD_PRIVATE = c::value_PNG_LIBPNG_BUILD_PRIVATE;
inline constexpr auto PNG_TRANSFORM_STRIP_ALPHA = c::value_PNG_TRANSFORM_STRIP_ALPHA;
inline constexpr auto PNG_TRANSFORM_EXPAND_16 = c::value_PNG_TRANSFORM_EXPAND_16;
inline constexpr auto PNG_FREE_UNKN = c::value_PNG_FREE_UNKN;
inline constexpr auto PNG_COLOR_TYPE_RGB_ALPHA = c::value_PNG_COLOR_TYPE_RGB_ALPHA;
inline constexpr auto PNG_FORMAT_FLAG_COLORMAP = c::value_PNG_FORMAT_FLAG_COLORMAP;
inline constexpr auto PNG_TRANSFORM_PACKING = c::value_PNG_TRANSFORM_PACKING;
inline constexpr auto PNG_FP_HALF = c::value_PNG_FP_HALF;
inline constexpr auto PNG_INFO_sRGB = c::value_PNG_INFO_sRGB;
inline constexpr auto PNG_FORMAT_ARGB_COLORMAP = c::value_PNG_FORMAT_ARGB_COLORMAP;
inline constexpr auto PNG_LIBPNG_VER_SONUM = c::value_PNG_LIBPNG_VER_SONUM;
inline constexpr auto PNG_INTERLACE_NONE = c::value_PNG_INTERLACE_NONE;
inline constexpr auto PNG_OFFSET_LAST = c::value_PNG_OFFSET_LAST;
inline constexpr auto PNG_FORMAT_RGBA_COLORMAP = c::value_PNG_FORMAT_RGBA_COLORMAP;
inline constexpr auto PNG_LIBPNG_VER_BUILD = c::value_PNG_LIBPNG_VER_BUILD;
inline constexpr auto PNG_COMPRESSION_TYPE_BASE = c::value_PNG_COMPRESSION_TYPE_BASE;
inline constexpr auto PNG_FLAG_MNG_EMPTY_PLTE = c::value_PNG_FLAG_MNG_EMPTY_PLTE;
inline constexpr auto PNG_sRGB_INTENT_ABSOLUTE = c::value_PNG_sRGB_INTENT_ABSOLUTE;
inline constexpr auto PNG_INFO_sPLT = c::value_PNG_INFO_sPLT;
inline constexpr auto PNG_HANDLE_CHUNK_IF_SAFE = c::value_PNG_HANDLE_CHUNK_IF_SAFE;
inline constexpr auto PNG_ERROR_ACTION_WARN = c::value_PNG_ERROR_ACTION_WARN;
inline constexpr auto PNG_TRANSFORM_SWAP_ENDIAN = c::value_PNG_TRANSFORM_SWAP_ENDIAN;
inline constexpr auto PNG_BACKGROUND_GAMMA_UNIQUE = c::value_PNG_BACKGROUND_GAMMA_UNIQUE;
inline constexpr auto PNG_INFO_oFFs = c::value_PNG_INFO_oFFs;
inline constexpr auto PNG_COLOR_TYPE_GA = c::value_PNG_COLOR_TYPE_GA;
inline constexpr auto PNG_LIBPNG_VER_RELEASE = c::value_PNG_LIBPNG_VER_RELEASE;
inline constexpr auto PNG_OPTION_UNSET = c::value_PNG_OPTION_UNSET;
inline constexpr auto PNG_LIBPNG_VER_SHAREDLIB = c::value_PNG_LIBPNG_VER_SHAREDLIB;
inline constexpr auto PNG_HANDLE_CHUNK_ALWAYS = c::value_PNG_HANDLE_CHUNK_ALWAYS;
inline constexpr auto PNG_FORMAT_FLAG_AFIRST = c::value_PNG_FORMAT_FLAG_AFIRST;
inline constexpr auto PNG_FILTER_TYPE_DEFAULT = c::value_PNG_FILTER_TYPE_DEFAULT;
inline constexpr auto PNG_IMAGE_FLAG_16BIT_sRGB = c::value_PNG_IMAGE_FLAG_16BIT_sRGB;
inline constexpr auto PNG_TRANSFORM_SWAP_ALPHA = c::value_PNG_TRANSFORM_SWAP_ALPHA;
inline constexpr auto PNG_IMAGE_VERSION = c::value_PNG_IMAGE_VERSION;
inline constexpr auto PNG_IO_READING = c::value_PNG_IO_READING;
inline constexpr auto PNG_TEXT_COMPRESSION_zTXt = c::value_PNG_TEXT_COMPRESSION_zTXt;
inline constexpr auto PNG_FORMAT_FLAG_ASSOCIATED_ALPHA = c::value_PNG_FORMAT_FLAG_ASSOCIATED_ALPHA;
inline constexpr auto PNG_TRANSFORM_IDENTITY = c::value_PNG_TRANSFORM_IDENTITY;
inline constexpr auto PNG_FREE_ROWS = c::value_PNG_FREE_ROWS;
inline constexpr auto PNG_FLAG_MNG_FILTER_64 = c::value_PNG_FLAG_MNG_FILTER_64;
inline constexpr auto PNG_FILLER_AFTER = c::value_PNG_FILLER_AFTER;
inline constexpr auto PNG_DESTROY_WILL_FREE_DATA = c::value_PNG_DESTROY_WILL_FREE_DATA;
inline constexpr auto PNG_SET_WILL_FREE_DATA = c::value_PNG_SET_WILL_FREE_DATA;
inline constexpr auto PNG_ALPHA_STANDARD = c::value_PNG_ALPHA_STANDARD;
inline constexpr auto PNG_FORMAT_RGB = c::value_PNG_FORMAT_RGB;
inline constexpr auto PNG_INFO_pCAL = c::value_PNG_INFO_pCAL;
inline constexpr auto PNG_TRANSFORM_STRIP_16 = c::value_PNG_TRANSFORM_STRIP_16;
inline constexpr auto PNG_FORMAT_ARGB = c::value_PNG_FORMAT_ARGB;
inline constexpr auto PNG_TRANSFORM_STRIP_FILLER_AFTER = c::value_PNG_TRANSFORM_STRIP_FILLER_AFTER;
inline constexpr auto PNG_INFO_IDAT = c::value_PNG_INFO_IDAT;
inline constexpr auto PNG_IO_WRITING = c::value_PNG_IO_WRITING;
inline constexpr auto PNG_MAXIMUM_INFLATE_WINDOW = c::value_PNG_MAXIMUM_INFLATE_WINDOW;
inline constexpr auto PNG_FREE_EXIF = c::value_PNG_FREE_EXIF;
inline constexpr auto PNG_FREE_PLTE = c::value_PNG_FREE_PLTE;
inline constexpr auto PNG_OPTION_INVALID = c::value_PNG_OPTION_INVALID;
inline constexpr auto PNG_TRANSFORM_INVERT_ALPHA = c::value_PNG_TRANSFORM_INVERT_ALPHA;
inline constexpr auto PNG_COLOR_TYPE_GRAY_ALPHA = c::value_PNG_COLOR_TYPE_GRAY_ALPHA;
inline constexpr auto PNG_CRC_WARN_DISCARD = c::value_PNG_CRC_WARN_DISCARD;
inline constexpr auto PNG_FORMAT_RGB_COLORMAP = c::value_PNG_FORMAT_RGB_COLORMAP;
inline constexpr auto PNG_IMAGE_ERROR = c::value_PNG_IMAGE_ERROR;
inline constexpr auto PNG_IO_SIGNATURE = c::value_PNG_IO_SIGNATURE;
inline constexpr auto PNG_INFO_tIME = c::value_PNG_INFO_tIME;
inline constexpr auto PNG_SCALE_LAST = c::value_PNG_SCALE_LAST;
inline constexpr auto PNG_IO_CHUNK_DATA = c::value_PNG_IO_CHUNK_DATA;
inline constexpr auto PNG_COLOR_MASK_COLOR = c::value_PNG_COLOR_MASK_COLOR;
inline constexpr auto PNG_IMAGE_FLAG_FAST = c::value_PNG_IMAGE_FLAG_FAST;
inline constexpr auto PNG_OFFSET_PIXEL = c::value_PNG_OFFSET_PIXEL;
inline constexpr auto PNG_FILTER_TYPE_BASE = c::value_PNG_FILTER_TYPE_BASE;
inline constexpr auto PNG_FORMAT_GA = c::value_PNG_FORMAT_GA;
inline constexpr auto PNG_GAMMA_MAC_18 = c::value_PNG_GAMMA_MAC_18;
inline constexpr auto PNG_LIBPNG_BUILD_STABLE = c::value_PNG_LIBPNG_BUILD_STABLE;
inline constexpr auto PNG_FILTER_VALUE_PAETH = c::value_PNG_FILTER_VALUE_PAETH;
inline constexpr auto PNG_OPTION_NEXT = c::value_PNG_OPTION_NEXT;
inline constexpr auto PNG_COLOR_MASK_ALPHA = c::value_PNG_COLOR_MASK_ALPHA;
inline constexpr auto PNG_INFO_iCCP = c::value_PNG_INFO_iCCP;
inline constexpr auto PNG_INFO_sBIT = c::value_PNG_INFO_sBIT;
inline constexpr auto PNG_SCALE_RADIAN = c::value_PNG_SCALE_RADIAN;
inline constexpr auto PNG_RESOLUTION_METER = c::value_PNG_RESOLUTION_METER;
inline constexpr auto PNG_CRC_NO_CHANGE = c::value_PNG_CRC_NO_CHANGE;
inline constexpr auto PNG_FILTER_SUB = c::value_PNG_FILTER_SUB;
inline constexpr auto PNG_FORMAT_BGR = c::value_PNG_FORMAT_BGR;
inline constexpr auto& PNG_HEADER_VERSION_STRING = c::value_PNG_HEADER_VERSION_STRING;
inline constexpr auto PNG_INFO_sCAL = c::value_PNG_INFO_sCAL;
inline constexpr auto PNG_FORMAT_FLAG_COLOR = c::value_PNG_FORMAT_FLAG_COLOR;
inline constexpr auto PNG_ALPHA_ASSOCIATED = c::value_PNG_ALPHA_ASSOCIATED;
inline constexpr auto PNG_FP_1 = c::value_PNG_FP_1;
inline constexpr auto PNG_ERROR_ACTION_ERROR = c::value_PNG_ERROR_ACTION_ERROR;
inline constexpr auto PNG_EQUATION_ARBITRARY = c::value_PNG_EQUATION_ARBITRARY;
inline constexpr auto PNG_KEYWORD_MAX_LENGTH = c::value_PNG_KEYWORD_MAX_LENGTH;
inline constexpr auto PNG_OPTION_ON = c::value_PNG_OPTION_ON;
inline constexpr auto PNG_SKIP_sRGB_CHECK_PROFILE = c::value_PNG_SKIP_sRGB_CHECK_PROFILE;
inline constexpr auto PNG_IO_CHUNK_HDR = c::value_PNG_IO_CHUNK_HDR;
inline constexpr auto PNG_TEXT_COMPRESSION_zTXt_WR = c::value_PNG_TEXT_COMPRESSION_zTXt_WR;
inline constexpr auto PNG_SCALE_UNKNOWN = c::value_PNG_SCALE_UNKNOWN;
inline constexpr auto PNG_FORMAT_FLAG_LINEAR = c::value_PNG_FORMAT_FLAG_LINEAR;
inline constexpr auto PNG_FORMAT_BGRA_COLORMAP = c::value_PNG_FORMAT_BGRA_COLORMAP;
inline constexpr auto PNG_INTRAPIXEL_DIFFERENCING = c::value_PNG_INTRAPIXEL_DIFFERENCING;
inline constexpr auto PNG_LIBPNG_BUILD_TYPE = c::value_PNG_LIBPNG_BUILD_TYPE;
inline constexpr auto PNG_LIBPNG_BUILD_BASE_TYPE = c::value_PNG_LIBPNG_BUILD_BASE_TYPE;
inline constexpr auto PNG_INFO_pHYs = c::value_PNG_INFO_pHYs;
inline constexpr auto PNG_DEFAULT_sRGB = c::value_PNG_DEFAULT_sRGB;
inline constexpr auto PNG_EQUATION_BASE_E = c::value_PNG_EQUATION_BASE_E;
inline constexpr auto PNG_LIBPNG_BUILD_PATCH = c::value_PNG_LIBPNG_BUILD_PATCH;
inline constexpr auto PNG_IO_NONE = c::value_PNG_IO_NONE;
inline constexpr auto PNG_INFO_gAMA = c::value_PNG_INFO_gAMA;
inline constexpr auto PNG_GAMMA_LINEAR = c::value_PNG_GAMMA_LINEAR;
inline constexpr auto PNG_FILTER_NONE = c::value_PNG_FILTER_NONE;
inline constexpr auto PNG_OPTION_OFF = c::value_PNG_OPTION_OFF;
inline constexpr auto PNG_ALPHA_OPTIMIZED = c::value_PNG_ALPHA_OPTIMIZED;
inline constexpr auto PNG_GAMMA_sRGB = c::value_PNG_GAMMA_sRGB;
inline constexpr auto PNG_sRGB_INTENT_RELATIVE = c::value_PNG_sRGB_INTENT_RELATIVE;
inline constexpr auto PNG_FP_MIN = c::value_PNG_FP_MIN;
inline constexpr auto& PNG_LIBPNG_VER_STRING = c::value_PNG_LIBPNG_VER_STRING;
inline constexpr auto PNG_EQUATION_LINEAR = c::value_PNG_EQUATION_LINEAR;
inline constexpr auto PNG_HAVE_PLTE = c::value_PNG_HAVE_PLTE;
inline constexpr auto PNG_FORMAT_LINEAR_RGB = c::value_PNG_FORMAT_LINEAR_RGB;
inline constexpr auto PNG_SCALE_METER = c::value_PNG_SCALE_METER;
inline constexpr auto PNG_FORMAT_ABGR_COLORMAP = c::value_PNG_FORMAT_ABGR_COLORMAP;
inline constexpr auto PNG_FILTER_HEURISTIC_UNWEIGHTED = c::value_PNG_FILTER_HEURISTIC_UNWEIGHTED;
inline constexpr auto PNG_FORMAT_RGBA = c::value_PNG_FORMAT_RGBA;
inline constexpr auto PNG_UINT_31_MAX = c::value_PNG_UINT_31_MAX;
inline constexpr auto PNG_INFO_hIST = c::value_PNG_INFO_hIST;
inline constexpr auto PNG_MAX_PALETTE_LENGTH = c::value_PNG_MAX_PALETTE_LENGTH;
inline constexpr auto PNG_RESOLUTION_LAST = c::value_PNG_RESOLUTION_LAST;
inline constexpr auto PNG_TRANSFORM_EXPAND = c::value_PNG_TRANSFORM_EXPAND;
inline constexpr auto PNG_FREE_ICCP = c::value_PNG_FREE_ICCP;
inline constexpr auto PNG_ALL_FILTERS = c::value_PNG_ALL_FILTERS;
inline constexpr auto PNG_BACKGROUND_GAMMA_SCREEN = c::value_PNG_BACKGROUND_GAMMA_SCREEN;
inline constexpr auto PNG_FREE_SCAL = c::value_PNG_FREE_SCAL;
inline constexpr auto PNG_FILTER_VALUE_NONE = c::value_PNG_FILTER_VALUE_NONE;
inline constexpr auto PNG_UINT_32_MAX = c::value_PNG_UINT_32_MAX;
inline constexpr auto PNG_ALPHA_PREMULTIPLIED = c::value_PNG_ALPHA_PREMULTIPLIED;
inline constexpr auto PNG_LIBPNG_VER = c::value_PNG_LIBPNG_VER;
inline constexpr auto PNG_CRC_WARN_USE = c::value_PNG_CRC_WARN_USE;
inline constexpr auto PNG_AFTER_IDAT = c::value_PNG_AFTER_IDAT;
inline constexpr auto PNG_FORMAT_BGRA = c::value_PNG_FORMAT_BGRA;
inline constexpr auto PNG_ERROR_ACTION_NONE = c::value_PNG_ERROR_ACTION_NONE;
inline constexpr auto PNG_LIBPNG_BUILD_ALPHA = c::value_PNG_LIBPNG_BUILD_ALPHA;
inline constexpr auto PNG_LIBPNG_VER_MAJOR = c::value_PNG_LIBPNG_VER_MAJOR;
inline constexpr auto PNG_GAMMA_THRESHOLD = c::value_PNG_GAMMA_THRESHOLD;
inline constexpr auto PNG_TRANSFORM_STRIP_FILLER_BEFORE = c::value_PNG_TRANSFORM_STRIP_FILLER_BEFORE;

// ── the simplified API's sizes ───────────────────────────────────────────
// `fmt` is a png_uint_32, as png_image::format is; `image` a png_image. Each
// evaluates its arguments once (the macros evaluate `image` or `fmt` several
// times) and yields the macro's type: png_uint_32, except where the macro's
// result follows a caller's argument (row_stride, image_size, b), which is a
// template parameter here, so the arithmetic is the macro's.
constexpr png_uint_32 PNG_IMAGE_SAMPLE_CHANNELS(png_uint_32 fmt) noexcept {
    return (fmt & (PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_ALPHA)) + 1;
}
constexpr png_uint_32 PNG_IMAGE_SAMPLE_COMPONENT_SIZE(png_uint_32 fmt) noexcept {
    return ((fmt & PNG_FORMAT_FLAG_LINEAR) >> 2) + 1;
}
constexpr png_uint_32 PNG_IMAGE_SAMPLE_SIZE(png_uint_32 fmt) noexcept {
    return PNG_IMAGE_SAMPLE_CHANNELS(fmt) * PNG_IMAGE_SAMPLE_COMPONENT_SIZE(fmt);
}
constexpr png_uint_32 PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(png_uint_32 fmt) noexcept {
    return PNG_IMAGE_SAMPLE_CHANNELS(fmt) * 256;
}
// The macro takes the name of one of the three PNG_IMAGE_SAMPLE_* macros.
constexpr png_uint_32 PNG_IMAGE_PIXEL_(png_uint_32 (*test)(png_uint_32) noexcept, png_uint_32 fmt) noexcept {
    return (fmt & PNG_FORMAT_FLAG_COLORMAP) ? 1 : test(fmt);
}
constexpr png_uint_32 PNG_IMAGE_PIXEL_CHANNELS(png_uint_32 fmt) noexcept {
    return PNG_IMAGE_PIXEL_(PNG_IMAGE_SAMPLE_CHANNELS, fmt);
}
constexpr png_uint_32 PNG_IMAGE_PIXEL_COMPONENT_SIZE(png_uint_32 fmt) noexcept {
    return PNG_IMAGE_PIXEL_(PNG_IMAGE_SAMPLE_COMPONENT_SIZE, fmt);
}
constexpr png_uint_32 PNG_IMAGE_PIXEL_SIZE(png_uint_32 fmt) noexcept {
    return PNG_IMAGE_PIXEL_(PNG_IMAGE_SAMPLE_SIZE, fmt);
}
constexpr png_uint_32 PNG_IMAGE_ROW_STRIDE(const png_image& image) noexcept {
    return PNG_IMAGE_PIXEL_CHANNELS(image.format) * image.width;
}
template <class RowStride>
constexpr auto PNG_IMAGE_BUFFER_SIZE(const png_image& image, RowStride row_stride) noexcept {
    return PNG_IMAGE_PIXEL_COMPONENT_SIZE(image.format) * image.height * row_stride;
}
constexpr png_uint_32 PNG_IMAGE_SIZE(const png_image& image) noexcept {
    return PNG_IMAGE_BUFFER_SIZE(image, PNG_IMAGE_ROW_STRIDE(image));
}
constexpr png_uint_32 PNG_IMAGE_COLORMAP_SIZE(const png_image& image) noexcept {
    return PNG_IMAGE_SAMPLE_SIZE(image.format) * image.colormap_entries;
}
constexpr bool PNG_IMAGE_FAILED(const png_image& png_cntrl) noexcept {
    return ((png_cntrl.warning_or_error) & 0x03) > 1;
}
constexpr png_uint_32 PNG_IMAGE_DATA_SIZE(const png_image& image) noexcept {
    return PNG_IMAGE_SIZE(image) + image.height;
}
// png.h defines it only when the program has not (#ifndef): deflateBound()'s
// bound for zlib itself, which compat.zlib is.
template <class B>
constexpr auto PNG_ZLIB_MAX_SIZE(B b) noexcept {
    return b + ((b + 7U) >> 3) + ((b + 63U) >> 6) + 11U;
}
constexpr png_alloc_size_t PNG_IMAGE_COMPRESSED_SIZE_MAX(const png_image& image) noexcept {
    return PNG_ZLIB_MAX_SIZE(static_cast<png_alloc_size_t>(PNG_IMAGE_DATA_SIZE(image)));
}
template <class ImageSize>
constexpr auto PNG_IMAGE_PNG_SIZE_MAX_(const png_image& image, ImageSize image_size) noexcept {
    return (8U /*sig*/ + 25U /*IHDR*/ + 16U /*gAMA*/ + 44U /*cHRM*/ + 12U /*IEND*/ +
            ((image.format & PNG_FORMAT_FLAG_COLORMAP)
                 ? /*colormap: PLTE, tRNS*/ 12U + 3U * image.colormap_entries /*PLTE data*/ +
                       ((image.format & PNG_FORMAT_FLAG_ALPHA) ? 12U /*tRNS*/ + image.colormap_entries : 0U)
                 : 0U) +
            12U) +
           (12U * (image_size / PNG_ZBUF_SIZE)) /*IDAT*/ + image_size;
}
constexpr png_alloc_size_t PNG_IMAGE_PNG_SIZE_MAX(const png_image& image) noexcept {
    return PNG_IMAGE_PNG_SIZE_MAX_(image, PNG_IMAGE_COMPRESSED_SIZE_MAX(image));
}

// ── Adam7 interlacing ────────────────────────────────────────────────────
// Templates over the argument types, so the arithmetic (and its type) is the
// macro's for whatever integer the caller passes.
template <class Pass>
constexpr auto PNG_PASS_START_ROW(Pass pass) noexcept { return ((1 & ~pass) << (3 - (pass >> 1))) & 7; }
template <class Pass>
constexpr auto PNG_PASS_START_COL(Pass pass) noexcept { return ((1 & pass) << (3 - ((pass + 1) >> 1))) & 7; }
template <class Pass>
constexpr auto PNG_PASS_ROW_OFFSET(Pass pass) noexcept { return pass > 2 ? (8 >> ((pass - 1) >> 1)) : 8; }
template <class Pass>
constexpr auto PNG_PASS_COL_OFFSET(Pass pass) noexcept { return 1 << ((7 - pass) >> 1); }
template <class Pass>
constexpr auto PNG_PASS_ROW_SHIFT(Pass pass) noexcept { return pass > 2 ? (8 - pass) >> 1 : 3; }
template <class Pass>
constexpr auto PNG_PASS_COL_SHIFT(Pass pass) noexcept { return pass > 1 ? (7 - pass) >> 1 : 3; }
template <class Height, class Pass>
constexpr auto PNG_PASS_ROWS(Height height, Pass pass) noexcept {
    return (height + (((1 << PNG_PASS_ROW_SHIFT(pass)) - 1) - PNG_PASS_START_ROW(pass))) >> PNG_PASS_ROW_SHIFT(pass);
}
template <class Width, class Pass>
constexpr auto PNG_PASS_COLS(Width width, Pass pass) noexcept {
    return (width + (((1 << PNG_PASS_COL_SHIFT(pass)) - 1) - PNG_PASS_START_COL(pass))) >> PNG_PASS_COL_SHIFT(pass);
}
template <class Y, class Pass>
constexpr auto PNG_ROW_FROM_PASS_ROW(Y y_in, Pass pass) noexcept {
    return (y_in << PNG_PASS_ROW_SHIFT(pass)) + PNG_PASS_START_ROW(pass);
}
template <class X, class Pass>
constexpr auto PNG_COL_FROM_PASS_COL(X x_in, Pass pass) noexcept {
    return (x_in << PNG_PASS_COL_SHIFT(pass)) + PNG_PASS_START_COL(pass);
}
template <class Pass, class Off>
constexpr auto PNG_PASS_MASK(Pass pass, Off off) noexcept {
    return ((0x110145AF >> (((7 - off) - pass) << 2)) & 0xF) | ((0x01145AF0 >> (((7 - off) - pass) << 2)) & 0xF0);
}
template <class Y, class Pass>
constexpr auto PNG_ROW_IN_INTERLACE_PASS(Y y, Pass pass) noexcept { return (PNG_PASS_MASK(pass, 0) >> (y & 7)) & 1; }
template <class X, class Pass>
constexpr auto PNG_COL_IN_INTERLACE_PASS(X x, Pass pass) noexcept { return (PNG_PASS_MASK(pass, 1) >> (x & 7)) & 1; }

// ── the read macros ──────────────────────────────────────────────────────
// png.h makes png_get_uint_32 / png_get_uint_16 / png_get_int_32 these
// macros (PNG_USE_READ_MACROS); the module exports the library's functions of
// those names, with the same results. `buf` is any pointer the macros take.
template <class Buf>
constexpr png_uint_32 PNG_get_uint_32(Buf buf) noexcept {
    return (static_cast<png_uint_32>(*buf) << 24) + (static_cast<png_uint_32>(*(buf + 1)) << 16) +
           (static_cast<png_uint_32>(*(buf + 2)) << 8) + static_cast<png_uint_32>(*(buf + 3));
}
template <class Buf>
constexpr png_uint_16 PNG_get_uint_16(Buf buf) noexcept {
    return static_cast<png_uint_16>((static_cast<unsigned int>(*buf) << 8) + static_cast<unsigned int>(*(buf + 1)));
}
template <class Buf>
constexpr png_int_32 PNG_get_int_32(Buf buf) noexcept {
    return static_cast<png_int_32>((*buf & 0x80) ? -static_cast<png_int_32>(((PNG_get_uint_32(buf) ^ 0xffffffffU) + 1U) & 0x7fffffffU)
                                                 : static_cast<png_int_32>(PNG_get_uint_32(buf)));
}

// ── compositing (gamma 1.0 space; alpha is the opacity) ──────────────────
// The macros assign to their first argument: here a reference. Which formula
// is png.h's choice by PNG_READ_COMPOSITE_NODIV_SUPPORTED, as here. `alpha` is
// evaluated once (the macros evaluate it twice).
template <class Composite, class Fg, class Alpha, class Bg>
constexpr void png_composite(Composite& composite, Fg fg, Alpha alpha, Bg bg) noexcept {
    if constexpr (PNG_READ_COMPOSITE_NODIV_SUPPORTED) {
        png_uint_16 temp = static_cast<png_uint_16>(static_cast<png_uint_16>(fg) * static_cast<png_uint_16>(alpha) +
                                                    static_cast<png_uint_16>(bg) * static_cast<png_uint_16>(255 - static_cast<png_uint_16>(alpha)) + 128);
        composite = static_cast<png_byte>(((temp + (temp >> 8)) >> 8) & 0xff);
    } else {
        composite = static_cast<png_byte>(0xff & ((static_cast<png_uint_16>(fg) * static_cast<png_uint_16>(alpha) +
                                                   static_cast<png_uint_16>(bg) * static_cast<png_uint_16>(255 - static_cast<png_uint_16>(alpha)) + 127) / 255));
    }
}
template <class Composite, class Fg, class Alpha, class Bg>
constexpr void png_composite_16(Composite& composite, Fg fg, Alpha alpha, Bg bg) noexcept {
    if constexpr (PNG_READ_COMPOSITE_NODIV_SUPPORTED) {
        png_uint_32 temp = static_cast<png_uint_32>(static_cast<png_uint_32>(fg) * static_cast<png_uint_32>(alpha) +
                                                    static_cast<png_uint_32>(bg) * (65535 - static_cast<png_uint_32>(alpha)) + 32768);
        composite = static_cast<png_uint_16>(0xffff & ((temp + (temp >> 16)) >> 16));
    } else {
        composite = static_cast<png_uint_16>(0xffff & ((static_cast<png_uint_32>(fg) * static_cast<png_uint_32>(alpha) +
                                                        static_cast<png_uint_32>(bg) * static_cast<png_uint_32>(65535 - static_cast<png_uint_32>(alpha)) + 32767) / 65535));
    }
}

// ── calls ────────────────────────────────────────────────────────────────
// png_check_sig: the pre-1.0 signature test (DEPRECATED in png.h).
inline bool png_check_sig(png_const_bytep sig, size_t n) { return png_sig_cmp(sig, 0, n) == 0; }

// The memory size png_image_write_to_memory needs for this image: the macro
// passes `&(image)` and `&(size)`, so both are references here.
inline int png_image_write_get_memory_size(png_image& image, png_alloc_size_t& size, int convert_to_8_bit,
                                           const void* buffer, png_int_32 row_stride, const void* colormap) {
    return png_image_write_to_memory(&image, nullptr, &size, convert_to_8_bit, buffer, row_stride, colormap);
}

// The jmp_buf libpng longjmps to on png_error: register the C library's
// longjmp, as the macro does, and return the buffer to `setjmp` in the caller's
// frame — `if (setjmp(png_jmpbuf(png_ptr))) { … }`. setjmp is the C library's
// macro: include <csetjmp> (or <mwrap/libpng/macros.hpp>) before the import.
inline jmp_buf& png_jmpbuf(png_structrp png_ptr) { return *png_set_longjmp_fn(png_ptr, longjmp, sizeof(jmp_buf)); }

// The version string of the library linked: png_get_header_ver(NULL).
inline const png_const_charp png_libpng_ver = png_get_header_ver(nullptr);

}  // namespace mcpplibs::mwrap::libpng::macros

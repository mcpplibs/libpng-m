/* pngtest — libpng's pngtest.c (release 1.6.43), ported to C++23 against
 * `import mcpplibs.mwrap.libpng;` instead of png.h.
 *
 * Copyright (c) 2018-2024 Cosmin Truta
 * Copyright (c) 1998-2002,2004,2006-2018 Glenn Randers-Pehrson
 * Copyright (c) 1996-1997 Andreas Dilger
 * Copyright (c) 1995-1996 Guy Eric Schalnat, Group 42, Inc.
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h (upstream/libpng/LICENSE in this repository).
 *
 * This program reads in a PNG image, writes it out again, and then
 * compares the two files.  If the files are identical, this shows that
 * the basic chunk handling, filtering, and (de)compression code is working
 * properly.  It does not currently test all of the transforms, although
 * it probably should.
 *
 * The port, for mcpplibs.mwrap.libpng (tools/run_conformance.py runs it on
 * the release's test images and compares every outcome with upstream's own
 * pngtest.c, built from the same tarball against png.h):
 *
 * - Every libpng name comes from the module: the API from
 *   mcpplibs.mwrap.libpng, the macros (png_jmpbuf, PNG_LIBPNG_VER_STRING,
 *   PNG_COLOR_TYPE_*, PNG_PASS_*, png_libpng_ver …) from
 *   mcpplibs::mwrap::libpng::macros. png.h is not included; setjmp is the C
 *   library's (<csetjmp>), as is stdout (<cstdio>); ZLIB_VERSION and
 *   Z_FILTERED are zlib's (<zlib.h>).
 * - pngtest.c's `#ifdef PNG_..._SUPPORTED` blocks are resolved for this
 *   build's configuration (compat.libpng's pnglibconf.h), which the
 *   static_asserts below check through the module's option constants:
 *   the stdio-free I/O callbacks, the PNG_DEBUG memory tracer and
 *   PNGTEST_TIMING, all compiled out upstream in this configuration, are
 *   left out.
 * - The C is C++: std:: functions, nullptr, casts. `row_buf` is static, as
 *   fpin and fpout already are upstream, so it keeps its value across the
 *   longjmp to the error handler that frees it.
 * The output is upstream's, byte for byte.
 */
#include <csetjmp>
#include <cstdio>  // stdout, a macro
#include <zlib.h>

import std;
import mcpplibs.mwrap.libpng;

using namespace mcpplibs::mwrap::libpng::macros;

/* Generate a compiler error if there is an old png.h in the search path. */
typedef png_libpng_version_1_6_43 Your_png_h_is_not_version_1_6_43;

/* Ensure that all version numbers in png.h are consistent with one another. */
static_assert(PNG_LIBPNG_VER == PNG_LIBPNG_VER_MAJOR * 10000 + PNG_LIBPNG_VER_MINOR * 100 + PNG_LIBPNG_VER_RELEASE &&
                  PNG_LIBPNG_VER_SHAREDLIB == PNG_LIBPNG_VER_MAJOR * 10 + PNG_LIBPNG_VER_MINOR &&
                  PNG_LIBPNG_VER_SHAREDLIB == PNG_LIBPNG_VER_SONUM && PNG_LIBPNG_VER_SHAREDLIB == PNG_LIBPNG_VER_DLLNUM,
              "Inconsistent version numbers in png.h");

/* The configuration this port is written for: what pngtest.c requires to do
 * anything at all, and every option its kept branches depend on.
 */
static_assert(PNG_READ_SUPPORTED && PNG_READ_bKGD_SUPPORTED && PNG_READ_cHRM_SUPPORTED && PNG_READ_gAMA_SUPPORTED &&
              PNG_READ_oFFs_SUPPORTED && PNG_READ_pCAL_SUPPORTED && PNG_READ_pHYs_SUPPORTED && PNG_READ_sBIT_SUPPORTED &&
              PNG_READ_sCAL_SUPPORTED && PNG_READ_sRGB_SUPPORTED && PNG_READ_sPLT_SUPPORTED && PNG_READ_tEXt_SUPPORTED &&
              PNG_READ_tIME_SUPPORTED && PNG_READ_zTXt_SUPPORTED && PNG_WRITE_INTERLACING_SUPPORTED);
static_assert(PNG_WRITE_SUPPORTED && PNG_WRITE_FILTER_SUPPORTED && PNG_STDIO_SUPPORTED && PNG_SETJMP_SUPPORTED &&
              PNG_BENIGN_ERRORS_SUPPORTED && PNG_TIME_RFC1123_SUPPORTED && PNG_READ_USER_TRANSFORM_SUPPORTED &&
              PNG_WRITE_USER_TRANSFORM_SUPPORTED && PNG_READ_USER_CHUNKS_SUPPORTED && PNG_TEXT_SUPPORTED &&
              PNG_WRITE_zTXt_SUPPORTED && PNG_WRITE_iTXt_SUPPORTED && PNG_FIXED_POINT_SUPPORTED && PNG_cHRM_SUPPORTED &&
              PNG_gAMA_SUPPORTED && PNG_iCCP_SUPPORTED && PNG_sRGB_SUPPORTED && PNG_bKGD_SUPPORTED &&
              PNG_READ_eXIf_SUPPORTED && PNG_WRITE_eXIf_SUPPORTED && PNG_hIST_SUPPORTED && PNG_oFFs_SUPPORTED &&
              PNG_pCAL_SUPPORTED && PNG_pHYs_SUPPORTED && PNG_sBIT_SUPPORTED && PNG_sCAL_SUPPORTED &&
              PNG_FLOATING_POINT_SUPPORTED && PNG_FLOATING_ARITHMETIC_SUPPORTED && PNG_sPLT_SUPPORTED &&
              PNG_tIME_SUPPORTED && PNG_tRNS_SUPPORTED && PNG_SET_UNKNOWN_CHUNKS_SUPPORTED &&
              PNG_SAVE_UNKNOWN_CHUNKS_SUPPORTED && PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED &&
              PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED && PNG_READ_UNKNOWN_CHUNKS_SUPPORTED &&
              PNG_READ_INTERLACING_SUPPORTED && PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED &&
              PNG_EASY_ACCESS_SUPPORTED && PNG_USER_MEM_SUPPORTED);
/* The ones whose code is left out because they are off here. */
static_assert(!PNG_DISABLE_ADLER32_CHECK_SUPPORTED);  // PNG_IGNORE_ADLER32 is not defined

/* This hack was introduced for historical reasons, and we are
 * still keeping it in libpng-1.6.x for compatibility reasons.
 */
#define STDERR stdout

static int tIME_chunk_present = 0;
static char tIME_string[29] = "tIME chunk is not present";
/* This use case is deprecated.
 * See the declaration of png_convert_to_rfc1123_buffer for more details.
 */

static int verbose = 0;
static int strict = 0;
static int relaxed = 0;
static int xfail = 0;
static int unsupported_chunks = 0; /* chunk unsupported by libpng in input */
static int error_count = 0;        /* count calls to png_error */
static int warning_count = 0;      /* count calls to png_warning */

/* Example of using row callbacks to make a simple progress meter */
static int status_pass = 1;
static int status_dots_requested = 0;
static int status_dots = 1;

static void read_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass) {
    /* The callback should always receive correct parameters. */
    if (png_ptr == nullptr) png_error(png_ptr, "read_row_callback: bad png_ptr");
    if (row_number > PNG_UINT_31_MAX) png_error(png_ptr, "read_row_callback: bad row number");
    if (pass < 0 || pass > 7) png_error(png_ptr, "read_row_callback: bad pass");

    if (status_pass != pass) {
        std::fprintf(stdout, "\n Pass %d: ", pass);
        status_pass = pass;
        status_dots = 31;
    }

    status_dots--;

    if (status_dots == 0) {
        std::fprintf(stdout, "\n         ");
        status_dots = 30;
    }

    std::fprintf(stdout, "r");
}

static void write_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass) {
    /* The callback should always receive correct parameters. */
    if (png_ptr == nullptr) png_error(png_ptr, "write_row_callback: bad png_ptr");
    if (row_number > PNG_UINT_31_MAX) png_error(png_ptr, "write_row_callback: bad row number");
    if (pass < 0 || pass > 7) png_error(png_ptr, "write_row_callback: bad pass");

    std::fprintf(stdout, "w");
}

/* Example of using a user transform callback (doesn't do anything at present).
 */
static void read_user_callback(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    /* The callback should always receive correct parameters. */
    if (png_ptr == nullptr) png_error(png_ptr, "read_user_callback: bad png_ptr");
    if (row_info == nullptr) png_error(png_ptr, "read_user_callback: bad row info");
    if (data == nullptr) png_error(png_ptr, "read_user_callback: bad data");
}

/* Example of using user transform callback (we don't transform anything,
 * but merely count the zero samples)
 */

static png_uint_32 zero_samples;

static void count_zero_samples(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    png_bytep dp = data;

    /* The callback should always receive correct parameters. */
    if (png_ptr == nullptr) png_error(png_ptr, "count_zero_samples: bad png_ptr");
    if (row_info == nullptr) png_error(png_ptr, "count_zero_samples: bad row info");
    if (data == nullptr) png_error(png_ptr, "count_zero_samples: bad data");

    /* Contents of row_info:
     *  png_uint_32 width      width of row
     *  png_uint_32 rowbytes   number of bytes in row
     *  png_byte color_type    color type of pixels
     *  png_byte bit_depth     bit depth of samples
     *  png_byte channels      number of channels (1-4)
     *  png_byte pixel_depth   bits per pixel (depth*channels)
     */

    /* Counts the number of zero samples (or zero pixels if color_type is 3 */

    if (row_info->color_type == 0 || row_info->color_type == 3) {
        int pos = 0;
        png_uint_32 n, nstop;

        for (n = 0, nstop = row_info->width; n < nstop; n++) {
            if (row_info->bit_depth == 1) {
                if (((*dp << pos++) & 0x80) == 0) zero_samples++;

                if (pos == 8) {
                    pos = 0;
                    dp++;
                }
            }

            if (row_info->bit_depth == 2) {
                if (((*dp << (pos += 2)) & 0xc0) == 0) zero_samples++;

                if (pos == 8) {
                    pos = 0;
                    dp++;
                }
            }

            if (row_info->bit_depth == 4) {
                if (((*dp << (pos += 4)) & 0xf0) == 0) zero_samples++;

                if (pos == 8) {
                    pos = 0;
                    dp++;
                }
            }

            if (row_info->bit_depth == 8)
                if (*dp++ == 0) zero_samples++;

            if (row_info->bit_depth == 16) {
                if ((*dp | *(dp + 1)) == 0) zero_samples++;
                dp += 2;
            }
        }
    } else /* Other color types */
    {
        png_uint_32 n, nstop;
        int channel;
        int color_channels = row_info->channels;
        if (row_info->color_type > 3) color_channels--;

        for (n = 0, nstop = row_info->width; n < nstop; n++) {
            for (channel = 0; channel < color_channels; channel++) {
                if (row_info->bit_depth == 8)
                    if (*dp++ == 0) zero_samples++;

                if (row_info->bit_depth == 16) {
                    if ((*dp | *(dp + 1)) == 0) zero_samples++;

                    dp += 2;
                }
            }
            if (row_info->color_type > 3) {
                dp++;
                if (row_info->bit_depth == 16) dp++;
            }
        }
    }
}

/* This function is called when there is a warning, but the library thinks
 * it can continue anyway.  Replacement functions don't have to do anything
 * here if you don't want to.  In the default configuration, png_ptr is
 * not used, but it is passed in case it may be useful.
 */
struct pngtest_error_parameters {
    const char* file_name;
};

static void pngtest_warning(png_structp png_ptr, png_const_charp message) {
    const char* name = "UNKNOWN (ERROR!)";
    auto* test = static_cast<pngtest_error_parameters*>(png_get_error_ptr(png_ptr));

    ++warning_count;

    if (test != nullptr && test->file_name != nullptr) name = test->file_name;

    std::fprintf(STDERR, "\n%s: libpng warning: %s\n", name, message);
}

/* This is the default error handling function.  Note that replacements for
 * this function MUST NOT RETURN, or the program will likely crash.  This
 * function is used by default, or if the program supplies NULL for the
 * error function pointer in png_set_error_fn().
 */
static void pngtest_error(png_structp png_ptr, png_const_charp message) {
    ++error_count;

    pngtest_warning(png_ptr, message);
    /* We can return because png_error calls the default handler, which is
     * actually OK in this case.
     */
}

/* Demonstration of user chunk support of the sTER and vpAg chunks */

/* (sTER is a public chunk not yet known by libpng.  vpAg is a private
chunk used in ImageMagick to store "virtual page" size).  */

struct user_chunk_info {
    png_const_infop info_ptr;
    png_uint_32 vpAg_width, vpAg_height;
    png_byte vpAg_units;
    png_byte sTER_mode;
    int location[2];
};

/* Used for location and order; zero means nothing. */
constexpr int have_sTER = 0x01;
constexpr int have_vpAg = 0x02;
constexpr int before_PLTE = 0x10;
constexpr int before_IDAT = 0x20;
constexpr int after_IDAT = 0x40;

static void init_user_chunk_info(png_const_infop info_ptr, user_chunk_info* chunk_data) {
    std::memset(chunk_data, 0, sizeof(*chunk_data));
    chunk_data->info_ptr = info_ptr;
}

static int set_chunk_location(png_structp png_ptr, user_chunk_info* chunk_data, int what) {
    int location;

    if ((chunk_data->location[0] & what) != 0 || (chunk_data->location[1] & what) != 0) return 0; /* we already have one of these */

    /* Find where we are (the code below zeroes info_ptr to indicate that the
     * chunks before the first IDAT have been read.)
     */
    if (chunk_data->info_ptr == nullptr) /* after IDAT */
        location = what | after_IDAT;

    else if (png_get_valid(png_ptr, chunk_data->info_ptr, PNG_INFO_PLTE) != 0)
        location = what | before_IDAT;

    else
        location = what | before_PLTE;

    if (chunk_data->location[0] == 0)
        chunk_data->location[0] = location;

    else
        chunk_data->location[1] = location;

    return 1; /* handled */
}

static int read_user_chunk_callback(png_struct* png_ptr, png_unknown_chunkp chunk) {
    auto* my_user_chunk_data = static_cast<user_chunk_info*>(png_get_user_chunk_ptr(png_ptr));

    if (my_user_chunk_data == nullptr) png_error(png_ptr, "lost pointer to user chunk data");

    /* Return one of the following:
     *    return -n;  chunk had an error
     *    return 0;   did not recognize
     *    return n;   success
     *
     * The unknown chunk structure contains the chunk data:
     * png_byte name[5];
     * png_byte *data;
     * size_t size;
     *
     * Note that libpng has already taken care of the CRC handling.
     */

    if (chunk->name[0] == 115 && chunk->name[1] == 84 && /* s  T */
        chunk->name[2] == 69 && chunk->name[3] == 82)    /* E  R */
    {
        /* Found sTER chunk */
        if (chunk->size != 1) return -1; /* Error return */

        if (chunk->data[0] != 0 && chunk->data[0] != 1) return -1; /* Invalid mode */

        if (set_chunk_location(png_ptr, my_user_chunk_data, have_sTER) != 0) {
            my_user_chunk_data->sTER_mode = chunk->data[0];
            return 1;
        }

        else
            return 0; /* duplicate sTER - give it to libpng */
    }

    if (chunk->name[0] != 118 || chunk->name[1] != 112 || /* v  p */
        chunk->name[2] != 65 || chunk->name[3] != 103)    /* A  g */
        return 0;                                         /* Did not recognize */

    /* Found ImageMagick vpAg chunk */

    if (chunk->size != 9) return -1; /* Error return */

    if (set_chunk_location(png_ptr, my_user_chunk_data, have_vpAg) == 0) return 0; /* duplicate vpAg */

    my_user_chunk_data->vpAg_width = png_get_uint_31(png_ptr, chunk->data);
    my_user_chunk_data->vpAg_height = png_get_uint_31(png_ptr, chunk->data + 4);
    my_user_chunk_data->vpAg_units = chunk->data[8];

    return 1;
}

static void write_sTER_chunk(png_structp write_ptr, user_chunk_info* data) {
    png_byte sTER[5] = {115, 84, 69, 82, '\0'};

    if (verbose != 0) std::fprintf(STDERR, "\n stereo mode = %d\n", data->sTER_mode);

    png_write_chunk(write_ptr, sTER, &data->sTER_mode, 1);
}

static void write_vpAg_chunk(png_structp write_ptr, user_chunk_info* data) {
    png_byte vpAg[5] = {118, 112, 65, 103, '\0'};

    png_byte vpag_chunk_data[9];

    if (verbose != 0)
        std::fprintf(STDERR, " vpAg = %lu x %lu, units = %d\n", static_cast<unsigned long>(data->vpAg_width),
                     static_cast<unsigned long>(data->vpAg_height), data->vpAg_units);

    png_save_uint_32(vpag_chunk_data, data->vpAg_width);
    png_save_uint_32(vpag_chunk_data + 4, data->vpAg_height);
    vpag_chunk_data[8] = data->vpAg_units;
    png_write_chunk(write_ptr, vpAg, vpag_chunk_data, 9);
}

static void write_chunks(png_structp write_ptr, user_chunk_info* data, int location) {
    int i;

    /* Notice that this preserves the original chunk order, however chunks
     * intercepted by the callback will be written *after* chunks passed to
     * libpng.  This will actually reverse a pair of sTER chunks or a pair of
     * vpAg chunks, resulting in an error later.  This is not worth worrying
     * about - the chunks should not be duplicated!
     */
    for (i = 0; i < 2; ++i) {
        if (data->location[i] == (location | have_sTER))
            write_sTER_chunk(write_ptr, data);

        else if (data->location[i] == (location | have_vpAg))
            write_vpAg_chunk(write_ptr, data);
    }
}
/* END of code to demonstrate user chunk support */

/* START of code to check that libpng has the required text support; this only
 * checks for the write support because if read support is missing the chunk
 * will simply not be reported back to pngtest.
 */
static void pngtest_check_text_support(png_structp png_ptr, png_textp text_ptr, int num_text) {
    while (num_text > 0) {
        switch (text_ptr[--num_text].compression) {
            case PNG_TEXT_COMPRESSION_NONE:
                break;

            case PNG_TEXT_COMPRESSION_zTXt:
                /* PNG_WRITE_zTXt_SUPPORTED: nothing to do */
                break;

            case PNG_ITXT_COMPRESSION_NONE:
            case PNG_ITXT_COMPRESSION_zTXt:
                /* PNG_WRITE_iTXt_SUPPORTED: nothing to do */
                break;

            default:
                /* This is an error */
                png_error(png_ptr, "invalid text chunk compression field");
                break;
        }
    }
}
/* END of code to check that libpng has the required text support */

/* Test one file */
static int test_one_file(const char* inname, const char* outname) {
    static png_FILE_p fpin;
    static png_FILE_p fpout; /* "static" prevents setjmp corruption */
    static png_bytep row_buf; /* (static in the port too: freed after a longjmp) */
    pngtest_error_parameters error_parameters;
    png_structp read_ptr;
    png_infop read_info_ptr, end_info_ptr;
    png_structp write_ptr;
    png_infop write_info_ptr;
    png_infop write_end_info_ptr;
    int interlace_preserved = 1;
    png_uint_32 y;
    png_uint_32 width, height;
    int bit_depth, color_type;
    user_chunk_info my_user_chunk_data;
    int pass, num_passes;

    row_buf = nullptr;
    error_parameters.file_name = inname;

    if ((fpin = std::fopen(inname, "rb")) == nullptr) {
        std::fprintf(STDERR, "Could not find input file %s\n", inname);
        return 1;
    }

    if ((fpout = std::fopen(outname, "wb")) == nullptr) {
        std::fprintf(STDERR, "Could not open output file %s\n", outname);
        std::fclose(fpin);
        return 1;
    }

    read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_set_error_fn(read_ptr, &error_parameters, pngtest_error, pngtest_warning);

    write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_set_error_fn(write_ptr, &error_parameters, pngtest_error, pngtest_warning);
    read_info_ptr = png_create_info_struct(read_ptr);
    end_info_ptr = png_create_info_struct(read_ptr);
    write_info_ptr = png_create_info_struct(write_ptr);
    write_end_info_ptr = png_create_info_struct(write_ptr);

    init_user_chunk_info(read_info_ptr, &my_user_chunk_data);
    png_set_read_user_chunk_fn(read_ptr, &my_user_chunk_data, read_user_chunk_callback);

    if (setjmp(png_jmpbuf(read_ptr))) {
        std::fprintf(STDERR, "%s -> %s: libpng read error\n", inname, outname);
        png_free(read_ptr, row_buf);
        row_buf = nullptr;
        if (verbose != 0) std::fprintf(STDERR, "   destroy read structs\n");
        png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
        if (verbose != 0) std::fprintf(STDERR, "   destroy write structs\n");
        png_destroy_info_struct(write_ptr, &write_end_info_ptr);
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        std::fclose(fpin);
        std::fclose(fpout);
        return 1;
    }

    if (setjmp(png_jmpbuf(write_ptr))) {
        std::fprintf(STDERR, "%s -> %s: libpng write error\n", inname, outname);
        png_free(read_ptr, row_buf);
        row_buf = nullptr;
        if (verbose != 0) std::fprintf(STDERR, "   destroying read structs\n");
        png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
        if (verbose != 0) std::fprintf(STDERR, "   destroying write structs\n");
        png_destroy_info_struct(write_ptr, &write_end_info_ptr);
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        std::fclose(fpin);
        std::fclose(fpout);
        return 1;
    }

    if (strict != 0) {
        /* Treat png_benign_error() as errors on read */
        png_set_benign_errors(read_ptr, 0);

        /* Treat them as errors on write */
        png_set_benign_errors(write_ptr, 0);

        /* if strict is not set, then app warnings and errors are treated as
         * warnings in release builds, but not in unstable builds; this can be
         * changed with '--relaxed'.
         */
    }

    else if (relaxed != 0) {
        /* Allow application (pngtest) errors and warnings to pass */
        png_set_benign_errors(read_ptr, 1);

        /* Turn off CRC checking while reading */
        png_set_crc_action(read_ptr, PNG_CRC_QUIET_USE, PNG_CRC_QUIET_USE);

        png_set_benign_errors(write_ptr, 1);
    }

    png_init_io(read_ptr, fpin);
    png_init_io(write_ptr, fpout);

    if (status_dots_requested == 1) {
        png_set_write_status_fn(write_ptr, write_row_callback);
        png_set_read_status_fn(read_ptr, read_row_callback);
    }

    else {
        png_set_write_status_fn(write_ptr, nullptr);
        png_set_read_status_fn(read_ptr, nullptr);
    }

    png_set_read_user_transform_fn(read_ptr, read_user_callback);
    zero_samples = 0;
    png_set_write_user_transform_fn(write_ptr, count_zero_samples);

    /* Preserve all the unknown chunks, if possible.  If this is disabled, then
     * even if the png_{get,set}_unknown_chunks stuff is enabled, we can't use
     * libpng to *save* the unknown chunks on read (because we can't switch the
     * save option on!)
     *
     * Notice that if SET_UNKNOWN_CHUNKS is *not* supported, the reader will
     * discard all unknown chunks, and the writer will write them all.
     */
    png_set_keep_unknown_chunks(read_ptr, PNG_HANDLE_CHUNK_ALWAYS, nullptr, 0);
    png_set_keep_unknown_chunks(write_ptr, PNG_HANDLE_CHUNK_ALWAYS, nullptr, 0);

    png_read_info(read_ptr, read_info_ptr);

    /* This is a bit of a hack; there is no obvious way in the callback function
     * to determine that the chunks before the first IDAT have been read, so
     * remove the info_ptr (which is only used to determine position relative to
     * PLTE) here to indicate that we are after the IDAT.
     */
    my_user_chunk_data.info_ptr = nullptr;

    {
        int interlace_type, compression_type, filter_type;

        if (png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type,
                         &compression_type, &filter_type) != 0) {
            png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type, interlace_type,
                         compression_type, filter_type);
            /* num_passes may not be available below if interlace support is not
             * provided by libpng for both read and write.
             */
            switch (interlace_type) {
                case PNG_INTERLACE_NONE:
                    num_passes = 1;
                    break;

                case PNG_INTERLACE_ADAM7:
                    num_passes = 7;
                    break;

                default:
                    png_error(read_ptr, "invalid interlace type");
                    /*NOT REACHED*/
            }
        }

        else
            png_error(read_ptr, "png_get_IHDR failed");
    }
    {
        png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;

        if (png_get_cHRM_fixed(read_ptr, read_info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y,
                               &blue_x, &blue_y) != 0) {
            png_set_cHRM_fixed(write_ptr, write_info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x,
                               blue_y);
        }
    }
    {
        png_fixed_point gamma;

        if (png_get_gAMA_fixed(read_ptr, read_info_ptr, &gamma) != 0) png_set_gAMA_fixed(write_ptr, write_info_ptr, gamma);
    }
    {
        png_charp name;
        png_bytep profile;
        png_uint_32 proflen;
        int compression_type;

        if (png_get_iCCP(read_ptr, read_info_ptr, &name, &compression_type, &profile, &proflen) != 0) {
            png_set_iCCP(write_ptr, write_info_ptr, name, compression_type, profile, proflen);
        }
    }
    {
        int intent;

        if (png_get_sRGB(read_ptr, read_info_ptr, &intent) != 0) png_set_sRGB(write_ptr, write_info_ptr, intent);
    }
    {
        png_colorp palette;
        int num_palette;

        if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette) != 0)
            png_set_PLTE(write_ptr, write_info_ptr, palette, num_palette);
    }
    {
        png_color_16p background;

        if (png_get_bKGD(read_ptr, read_info_ptr, &background) != 0) png_set_bKGD(write_ptr, write_info_ptr, background);
    }
    {
        png_bytep exif = nullptr;
        png_uint_32 exif_length;

        if (png_get_eXIf_1(read_ptr, read_info_ptr, &exif_length, &exif) != 0) {
            if (exif_length > 1)
                std::fprintf(STDERR, " eXIf type %c%c, %lu bytes\n", exif[0], exif[1], static_cast<unsigned long>(exif_length));
            png_set_eXIf_1(write_ptr, write_info_ptr, exif_length, exif);
        }
    }
    {
        png_uint_16p hist;

        if (png_get_hIST(read_ptr, read_info_ptr, &hist) != 0) png_set_hIST(write_ptr, write_info_ptr, hist);
    }
    {
        png_int_32 offset_x, offset_y;
        int unit_type;

        if (png_get_oFFs(read_ptr, read_info_ptr, &offset_x, &offset_y, &unit_type) != 0)
            png_set_oFFs(write_ptr, write_info_ptr, offset_x, offset_y, unit_type);
    }
    {
        png_charp purpose, units;
        png_charpp params;
        png_int_32 X0, X1;
        int type, nparams;

        if (png_get_pCAL(read_ptr, read_info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params) != 0)
            png_set_pCAL(write_ptr, write_info_ptr, purpose, X0, X1, type, nparams, units, params);
    }
    {
        png_uint_32 res_x, res_y;
        int unit_type;

        if (png_get_pHYs(read_ptr, read_info_ptr, &res_x, &res_y, &unit_type) != 0)
            png_set_pHYs(write_ptr, write_info_ptr, res_x, res_y, unit_type);
    }
    {
        png_color_8p sig_bit;

        if (png_get_sBIT(read_ptr, read_info_ptr, &sig_bit) != 0) png_set_sBIT(write_ptr, write_info_ptr, sig_bit);
    }
    {
        int unit;
        double scal_width, scal_height;

        if (png_get_sCAL(read_ptr, read_info_ptr, &unit, &scal_width, &scal_height) != 0)
            png_set_sCAL(write_ptr, write_info_ptr, unit, scal_width, scal_height);
    }

    {
        png_sPLT_tp entries;

        int num_entries = png_get_sPLT(read_ptr, read_info_ptr, &entries);
        if (num_entries != 0) png_set_sPLT(write_ptr, write_info_ptr, entries, num_entries);
    }

    {
        png_textp text_ptr;
        int num_text;

        if (png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text) > 0) {
            pngtest_check_text_support(read_ptr, text_ptr, num_text);

            if (verbose != 0) {
                int i;

                std::fprintf(STDERR, "\n");
                for (i = 0; i < num_text; i++) {
                    std::fprintf(STDERR, "   Text compression[%d]=%d\n", i, text_ptr[i].compression);
                }
            }

            png_set_text(write_ptr, write_info_ptr, text_ptr, num_text);
        }
    }
    {
        png_timep mod_time;

        if (png_get_tIME(read_ptr, read_info_ptr, &mod_time) != 0) {
            png_set_tIME(write_ptr, write_info_ptr, mod_time);
            if (png_convert_to_rfc1123_buffer(tIME_string, mod_time) != 0)
                tIME_string[(sizeof tIME_string) - 1] = '\0';

            else {
                std::strncpy(tIME_string, "*** invalid time ***", (sizeof tIME_string));
                tIME_string[(sizeof tIME_string) - 1] = '\0';
            }

            tIME_chunk_present++;
        }
    }
    {
        png_bytep trans_alpha;
        int num_trans;
        png_color_16p trans_color;

        if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans, &trans_color) != 0) {
            int sample_max = (1 << bit_depth);
            /* libpng doesn't reject a tRNS chunk with out-of-range samples */
            if (!((color_type == PNG_COLOR_TYPE_GRAY && static_cast<int>(trans_color->gray) > sample_max) ||
                  (color_type == PNG_COLOR_TYPE_RGB &&
                   (static_cast<int>(trans_color->red) > sample_max || static_cast<int>(trans_color->green) > sample_max ||
                    static_cast<int>(trans_color->blue) > sample_max))))
                png_set_tRNS(write_ptr, write_info_ptr, trans_alpha, num_trans, trans_color);
        }
    }
    {
        png_unknown_chunkp unknowns;
        int num_unknowns = png_get_unknown_chunks(read_ptr, read_info_ptr, &unknowns);

        if (num_unknowns != 0) png_set_unknown_chunks(write_ptr, write_info_ptr, unknowns, num_unknowns);
    }

    /* Write the info in two steps so that if we write the 'unknown' chunks here
     * they go to the correct place.
     */
    png_write_info_before_PLTE(write_ptr, write_info_ptr);

    write_chunks(write_ptr, &my_user_chunk_data, before_PLTE); /* before PLTE */

    png_write_info(write_ptr, write_info_ptr);

    write_chunks(write_ptr, &my_user_chunk_data, before_IDAT); /* after PLTE */

    png_write_info(write_ptr, write_end_info_ptr);

    write_chunks(write_ptr, &my_user_chunk_data, after_IDAT); /* after IDAT */

    /* Both must be defined for libpng to be able to handle the interlace,
     * otherwise it gets handled below by simply reading and writing the passes
     * directly.
     */
    if (png_set_interlace_handling(read_ptr) != num_passes)
        png_error(write_ptr, "png_set_interlace_handling(read): wrong pass count ");
    if (png_set_interlace_handling(write_ptr) != num_passes)
        png_error(write_ptr, "png_set_interlace_handling(write): wrong pass count ");

    for (pass = 0; pass < num_passes; pass++) {
        const png_uint_32 pass_height = height;

        for (y = 0; y < pass_height; y++) {
            row_buf = static_cast<png_bytep>(png_malloc(read_ptr, png_get_rowbytes(read_ptr, read_info_ptr)));

            png_read_rows(read_ptr, static_cast<png_bytepp>(&row_buf), nullptr, 1);

            png_write_rows(write_ptr, static_cast<png_bytepp>(&row_buf), 1);

            png_free(read_ptr, row_buf);
            row_buf = nullptr;
        }
    }

    png_free_data(read_ptr, read_info_ptr, PNG_FREE_UNKN, -1);
    png_free_data(write_ptr, write_info_ptr, PNG_FREE_UNKN, -1);

    png_read_end(read_ptr, end_info_ptr);
    {
        png_textp text_ptr;
        int num_text;

        if (png_get_text(read_ptr, end_info_ptr, &text_ptr, &num_text) > 0) {
            pngtest_check_text_support(read_ptr, text_ptr, num_text);

            if (verbose != 0) {
                int i;

                std::fprintf(STDERR, "\n");
                for (i = 0; i < num_text; i++) {
                    std::fprintf(STDERR, "   Text compression[%d]=%d\n", i, text_ptr[i].compression);
                }
            }

            png_set_text(write_ptr, write_end_info_ptr, text_ptr, num_text);
        }
    }
    {
        png_bytep exif = nullptr;
        png_uint_32 exif_length;

        if (png_get_eXIf_1(read_ptr, end_info_ptr, &exif_length, &exif) != 0) {
            if (exif_length > 1)
                std::fprintf(STDERR, " eXIf type %c%c, %lu bytes\n", exif[0], exif[1], static_cast<unsigned long>(exif_length));
            png_set_eXIf_1(write_ptr, write_end_info_ptr, exif_length, exif);
        }
    }
    {
        png_timep mod_time;

        if (png_get_tIME(read_ptr, end_info_ptr, &mod_time) != 0) {
            png_set_tIME(write_ptr, write_end_info_ptr, mod_time);
            if (png_convert_to_rfc1123_buffer(tIME_string, mod_time) != 0)
                tIME_string[(sizeof tIME_string) - 1] = '\0';

            else {
                std::strncpy(tIME_string, "*** invalid time ***", sizeof tIME_string);
                tIME_string[(sizeof tIME_string) - 1] = '\0';
            }

            tIME_chunk_present++;
        }
    }
    {
        png_unknown_chunkp unknowns;
        int num_unknowns = png_get_unknown_chunks(read_ptr, end_info_ptr, &unknowns);

        if (num_unknowns != 0) png_set_unknown_chunks(write_ptr, write_end_info_ptr, unknowns, num_unknowns);
    }

    /* Normally one would use Z_DEFAULT_STRATEGY for text compression.
     * This is here just to make pngtest replicate the results from libpng
     * versions prior to 1.5.4, and to test this new API.
     */
    png_set_text_compression_strategy(write_ptr, Z_FILTERED);

    /* When the unknown vpAg/sTER chunks are written by pngtest the only way to
     * do it is to write them *before* calling png_write_end.  When unknown
     * chunks are written by libpng, however, they are written just before IEND.
     * There seems to be no way round this, however vpAg/sTER are not expected
     * after IDAT.
     */
    write_chunks(write_ptr, &my_user_chunk_data, after_IDAT);

    png_write_end(write_ptr, write_end_info_ptr);

    if (verbose != 0) {
        png_uint_32 iwidth, iheight;
        iwidth = png_get_image_width(write_ptr, write_info_ptr);
        iheight = png_get_image_height(write_ptr, write_info_ptr);
        std::fprintf(STDERR, "\n Image width = %lu, height = %lu\n", static_cast<unsigned long>(iwidth),
                     static_cast<unsigned long>(iheight));
    }

    png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
    png_destroy_info_struct(write_ptr, &write_end_info_ptr);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    std::fclose(fpin);
    std::fclose(fpout);

    /* Summarize any warnings or errors and in 'strict' mode fail the test.
     * Unsupported chunks can result in warnings, in that case ignore the strict
     * setting, otherwise fail the test on warnings as well as errors.
     */
    if (error_count > 0) {
        /* We don't really expect to get here because of the setjmp handling
         * above, but this is safe.
         */
        std::fprintf(STDERR, "\n  %s: %d libpng errors found (%d warnings)", inname, error_count, warning_count);

        if (strict != 0) return 1;
    }

    /* If there is no write support nothing was written! */
    else if (unsupported_chunks > 0) {
        std::fprintf(STDERR, "\n  %s: unsupported chunks (%d)%s", inname, unsupported_chunks, strict ? ": IGNORED --strict!" : "");
    }

    else if (warning_count > 0) {
        std::fprintf(STDERR, "\n  %s: %d libpng warnings found", inname, warning_count);

        if (strict != 0) return 1;
    }

    if ((fpin = std::fopen(inname, "rb")) == nullptr) {
        std::fprintf(STDERR, "Could not find file %s\n", inname);
        return 1;
    }

    if ((fpout = std::fopen(outname, "rb")) == nullptr) {
        std::fprintf(STDERR, "Could not find file %s\n", outname);
        std::fclose(fpin);
        return 1;
    }

    if (interlace_preserved != 0) /* else the files will be changed */
    {
        for (;;) {
            static int wrote_question = 0;
            std::size_t num_in, num_out;
            char inbuf[256], outbuf[256];

            num_in = std::fread(inbuf, 1, sizeof inbuf, fpin);
            num_out = std::fread(outbuf, 1, sizeof outbuf, fpout);

            if (num_in != num_out) {
                std::fprintf(STDERR, "\nFiles %s and %s are of a different size\n", inname, outname);

                if (wrote_question == 0 && unsupported_chunks == 0) {
                    std::fprintf(STDERR,
                                 "   Was %s written with the same maximum IDAT"
                                 " chunk size (%d bytes),",
                                 inname, PNG_ZBUF_SIZE);
                    std::fprintf(STDERR, "\n   filtering heuristic (libpng default), compression");
                    std::fprintf(STDERR, " level (zlib default),\n   and zlib version (%s)?\n\n", ZLIB_VERSION);
                    wrote_question = 1;
                }

                std::fclose(fpin);
                std::fclose(fpout);

                if (strict != 0 && unsupported_chunks == 0)
                    return 1;

                else
                    return 0;
            }

            if (num_in == 0) break;

            if (std::memcmp(inbuf, outbuf, num_in)) {
                std::fprintf(STDERR, "\nFiles %s and %s are different\n", inname, outname);

                if (wrote_question == 0 && unsupported_chunks == 0) {
                    std::fprintf(STDERR,
                                 "   Was %s written with the same maximum"
                                 " IDAT chunk size (%d bytes),",
                                 inname, PNG_ZBUF_SIZE);
                    std::fprintf(STDERR, "\n   filtering heuristic (libpng default), compression");
                    std::fprintf(STDERR, " level (zlib default),\n   and zlib version (%s)?\n\n", ZLIB_VERSION);
                    wrote_question = 1;
                }

                std::fclose(fpin);
                std::fclose(fpout);

                /* NOTE: the unsupported_chunks escape is permitted here because
                 * unsupported text chunk compression will result in the compression
                 * mode being changed (to NONE) yet, in the test case, the result
                 * can be exactly the same size!
                 */
                if (strict != 0 && unsupported_chunks == 0)
                    return 1;

                else
                    return 0;
            }
        }
    }

    std::fclose(fpin);
    std::fclose(fpout);

    return 0;
}

/* Input and output filenames */
static const char* inname = "pngtest.png";
static const char* outname = "pngout.png";

int main(int argc, char* argv[]) {
    int multiple = 0;
    int ierror = 0;

    png_structp dummy_ptr;

    std::fprintf(STDERR, "\n Testing libpng version %s\n", PNG_LIBPNG_VER_STRING);
    std::fprintf(STDERR, "   with zlib   version %s\n", ZLIB_VERSION);
    std::fprintf(STDERR, "%s", png_get_copyright(nullptr));
    /* Show the version of libpng used in building the library */
    std::fprintf(STDERR, " library (%lu):%s", static_cast<unsigned long>(png_access_version_number()),
                 png_get_header_version(nullptr));

    /* Show the version of libpng used in building the application */
    std::fprintf(STDERR, " pngtest (%lu):%s", static_cast<unsigned long>(PNG_LIBPNG_VER), PNG_HEADER_VERSION_STRING);

    if (std::strcmp(png_libpng_ver, PNG_LIBPNG_VER_STRING) != 0) {
        std::fprintf(STDERR, "Warning: mismatching versions of png.h and png.c\n");
        std::fprintf(STDERR, "  png.h version string: %s\n", PNG_LIBPNG_VER_STRING);
        std::fprintf(STDERR, "  png.c version string: %s\n\n", png_libpng_ver);
        ++ierror;
    }

    if (argc > 1) {
        if (std::strcmp(argv[1], "-m") == 0) {
            multiple = 1;
            status_dots_requested = 0;
        }

        else if (std::strcmp(argv[1], "-mv") == 0 || std::strcmp(argv[1], "-vm") == 0) {
            multiple = 1;
            verbose = 1;
            status_dots_requested = 1;
        }

        else if (std::strcmp(argv[1], "-v") == 0) {
            verbose = 1;
            status_dots_requested = 1;
            inname = argv[2];
        }

        else if (std::strcmp(argv[1], "--strict") == 0) {
            status_dots_requested = 0;
            verbose = 1;
            inname = argv[2];
            strict++;
            relaxed = 0;
            multiple = 1;
        }

        else if (std::strcmp(argv[1], "--relaxed") == 0) {
            status_dots_requested = 0;
            verbose = 1;
            inname = argv[2];
            strict = 0;
            relaxed++;
            multiple = 1;
        } else if (std::strcmp(argv[1], "--xfail") == 0) {
            status_dots_requested = 0;
            verbose = 1;
            inname = argv[2];
            strict = 0;
            xfail++;
            relaxed++;
            multiple = 1;
        }

        else {
            inname = argv[1];
            status_dots_requested = 0;
        }
    }

    if (multiple == 0 && argc == 3 + verbose) outname = argv[2 + verbose];

    if ((multiple == 0 && argc > 3 + verbose) || (multiple != 0 && argc < 2)) {
        std::fprintf(STDERR, "usage: %s [infile.png] [outfile.png]\n\t%s -m {infile.png}\n", argv[0], argv[0]);
        std::fprintf(STDERR, "  reads/writes one PNG file (without -m) or multiple files (-m)\n");
        std::fprintf(STDERR, "  with -m %s is used as a temporary file\n", outname);
        std::exit(1);
    }

    if (multiple != 0) {
        int i;
        for (i = 2; i < argc; ++i) {
            int kerror;
            std::fprintf(STDERR, "\n Testing %s:", argv[i]);
            kerror = test_one_file(argv[i], outname);
            if (kerror == 0) {
                std::fprintf(STDERR, "\n PASS (%lu zero samples)\n", static_cast<unsigned long>(zero_samples));
                if (tIME_chunk_present != 0) std::fprintf(STDERR, " tIME = %s\n", tIME_string);

                tIME_chunk_present = 0;
            }

            else {
                if (xfail)
                    std::fprintf(STDERR, " XFAIL\n");
                else {
                    std::fprintf(STDERR, " FAIL\n");
                    ierror += kerror;
                }
            }
        }
    }

    else {
        int i;
        for (i = 0; i < 3; ++i) {
            int kerror;
            if (i == 1)
                status_dots_requested = 1;

            else if (verbose == 0)
                status_dots_requested = 0;

            if (i == 0 || verbose == 1 || ierror != 0) {
                std::fprintf(STDERR, "\n Testing %s:", inname);
            }

            kerror = test_one_file(inname, outname);

            if (kerror == 0) {
                if (verbose == 1 || i == 2) {
                    std::fprintf(STDERR, "\n PASS (%lu zero samples)\n", static_cast<unsigned long>(zero_samples));
                    if (tIME_chunk_present != 0) std::fprintf(STDERR, " tIME = %s\n", tIME_string);
                }
            }

            else {
                if (verbose == 0 && i != 2) {
                    std::fprintf(STDERR, "\n Testing %s:", inname);
                }

                if (xfail)
                    std::fprintf(STDERR, " XFAIL\n");
                else {
                    std::fprintf(STDERR, " FAIL\n");
                    ierror += kerror;
                }
            }
        }
    }

    if (ierror == 0)
        std::fprintf(STDERR, " libpng passes test\n");

    else
        std::fprintf(STDERR, " libpng FAILS test\n");

    dummy_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    std::fprintf(STDERR, " Default limits:\n");
    std::fprintf(STDERR, "  width_max  = %lu\n", static_cast<unsigned long>(png_get_user_width_max(dummy_ptr)));
    std::fprintf(STDERR, "  height_max = %lu\n", static_cast<unsigned long>(png_get_user_height_max(dummy_ptr)));
    if (png_get_chunk_cache_max(dummy_ptr) == 0)
        std::fprintf(STDERR, "  cache_max  = unlimited\n");
    else
        std::fprintf(STDERR, "  cache_max  = %lu\n", static_cast<unsigned long>(png_get_chunk_cache_max(dummy_ptr)));
    if (png_get_chunk_malloc_max(dummy_ptr) == 0)
        std::fprintf(STDERR, "  malloc_max = unlimited\n");
    else
        std::fprintf(STDERR, "  malloc_max = %lu\n", static_cast<unsigned long>(png_get_chunk_malloc_max(dummy_ptr)));
    png_destroy_read_struct(&dummy_ptr, nullptr, nullptr);

    return (ierror != 0);
}

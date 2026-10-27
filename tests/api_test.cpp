// The libpng API through the module: the simplified API (memory and stdio
// round trips, formats, colour maps, sizes), the low-level writer and reader
// (custom I/O, ancillary chunks, transforms, Adam7 row by row, the
// high-level png_read_png / png_write_png), gamma and sRGB, progressive
// reading, unknown chunks, user memory functions, and the error path:
// png_error longjmps to a setjmp on png_jmpbuf — the module's function —
// across the module boundary. Spelled as png.h documents it.
#include <csetjmp>  // setjmp is the C library's macro
#include <gtest/gtest.h>

import std;
import mcpplibs.mwrap.libpng;

using namespace mcpplibs::mwrap::libpng::macros;  // PNG_COLOR_TYPE_RGB, PNG_IMAGE_SIZE, png_jmpbuf …

namespace {

using Bytes = std::vector<png_byte>;

// ── the simplified API ───────────────────────────────────────────────────

png_image blank(png_uint_32 width, png_uint_32 height, png_uint_32 format) {
    png_image image{};
    image.version = PNG_IMAGE_VERSION;
    image.width = width;
    image.height = height;
    image.format = format;
    return image;
}

Bytes encode(png_image image, const void* pixels, png_int_32 stride = 0, const void* colormap = nullptr) {
    png_alloc_size_t size = 0;
    EXPECT_TRUE(png_image_write_get_memory_size(image, size, 0, pixels, stride, colormap)) << image.message;
    EXPECT_GT(size, 0u);
    EXPECT_LE(size, PNG_IMAGE_PNG_SIZE_MAX(image));
    Bytes out(size);
    EXPECT_TRUE(png_image_write_to_memory(&image, out.data(), &size, 0, pixels, stride, colormap)) << image.message;
    out.resize(size);
    return out;
}

TEST(Simplified, RgbaRoundTripInMemory) {
    constexpr png_uint_32 w = 7, h = 5;
    png_image image = blank(w, h, PNG_FORMAT_RGBA);
    ASSERT_EQ(PNG_IMAGE_ROW_STRIDE(image), 4 * w);
    ASSERT_EQ(PNG_IMAGE_SIZE(image), 4 * w * h);
    Bytes pixels(PNG_IMAGE_SIZE(image));
    for (std::size_t i = 0; i < pixels.size(); ++i) pixels[i] = static_cast<png_byte>(i * 37 + 11);
    for (std::size_t i = 3; i < pixels.size(); i += 4) pixels[i] = 255;  // opaque: no premultiplication loss
    const Bytes file = encode(image, pixels.data());
    ASSERT_EQ(png_sig_cmp(file.data(), 0, 8), 0);
    ASSERT_TRUE(png_check_sig(file.data(), 8));

    png_image read{};
    read.version = PNG_IMAGE_VERSION;
    ASSERT_TRUE(png_image_begin_read_from_memory(&read, file.data(), file.size())) << read.message;
    EXPECT_EQ(read.width, w);
    EXPECT_EQ(read.height, h);
    EXPECT_TRUE(read.format & PNG_FORMAT_FLAG_COLOR);
    read.format = PNG_FORMAT_RGBA;
    Bytes back(PNG_IMAGE_SIZE(read));
    ASSERT_TRUE(png_image_finish_read(&read, nullptr, back.data(), 0, nullptr)) << read.message;
    EXPECT_FALSE(PNG_IMAGE_FAILED(read));
    EXPECT_EQ(back, pixels);
}

TEST(Simplified, FormatsAndConversions) {
    // Gray in, then read as BGR, as ARGB and as 16-bit linear.
    constexpr png_uint_32 w = 16, h = 3;
    png_image image = blank(w, h, PNG_FORMAT_GRAY);
    Bytes gray(PNG_IMAGE_SIZE(image));
    for (std::size_t i = 0; i < gray.size(); ++i) gray[i] = static_cast<png_byte>(i * 5);
    const Bytes file = encode(image, gray.data());

    for (png_uint_32 format : {PNG_FORMAT_BGR, PNG_FORMAT_ARGB, PNG_FORMAT_GA, PNG_FORMAT_AG}) {
        png_image read{};
        read.version = PNG_IMAGE_VERSION;
        ASSERT_TRUE(png_image_begin_read_from_memory(&read, file.data(), file.size()));
        EXPECT_EQ(read.format, PNG_FORMAT_GRAY);
        read.format = format;
        Bytes back(PNG_IMAGE_SIZE(read));
        ASSERT_TRUE(png_image_finish_read(&read, nullptr, back.data(), 0, nullptr)) << read.message;
        const auto channels = PNG_IMAGE_PIXEL_CHANNELS(format);
        const bool afirst = format & PNG_FORMAT_FLAG_AFIRST;
        for (std::size_t p = 0; p < gray.size(); ++p) {
            const png_byte* px = back.data() + p * channels;
            const png_byte* colour = px + (afirst ? 1 : 0);
            EXPECT_EQ(colour[0], gray[p]) << "format " << format << " pixel " << p;
            if (format & PNG_FORMAT_FLAG_ALPHA) EXPECT_EQ(px[afirst ? 0 : channels - 1], 255);
        }
    }
    png_image linear{};
    linear.version = PNG_IMAGE_VERSION;
    ASSERT_TRUE(png_image_begin_read_from_memory(&linear, file.data(), file.size()));
    linear.format = PNG_FORMAT_LINEAR_Y;
    ASSERT_EQ(PNG_IMAGE_PIXEL_COMPONENT_SIZE(linear.format), 2u);
    std::vector<png_uint_16> y(PNG_IMAGE_SIZE(linear) / 2);
    ASSERT_TRUE(png_image_finish_read(&linear, nullptr, y.data(), 0, nullptr)) << linear.message;
    EXPECT_EQ(y.front(), 0);
    EXPECT_TRUE(std::ranges::is_sorted(y));  // sRGB to linear is monotonic
    EXPECT_GT(y.back(), 30000);
}

TEST(Simplified, ColorMapped) {
    // A palette image: 4 colours, one translucent.
    constexpr png_uint_32 w = 4, h = 4;
    png_image image = blank(w, h, PNG_FORMAT_RGBA_COLORMAP);
    image.colormap_entries = 4;
    ASSERT_EQ(PNG_IMAGE_PIXEL_SIZE(image.format), 1u);
    ASSERT_EQ(PNG_IMAGE_COLORMAP_SIZE(image), 16u);
    const png_byte colormap[16] = {255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 10, 20, 30, 128};
    Bytes index(PNG_IMAGE_SIZE(image));
    for (std::size_t i = 0; i < index.size(); ++i) index[i] = static_cast<png_byte>(i % 4);
    const Bytes file = encode(image, index.data(), 0, colormap);

    png_image read{};
    read.version = PNG_IMAGE_VERSION;
    ASSERT_TRUE(png_image_begin_read_from_memory(&read, file.data(), file.size()));
    EXPECT_TRUE(read.format & PNG_FORMAT_FLAG_COLORMAP);
    EXPECT_EQ(read.colormap_entries, 4u);
    read.format = PNG_FORMAT_RGBA_COLORMAP;
    std::vector<png_byte> map(PNG_IMAGE_COLORMAP_SIZE(read)), back(PNG_IMAGE_SIZE(read));
    ASSERT_LE(map.size(), static_cast<std::size_t>(PNG_IMAGE_MAXIMUM_COLORMAP_COMPONENTS(read.format)));
    ASSERT_TRUE(png_image_finish_read(&read, nullptr, back.data(), 0, map.data())) << read.message;
    for (std::size_t i = 0; i < back.size(); ++i) {
        const png_byte* rgba = map.data() + back[i] * 4;
        const png_byte* want = colormap + index[i] * 4;
        EXPECT_TRUE(std::equal(rgba, rgba + 4, want)) << "pixel " << i;
    }
}

TEST(Simplified, StdioFileAndNegativeStride) {
    const auto path = (std::filesystem::temp_directory_path() / "mwrap-libpng-stdio.png").string();
    constexpr png_uint_32 w = 3, h = 4;
    png_image image = blank(w, h, PNG_FORMAT_RGB);
    Bytes rows(PNG_IMAGE_SIZE(image));
    for (std::size_t i = 0; i < rows.size(); ++i) rows[i] = static_cast<png_byte>(200 - i);
    const auto stride = static_cast<png_int_32>(PNG_IMAGE_ROW_STRIDE(image));
    ASSERT_TRUE(png_image_write_to_file(&image, path.c_str(), 0, rows.data(), stride, nullptr)) << image.message;

    png_image read{};
    read.version = PNG_IMAGE_VERSION;
    ASSERT_TRUE(png_image_begin_read_from_file(&read, path.c_str())) << read.message;
    read.format = PNG_FORMAT_RGB;
    Bytes flipped(PNG_IMAGE_SIZE(read));
    // A negative stride: the bottom row first in memory.
    ASSERT_TRUE(png_image_finish_read(&read, nullptr, flipped.data(), -stride, nullptr)) << read.message;
    for (png_uint_32 y = 0; y < h; ++y)
        EXPECT_TRUE(std::equal(rows.begin() + y * stride, rows.begin() + (y + 1) * stride, flipped.begin() + (h - 1 - y) * stride));
    std::filesystem::remove(path);
}

TEST(Simplified, ErrorsAreReportedInTheControlStructure) {
    const png_byte garbage[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    png_image read{};
    read.version = PNG_IMAGE_VERSION;
    EXPECT_FALSE(png_image_begin_read_from_memory(&read, garbage, sizeof garbage));
    EXPECT_TRUE(PNG_IMAGE_FAILED(read));
    EXPECT_EQ(read.warning_or_error & 3, static_cast<png_uint_32>(PNG_IMAGE_ERROR));
    EXPECT_STREQ(read.message, "Not a PNG file");
    png_image_free(&read);
}

// ── the low-level API ────────────────────────────────────────────────────

struct Sink {
    Bytes data;
    std::size_t at = 0;
    int flushes = 0;
};

void write_to(png_structp png, png_bytep bytes, size_t n) {
    auto* sink = static_cast<Sink*>(png_get_io_ptr(png));
    sink->data.insert(sink->data.end(), bytes, bytes + n);
}
void flush_to(png_structp png) { ++static_cast<Sink*>(png_get_io_ptr(png))->flushes; }
void read_from(png_structp png, png_bytep out, size_t n) {
    auto* source = static_cast<Sink*>(png_get_io_ptr(png));
    if (source->at + n > source->data.size()) png_error(png, "read past the end");
    std::copy_n(source->data.begin() + static_cast<std::ptrdiff_t>(source->at), n, out);
    source->at += n;
}

// An 8-bit palette image with tRNS, gAMA, sRGB-free cHRM, text, time, pHYs,
// a private chunk and Adam7 interlacing, written by the low-level writer.
constexpr png_uint_32 W = 13, H = 11;
png_byte pixel_index(png_uint_32 x, png_uint_32 y) { return static_cast<png_byte>((x * 3 + y * 5) % 6); }

Bytes write_indexed(bool interlaced) {
    Sink sink;
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    EXPECT_TRUE(png && info);
    png_set_write_fn(png, &sink, write_to, flush_to);
    png_set_IHDR(png, info, W, H, 8, PNG_COLOR_TYPE_PALETTE, interlaced ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_color palette[6] = {{0, 0, 0}, {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {255, 255, 255}};
    png_set_PLTE(png, info, palette, 6);
    png_byte alpha[6] = {255, 128, 255, 255, 0, 255};
    png_set_tRNS(png, info, alpha, 6, nullptr);
    png_set_gAMA_fixed(png, info, 45455);
    png_set_cHRM(png, info, 0.3127, 0.3290, 0.64, 0.33, 0.30, 0.60, 0.15, 0.06);
    png_set_pHYs(png, info, 2835, 2835, PNG_RESOLUTION_METER);
    png_text text[2]{};
    text[0].compression = PNG_TEXT_COMPRESSION_NONE;
    text[0].key = const_cast<char*>("Title");
    text[0].text = const_cast<char*>("mwrap");
    text[1].compression = PNG_TEXT_COMPRESSION_zTXt;
    text[1].key = const_cast<char*>("Comment");
    text[1].text = const_cast<char*>("written through import mcpplibs.mwrap.libpng");
    png_set_text(png, info, text, 2);
    png_time when{};
    png_convert_from_time_t(&when, 1700000000);
    png_set_tIME(png, info, &when);
    png_unknown_chunk chunk{};
    std::memcpy(chunk.name, "mwRp", 5);
    png_byte payload[] = {'o', 'k'};
    chunk.data = payload;
    chunk.size = sizeof payload;
    chunk.location = PNG_HAVE_PLTE;
    png_set_keep_unknown_chunks(png, PNG_HANDLE_CHUNK_ALWAYS, reinterpret_cast<png_const_bytep>("mwRp"), 1);
    png_set_unknown_chunks(png, info, &chunk, 1);
    png_set_filter(png, PNG_FILTER_TYPE_BASE, PNG_ALL_FILTERS);
    png_set_compression_level(png, 9);
    png_write_info(png, info);
    const int passes = png_set_interlace_handling(png);
    EXPECT_EQ(passes, interlaced ? PNG_INTERLACE_ADAM7_PASSES : 1);
    Bytes row(W);
    for (int pass = 0; pass < passes; ++pass)
        for (png_uint_32 y = 0; y < H; ++y) {
            for (png_uint_32 x = 0; x < W; ++x) row[x] = pixel_index(x, y);
            png_write_row(png, row.data());
            if (pass == 0 && y == 0) png_write_flush(png);  // mid-image: a sync flush and the flush callback
        }
    png_write_end(png, info);
    png_destroy_write_struct(&png, &info);
    EXPECT_GE(sink.flushes, 1);
    return sink.data;
}

struct Reader {
    Sink source;
    png_structp png = nullptr;
    png_infop info = nullptr;
    explicit Reader(Bytes file) : source{std::move(file)} {
        png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        info = png_create_info_struct(png);
        png_set_read_fn(png, &source, read_from);
    }
    ~Reader() { png_destroy_read_struct(&png, &info, nullptr); }
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;
};

TEST(LowLevel, AncillaryChunksAndExpansion) {
    for (bool interlaced : {false, true}) {
        Reader r(write_indexed(interlaced));
        png_set_keep_unknown_chunks(r.png, PNG_HANDLE_CHUNK_ALWAYS, reinterpret_cast<png_const_bytep>("mwRp"), 1);
        png_read_info(r.png, r.info);
        EXPECT_EQ(png_get_image_width(r.png, r.info), W);
        EXPECT_EQ(png_get_image_height(r.png, r.info), H);
        EXPECT_EQ(png_get_color_type(r.png, r.info), PNG_COLOR_TYPE_PALETTE);
        EXPECT_EQ(png_get_interlace_type(r.png, r.info), interlaced ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE);
        EXPECT_TRUE(png_get_valid(r.png, r.info, PNG_INFO_PLTE | PNG_INFO_tRNS | PNG_INFO_gAMA | PNG_INFO_cHRM | PNG_INFO_pHYs | PNG_INFO_tIME));
        png_fixed_point gamma = 0;
        ASSERT_EQ(png_get_gAMA_fixed(r.png, r.info, &gamma), PNG_INFO_gAMA);
        EXPECT_EQ(gamma, 45455);
        double white_x = 0, white_y = 0, rx, ry, gx, gy, bx, by;
        ASSERT_TRUE(png_get_cHRM(r.png, r.info, &white_x, &white_y, &rx, &ry, &gx, &gy, &bx, &by));
        EXPECT_NEAR(white_x, 0.3127, 1e-5);
        EXPECT_NEAR(by, 0.06, 1e-5);
        EXPECT_EQ(png_get_pixels_per_meter(r.png, r.info), 2835u);
        png_timep when = nullptr;
        ASSERT_EQ(png_get_tIME(r.png, r.info, &when), PNG_INFO_tIME);
        char rfc1123[29];
        ASSERT_TRUE(png_convert_to_rfc1123_buffer(rfc1123, when));
        EXPECT_STREQ(rfc1123, "14 Nov 2023 22:13:20 +0000");
        // Expand the palette and tRNS to RGBA, then read the rows (libpng
        // de-interlaces: png_read_image handles the passes).
        png_set_expand(r.png);
        png_set_interlace_handling(r.png);
        png_read_update_info(r.png, r.info);
        EXPECT_EQ(png_get_rowbytes(r.png, r.info), W * 4);
        EXPECT_EQ(png_get_channels(r.png, r.info), 4);
        std::vector<Bytes> rows(H, Bytes(W * 4));
        std::vector<png_bytep> pointers;
        for (auto& row : rows) pointers.push_back(row.data());
        png_read_image(r.png, pointers.data());
        png_read_end(r.png, r.info);
        const png_byte palette[6][4] = {{0, 0, 0, 255}, {255, 0, 0, 128}, {0, 255, 0, 255}, {0, 0, 255, 255}, {255, 255, 0, 0}, {255, 255, 255, 255}};
        for (png_uint_32 y = 0; y < H; ++y)
            for (png_uint_32 x = 0; x < W; ++x)
                EXPECT_TRUE(std::equal(rows[y].begin() + x * 4, rows[y].begin() + x * 4 + 4, palette[pixel_index(x, y)])) << x << "," << y;
        png_textp text = nullptr;
        int count = 0;
        png_get_text(r.png, r.info, &text, &count);
        ASSERT_EQ(count, 2);
        EXPECT_STREQ(text[1].key, "Comment");
        EXPECT_STREQ(text[1].text, "written through import mcpplibs.mwrap.libpng");
        EXPECT_EQ(text[1].compression, PNG_TEXT_COMPRESSION_zTXt);
        png_unknown_chunkp unknown = nullptr;
        ASSERT_EQ(png_get_unknown_chunks(r.png, r.info, &unknown), 1);
        EXPECT_STREQ(reinterpret_cast<const char*>(unknown[0].name), "mwRp");
        EXPECT_EQ(unknown[0].size, 2u);
    }
}

TEST(LowLevel, Adam7RowByRowMatchesThePassMacros) {
    // Read the interlaced image pass by pass without libpng's de-interlacing:
    // each pass row is the columns PNG_COL_FROM_PASS_COL maps back.
    Reader r(write_indexed(true));
    png_read_info(r.png, r.info);
    png_set_packing(r.png);
    png_read_update_info(r.png, r.info);
    for (int pass = 0; pass < PNG_INTERLACE_ADAM7_PASSES; ++pass) {
        const png_uint_32 cols = PNG_PASS_COLS(W, pass), rows = PNG_PASS_ROWS(H, pass);
        if (cols == 0) continue;  // libpng skips an empty pass
        Bytes row(W);
        for (png_uint_32 py = 0; py < rows; ++py) {
            png_read_row(r.png, row.data(), nullptr);
            const png_uint_32 y = PNG_ROW_FROM_PASS_ROW(py, pass);
            EXPECT_TRUE(PNG_ROW_IN_INTERLACE_PASS(y, pass));
            for (png_uint_32 px = 0; px < cols; ++px) {
                const png_uint_32 x = PNG_COL_FROM_PASS_COL(px, pass);
                EXPECT_TRUE(PNG_COL_IN_INTERLACE_PASS(x, pass));
                EXPECT_EQ(row[px], pixel_index(x, y)) << "pass " << pass << " at " << x << "," << y;
            }
        }
    }
    png_read_end(r.png, nullptr);
}

TEST(LowLevel, HighLevelTransformsAndBgr) {
    // png_write_png / png_read_png with PNG_TRANSFORM_* flags.
    constexpr png_uint_32 w = 5, h = 2;
    Sink sink;
    {
        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        png_infop info = png_create_info_struct(png);
        png_set_write_fn(png, &sink, write_to, nullptr);
        png_set_IHDR(png, info, w, h, 16, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        std::vector<Bytes> rows(h, Bytes(w * 6));
        for (png_uint_32 y = 0; y < h; ++y)
            for (png_uint_32 x = 0; x < w; ++x)
                for (int c = 0; c < 3; ++c) png_save_uint_16(rows[y].data() + x * 6 + c * 2, static_cast<unsigned>(1000 * (c + 1) + 100 * x + y));
        std::vector<png_bytep> pointers;
        for (auto& row : rows) pointers.push_back(row.data());
        png_set_rows(png, info, pointers.data());
        png_write_png(png, info, PNG_TRANSFORM_IDENTITY, nullptr);
        png_destroy_write_struct(&png, &info);
    }
    Reader r(std::move(sink.data));
    png_read_png(r.png, r.info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_BGR, nullptr);
    EXPECT_EQ(png_get_bit_depth(r.png, r.info), 8);
    png_bytepp rows = png_get_rows(r.png, r.info);
    for (png_uint_32 y = 0; y < h; ++y)
        for (png_uint_32 x = 0; x < w; ++x)
            for (int c = 0; c < 3; ++c) {
                const unsigned v16 = 1000 * (3 - c) + 100 * x + y;  // BGR: blue first
                EXPECT_EQ(rows[y][x * 3 + c], v16 >> 8) << x << "," << y << "," << c;
            }
}

TEST(LowLevel, GammaAndSrgb) {
    // An sRGB image: png_set_sRGB_gAMA_and_cHRM writes sRGB, gAMA and cHRM.
    constexpr png_uint_32 w = 256;
    Sink sink;
    {
        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        png_infop info = png_create_info_struct(png);
        png_set_write_fn(png, &sink, write_to, nullptr);
        png_set_IHDR(png, info, w, 1, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
        png_set_sRGB_gAMA_and_cHRM(png, info, PNG_sRGB_INTENT_PERCEPTUAL);
        png_write_info(png, info);
        Bytes row(w);
        std::iota(row.begin(), row.end(), png_byte{0});
        png_write_row(png, row.data());
        png_write_end(png, nullptr);
        png_destroy_write_struct(&png, &info);
    }
    Reader r(std::move(sink.data));
    png_read_info(r.png, r.info);
    int intent = -1;
    ASSERT_EQ(png_get_sRGB(r.png, r.info, &intent), PNG_INFO_sRGB);
    EXPECT_EQ(intent, PNG_sRGB_INTENT_PERCEPTUAL);
    double gamma = 0;
    ASSERT_EQ(png_get_gAMA(r.png, r.info, &gamma), PNG_INFO_gAMA);
    EXPECT_NEAR(gamma, 0.45455, 1e-5);
    // Decode to linear light, 16 bits: the sRGB curve, monotonic, endpoints kept.
    png_set_gamma(r.png, 1.0, 1.0 / 2.2);
    png_set_expand_16(r.png);
    png_read_update_info(r.png, r.info);
    ASSERT_EQ(png_get_bit_depth(r.png, r.info), 16);
    Bytes row(w * 2);
    png_read_row(r.png, row.data(), nullptr);
    std::vector<png_uint_32> linear;
    for (png_uint_32 x = 0; x < w; ++x) linear.push_back(PNG_get_uint_16(row.data() + 2 * x));
    EXPECT_EQ(linear.front(), 0u);
    EXPECT_EQ(linear.back(), 65535u);
    EXPECT_TRUE(std::ranges::is_sorted(linear));
    EXPECT_LT(linear[128], 65535u / 4);  // mid-grey is far below half in linear light
    png_read_end(r.png, nullptr);
}

// ── progressive reading ──────────────────────────────────────────────────

struct Progressive {
    png_uint_32 width = 0, height = 0, rows_seen = 0;
    std::vector<Bytes> image;
    bool ended = false;
};

void on_info(png_structp png, png_infop info) {
    auto* p = static_cast<Progressive*>(png_get_progressive_ptr(png));
    p->width = png_get_image_width(png, info);
    p->height = png_get_image_height(png, info);
    png_set_expand(png);
    png_set_interlace_handling(png);
    png_read_update_info(png, info);
    p->image.assign(p->height, Bytes(png_get_rowbytes(png, info)));
}
void on_row(png_structp png, png_bytep row, png_uint_32 y, int) {
    auto* p = static_cast<Progressive*>(png_get_progressive_ptr(png));
    png_progressive_combine_row(png, p->image[y].data(), row);
    ++p->rows_seen;
}
void on_end(png_structp png, png_infop) { static_cast<Progressive*>(png_get_progressive_ptr(png))->ended = true; }

TEST(Progressive, FedInSmallPieces) {
    Bytes file = write_indexed(true);
    Progressive state;
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    png_set_progressive_read_fn(png, &state, on_info, on_row, on_end);
    for (std::size_t at = 0; at < file.size(); at += 7)
        png_process_data(png, info, file.data() + at, std::min<std::size_t>(7, file.size() - at));
    png_destroy_read_struct(&png, &info, nullptr);
    EXPECT_TRUE(state.ended);
    EXPECT_EQ(state.width, W);
    EXPECT_GE(state.rows_seen, H);  // a row per pass it appears in
    for (png_uint_32 y = 0; y < H; ++y)
        for (png_uint_32 x = 0; x < W; ++x) EXPECT_EQ(state.image[y][x * 4 + 3], pixel_index(x, y) == 1 ? 128 : pixel_index(x, y) == 4 ? 0 : 255);
}

// ── user memory, version, options, integers ──────────────────────────────

struct Allocations {
    int live = 0, total = 0;
};
png_voidp counting_malloc(png_structp png, png_alloc_size_t size) {
    auto* a = static_cast<Allocations*>(png_get_mem_ptr(png));
    ++a->live;
    ++a->total;
    return std::malloc(size);
}
void counting_free(png_structp png, png_voidp p) {
    if (!p) return;
    --static_cast<Allocations*>(png_get_mem_ptr(png))->live;
    std::free(p);
}

TEST(Library, UserMemoryVersionOptionsIntegers) {
    Allocations a;
    {
        png_structp png = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr, &a, counting_malloc, counting_free);
        png_infop info = png_create_info_struct(png);
        EXPECT_GT(a.total, 0);
        EXPECT_EQ(png_set_option(png, PNG_MAXIMUM_INFLATE_WINDOW, 1), PNG_OPTION_UNSET);
        EXPECT_EQ(png_set_option(png, PNG_MAXIMUM_INFLATE_WINDOW, 0), PNG_OPTION_ON);
        EXPECT_EQ(png_set_option(png, PNG_OPTION_NEXT, 1), PNG_OPTION_INVALID);
        png_set_user_limits(png, 100, 100);
        EXPECT_EQ(png_get_user_width_max(png), 100u);
        png_destroy_read_struct(&png, &info, nullptr);
    }
    EXPECT_EQ(a.live, 0);
    EXPECT_EQ(png_access_version_number(), static_cast<png_uint_32>(PNG_LIBPNG_VER));
    EXPECT_STREQ(png_get_libpng_ver(nullptr), PNG_LIBPNG_VER_STRING);
    EXPECT_STREQ(png_libpng_ver, PNG_LIBPNG_VER_STRING);
    // png.c appends a newline when the C compiler defines __STDC__ (not MSVC's C mode).
    const std::string header = png_get_header_version(nullptr);
    EXPECT_TRUE(header == PNG_HEADER_VERSION_STRING || header == std::string(PNG_HEADER_VERSION_STRING) + "\n") << header;
    png_byte b[4];
    png_save_uint_32(b, 0xdeadbeefU);
    EXPECT_EQ(png_get_uint_32(b), 0xdeadbeefU);
    EXPECT_EQ(PNG_get_uint_32(b), 0xdeadbeefU);
    png_save_int_32(b, -123456);
    EXPECT_EQ(png_get_int_32(b), -123456);
    EXPECT_EQ(PNG_get_int_32(b), -123456);
    png_save_uint_16(b, 0xbeef);
    EXPECT_EQ(png_get_uint_16(b), 0xbeef);
    EXPECT_EQ(PNG_get_uint_16(b), 0xbeef);
    png_byte composite = 0;
    png_composite(composite, 255, 128, 0);
    EXPECT_EQ(composite, 128);
    png_uint_16 composite16 = 0;
    png_composite_16(composite16, 65535, 32768, 0);
    EXPECT_EQ(composite16, 32768);
}

// ── the error path: png_error longjmps to the caller's setjmp ────────────

struct Errors {
    std::string error;
    std::vector<std::string> warnings;
};
void record_error(png_structp png, png_const_charp message) {
    static_cast<Errors*>(png_get_error_ptr(png))->error = message;
    png_longjmp(png, 1);  // as libpng's default handler does: to png_jmpbuf's buffer
}
void record_warning(png_structp png, png_const_charp message) { static_cast<Errors*>(png_get_error_ptr(png))->warnings.push_back(message); }

// Plain C-shaped functions: nothing with a destructor lives between the
// setjmp and the longjmp that returns to it.
int read_rows_or_fail(png_structp png, png_infop info, png_bytep row) {
    if (setjmp(png_jmpbuf(png))) return 1;  // libpng's error handler longjmped here
    png_read_info(png, info);
    png_set_expand(png);
    png_read_update_info(png, info);
    for (png_uint_32 y = 0; y < png_get_image_height(png, info); ++y) png_read_row(png, row, nullptr);
    png_read_end(png, info);
    return 0;
}

int fail_by_png_error(png_structp png) {
    if (setjmp(png_jmpbuf(png))) return 1;
    png_error(png, "raised by the caller");
    return 0;  // not reached
}

TEST(ErrorPath, CorruptDataLongjmpsToSetjmp) {
    Bytes file = write_indexed(false);
    // Corrupt the image data: flip bytes in the IDAT stream after its header.
    const auto idat = std::search(file.begin(), file.end(), std::begin("IDAT"), std::end("IDAT") - 1);
    ASSERT_NE(idat, file.end());
    for (auto it = idat + 6; it < idat + 20; ++it) *it ^= 0x5a;
    for (bool own_handler : {false, true}) {
        Errors errors;
        Sink source{file};
        png_structp png = own_handler ? png_create_read_struct(PNG_LIBPNG_VER_STRING, &errors, record_error, record_warning)
                                      : png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        png_infop info = png_create_info_struct(png);
        png_set_read_fn(png, &source, read_from);
        png_set_crc_action(png, PNG_CRC_QUIET_USE, PNG_CRC_QUIET_USE);  // reach the inflate error, not the CRC
        Bytes row(W * 4);
        EXPECT_EQ(read_rows_or_fail(png, info, row.data()), 1);
        if (own_handler) EXPECT_FALSE(errors.error.empty());
        png_destroy_read_struct(&png, &info, nullptr);  // clean up after the jump, as libpng documents
    }
}

TEST(ErrorPath, TruncatedFileAndCallerRaisedError) {
    Bytes file = write_indexed(false);
    file.resize(file.size() / 2);
    Errors errors;
    Sink source{file};
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, &errors, record_error, record_warning);
    png_infop info = png_create_info_struct(png);
    png_set_read_fn(png, &source, read_from);
    Bytes row(W * 4);
    EXPECT_EQ(read_rows_or_fail(png, info, row.data()), 1);
    EXPECT_EQ(errors.error, "read past the end");  // raised by read_from through png_error
    EXPECT_EQ(fail_by_png_error(png), 1);
    EXPECT_EQ(errors.error, "raised by the caller");
    png_warning(png, "a warning");
    png_chunk_warning(png, "a chunk warning");
    ASSERT_EQ(errors.warnings.size(), 2u);
    EXPECT_EQ(errors.warnings[0], "a warning");
    png_destroy_read_struct(&png, &info, nullptr);
}

}  // namespace


/**
 * @class Compression
 * @brief Provides compression and decompression functionality supporting multiple algorithms.
 * 
 * This class implements compression and decompression operations using three different algorithms:
 * FastLZ, Deflate/Gzip, and Zstandard (ZSTD). It manages compression levels and parameters for
 * each algorithm.
 * 
 * @note Static member variables control compression parameters:
 *       - zlib_level: Compression level for deflate/gzip (default: Z_DEFAULT_COMPRESSION)
 *       - gzip_level: Compression level for gzip (default: Z_DEFAULT_COMPRESSION)
 *       - zstd_level: Compression level for Zstandard (default: 3)
 *       - zstd_long_distance_matching: Enable long distance matching for ZSTD (default: false)
 *       - zstd_window_log_size: Window log size for ZSTD (default: 27)
 * 
 * @enum Mode
 * @brief Compression algorithm modes
 * - MODE_FASTLZ: FastLZ compression algorithm
 * - MODE_DEFLATE: Deflate compression algorithm
 * - MODE_GZIP: Gzip compression algorithm
 * - MODE_ZSTD: Zstandard compression algorithm
 */

/**
 * @brief Compresses data using the specified compression mode.
 * 
 * @param[out] p_dst Destination buffer for compressed data
 * @param[in] p_src Source data to compress
 * @param[in] p_src_size Size of source data in bytes
 * @param[in] p_mode Compression algorithm mode to use
 * @return Compressed data size in bytes, or -1 on failure
 * 
 * @note For FastLZ with source size < 16 bytes, the buffer is padded with zeros
 */

/**
 * @brief Calculates the maximum possible compressed buffer size.
 * 
 * @param[in] p_src_size Size of source data in bytes
 * @param[in] p_mode Compression algorithm mode
 * @return Maximum required buffer size for compressed data, or -1 on failure
 * 
 * @note This method should be called before compression to allocate adequate output buffer
 */

/**
 * @brief Decompresses data using the specified compression mode.
 * 
 * @param[out] p_dst Destination buffer for decompressed data
 * @param[in] p_dst_max_size Maximum size of destination buffer
 * @param[in] p_src Compressed source data
 * @param[in] p_src_size Size of compressed data in bytes
 * @param[in] p_mode Compression algorithm mode used during compression
 * @return Decompressed data size in bytes, or -1 on failure
 * 
 * @note For FastLZ with destination size < 16 bytes, decompression uses a temporary buffer
 */
#include "compression.h"

#include "core/io/zip_io.h"
#include "core/os/copymem.h"
#include "core/project_settings.h"

#include "thirdparty/misc/fastlz.h"

#include <zlib.h>
#include <zstd.h>

int Compression::compress(uint8_t *p_dst, const uint8_t *p_src, int p_src_size, Mode p_mode) {

	switch (p_mode) {
		case MODE_FASTLZ: {

			if (p_src_size < 16) {
				uint8_t src[16];
				zeromem(&src[p_src_size], 16 - p_src_size);
				copymem(src, p_src, p_src_size);
				return fastlz_compress(src, 16, p_dst);
			} else {
				return fastlz_compress(p_src, p_src_size, p_dst);
			}

		} break;
		case MODE_DEFLATE:
		case MODE_GZIP: {

			int window_bits = p_mode == MODE_DEFLATE ? 15 : 15 + 16;

			z_stream strm;
			strm.zalloc = zipio_alloc;
			strm.zfree = zipio_free;
			strm.opaque = Z_NULL;
			int level = p_mode == MODE_DEFLATE ? zlib_level : gzip_level;
			int err = deflateInit2(&strm, level, Z_DEFLATED, window_bits, 8, Z_DEFAULT_STRATEGY);
			if (err != Z_OK)
				return -1;

			strm.avail_in = p_src_size;
			int aout = deflateBound(&strm, p_src_size);
			strm.avail_out = aout;
			strm.next_in = (Bytef *)p_src;
			strm.next_out = p_dst;
			deflate(&strm, Z_FINISH);
			aout = aout - strm.avail_out;
			deflateEnd(&strm);
			return aout;

		} break;
		case MODE_ZSTD: {
			ZSTD_CCtx *cctx = ZSTD_createCCtx();
			ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, zstd_level);
			if (zstd_long_distance_matching) {
				ZSTD_CCtx_setParameter(cctx, ZSTD_c_enableLongDistanceMatching, 1);
				ZSTD_CCtx_setParameter(cctx, ZSTD_c_windowLog, zstd_window_log_size);
			}
			int max_dst_size = get_max_compressed_buffer_size(p_src_size, MODE_ZSTD);
			int ret = ZSTD_compressCCtx(cctx, p_dst, max_dst_size, p_src, p_src_size, zstd_level);
			ZSTD_freeCCtx(cctx);
			return ret;
		} break;
	}

	ERR_FAIL_V(-1);
}

int Compression::get_max_compressed_buffer_size(int p_src_size, Mode p_mode) {

	switch (p_mode) {
		case MODE_FASTLZ: {

			int ss = p_src_size + p_src_size * 6 / 100;
			if (ss < 66)
				ss = 66;
			return ss;

		} break;
		case MODE_DEFLATE:
		case MODE_GZIP: {

			int window_bits = p_mode == MODE_DEFLATE ? 15 : 15 + 16;

			z_stream strm;
			strm.zalloc = zipio_alloc;
			strm.zfree = zipio_free;
			strm.opaque = Z_NULL;
			int err = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, window_bits, 8, Z_DEFAULT_STRATEGY);
			if (err != Z_OK)
				return -1;
			int aout = deflateBound(&strm, p_src_size);
			deflateEnd(&strm);
			return aout;
		} break;
		case MODE_ZSTD: {

			return ZSTD_compressBound(p_src_size);
		} break;
	}

	ERR_FAIL_V(-1);
}

int Compression::decompress(uint8_t *p_dst, int p_dst_max_size, const uint8_t *p_src, int p_src_size, Mode p_mode) {

	switch (p_mode) {
		case MODE_FASTLZ: {

			int ret_size = 0;

			if (p_dst_max_size < 16) {
				uint8_t dst[16];
				ret_size = fastlz_decompress(p_src, p_src_size, dst, 16);
				copymem(p_dst, dst, p_dst_max_size);
			} else {
				ret_size = fastlz_decompress(p_src, p_src_size, p_dst, p_dst_max_size);
			}
			return ret_size;
		} break;
		case MODE_DEFLATE:
		case MODE_GZIP: {

			int window_bits = p_mode == MODE_DEFLATE ? 15 : 15 + 16;

			z_stream strm;
			strm.zalloc = zipio_alloc;
			strm.zfree = zipio_free;
			strm.opaque = Z_NULL;
			strm.avail_in = 0;
			strm.next_in = Z_NULL;
			int err = inflateInit2(&strm, window_bits);
			ERR_FAIL_COND_V(err != Z_OK, -1);

			strm.avail_in = p_src_size;
			strm.avail_out = p_dst_max_size;
			strm.next_in = (Bytef *)p_src;
			strm.next_out = p_dst;

			err = inflate(&strm, Z_FINISH);
			int total = strm.total_out;
			inflateEnd(&strm);
			ERR_FAIL_COND_V(err != Z_STREAM_END, -1);
			return total;
		} break;
		case MODE_ZSTD: {
			ZSTD_DCtx *dctx = ZSTD_createDCtx();
			if (zstd_long_distance_matching) {
				ZSTD_DCtx_setParameter(dctx, ZSTD_d_windowLogMax, zstd_window_log_size);
			}
			int ret = ZSTD_decompressDCtx(dctx, p_dst, p_dst_max_size, p_src, p_src_size);
			ZSTD_freeDCtx(dctx);
			return ret;
		} break;
	}

	ERR_FAIL_V(-1);
}

int Compression::zlib_level = Z_DEFAULT_COMPRESSION;
int Compression::gzip_level = Z_DEFAULT_COMPRESSION;
int Compression::zstd_level = 3;
bool Compression::zstd_long_distance_matching = false;
int Compression::zstd_window_log_size = 27; // ZSTD_WINDOWLOG_LIMIT_DEFAULT

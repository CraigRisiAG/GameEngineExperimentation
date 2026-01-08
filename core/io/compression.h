
/// @class Compression
/// @brief Provides static methods and configuration for data compression and decompression.
/// 
/// This class offers a unified interface for multiple compression algorithms including FastLZ, DEFLATE, Zstandard (ZSTD),
/// and GZIP. It allows configuration of compression levels and ZSTD-specific parameters to balance between compression
/// ratio and performance based on application requirements.
/// 
/// @note All compression methods are static and can be called without instantiating the class.
/// @note The default compression mode is MODE_ZSTD.
#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "core/typedefs.h"

class Compression {

public:
	/// @brief Compression level for zlib (DEFLATE). Higher values result in better compression but slower speed.
	static int zlib_level;

	/// @brief Compression level for GZIP. Higher values result in better compression but slower speed.
	static int gzip_level;

	/// @brief Compression level for Zstandard (ZSTD). Higher values result in better compression but slower speed.
	static int zstd_level;

	/// @brief Enable long distance matching for ZSTD compression.
	static bool zstd_long_distance_matching;

	/// @brief Window log size for ZSTD compression. Controls the maximum back-reference distance.
	static int zstd_window_log_size;

	/// @enum Mode
	/// @brief Supported compression algorithms.
	enum Mode {
		MODE_FASTLZ,
		MODE_DEFLATE,
		MODE_ZSTD,
		MODE_GZIP
	};

	static int compress(uint8_t *p_dst, const uint8_t *p_src, int p_src_size, Mode p_mode = MODE_ZSTD);
	static int get_max_compressed_buffer_size(int p_src_size, Mode p_mode = MODE_ZSTD);
	static int decompress(uint8_t *p_dst, int p_dst_max_size, const uint8_t *p_src, int p_src_size, Mode p_mode = MODE_ZSTD);

	Compression();
};

#endif // COMPRESSION_H

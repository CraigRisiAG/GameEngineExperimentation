
/**
 * @class CryptoCore
 * @brief Provides cryptographic operations including hashing, encryption, and encoding.
 * 
 * This class offers static methods and nested context classes for performing various
 * cryptographic operations such as MD5, SHA1, SHA256 hashing, AES encryption/decryption,
 * and Base64 encoding/decoding.
 */

/**
 * @class CryptoCore::MD5Context
 * @brief Context class for MD5 hash computation.
 * 
 * Manages the state of an MD5 hash operation. Use start() to initialize,
 * update() to process data, and finish() to retrieve the final hash.
 * 
 * @see start(), update(), finish()
 */

/**
 * @class CryptoCore::SHA1Context
 * @brief Context class for SHA1 hash computation.
 * 
 * Manages the state of a SHA1 hash operation. Use start() to initialize,
 * update() to process data, and finish() to retrieve the final hash.
 * 
 * @see start(), update(), finish()
 */

/**
 * @class CryptoCore::SHA256Context
 * @brief Context class for SHA256 hash computation.
 * 
 * Manages the state of a SHA256 hash operation. Use start() to initialize,
 * update() to process data, and finish() to retrieve the final hash.
 * 
 * @see start(), update(), finish()
 */

/**
 * @class CryptoCore::AESContext
 * @brief Context class for AES encryption and decryption operations.
 * 
 * Manages the state of AES encryption/decryption. Set keys using set_encode_key()
 * or set_decode_key(), then use encrypt_ecb() or decrypt_ecb() for block operations.
 * 
 * @see set_encode_key(), set_decode_key(), encrypt_ecb(), decrypt_ecb()
 */

/**
 * @brief Encodes data to Base64 string representation.
 * @param p_src Source data buffer
 * @param p_src_len Length of source data
 * @return Base64 encoded string
 */

/**
 * @brief Encodes data to Base64 format.
 * @param r_dst Destination buffer for encoded data
 * @param p_dst_len Maximum length of destination buffer
 * @param r_len Output parameter containing the encoded data length
 * @param p_src Source data buffer
 * @param p_src_len Length of source data
 * @return Error code indicating success or failure
 */

/**
 * @brief Decodes Base64 formatted data.
 * @param r_dst Destination buffer for decoded data
 * @param p_dst_len Maximum length of destination buffer
 * @param r_len Output parameter containing the decoded data length
 * @param p_src Base64 encoded source buffer
 * @param p_src_len Length of source data
 * @return Error code indicating success or failure
 */

/**
 * @brief Computes MD5 hash of data.
 * @param p_src Source data buffer
 * @param p_src_len Length of source data
 * @param r_hash Output hash array (16 bytes)
 * @return Error code indicating success or failure
 */

/**
 * @brief Computes SHA1 hash of data.
 * @param p_src Source data buffer
 * @param p_src_len Length of source data
 * @param r_hash Output hash array (20 bytes)
 * @return Error code indicating success or failure
 */

/**
 * @brief Computes SHA256 hash of data.
 * @param p_src Source data buffer
 * @param p_src_len Length of source data
 * @param r_hash Output hash array (32 bytes)
 * @return Error code indicating success or failure
 */
#ifndef CRYPTO_CORE_H
#define CRYPTO_CORE_H

#include "core/reference.h"

class CryptoCore {

public:
	class MD5Context {

	private:
		void *ctx; // To include, or not to include...

	public:
		MD5Context();
		~MD5Context();

		Error start();
		Error update(const uint8_t *p_src, size_t p_len);
		Error finish(unsigned char r_hash[16]);
	};

	class SHA1Context {

	private:
		void *ctx; // To include, or not to include...

	public:
		SHA1Context();
		~SHA1Context();

		Error start();
		Error update(const uint8_t *p_src, size_t p_len);
		Error finish(unsigned char r_hash[20]);
	};

	class SHA256Context {

	private:
		void *ctx; // To include, or not to include...

	public:
		SHA256Context();
		~SHA256Context();

		Error start();
		Error update(const uint8_t *p_src, size_t p_len);
		Error finish(unsigned char r_hash[32]);
	};

	class AESContext {

	private:
		void *ctx; // To include, or not to include...

	public:
		AESContext();
		~AESContext();

		Error set_encode_key(const uint8_t *p_key, size_t p_bits);
		Error set_decode_key(const uint8_t *p_key, size_t p_bits);
		Error encrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]);
		Error decrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]);
	};

	static String b64_encode_str(const uint8_t *p_src, int p_src_len);
	static Error b64_encode(uint8_t *r_dst, int p_dst_len, size_t *r_len, const uint8_t *p_src, int p_src_len);
	static Error b64_decode(uint8_t *r_dst, int p_dst_len, size_t *r_len, const uint8_t *p_src, int p_src_len);

	static Error md5(const uint8_t *p_src, int p_src_len, unsigned char r_hash[16]);
	static Error sha1(const uint8_t *p_src, int p_src_len, unsigned char r_hash[20]);
	static Error sha256(const uint8_t *p_src, int p_src_len, unsigned char r_hash[32]);
};
#endif // CRYPTO_CORE_H

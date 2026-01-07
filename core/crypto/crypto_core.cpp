
/**
 * @file crypto_core.cpp
 * @brief Cryptographic operations implementation using mbedTLS library.
 * 
 * This file provides implementations for various cryptographic algorithms:
 * - MD5 hashing with context-based API
 * - SHA1 hashing with context-based API
 * - SHA256 hashing with context-based API
 * - AES encryption/decryption with ECB mode
 * - Base64 encoding/decoding
 * 
 * All hash and cipher contexts manage their own memory allocation and
 * follow RAII principles for proper resource cleanup.
 */

/**
 * @class CryptoCore::MD5Context
 * @brief MD5 hashing context wrapper.
 * 
 * Manages MD5 hash computation with incremental update capability.
 * Memory is automatically allocated in constructor and freed in destructor.
 */

/**
 * @brief Initialize MD5 hashing context.
 * @return OK on success, FAILED on error
 */

/**
 * @brief Update MD5 hash with new data.
 * @param p_src Pointer to input data
 * @param p_len Length of input data in bytes
 * @return OK on success, FAILED on error
 */

/**
 * @brief Finalize MD5 hash computation.
 * @param r_hash Output buffer for 16-byte MD5 hash
 * @return OK on success, FAILED on error
 */

/**
 * @class CryptoCore::SHA1Context
 * @brief SHA1 hashing context wrapper.
 * 
 * Manages SHA1 hash computation with incremental update capability.
 * Memory is automatically allocated in constructor and freed in destructor.
 */

/**
 * @brief Initialize SHA1 hashing context.
 * @return OK on success, FAILED on error
 */

/**
 * @brief Update SHA1 hash with new data.
 * @param p_src Pointer to input data
 * @param p_len Length of input data in bytes
 * @return OK on success, FAILED on error
 */

/**
 * @brief Finalize SHA1 hash computation.
 * @param r_hash Output buffer for 20-byte SHA1 hash
 * @return OK on success, FAILED on error
 */

/**
 * @class CryptoCore::SHA256Context
 * @brief SHA256 hashing context wrapper.
 * 
 * Manages SHA256 hash computation with incremental update capability.
 * Memory is automatically allocated in constructor and freed in destructor.
 */

/**
 * @brief Initialize SHA256 hashing context.
 * @return OK on success, FAILED on error
 */

/**
 * @brief Update SHA256 hash with new data.
 * @param p_src Pointer to input data
 * @param p_len Length of input data in bytes
 * @return OK on success, FAILED on error
 */

/**
 * @brief Finalize SHA256 hash computation.
 * @param r_hash Output buffer for 32-byte SHA256 hash
 * @return OK on success, FAILED on error
 */

/**
 * @class CryptoCore::AESContext
 * @brief AES encryption/decryption context wrapper.
 * 
 * Manages AES cipher operations in ECB mode.
 * Supports both encryption and decryption with separate key setup.
 * Memory is automatically allocated in constructor and freed in destructor.
 */

/**
 * @brief Set AES encryption key.
 * @param p_key Pointer to key data
 * @param p_bits Key size in bits (128, 192, or 256)
 * @return OK on success, FAILED on error
 */

/**
 * @brief Set AES decryption key.
 * @param p_key Pointer to key data
 * @param p_bits Key size in bits (128, 192, or 256)
 * @return OK on success, FAILED on error
 */

/**
 * @brief Encrypt 16-byte block using AES-ECB.
 * @param p_src Input 16-byte plaintext block
 * @param r_dst Output 16-byte ciphertext block
 * @return OK on success, FAILED on error
 */

/**
 * @brief Decrypt 16-byte block using AES-ECB.
 * @param p_src Input 16-byte ciphertext block
 * @param r_dst Output 16-byte plaintext block
 * @return OK on success, FAILED on error
 */

/**
 * @brief Encode binary data to Base64 string.
 * @param p_src Pointer to input binary data
 * @param p_src_len Length of input data in bytes
 * @return Base64-encoded string, empty string on error
 */

/**
 * @brief Encode binary data to Base64.
 * @param r_dst Output buffer for Base64 data
 * @param p_dst_len Size of output buffer
 * @param r_len Pointer to store actual output length
 * @param p_src Pointer to input binary data
 * @param p_src_len Length of input data in bytes
 * @return OK on success, FAILED on error
 */

/**
 * @brief Decode Base64 data to binary.
 * @param r_dst Output buffer for decoded binary data
 * @param p_dst_len Size of output buffer
 * @param r_len Pointer to store actual output length
 * @param p_src Pointer to Base64 input data
 * @param p_src_len Length of input data in bytes
 * @return OK on success, FAILED on error
 */

/**
 * @brief Compute MD5 hash of data.
 * @param p_src Pointer to input data
 * @param p_src_len Length of input data in bytes
 * @param r_hash Output buffer for 16-byte MD5 hash
 * @return OK on success, FAILED on error
 */

/**
 * @brief Compute SHA1 hash of data.
 * @param p_src Pointer to input data
 * @param p_src_len Length of input data in bytes
 * @param r_hash Output buffer for 20-byte SHA1 hash
 * @return OK on success, FAILED on error
 */

/**
 * @brief Compute SHA256 hash of data.
 * @param p_src Pointer to input data
 * @param p_src_len Length of input data in bytes
 * @param r_hash Output buffer for 32-byte SHA256 hash
 * @return OK on success, FAILED on error
 */
#include "crypto_core.h"

#include <mbedtls/aes.h>
#include <mbedtls/base64.h>
#include <mbedtls/md5.h>
#include <mbedtls/sha1.h>
#include <mbedtls/sha256.h>

// MD5
CryptoCore::MD5Context::MD5Context() {
	ctx = memalloc(sizeof(mbedtls_md5_context));
	mbedtls_md5_init((mbedtls_md5_context *)ctx);
}

CryptoCore::MD5Context::~MD5Context() {
	mbedtls_md5_free((mbedtls_md5_context *)ctx);
	memfree((mbedtls_md5_context *)ctx);
}

Error CryptoCore::MD5Context::start() {
	int ret = mbedtls_md5_starts_ret((mbedtls_md5_context *)ctx);
	return ret ? FAILED : OK;
}

Error CryptoCore::MD5Context::update(const uint8_t *p_src, size_t p_len) {
	int ret = mbedtls_md5_update_ret((mbedtls_md5_context *)ctx, p_src, p_len);
	return ret ? FAILED : OK;
}

Error CryptoCore::MD5Context::finish(unsigned char r_hash[16]) {
	int ret = mbedtls_md5_finish_ret((mbedtls_md5_context *)ctx, r_hash);
	return ret ? FAILED : OK;
}

// SHA1
CryptoCore::SHA1Context::SHA1Context() {
	ctx = memalloc(sizeof(mbedtls_sha1_context));
	mbedtls_sha1_init((mbedtls_sha1_context *)ctx);
}

CryptoCore::SHA1Context::~SHA1Context() {
	mbedtls_sha1_free((mbedtls_sha1_context *)ctx);
	memfree((mbedtls_sha1_context *)ctx);
}

Error CryptoCore::SHA1Context::start() {
	int ret = mbedtls_sha1_starts_ret((mbedtls_sha1_context *)ctx);
	return ret ? FAILED : OK;
}

Error CryptoCore::SHA1Context::update(const uint8_t *p_src, size_t p_len) {
	int ret = mbedtls_sha1_update_ret((mbedtls_sha1_context *)ctx, p_src, p_len);
	return ret ? FAILED : OK;
}

Error CryptoCore::SHA1Context::finish(unsigned char r_hash[20]) {
	int ret = mbedtls_sha1_finish_ret((mbedtls_sha1_context *)ctx, r_hash);
	return ret ? FAILED : OK;
}

// SHA256
CryptoCore::SHA256Context::SHA256Context() {
	ctx = memalloc(sizeof(mbedtls_sha256_context));
	mbedtls_sha256_init((mbedtls_sha256_context *)ctx);
}

CryptoCore::SHA256Context::~SHA256Context() {
	mbedtls_sha256_free((mbedtls_sha256_context *)ctx);
	memfree((mbedtls_sha256_context *)ctx);
}

Error CryptoCore::SHA256Context::start() {
	int ret = mbedtls_sha256_starts_ret((mbedtls_sha256_context *)ctx, 0);
	return ret ? FAILED : OK;
}

Error CryptoCore::SHA256Context::update(const uint8_t *p_src, size_t p_len) {
	int ret = mbedtls_sha256_update_ret((mbedtls_sha256_context *)ctx, p_src, p_len);
	return ret ? FAILED : OK;
}

Error CryptoCore::SHA256Context::finish(unsigned char r_hash[32]) {
	int ret = mbedtls_sha256_finish_ret((mbedtls_sha256_context *)ctx, r_hash);
	return ret ? FAILED : OK;
}

// AES256
CryptoCore::AESContext::AESContext() {
	ctx = memalloc(sizeof(mbedtls_aes_context));
	mbedtls_aes_init((mbedtls_aes_context *)ctx);
}

CryptoCore::AESContext::~AESContext() {
	mbedtls_aes_free((mbedtls_aes_context *)ctx);
	memfree((mbedtls_aes_context *)ctx);
}

Error CryptoCore::AESContext::set_encode_key(const uint8_t *p_key, size_t p_bits) {
	int ret = mbedtls_aes_setkey_enc((mbedtls_aes_context *)ctx, p_key, p_bits);
	return ret ? FAILED : OK;
}

Error CryptoCore::AESContext::set_decode_key(const uint8_t *p_key, size_t p_bits) {
	int ret = mbedtls_aes_setkey_dec((mbedtls_aes_context *)ctx, p_key, p_bits);
	return ret ? FAILED : OK;
}

Error CryptoCore::AESContext::encrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]) {
	int ret = mbedtls_aes_crypt_ecb((mbedtls_aes_context *)ctx, MBEDTLS_AES_ENCRYPT, p_src, r_dst);
	return ret ? FAILED : OK;
}

Error CryptoCore::AESContext::decrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]) {
	int ret = mbedtls_aes_crypt_ecb((mbedtls_aes_context *)ctx, MBEDTLS_AES_DECRYPT, p_src, r_dst);
	return ret ? FAILED : OK;
}

// CryptoCore
String CryptoCore::b64_encode_str(const uint8_t *p_src, int p_src_len) {
	int b64len = p_src_len / 3 * 4 + 4 + 1;
	Vector<uint8_t> b64buff;
	b64buff.resize(b64len);
	uint8_t *w64 = b64buff.ptrw();
	size_t strlen = 0;
	int ret = b64_encode(&w64[0], b64len, &strlen, p_src, p_src_len);
	w64[strlen] = 0;
	return ret ? String() : (const char *)&w64[0];
}

Error CryptoCore::b64_encode(uint8_t *r_dst, int p_dst_len, size_t *r_len, const uint8_t *p_src, int p_src_len) {
	int ret = mbedtls_base64_encode(r_dst, p_dst_len, r_len, p_src, p_src_len);
	return ret ? FAILED : OK;
}

Error CryptoCore::b64_decode(uint8_t *r_dst, int p_dst_len, size_t *r_len, const uint8_t *p_src, int p_src_len) {
	int ret = mbedtls_base64_decode(r_dst, p_dst_len, r_len, p_src, p_src_len);
	return ret ? FAILED : OK;
}

Error CryptoCore::md5(const uint8_t *p_src, int p_src_len, unsigned char r_hash[16]) {
	int ret = mbedtls_md5_ret(p_src, p_src_len, r_hash);
	return ret ? FAILED : OK;
}

Error CryptoCore::sha1(const uint8_t *p_src, int p_src_len, unsigned char r_hash[20]) {
	int ret = mbedtls_sha1_ret(p_src, p_src_len, r_hash);
	return ret ? FAILED : OK;
}

Error CryptoCore::sha256(const uint8_t *p_src, int p_src_len, unsigned char r_hash[32]) {
	int ret = mbedtls_sha256_ret(p_src, p_src_len, r_hash, 0);
	return ret ? FAILED : OK;
}

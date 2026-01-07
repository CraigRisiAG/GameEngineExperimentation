
/// @class HashingContext
/// @brief Provides an interface for computing cryptographic hashes with support for MD5, SHA1, and SHA256 algorithms.
/// 
/// HashingContext allows incremental hashing of data through a three-step process: start(), update(), and finish().
/// The context maintains internal state for the selected hash algorithm and ensures proper resource management.
/// 
/// @note Only one hashing operation can be active at a time per HashingContext instance.
/// @note The finish() method automatically cleans up the internal context.

/// @enum HashType
/// @brief Enumeration of supported hash algorithms.
/// @var HASH_MD5 - MD5 hash algorithm (128-bit output)
/// @var HASH_SHA1 - SHA-1 hash algorithm (160-bit output)
/// @var HASH_SHA256 - SHA-256 hash algorithm (256-bit output)

/// @method Error start(HashType p_type)
/// @brief Initializes a new hashing context with the specified hash algorithm.
/// @param p_type The hash algorithm type to use (HASH_MD5, HASH_SHA1, or HASH_SHA256)
/// @return OK if successful, ERR_ALREADY_IN_USE if a context is already active, ERR_UNAVAILABLE if initialization fails
/// @note Must be called before update() and finish()

/// @method Error update(PackedByteArray p_chunk)
/// @brief Updates the hash with the provided data chunk.
/// @param p_chunk The data to hash
/// @return OK if successful, ERR_UNCONFIGURED if start() was not called, FAILED if the chunk is empty
/// @note Can be called multiple times to hash data incrementally

/// @method PackedByteArray finish()
/// @brief Finalizes the hash computation and returns the digest.
/// @return The computed hash as a PackedByteArray (16 bytes for MD5, 20 for SHA1, 32 for SHA256), or empty array on error
/// @note Automatically cleans up the internal context after completion

/// @method void _create_ctx(HashType p_type)
/// @brief Internal helper that allocates and initializes the appropriate hash context.
/// @param p_type The hash algorithm type to initialize
/// @private

/// @method void _delete_ctx()
/// @brief Internal helper that deallocates the current hash context.
/// @private

/// @method void _bind_methods()
/// @brief Binds the class methods to the scripting interface.
/// @private
#include "hashing_context.h"

#include "core/crypto/crypto_core.h"

Error HashingContext::start(HashType p_type) {
	ERR_FAIL_COND_V(ctx != NULL, ERR_ALREADY_IN_USE);
	_create_ctx(p_type);
	ERR_FAIL_COND_V(ctx == NULL, ERR_UNAVAILABLE);
	switch (type) {
		case HASH_MD5:
			return ((CryptoCore::MD5Context *)ctx)->start();
		case HASH_SHA1:
			return ((CryptoCore::SHA1Context *)ctx)->start();
		case HASH_SHA256:
			return ((CryptoCore::SHA256Context *)ctx)->start();
	}
	return ERR_UNAVAILABLE;
}

Error HashingContext::update(PackedByteArray p_chunk) {
	ERR_FAIL_COND_V(ctx == NULL, ERR_UNCONFIGURED);
	size_t len = p_chunk.size();
	ERR_FAIL_COND_V(len == 0, FAILED);
	const uint8_t *r = p_chunk.ptr();
	switch (type) {
		case HASH_MD5:
			return ((CryptoCore::MD5Context *)ctx)->update(&r[0], len);
		case HASH_SHA1:
			return ((CryptoCore::SHA1Context *)ctx)->update(&r[0], len);
		case HASH_SHA256:
			return ((CryptoCore::SHA256Context *)ctx)->update(&r[0], len);
	}
	return ERR_UNAVAILABLE;
}

PackedByteArray HashingContext::finish() {
	ERR_FAIL_COND_V(ctx == NULL, PackedByteArray());
	PackedByteArray out;
	Error err = FAILED;
	switch (type) {
		case HASH_MD5:
			out.resize(16);
			err = ((CryptoCore::MD5Context *)ctx)->finish(out.ptrw());
			break;
		case HASH_SHA1:
			out.resize(20);
			err = ((CryptoCore::SHA1Context *)ctx)->finish(out.ptrw());
			break;
		case HASH_SHA256:
			out.resize(32);
			err = ((CryptoCore::SHA256Context *)ctx)->finish(out.ptrw());
			break;
	}
	_delete_ctx();
	ERR_FAIL_COND_V(err != OK, PackedByteArray());
	return out;
}

void HashingContext::_create_ctx(HashType p_type) {
	type = p_type;
	switch (type) {
		case HASH_MD5:
			ctx = memnew(CryptoCore::MD5Context);
			break;
		case HASH_SHA1:
			ctx = memnew(CryptoCore::SHA1Context);
			break;
		case HASH_SHA256:
			ctx = memnew(CryptoCore::SHA256Context);
			break;
		default:
			ctx = NULL;
	}
}

void HashingContext::_delete_ctx() {

	switch (type) {
		case HASH_MD5:
			memdelete((CryptoCore::MD5Context *)ctx);
			break;
		case HASH_SHA1:
			memdelete((CryptoCore::SHA1Context *)ctx);
			break;
		case HASH_SHA256:
			memdelete((CryptoCore::SHA256Context *)ctx);
			break;
	}
	ctx = NULL;
}

void HashingContext::_bind_methods() {
	ClassDB::bind_method(D_METHOD("start", "type"), &HashingContext::start);
	ClassDB::bind_method(D_METHOD("update", "chunk"), &HashingContext::update);
	ClassDB::bind_method(D_METHOD("finish"), &HashingContext::finish);
	BIND_ENUM_CONSTANT(HASH_MD5);
	BIND_ENUM_CONSTANT(HASH_SHA1);
	BIND_ENUM_CONSTANT(HASH_SHA256);
}

HashingContext::HashingContext() {
	ctx = NULL;
}

HashingContext::~HashingContext() {
	if (ctx != NULL)
		_delete_ctx();
}

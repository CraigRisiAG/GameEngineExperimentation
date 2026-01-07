
/**
 * @class HashingContext
 * @brief A reference-counted wrapper for cryptographic hashing operations.
 * 
 * Provides an interface for computing cryptographic hashes of data streams.
 * Supports multiple hashing algorithms including MD5, SHA1, and SHA256.
 * 
 * @note This class is reference-counted and should be used with Ref<> pointers.
 * 
 * @see Reference
 */

/**
 * @enum HashType
 * @brief Enumeration of supported hash algorithms.
 * 
 * @var HASH_MD5 MD5 hashing algorithm
 * @var HASH_SHA1 SHA1 hashing algorithm
 * @var HASH_SHA256 SHA256 hashing algorithm
 */

/**
 * @fn Error start(HashType p_type)
 * @brief Initializes a new hashing context with the specified algorithm.
 * 
 * @param p_type The hashing algorithm to use
 * @return Error code indicating success or failure
 * 
 * @note Must be called before update() and finish()
 */

/**
 * @fn Error update(PackedByteArray p_chunk)
 * @brief Processes a chunk of data for hashing.
 * 
 * @param p_chunk The data to process
 * @return Error code indicating success or failure
 * 
 * @note start() must be called before this method
 */

/**
 * @fn PackedByteArray finish()
 * @brief Completes the hashing operation and returns the computed hash.
 * 
 * @return The computed hash digest as a byte array
 * 
 * @note This method finalizes the hashing context
 */
#ifndef HASHING_CONTEXT_H
#define HASHING_CONTEXT_H

#include "core/reference.h"

class HashingContext : public Reference {
	GDCLASS(HashingContext, Reference);

public:
	enum HashType {
		HASH_MD5,
		HASH_SHA1,
		HASH_SHA256
	};

private:
	void *ctx;
	HashType type;

protected:
	static void _bind_methods();
	void _create_ctx(HashType p_type);
	void _delete_ctx();

public:
	Error start(HashType p_type);
	Error update(PackedByteArray p_chunk);
	PackedByteArray finish();

	HashingContext();
	~HashingContext();
};

VARIANT_ENUM_CAST(HashingContext::HashType);

#endif // HASHING_CONTEXT_H

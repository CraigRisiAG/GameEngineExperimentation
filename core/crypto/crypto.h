

/**
 * @file crypto.h
 * @brief Cryptographic operations and resource management for the game engine.
 * 
 * This module provides interfaces and implementations for cryptographic key generation,
 * X.509 certificate handling, and general cryptographic operations. It includes resource
 * loading and saving functionality for crypto assets.
 */

/**
 * @class CryptoKey
 * @brief Abstract base class for cryptographic key management.
 * 
 * Provides an interface for loading and saving cryptographic keys from/to disk.
 * Subclasses should implement specific key types (RSA, etc.).
 */

/**
 * @brief Creates a new CryptoKey instance.
 * @return Pointer to newly created CryptoKey, or nullptr if creation fails.
 */

/**
 * @brief Loads a cryptographic key from a file.
 * @param p_path Path to the key file.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Saves the cryptographic key to a file.
 * @param p_path Path where the key should be saved.
 * @return Error code indicating success or failure.
 */

/**
 * @class X509Certificate
 * @brief Manages X.509 certificate data and operations.
 * 
 * Handles loading, saving, and management of X.509 digital certificates used for
 * authentication and encryption operations.
 */

/**
 * @brief Creates a new X509Certificate instance.
 * @return Pointer to newly created X509Certificate, or nullptr if creation fails.
 */

/**
 * @brief Loads a certificate from a file.
 * @param p_path Path to the certificate file.
 * @return Error code indicating success or failure.
 */

/**
 * @brief Loads a certificate from memory buffer.
 * @param p_buffer Pointer to certificate data in memory.
 * @param p_len Length of the certificate data in bytes.
 * @return Error code indicating success or failure.
 */

/**
 * @class Crypto
 * @brief Core cryptographic operations interface.
 * 
 * Provides functionality for random byte generation, RSA key generation, and
 * self-signed certificate creation. Manages default system certificates.
 */

/**
 * @brief Creates a new Crypto instance.
 * @return Pointer to newly created Crypto object.
 */

/**
 * @brief Loads default system certificates.
 * @param p_path Path to the certificate store or configuration file.
 */

/**
 * @brief Generates cryptographically secure random bytes.
 * @param p_bytes Number of random bytes to generate.
 * @return PackedByteArray containing the random bytes.
 */

/**
 * @brief Generates an RSA key pair.
 * @param p_bytes Key size in bits (typically 2048 or 4096).
 * @return Reference to newly generated CryptoKey containing the RSA key pair.
 */

/**
 * @brief Generates a self-signed X.509 certificate.
 * @param p_key The cryptographic key to use for signing.
 * @param p_issuer_name Distinguished name of the issuer.
 * @param p_not_before Validity period start date (format: YYYYMMDDHHMMSS).
 * @param p_not_after Validity period end date (format: YYYYMMDDHHMMSS).
 * @return Reference to newly generated X509Certificate.
 */

/**
 * @class ResourceFormatLoaderCrypto
 * @brief Resource loader for cryptographic assets.
 * 
 * Handles loading of CryptoKey and X509Certificate resources from disk.
 */

/**
 * @class ResourceFormatSaverCrypto
 * @brief Resource saver for cryptographic assets.
 * 
 * Handles saving of CryptoKey and X509Certificate resources to disk.
 */
#ifndef CRYPTO_H
#define CRYPTO_H

#include "core/reference.h"
#include "core/resource.h"

#include "core/io/resource_loader.h"
#include "core/io/resource_saver.h"

class CryptoKey : public Resource {
	GDCLASS(CryptoKey, Resource);

protected:
	static void _bind_methods();
	static CryptoKey *(*_create)();

public:
	static CryptoKey *create();
	virtual Error load(String p_path) = 0;
	virtual Error save(String p_path) = 0;
};

class X509Certificate : public Resource {
	GDCLASS(X509Certificate, Resource);

protected:
	static void _bind_methods();
	static X509Certificate *(*_create)();

public:
	static X509Certificate *create();
	virtual Error load(String p_path) = 0;
	virtual Error load_from_memory(const uint8_t *p_buffer, int p_len) = 0;
	virtual Error save(String p_path) = 0;
};

class Crypto : public Reference {
	GDCLASS(Crypto, Reference);

protected:
	static void _bind_methods();
	static Crypto *(*_create)();
	static void (*_load_default_certificates)(String p_path);

public:
	static Crypto *create();
	static void load_default_certificates(String p_path);

	virtual PackedByteArray generate_random_bytes(int p_bytes);
	virtual Ref<CryptoKey> generate_rsa(int p_bytes);
	virtual Ref<X509Certificate> generate_self_signed_certificate(Ref<CryptoKey> p_key, String p_issuer_name, String p_not_before, String p_not_after);

	Crypto();
};

class ResourceFormatLoaderCrypto : public ResourceFormatLoader {
	GDCLASS(ResourceFormatLoaderCrypto, ResourceFormatLoader);

public:
	virtual RES load(const String &p_path, const String &p_original_path = "", Error *r_error = NULL, bool p_use_sub_threads = false, float *r_progress = nullptr);
	virtual void get_recognized_extensions(List<String> *p_extensions) const;
	virtual bool handles_type(const String &p_type) const;
	virtual String get_resource_type(const String &p_path) const;
};

class ResourceFormatSaverCrypto : public ResourceFormatSaver {
	GDCLASS(ResourceFormatSaverCrypto, ResourceFormatSaver);

public:
	virtual Error save(const String &p_path, const RES &p_resource, uint32_t p_flags = 0);
	virtual void get_recognized_extensions(const RES &p_resource, List<String> *p_extensions) const;
	virtual bool recognize(const RES &p_resource) const;
};

#endif // CRYPTO_H

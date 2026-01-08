

/**
 * @class FileAccessEncrypted
 * @brief Provides encrypted file access with AES256 encryption support.
 * 
 * FileAccessEncrypted is a specialized file access class that extends FileAccess
 * to support reading and writing encrypted files. It wraps an existing FileAccess
 * object and handles encryption/decryption transparently.
 * 
 * @details
 * The class supports two operational modes:
 * - MODE_READ: Reading and decrypting existing encrypted files
 * - MODE_WRITE_AES256: Writing and encrypting files using AES256 algorithm
 * 
 * Files can be opened using either a raw binary key or a password-derived key.
 * The class maintains an internal buffer of decrypted data for read operations
 * and handles seek operations within the encrypted content.
 * 
 * @note All read/write operations work on the decrypted data transparently.
 *       The underlying FileAccess object manages the actual encrypted file I/O.
 */
#ifndef FILE_ACCESS_ENCRYPTED_H
#define FILE_ACCESS_ENCRYPTED_H

#include "core/os/file_access.h"

class FileAccessEncrypted : public FileAccess {
public:
	enum Mode {
		MODE_READ,
		MODE_WRITE_AES256,
		MODE_MAX
	};

private:
	Mode mode;
	Vector<uint8_t> key;
	bool writing;
	FileAccess *file;
	size_t base;
	size_t length;
	Vector<uint8_t> data;
	mutable int pos;
	mutable bool eofed;

public:
	Error open_and_parse(FileAccess *p_base, const Vector<uint8_t> &p_key, Mode p_mode);
	Error open_and_parse_password(FileAccess *p_base, const String &p_key, Mode p_mode);

	virtual Error _open(const String &p_path, int p_mode_flags); ///< open a file
	virtual void close(); ///< close a file
	virtual bool is_open() const; ///< true when file is open

	virtual String get_path() const; /// returns the path for the current open file
	virtual String get_path_absolute() const; /// returns the absolute path for the current open file

	virtual void seek(size_t p_position); ///< seek to a given position
	virtual void seek_end(int64_t p_position = 0); ///< seek from the end of file
	virtual size_t get_position() const; ///< get position in the file
	virtual size_t get_len() const; ///< get size of the file

	virtual bool eof_reached() const; ///< reading passed EOF

	virtual uint8_t get_8() const; ///< get a byte
	virtual int get_buffer(uint8_t *p_dst, int p_length) const;

	virtual Error get_error() const; ///< get last error

	virtual void flush();
	virtual void store_8(uint8_t p_dest); ///< store a byte
	virtual void store_buffer(const uint8_t *p_src, int p_length); ///< store an array of bytes

	virtual bool file_exists(const String &p_name); ///< return true if a file exists

	virtual uint64_t _get_modified_time(const String &p_file);
	virtual uint32_t _get_unix_permissions(const String &p_file);
	virtual Error _set_unix_permissions(const String &p_file, uint32_t p_permissions);

	FileAccessEncrypted();
	~FileAccessEncrypted();
};

#endif // FILE_ACCESS_ENCRYPTED_H

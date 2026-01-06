
/// @class DirAccessOSX
/// @brief macOS-specific directory access implementation.
///
/// DirAccessOSX extends DirAccessUnix to provide macOS-specific file system
/// operations and directory handling. It manages directory traversal and drive
/// access on macOS platforms.
///
/// @note This class is only compiled when UNIX_ENABLED or LIBC_FILEIO_ENABLED
/// is defined.
///
/// @see DirAccessUnix
/// @see DirAccess
class DirAccessOSX : public DirAccessUnix {
protected:
	/// @brief Converts and fixes Unicode-encoded file names for macOS.
	///
	/// Handles platform-specific Unicode normalization and character encoding
	/// to ensure proper file name handling on macOS file systems.
	///
	/// @param p_name The file name to process.
	/// @return The corrected Unicode file name string.
	virtual String fix_unicode_name(const char *p_name) const;

	/// @brief Retrieves the total number of mounted drives.
	///
	/// @return The count of accessible drives on the system.
	virtual int get_drive_count();

	/// @brief Gets the name of a specific drive by index.
	///
	/// @param p_drive The zero-based index of the drive.
	/// @return The name/path of the drive at the specified index.
	virtual String get_drive(int p_drive);
};

#endif //UNIX ENABLED
#endif

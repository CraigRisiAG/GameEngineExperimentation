


/**
 * @class PoolAllocator
 * @brief Generic memory pool allocator with locking, compacting, and alignment support.
 * 
 * A thread-safe memory pool allocator designed to manage allocation in specific memory regions
 * such as texture memory, audio sample memory, or general-purpose memory. It provides efficient
 * allocation, deallocation, and resizing of memory blocks with optional locking mechanisms.
 * 
 * @note Alignment support is currently a TODO item.
 * @note Platform-specific abstraction for non-reachable memory (e.g., GPU memory) is a TODO item.
 * 
 * @author Juan Linietsky <reduzio@gmail.com>
 * 
 * @see POOL_ALLOCATOR_INVALID_ID for error handling
 */

/**
 * @brief Allocates memory from the pool.
 * @param p_size The size in bytes of memory to allocate.
 * @return An ID representing the allocated memory block on success, or POOL_ALLOCATOR_INVALID_ID on failure.
 */

/**
 * @brief Frees a previously allocated memory block.
 * @param p_mem The ID of the memory block to free.
 */

/**
 * @brief Resizes an existing memory allocation.
 * @param p_mem The ID of the memory block to resize.
 * @param p_new_size The new size in bytes.
 * @return An error code indicating success or failure.
 */

/**
 * @brief Retrieves the size of an allocated memory block.
 * @param p_mem The ID of the memory block.
 * @return The size in bytes of the allocated memory block.
 */

/**
 * @brief Gets the total free memory available in the pool.
 * @return The amount of free memory in bytes.
 */

/**
 * @brief Gets the total memory currently in use.
 * @return The amount of used memory in bytes.
 */

/**
 * @brief Gets the peak free memory recorded since pool creation.
 * @return The peak free memory in bytes.
 */

/**
 * @brief Locks an allocated memory block to prevent compaction.
 * @param p_mem The ID of the memory block to lock.
 * @return An error code indicating success or failure.
 * @note This method is a TODO and may be moved elsewhere.
 */

/**
 * @brief Retrieves a pointer to the allocated memory block.
 * @param p_mem The ID of the memory block.
 * @return A pointer to the memory block, or nullptr if invalid.
 */

/**
 * @brief Retrieves a const pointer to the allocated memory block.
 * @param p_mem The ID of the memory block.
 * @return A const pointer to the memory block, or nullptr if invalid.
 */

/**
 * @brief Unlocks a previously locked memory block.
 * @param p_mem The ID of the memory block to unlock.
 */

/**
 * @brief Checks if a memory block is currently locked.
 * @param p_mem The ID of the memory block.
 * @return true if the memory block is locked, false otherwise.
 */

/**
 * @brief Constructs a PoolAllocator with a specified size.
 * @param p_size The total size of the memory pool in bytes.
 * @param p_needs_locking Whether to enable thread-safe locking (default: false).
 * @param p_max_entries The maximum number of allocations allowed (default: DEFAULT_MAX_ALLOCS).
 */

/**
 * @brief Constructs a PoolAllocator using externally provided memory.
 * @param p_mem Pointer to the pre-allocated memory region.
 * @param p_size The total size of the memory region in bytes.
 * @param p_align The alignment requirement in bytes (default: 1).
 * @param p_needs_locking Whether to enable thread-safe locking (default: false).
 * @param p_max_entries The maximum number of allocations allowed (default: DEFAULT_MAX_ALLOCS).
 */

/**
 * @brief Constructs a PoolAllocator with alignment and size parameters.
 * @param p_align The alignment requirement in bytes.
 * @param p_size The total size of the memory pool in bytes.
 * @param p_needs_locking Whether to enable thread-safe locking (default: false).
 * @param p_max_entries The maximum number of allocations allowed (default: DEFAULT_MAX_ALLOCS).
 */

/**
 * @brief Destructs the PoolAllocator and releases all resources.
 */

/**
 * @brief Virtual method for custom multi-threaded locking implementation.
 * @note Reimplement this method to provide custom locking behavior.
 */

/**
 * @brief Virtual method for custom multi-threaded unlocking implementation.
 * @note Reimplement this method to provide custom unlocking behavior.
 */
#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include "core/typedefs.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
 * Generic Pool Allocator.
 * This is a generic memory pool allocator, with locking, compacting and alignment. (@TODO alignment)
 * It used as a standard way to manage alloction in a specific region of memory, such as texture memory,
 * audio sample memory, or just any kind of memory overall.
 * (@TODO) abstraction should be greater, because in many platforms, you need to manage a nonreachable memory.
*/

enum {

	POOL_ALLOCATOR_INVALID_ID = -1 ///< default invalid value. use INVALID_ID( id ) to test
};

class PoolAllocator {
public:
	typedef int ID;

private:
	enum {
		CHECK_BITS = 8,
		CHECK_LEN = (1 << CHECK_BITS),
		CHECK_MASK = CHECK_LEN - 1

	};

	struct Entry {

		unsigned int pos;
		unsigned int len;
		unsigned int lock;
		unsigned int check;

		inline void clear() {
			pos = 0;
			len = 0;
			lock = 0;
			check = 0;
		}
		Entry() { clear(); }
	};

	typedef int EntryArrayPos;
	typedef int EntryIndicesPos;

	Entry *entry_array;
	int *entry_indices;
	int entry_max;
	int entry_count;

	uint8_t *pool;
	void *mem_ptr;
	int pool_size;

	int free_mem;
	int free_mem_peak;

	unsigned int check_count;
	int align;

	bool needs_locking;

	inline int entry_end(const Entry &p_entry) const {
		return p_entry.pos + aligned(p_entry.len);
	}
	inline int aligned(int p_size) const {

		int rem = p_size % align;
		if (rem)
			p_size += align - rem;

		return p_size;
	}

	void compact(int p_up_to = -1);
	void compact_up(int p_from = 0);
	bool get_free_entry(EntryArrayPos *p_pos);
	bool find_hole(EntryArrayPos *p_pos, int p_for_size);
	bool find_entry_index(EntryIndicesPos *p_map_pos, Entry *p_entry);
	Entry *get_entry(ID p_mem);
	const Entry *get_entry(ID p_mem) const;

	void create_pool(void *p_mem, int p_size, int p_max_entries);

protected:
	virtual void mt_lock() const; ///< Reimplement for custom mt locking
	virtual void mt_unlock() const; ///< Reimplement for custom mt locking

public:
	enum {
		DEFAULT_MAX_ALLOCS = 4096,
	};

	ID alloc(int p_size); ///< Alloc memory, get an ID on success, POOL_ALOCATOR_INVALID_ID on failure
	void free(ID p_mem); ///< Free allocated memory
	Error resize(ID p_mem, int p_new_size); ///< resize a memory chunk
	int get_size(ID p_mem) const;

	int get_free_mem(); ///< get free memory
	int get_used_mem() const;
	int get_free_peak(); ///< get free memory

	Error lock(ID p_mem); //@todo move this out
	void *get(ID p_mem);
	const void *get(ID p_mem) const;
	void unlock(ID p_mem);
	bool is_locked(ID p_mem) const;

	PoolAllocator(int p_size, bool p_needs_locking = false, int p_max_entries = DEFAULT_MAX_ALLOCS);
	PoolAllocator(void *p_mem, int p_size, int p_align = 1, bool p_needs_locking = false, int p_max_entries = DEFAULT_MAX_ALLOCS);
	PoolAllocator(int p_align, int p_size, bool p_needs_locking = false, int p_max_entries = DEFAULT_MAX_ALLOCS);

	virtual ~PoolAllocator();
};

#endif // POOL_ALLOCATOR_H

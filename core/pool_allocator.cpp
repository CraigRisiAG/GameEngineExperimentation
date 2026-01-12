

/**
 * @file pool_allocator.cpp
 * @brief Implementation of a pool-based memory allocator with support for allocation, deallocation, and memory compaction.
 * 
 * This file provides the implementation of the PoolAllocator class, which manages a fixed-size memory pool
 * and allocates/deallocates memory in chunks. Key features include:
 * - Fragmentation management through memory compaction (forward and backward)
 * - Entry-based tracking of allocated blocks with position, size, and lock status
 * - Thread-safe operations through mutex locking
 * - Memory hole detection and reuse
 * - Resize operations with automatic compaction when necessary
 * - Lock/unlock mechanism to prevent memory relocation during access
 * 
 * @note The allocator maintains an entry array to track allocated blocks and an entry indices array
 *       to maintain sorted order of entries by position in the pool.
 */

/**
 * @brief Moves memory in the pool to compact it, eliminating fragmentation.
 * 
 * @param m_entry Reference to the entry to move
 * @param m_to_pos The target position in the pool where the entry should be moved
 * 
 * Macro that copies an allocated block to a new position and updates the entry's position field.
 * Used during memory compaction operations.
 */

/**
 * @brief Acquires a lock for thread-safe operations.
 * 
 * Currently a no-op placeholder that can be implemented for actual mutex locking
 * when thread safety is required.
 */

/**
 * @brief Releases a lock for thread-safe operations.
 * 
 * Currently a no-op placeholder that can be implemented for actual mutex unlocking
 * when thread safety is required.
 */

/**
 * @brief Finds a free entry slot in the entry array.
 * 
 * @param p_pos Output parameter that receives the index of the free entry
 * @return true if a free entry was found, false if the entry array is full
 * 
 * Searches for an entry with length 0 (indicating it's free/unused).
 */

/**
 * @brief Finds a suitable hole (gap) in the pool to accommodate a new allocation.
 * 
 * @param p_pos Output parameter receiving the index where the hole is found
 * @param p_for_size The size needed to fit in the hole
 * @return true if a hole was found, false otherwise
 * 
 * Scans through allocated entries to find gaps between them, then checks
 * if remaining space at the pool's end is sufficient. The hole position is
 * the entry index after which the hole exists.
 */

/**
 * @brief Compacts memory forward up to a specified entry index.
 * 
 * @param p_up_to The index up to which compaction should occur (default: all entries)
 * 
 * Moves allocated blocks forward (toward lower addresses) to eliminate holes,
 * skipping locked entries. This reduces fragmentation when allocating new memory.
 */

/**
 * @brief Compacts memory backward from a specified entry index.
 * 
 * @param p_from The starting entry index for backward compaction
 * 
 * Moves allocated blocks backward (toward higher addresses) to eliminate holes
 * after a specified position, skipping locked entries. Used when trying to
 * resize an allocation by creating space after it.
 */

/**
 * @brief Finds the index position of an entry within the entry indices array.
 * 
 * @param p_map_pos Output parameter receiving the position in entry_indices
 * @param p_entry Pointer to the entry to locate
 * @return true if the entry was found, false otherwise
 */

/**
 * @brief Allocates a block of memory from the pool.
 * 
 * @param p_size The size in bytes to allocate
 * @return An ID handle for the allocated block, or POOL_ALLOCATOR_INVALID_ID on failure
 * 
 * Attempts to find or create a hole of sufficient size. If necessary, compacts
 * memory to make room. The returned ID combines an entry index and a check value
 * for validity verification.
 */

/**
 * @brief Retrieves the entry structure associated with an allocation ID.
 * 
 * @param p_mem The allocation ID returned by alloc()
 * @return Pointer to the entry, or NULL if invalid
 * 
 * Non-const version. Validates the entry through check bits and length verification.
 */

/**
 * @brief Retrieves the entry structure associated with an allocation ID (const).
 * 
 * @param p_mem The allocation ID returned by alloc()
 * @return Pointer to the entry, or NULL if invalid
 * 
 * Const version. Validates the entry through check bits and length verification.
 */

/**
 * @brief Frees a previously allocated memory block.
 * 
 * @param p_mem The allocation ID to free
 * 
 * Removes the entry from the sorted entry indices array and updates free memory count.
 * The operation fails if the entry is locked.
 */

/**
 * @brief Gets the allocated size of a memory block.
 * 
 * @param p_mem The allocation ID
 * @return The size in bytes, or 0 if invalid
 * 
 * Thread-safe operation that returns the requested allocation size (not aligned).
 */

/**
 * @brief Resizes an existing allocation.
 * 
 * @param p_mem The allocation ID to resize
 * @param p_new_size The new size in bytes
 * @return OK on success, or an error code (ERR_INVALID_PARAMETER, ERR_OUT_OF_MEMORY, etc.)
 * 
 * Handles three cases: no change in aligned size, shrinking, and expansion.
 * Expansion may trigger forward and/or backward compaction to make room.
 */

/**
 * @brief Locks a memory allocation to prevent its relocation during compaction.
 * 
 * @param p_mem The allocation ID to lock
 * @return OK on success, or ERR_INVALID_PARAMETER if the ID is invalid
 * 
 * Increments a lock counter. The get() method requires lock count > 0.
 * Only applicable if the allocator was created with needs_locking enabled.
 */

/**
 * @brief Checks if a memory allocation is currently locked.
 * 
 * @param p_mem The allocation ID to check
 * @return true if locked, false otherwise
 * 
 * Only meaningful if the allocator was created with needs_locking enabled.
 */

/**
 * @brief Retrieves a pointer to allocated memory (const version).
 * 
 * @param p_mem The allocation ID
 * @return Pointer to the memory, or NULL on failure
 * 
 * If locking is required, the block must be locked before calling this method.
 * Performs bounds checking and lock validation.
 */

/**
 * @brief Retrieves a pointer to allocated memory (non-const version).
 * 
 * @param p_mem The allocation ID
 * @return Pointer to the memory, or NULL on failure
 * 
 * If locking is required, the block must be locked before calling this method.
 * Performs bounds checking and lock validation.
 */

/**
 * @brief Unlocks a previously locked memory allocation.
 * 
 * @param p_mem The allocation ID to unlock
 * 
 * Decrements the lock counter. Has no effect if locking is not enabled.
 */

/**
 * @brief Gets the total amount of used memory in the pool.
 * 
 * @return Used memory in bytes
 * 
 * Calculated as pool_size - free_mem.
 */

/**
 * @brief Gets the peak (minimum) amount of free memory recorded.
 * 
 * @return Free memory in bytes at peak usage
 */

/**
 * @brief Gets the current amount of free memory in the pool.
 * 
 * @return Free memory in bytes
 */

/**
 * @brief Initializes the pool allocator with given memory and parameters.
 * 
 * @param p_mem Pointer to the memory pool
 * @param p_size Size of the pool in bytes
 * @param p_max_entries Maximum number of simultaneous allocations
 * 
 * Sets up entry array, entry indices array, and initializes tracking variables.
 */

/**
 * @brief Constructor that allocates its own pool memory.
 * 
 * @param p_size Size of the pool to allocate in bytes
 * @param p_needs_locking Enable lock/unlock mechanism
 * @param p_max_entries Maximum number of simultaneous allocations
 * 
 * The allocator allocates its own memory via memalloc() and manages it internally.
 */

/**
 * @brief Constructor for use with externally-provided memory.
 * 
 * @param p_mem Pointer to existing memory pool
 * @param p_size Size of the provided pool in bytes
 * @param p_align Alignment requirement in bytes
 * @param p_needs_locking Enable lock/unlock mechanism
 * @param p_max_entries Maximum number of simultaneous allocations
 * 
 * Adjusts the memory pointer and size if alignment is needed, but does not
 * allocate new memory. User is responsible for the memory's lifetime.
 */

/**
 * @brief Constructor for static allocation with guaranteed alignment.
 * 
 * @param p_align Required alignment in bytes (must be >= 1)
 * @param p_size Size of the pool in bytes
 * @param p_needs_locking Enable lock/unlock mechanism
 * @param p_max_entries Maximum number of simultaneous allocations
 * 
 * Allocates memory via Memory::alloc_static() with extra space for alignment adjustment.
 */

/**
 * @brief Destructor that cleans up allocated resources.
 * 
 * Frees the pool memory if it was allocated by the constructor and deletes
 * the entry array and entry indices array.
 */
#include "pool_allocator.h"

#include "core/error_macros.h"
#include "core/os/copymem.h"
#include "core/os/memory.h"
#include "core/os/os.h"
#include "core/print_string.h"

#include <assert.h>

#define COMPACT_CHUNK(m_entry, m_to_pos)                      \
	do {                                                      \
		void *_dst = &((unsigned char *)pool)[m_to_pos];      \
		void *_src = &((unsigned char *)pool)[(m_entry).pos]; \
		movemem(_dst, _src, aligned((m_entry).len));          \
		(m_entry).pos = m_to_pos;                             \
	} while (0);

void PoolAllocator::mt_lock() const {
}

void PoolAllocator::mt_unlock() const {
}

bool PoolAllocator::get_free_entry(EntryArrayPos *p_pos) {

	if (entry_count == entry_max)
		return false;

	for (int i = 0; i < entry_max; i++) {

		if (entry_array[i].len == 0) {
			*p_pos = i;
			return true;
		}
	}

	ERR_PRINT("Out of memory Chunks!");

	return false; //
}

/**
 * Find a hole
 * @param p_pos The hole is behind the block pointed by this variable upon return. if pos==entry_count, then allocate at end
 * @param p_for_size hole size
 * @return false if hole found, true if no hole found
 */
bool PoolAllocator::find_hole(EntryArrayPos *p_pos, int p_for_size) {

	/* position where previous entry ends. Defaults to zero (begin of pool) */

	int prev_entry_end_pos = 0;

	for (int i = 0; i < entry_count; i++) {

		Entry &entry = entry_array[entry_indices[i]];

		/* determine hole size to previous entry */

		int hole_size = entry.pos - prev_entry_end_pos;

		/* determine if what we want fits in that hole */
		if (hole_size >= p_for_size) {
			*p_pos = i;
			return true;
		}

		/* prepare for next one */
		prev_entry_end_pos = entry_end(entry);
	}

	/* No holes between entries, check at the end..*/

	if ((pool_size - prev_entry_end_pos) >= p_for_size) {
		*p_pos = entry_count;
		return true;
	}

	return false;
}

void PoolAllocator::compact(int p_up_to) {

	uint32_t prev_entry_end_pos = 0;

	if (p_up_to < 0)
		p_up_to = entry_count;
	for (int i = 0; i < p_up_to; i++) {

		Entry &entry = entry_array[entry_indices[i]];

		/* determine hole size to previous entry */

		int hole_size = entry.pos - prev_entry_end_pos;

		/* if we can compact, do it */
		if (hole_size > 0 && !entry.lock) {

			COMPACT_CHUNK(entry, prev_entry_end_pos);
		}

		/* prepare for next one */
		prev_entry_end_pos = entry_end(entry);
	}
}

void PoolAllocator::compact_up(int p_from) {

	uint32_t next_entry_end_pos = pool_size; // - static_area_size;

	for (int i = entry_count - 1; i >= p_from; i--) {

		Entry &entry = entry_array[entry_indices[i]];

		/* determine hole size to nextious entry */

		int hole_size = next_entry_end_pos - (entry.pos + aligned(entry.len));

		/* if we can compact, do it */
		if (hole_size > 0 && !entry.lock) {

			COMPACT_CHUNK(entry, (next_entry_end_pos - aligned(entry.len)));
		}

		/* prepare for next one */
		next_entry_end_pos = entry.pos;
	}
}

bool PoolAllocator::find_entry_index(EntryIndicesPos *p_map_pos, Entry *p_entry) {

	EntryArrayPos entry_pos = entry_max;

	for (int i = 0; i < entry_count; i++) {

		if (&entry_array[entry_indices[i]] == p_entry) {

			entry_pos = i;
			break;
		}
	}

	if (entry_pos == entry_max)
		return false;

	*p_map_pos = entry_pos;
	return true;
}

PoolAllocator::ID PoolAllocator::alloc(int p_size) {

	ERR_FAIL_COND_V(p_size < 1, POOL_ALLOCATOR_INVALID_ID);
#ifdef DEBUG_ENABLED
	if (p_size > free_mem) OS::get_singleton()->debug_break();
#endif
	ERR_FAIL_COND_V(p_size > free_mem, POOL_ALLOCATOR_INVALID_ID);

	mt_lock();

	if (entry_count == entry_max) {
		mt_unlock();
		ERR_PRINT("entry_count==entry_max");
		return POOL_ALLOCATOR_INVALID_ID;
	}

	int size_to_alloc = aligned(p_size);

	EntryIndicesPos new_entry_indices_pos;

	if (!find_hole(&new_entry_indices_pos, size_to_alloc)) {
		/* No hole could be found, try compacting mem */
		compact();
		/* Then search again */

		if (!find_hole(&new_entry_indices_pos, size_to_alloc)) {
			mt_unlock();
			ERR_FAIL_V_MSG(POOL_ALLOCATOR_INVALID_ID, "Memory can't be compacted further.");
		}
	}

	EntryArrayPos new_entry_array_pos;

	bool found_free_entry = get_free_entry(&new_entry_array_pos);

	if (!found_free_entry) {
		mt_unlock();
		ERR_FAIL_V_MSG(POOL_ALLOCATOR_INVALID_ID, "No free entry found in PoolAllocator.");
	}

	/* move all entry indices up, make room for this one */
	for (int i = entry_count; i > new_entry_indices_pos; i--) {

		entry_indices[i] = entry_indices[i - 1];
	}

	entry_indices[new_entry_indices_pos] = new_entry_array_pos;

	entry_count++;

	Entry &entry = entry_array[entry_indices[new_entry_indices_pos]];

	entry.len = p_size;
	entry.pos = (new_entry_indices_pos == 0) ? 0 : entry_end(entry_array[entry_indices[new_entry_indices_pos - 1]]); //alloc either at beginning or end of previous
	entry.lock = 0;
	entry.check = (check_count++) & CHECK_MASK;
	free_mem -= size_to_alloc;
	if (free_mem < free_mem_peak)
		free_mem_peak = free_mem;

	ID retval = (entry_indices[new_entry_indices_pos] << CHECK_BITS) | entry.check;
	mt_unlock();

	//ERR_FAIL_COND_V( (uintptr_t)get(retval)%align != 0, retval );

	return retval;
}

PoolAllocator::Entry *PoolAllocator::get_entry(ID p_mem) {

	unsigned int check = p_mem & CHECK_MASK;
	int entry = p_mem >> CHECK_BITS;
	ERR_FAIL_INDEX_V(entry, entry_max, NULL);
	ERR_FAIL_COND_V(entry_array[entry].check != check, NULL);
	ERR_FAIL_COND_V(entry_array[entry].len == 0, NULL);

	return &entry_array[entry];
}

const PoolAllocator::Entry *PoolAllocator::get_entry(ID p_mem) const {

	unsigned int check = p_mem & CHECK_MASK;
	int entry = p_mem >> CHECK_BITS;
	ERR_FAIL_INDEX_V(entry, entry_max, NULL);
	ERR_FAIL_COND_V(entry_array[entry].check != check, NULL);
	ERR_FAIL_COND_V(entry_array[entry].len == 0, NULL);

	return &entry_array[entry];
}

void PoolAllocator::free(ID p_mem) {

	mt_lock();
	Entry *e = get_entry(p_mem);
	if (!e) {
		mt_unlock();
		ERR_PRINT("!e");
		return;
	}
	if (e->lock) {
		mt_unlock();
		ERR_PRINT("e->lock");
		return;
	}

	EntryIndicesPos entry_indices_pos;

	bool index_found = find_entry_index(&entry_indices_pos, e);
	if (!index_found) {

		mt_unlock();
		ERR_FAIL_COND(!index_found);
	}

	for (int i = entry_indices_pos; i < (entry_count - 1); i++) {

		entry_indices[i] = entry_indices[i + 1];
	}

	entry_count--;
	free_mem += aligned(e->len);
	e->clear();
	mt_unlock();
}

int PoolAllocator::get_size(ID p_mem) const {

	int size;
	mt_lock();

	const Entry *e = get_entry(p_mem);
	if (!e) {

		mt_unlock();
		ERR_PRINT("!e");
		return 0;
	}

	size = e->len;

	mt_unlock();

	return size;
}

Error PoolAllocator::resize(ID p_mem, int p_new_size) {

	mt_lock();
	Entry *e = get_entry(p_mem);

	if (!e) {
		mt_unlock();
		ERR_FAIL_COND_V(!e, ERR_INVALID_PARAMETER);
	}

	if (needs_locking && e->lock) {
		mt_unlock();
		ERR_FAIL_COND_V(e->lock, ERR_ALREADY_IN_USE);
	}

	uint32_t alloc_size = aligned(p_new_size);

	if ((uint32_t)aligned(e->len) == alloc_size) {

		e->len = p_new_size;
		mt_unlock();
		return OK;
	} else if (e->len > (uint32_t)p_new_size) {

		free_mem += aligned(e->len);
		free_mem -= alloc_size;
		e->len = p_new_size;
		mt_unlock();
		return OK;
	}

	//p_new_size = align(p_new_size)
	int _free = free_mem; // - static_area_size;

	if (uint32_t(_free + aligned(e->len)) < alloc_size) {
		mt_unlock();
		ERR_FAIL_V(ERR_OUT_OF_MEMORY);
	};

	EntryIndicesPos entry_indices_pos;

	bool index_found = find_entry_index(&entry_indices_pos, e);

	if (!index_found) {

		mt_unlock();
		ERR_FAIL_COND_V(!index_found, ERR_BUG);
	}

	//no need to move stuff around, it fits before the next block
	uint32_t next_pos;
	if (entry_indices_pos + 1 == entry_count) {
		next_pos = pool_size; // - static_area_size;
	} else {
		next_pos = entry_array[entry_indices[entry_indices_pos + 1]].pos;
	};

	if ((next_pos - e->pos) > alloc_size) {
		free_mem += aligned(e->len);
		e->len = p_new_size;
		free_mem -= alloc_size;
		mt_unlock();
		return OK;
	}
	//it doesn't fit, compact around BEFORE current index (make room behind)

	compact(entry_indices_pos + 1);

	if ((next_pos - e->pos) > alloc_size) {
		//now fits! hooray!
		free_mem += aligned(e->len);
		e->len = p_new_size;
		free_mem -= alloc_size;
		mt_unlock();
		if (free_mem < free_mem_peak)
			free_mem_peak = free_mem;
		return OK;
	}

	//STILL doesn't fit, compact around AFTER current index (make room after)

	compact_up(entry_indices_pos + 1);

	if ((entry_array[entry_indices[entry_indices_pos + 1]].pos - e->pos) > alloc_size) {
		//now fits! hooray!
		free_mem += aligned(e->len);
		e->len = p_new_size;
		free_mem -= alloc_size;
		mt_unlock();
		if (free_mem < free_mem_peak)
			free_mem_peak = free_mem;
		return OK;
	}

	mt_unlock();
	ERR_FAIL_V(ERR_OUT_OF_MEMORY);
}

Error PoolAllocator::lock(ID p_mem) {

	if (!needs_locking)
		return OK;
	mt_lock();
	Entry *e = get_entry(p_mem);
	if (!e) {

		mt_unlock();
		ERR_PRINT("!e");
		return ERR_INVALID_PARAMETER;
	}
	e->lock++;
	mt_unlock();
	return OK;
}

bool PoolAllocator::is_locked(ID p_mem) const {

	if (!needs_locking)
		return false;

	mt_lock();
	const Entry *e = ((PoolAllocator *)(this))->get_entry(p_mem);
	if (!e) {

		mt_unlock();
		ERR_PRINT("!e");
		return false;
	}
	bool locked = e->lock;
	mt_unlock();
	return locked;
}

const void *PoolAllocator::get(ID p_mem) const {

	if (!needs_locking) {

		const Entry *e = get_entry(p_mem);
		ERR_FAIL_COND_V(!e, NULL);
		return &pool[e->pos];
	}

	mt_lock();
	const Entry *e = get_entry(p_mem);

	if (!e) {

		mt_unlock();
		ERR_FAIL_COND_V(!e, NULL);
	}
	if (e->lock == 0) {

		mt_unlock();
		ERR_PRINT("e->lock == 0");
		return NULL;
	}

	if ((int)e->pos >= pool_size) {

		mt_unlock();
		ERR_PRINT("e->pos<0 || e->pos>=pool_size");
		return NULL;
	}
	const void *ptr = &pool[e->pos];

	mt_unlock();

	return ptr;
}

void *PoolAllocator::get(ID p_mem) {

	if (!needs_locking) {

		Entry *e = get_entry(p_mem);
		ERR_FAIL_COND_V(!e, NULL);
		return &pool[e->pos];
	}

	mt_lock();
	Entry *e = get_entry(p_mem);

	if (!e) {

		mt_unlock();
		ERR_FAIL_COND_V(!e, NULL);
	}
	if (e->lock == 0) {

		//assert(0);
		mt_unlock();
		ERR_PRINT("e->lock == 0");
		return NULL;
	}

	if ((int)e->pos >= pool_size) {

		mt_unlock();
		ERR_PRINT("e->pos<0 || e->pos>=pool_size");
		return NULL;
	}
	void *ptr = &pool[e->pos];

	mt_unlock();

	return ptr;
}
void PoolAllocator::unlock(ID p_mem) {

	if (!needs_locking)
		return;
	mt_lock();
	Entry *e = get_entry(p_mem);
	if (!e) {
		mt_unlock();
		ERR_FAIL_COND(!e);
	}
	if (e->lock == 0) {
		mt_unlock();
		ERR_PRINT("e->lock == 0");
		return;
	}
	e->lock--;
	mt_unlock();
}

int PoolAllocator::get_used_mem() const {

	return pool_size - free_mem;
}

int PoolAllocator::get_free_peak() {

	return free_mem_peak;
}

int PoolAllocator::get_free_mem() {

	return free_mem;
}

void PoolAllocator::create_pool(void *p_mem, int p_size, int p_max_entries) {

	pool = (uint8_t *)p_mem;
	pool_size = p_size;

	entry_array = memnew_arr(Entry, p_max_entries);
	entry_indices = memnew_arr(int, p_max_entries);
	entry_max = p_max_entries;
	entry_count = 0;

	free_mem = p_size;
	free_mem_peak = p_size;

	check_count = 0;
}

PoolAllocator::PoolAllocator(int p_size, bool p_needs_locking, int p_max_entries) {

	mem_ptr = memalloc(p_size);
	ERR_FAIL_COND(!mem_ptr);
	align = 1;
	create_pool(mem_ptr, p_size, p_max_entries);
	needs_locking = p_needs_locking;
}

PoolAllocator::PoolAllocator(void *p_mem, int p_size, int p_align, bool p_needs_locking, int p_max_entries) {

	if (p_align > 1) {

		uint8_t *mem8 = (uint8_t *)p_mem;
		uint64_t ofs = (uint64_t)mem8;
		if (ofs % p_align) {
			int dif = p_align - (ofs % p_align);
			mem8 += p_align - (ofs % p_align);
			p_size -= dif;
			p_mem = (void *)mem8;
		};
	};

	create_pool(p_mem, p_size, p_max_entries);
	needs_locking = p_needs_locking;
	align = p_align;
	mem_ptr = NULL;
}

PoolAllocator::PoolAllocator(int p_align, int p_size, bool p_needs_locking, int p_max_entries) {

	ERR_FAIL_COND(p_align < 1);
	mem_ptr = Memory::alloc_static(p_size + p_align, true);
	uint8_t *mem8 = (uint8_t *)mem_ptr;
	uint64_t ofs = (uint64_t)mem8;
	if (ofs % p_align)
		mem8 += p_align - (ofs % p_align);
	create_pool(mem8, p_size, p_max_entries);
	needs_locking = p_needs_locking;
	align = p_align;
}

PoolAllocator::~PoolAllocator() {

	if (mem_ptr)
		memfree(mem_ptr);

	memdelete_arr(entry_array);
	memdelete_arr(entry_indices);
}

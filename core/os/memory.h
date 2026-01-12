

/**
 * @file memory.h
 * @brief Memory management utilities for the game engine.
 * 
 * This header provides a comprehensive memory management system including:
 * - Static memory allocation/deallocation with optional alignment padding
 * - Memory usage tracking and statistics (debug builds)
 * - Custom new/delete operators for memory pooling and descriptions
 * - Template-based memory management for single objects and arrays
 * - Pre/post-initialization hooks for custom object lifecycle handling
 * 
 * @note PAD_ALIGN must be set to a value greater than 16 for alignment operations.
 */

/**
 * @class Memory
 * @brief Static memory management interface for allocation and deallocation.
 * 
 * Provides static methods for allocating, reallocating, and freeing memory with
 * optional padding alignment. Tracks memory usage statistics in debug builds.
 * 
 * @var mem_usage Current memory usage (debug builds only)
 * @var max_usage Maximum recorded memory usage (debug builds only)
 * @var alloc_count Total number of active allocations
 */

/**
 * @class DefaultAllocator
 * @brief Default memory allocator wrapper for the Memory system.
 * 
 * Provides inline allocation and deallocation methods that delegate to
 * Memory::alloc_static() and Memory::free_static() without alignment padding.
 */

/**
 * @brief Creates a new object with optional memory description.
 * @param p_size Size in bytes to allocate
 * @param p_description Human-readable description for memory tracking
 * @return Pointer to allocated memory
 */

/**
 * @brief Creates a new object using a custom allocator function.
 * @param p_size Size in bytes to allocate
 * @param p_allocfunc Custom allocation function
 * @return Pointer to allocated memory
 */

/**
 * @brief Placement new operator for pre-allocated memory.
 * @param p_size Size of the object
 * @param p_pointer Pointer to pre-allocated memory
 * @param check Size verification value
 * @param p_description Memory description for tracking
 * @return The provided p_pointer if valid
 */

/**
 * @def memnew(m_class)
 * @brief Allocates and constructs a single object.
 * @param m_class Class type to instantiate
 * @return Pointer to newly constructed object
 */

/**
 * @def memnew_allocator(m_class, m_allocator)
 * @brief Allocates and constructs an object using a custom allocator.
 * @param m_class Class type to instantiate
 * @param m_allocator Allocator class with alloc() method
 * @return Pointer to newly constructed object
 */

/**
 * @def memnew_placement(m_placement, m_class)
 * @brief Constructs an object in pre-allocated memory.
 * @param m_placement Pointer to pre-allocated memory
 * @param m_class Class type to construct
 * @return Pointer to constructed object
 */

/**
 * @def memdelete_notnull(m_v)
 * @brief Safely deletes an object only if pointer is non-null.
 * @param m_v Pointer to object to delete
 */

/**
 * @def memnew_arr(m_class, m_count)
 * @brief Allocates and constructs an array of objects.
 * @param m_class Element type
 * @param m_count Number of elements
 * @return Pointer to array of constructed objects
 * 
 * @note Array length can be retrieved with memarr_len()
 */

/**
 * @brief Retrieves the length of an array allocated with memnew_arr().
 * @tparam T Element type of the array
 * @param p_class Pointer to array
 * @return Number of elements in the array
 */

/**
 * @brief Destructs and deallocates an array allocated with memnew_arr().
 * @tparam T Element type of the array
 * @param p_class Pointer to array to delete
 * 
 * @note Calls destructors for non-trivial types
 */
#ifndef MEMORY_H
#define MEMORY_H

#include "core/error_macros.h"
#include "core/safe_refcount.h"

#include <stddef.h>

#ifndef PAD_ALIGN
#define PAD_ALIGN 16 //must always be greater than this at much
#endif

class Memory {

	Memory();
#ifdef DEBUG_ENABLED
	static uint64_t mem_usage;
	static uint64_t max_usage;
#endif

	static uint64_t alloc_count;

public:
	static void *alloc_static(size_t p_bytes, bool p_pad_align = false);
	static void *realloc_static(void *p_memory, size_t p_bytes, bool p_pad_align = false);
	static void free_static(void *p_ptr, bool p_pad_align = false);

	static uint64_t get_mem_available();
	static uint64_t get_mem_usage();
	static uint64_t get_mem_max_usage();
};

class DefaultAllocator {
public:
	_FORCE_INLINE_ static void *alloc(size_t p_memory) { return Memory::alloc_static(p_memory, false); }
	_FORCE_INLINE_ static void free(void *p_ptr) { Memory::free_static(p_ptr, false); }
};

void *operator new(size_t p_size, const char *p_description); ///< operator new that takes a description and uses MemoryStaticPool
void *operator new(size_t p_size, void *(*p_allocfunc)(size_t p_size)); ///< operator new that takes a description and uses MemoryStaticPool

void *operator new(size_t p_size, void *p_pointer, size_t check, const char *p_description); ///< operator new that takes a description and uses a pointer to the preallocated memory

#ifdef _MSC_VER
// When compiling with VC++ 2017, the above declarations of placement new generate many irrelevant warnings (C4291).
// The purpose of the following definitions is to muffle these warnings, not to provide a usable implementation of placement delete.
void operator delete(void *p_mem, const char *p_description);
void operator delete(void *p_mem, void *(*p_allocfunc)(size_t p_size));
void operator delete(void *p_mem, void *p_pointer, size_t check, const char *p_description);
#endif

#define memalloc(m_size) Memory::alloc_static(m_size)
#define memrealloc(m_mem, m_size) Memory::realloc_static(m_mem, m_size)
#define memfree(m_size) Memory::free_static(m_size)

_ALWAYS_INLINE_ void postinitialize_handler(void *) {}

template <class T>
_ALWAYS_INLINE_ T *_post_initialize(T *p_obj) {

	postinitialize_handler(p_obj);
	return p_obj;
}

#define memnew(m_class) _post_initialize(new ("") m_class)

_ALWAYS_INLINE_ void *operator new(size_t p_size, void *p_pointer, size_t check, const char *p_description) {
	//void *failptr=0;
	//ERR_FAIL_COND_V( check < p_size , failptr); /** bug, or strange compiler, most likely */

	return p_pointer;
}

#define memnew_allocator(m_class, m_allocator) _post_initialize(new (m_allocator::alloc) m_class)
#define memnew_placement(m_placement, m_class) _post_initialize(new (m_placement, sizeof(m_class), "") m_class)

_ALWAYS_INLINE_ bool predelete_handler(void *) {
	return true;
}

template <class T>
void memdelete(T *p_class) {

	if (!predelete_handler(p_class))
		return; // doesn't want to be deleted
	if (!__has_trivial_destructor(T))
		p_class->~T();

	Memory::free_static(p_class, false);
}

template <class T, class A>
void memdelete_allocator(T *p_class) {

	if (!predelete_handler(p_class))
		return; // doesn't want to be deleted
	if (!__has_trivial_destructor(T))
		p_class->~T();

	A::free(p_class);
}

#define memdelete_notnull(m_v)   \
	{                            \
		if (m_v) memdelete(m_v); \
	}

#define memnew_arr(m_class, m_count) memnew_arr_template<m_class>(m_count)

template <typename T>
T *memnew_arr_template(size_t p_elements, const char *p_descr = "") {

	if (p_elements == 0)
		return 0;
	/** overloading operator new[] cannot be done , because it may not return the real allocated address (it may pad the 'element count' before the actual array). Because of that, it must be done by hand. This is the
	same strategy used by std::vector, and the Vector class, so it should be safe.*/

	size_t len = sizeof(T) * p_elements;
	uint64_t *mem = (uint64_t *)Memory::alloc_static(len, true);
	T *failptr = 0; //get rid of a warning
	ERR_FAIL_COND_V(!mem, failptr);
	*(mem - 1) = p_elements;

	if (!__has_trivial_constructor(T)) {
		T *elems = (T *)mem;

		/* call operator new */
		for (size_t i = 0; i < p_elements; i++) {
			new (&elems[i], sizeof(T), p_descr) T;
		}
	}

	return (T *)mem;
}

/**
 * Wonders of having own array functions, you can actually check the length of
 * an allocated-with memnew_arr() array
 */

template <typename T>
size_t memarr_len(const T *p_class) {

	uint64_t *ptr = (uint64_t *)p_class;
	return *(ptr - 1);
}

template <typename T>
void memdelete_arr(T *p_class) {

	uint64_t *ptr = (uint64_t *)p_class;

	if (!__has_trivial_destructor(T)) {
		uint64_t elem_count = *(ptr - 1);

		for (uint64_t i = 0; i < elem_count; i++) {
			p_class[i].~T();
		}
	}

	Memory::free_static(ptr, true);
}

struct _GlobalNil {

	int color;
	_GlobalNil *right;
	_GlobalNil *left;
	_GlobalNil *parent;

	_GlobalNil();
};

struct _GlobalNilClass {

	static _GlobalNil _nil;
};

#endif // MEMORY_H

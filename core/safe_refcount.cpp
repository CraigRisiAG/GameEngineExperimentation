

/**
 * @file safe_refcount.cpp
 * @brief MSVC-Windows implementation of atomic reference counting operations.
 * 
 * This file provides inline implementations of atomic operations for safe reference counting
 * on Windows platforms using MSVC compiler. It supports both 32-bit and 64-bit atomic operations
 * using Windows Interlocked API functions.
 * 
 * @details
 * The implementation uses Windows Interlocked functions for thread-safe atomic operations:
 * - InterlockedCompareExchange/InterlockedCompareExchange64 for conditional increments
 * - InterlockedIncrement/InterlockedIncrement64 for atomic increments
 * - InterlockedDecrement/InterlockedDecrement64 for atomic decrements
 * - InterlockedAdd/InterlockedAdd64 for atomic additions
 * - InterlockedExchangeAdd/InterlockedExchangeAdd64 for atomic subtractions
 * 
 * Two sets of functions are provided: one for uint32_t and one for uint64_t operations.
 * All functions are marked as inline for performance-critical reference counting operations.
 * 
 * @note This implementation is specific to Microsoft Visual C++ on Windows platforms.
 *       Compiler guard: #if defined(_MSC_VER)
 */

/**
 * @brief Atomically increments a reference counter only if it is non-zero.
 * @param pw Pointer to the volatile counter to increment.
 * @return The new value after increment, or 0 if the counter was already 0.
 * @details Uses compare-and-swap in a loop to ensure atomic operation.
 *          Returns 0 if the counter cannot be incremented (already at 0).
 */

/**
 * @brief Atomically decrements a reference counter.
 * @param pw Pointer to the volatile counter to decrement.
 * @return The new value after decrement.
 */

/**
 * @brief Atomically increments a reference counter.
 * @param pw Pointer to the volatile counter to increment.
 * @return The new value after increment.
 */

/**
 * @brief Atomically adds a value to a counter.
 * @param pw Pointer to the volatile counter.
 * @param val The value to add.
 * @return The new value after addition.
 */

/**
 * @brief Atomically subtracts a value from a counter.
 * @param pw Pointer to the volatile counter.
 * @param val The value to subtract.
 * @return The new value after subtraction.
 */

/**
 * @brief Atomically updates a counter to a greater value using compare-and-swap.
 * @param pw Pointer to the volatile counter.
 * @param val The value to compare against and potentially set.
 * @return The greater of the current value or the provided value.
 * @details If the current value is already greater than or equal to val, returns current value.
 *          Otherwise, atomically sets the counter to val and returns val.
 */
#include "safe_refcount.h"

#if defined(_MSC_VER)

/* Implementation for MSVC-Windows */

// don't pollute my namespace!
#include <windows.h>

#define ATOMIC_CONDITIONAL_INCREMENT_BODY(m_pw, m_win_type, m_win_cmpxchg, m_cpp_type) \
	/* try to increment until it actually works */                                     \
	/* taken from boost */                                                             \
	while (true) {                                                                     \
		m_cpp_type tmp = static_cast<m_cpp_type const volatile &>(*(m_pw));            \
		if (tmp == 0)                                                                  \
			return 0; /* if zero, can't add to it anymore */                           \
		if (m_win_cmpxchg((m_win_type volatile *)(m_pw), tmp + 1, tmp) == tmp)         \
			return tmp + 1;                                                            \
	}

#define ATOMIC_EXCHANGE_IF_GREATER_BODY(m_pw, m_val, m_win_type, m_win_cmpxchg, m_cpp_type) \
	while (true) {                                                                          \
		m_cpp_type tmp = static_cast<m_cpp_type const volatile &>(*(m_pw));                 \
		if (tmp >= m_val)                                                                   \
			return tmp; /* already greater, or equal */                                     \
		if (m_win_cmpxchg((m_win_type volatile *)(m_pw), m_val, tmp) == tmp)                \
			return m_val;                                                                   \
	}

_ALWAYS_INLINE_ uint32_t _atomic_conditional_increment_impl(volatile uint32_t *pw){

	ATOMIC_CONDITIONAL_INCREMENT_BODY(pw, LONG, InterlockedCompareExchange, uint32_t)
}

_ALWAYS_INLINE_ uint32_t _atomic_decrement_impl(volatile uint32_t *pw) {

	return InterlockedDecrement((LONG volatile *)pw);
}

_ALWAYS_INLINE_ uint32_t _atomic_increment_impl(volatile uint32_t *pw) {

	return InterlockedIncrement((LONG volatile *)pw);
}

_ALWAYS_INLINE_ uint32_t _atomic_sub_impl(volatile uint32_t *pw, volatile uint32_t val) {

	return InterlockedExchangeAdd((LONG volatile *)pw, -(int32_t)val) - val;
}

_ALWAYS_INLINE_ uint32_t _atomic_add_impl(volatile uint32_t *pw, volatile uint32_t val) {

	return InterlockedAdd((LONG volatile *)pw, val);
}

_ALWAYS_INLINE_ uint32_t _atomic_exchange_if_greater_impl(volatile uint32_t *pw, volatile uint32_t val){

	ATOMIC_EXCHANGE_IF_GREATER_BODY(pw, val, LONG, InterlockedCompareExchange, uint32_t)
}

_ALWAYS_INLINE_ uint64_t _atomic_conditional_increment_impl(volatile uint64_t *pw){

	ATOMIC_CONDITIONAL_INCREMENT_BODY(pw, LONGLONG, InterlockedCompareExchange64, uint64_t)
}

_ALWAYS_INLINE_ uint64_t _atomic_decrement_impl(volatile uint64_t *pw) {

	return InterlockedDecrement64((LONGLONG volatile *)pw);
}

_ALWAYS_INLINE_ uint64_t _atomic_increment_impl(volatile uint64_t *pw) {

	return InterlockedIncrement64((LONGLONG volatile *)pw);
}

_ALWAYS_INLINE_ uint64_t _atomic_sub_impl(volatile uint64_t *pw, volatile uint64_t val) {

	return InterlockedExchangeAdd64((LONGLONG volatile *)pw, -(int64_t)val) - val;
}

_ALWAYS_INLINE_ uint64_t _atomic_add_impl(volatile uint64_t *pw, volatile uint64_t val) {

	return InterlockedAdd64((LONGLONG volatile *)pw, val);
}

_ALWAYS_INLINE_ uint64_t _atomic_exchange_if_greater_impl(volatile uint64_t *pw, volatile uint64_t val){

	ATOMIC_EXCHANGE_IF_GREATER_BODY(pw, val, LONGLONG, InterlockedCompareExchange64, uint64_t)
}

// The actual advertised functions; they'll call the right implementation

uint32_t atomic_conditional_increment(volatile uint32_t *pw) {
	return _atomic_conditional_increment_impl(pw);
}

uint32_t atomic_decrement(volatile uint32_t *pw) {
	return _atomic_decrement_impl(pw);
}

uint32_t atomic_increment(volatile uint32_t *pw) {
	return _atomic_increment_impl(pw);
}

uint32_t atomic_sub(volatile uint32_t *pw, volatile uint32_t val) {
	return _atomic_sub_impl(pw, val);
}

uint32_t atomic_add(volatile uint32_t *pw, volatile uint32_t val) {
	return _atomic_add_impl(pw, val);
}

uint32_t atomic_exchange_if_greater(volatile uint32_t *pw, volatile uint32_t val) {
	return _atomic_exchange_if_greater_impl(pw, val);
}

uint64_t atomic_conditional_increment(volatile uint64_t *pw) {
	return _atomic_conditional_increment_impl(pw);
}

uint64_t atomic_decrement(volatile uint64_t *pw) {
	return _atomic_decrement_impl(pw);
}

uint64_t atomic_increment(volatile uint64_t *pw) {
	return _atomic_increment_impl(pw);
}

uint64_t atomic_sub(volatile uint64_t *pw, volatile uint64_t val) {
	return _atomic_sub_impl(pw, val);
}

uint64_t atomic_add(volatile uint64_t *pw, volatile uint64_t val) {
	return _atomic_add_impl(pw, val);
}

uint64_t atomic_exchange_if_greater(volatile uint64_t *pw, volatile uint64_t val) {
	return _atomic_exchange_if_greater_impl(pw, val);
}
#endif

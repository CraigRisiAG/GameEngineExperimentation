


/**
 * @file simple_type.h
 * @brief Utility template specializations for extracting the underlying simple type.
 * 
 * This header provides template metaprogramming utilities to strip qualifiers
 * (references and const) from types and obtain the base simple type.
 * 
 * @details
 * GetSimpleTypeT is a traits class that uses template specialization to handle:
 * - Plain types (e.g., int -> int)
 * - Reference types (e.g., int& -> int)
 * - Const-qualified types (e.g., const int -> int)
 * 
 * @note
 * This utility is useful for type extraction in generic programming and template
 * metaprogramming where you need to work with the underlying type regardless of
 * how it's qualified.
 * 
 * @example
 * GetSimpleTypeT<int>::type_t result;        // type_t = int
 * GetSimpleTypeT<int&>::type_t result;       // type_t = int
 * GetSimpleTypeT<const int>::type_t result;  // type_t = int
 */
#ifndef SIMPLE_TYPE_H
#define SIMPLE_TYPE_H

/* Batch of specializations to obtain the actual simple type */

template <class T>
struct GetSimpleTypeT {

	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T &> {

	typedef T type_t;
};

template <class T>
struct GetSimpleTypeT<T const> {

	typedef T type_t;
};

#endif // SIMPLE_TYPE_H



/**
 * @file pair.h
 * @brief A generic pair template container and related utilities.
 * 
 * This header defines a simple Pair template struct that holds two values of different types,
 * along with comparison operators and a sorting functor.
 */

/**
 * @template F The type of the first element
 * @template S The type of the second element
 * @struct Pair
 * @brief A generic container for storing two values of potentially different types.
 * 
 * The Pair struct provides a simple way to group two related values together.
 * It includes default and parameterized constructors for convenient initialization.
 */

/**
 * @template F The type of the first element
 * @template S The type of the second element
 * @brief Equality operator for Pair objects.
 * 
 * Compares two Pair objects by checking if both their first and second elements are equal.
 * 
 * @param pair The first Pair to compare
 * @param other The second Pair to compare
 * @return true if both pairs have equal first and second elements, false otherwise
 */

/**
 * @template F The type of the first element
 * @template S The type of the second element
 * @brief Inequality operator for Pair objects.
 * 
 * Compares two Pair objects by checking if either their first or second elements differ.
 * 
 * @param pair The first Pair to compare
 * @param other The second Pair to compare
 * @return true if either the first or second elements differ, false otherwise
 */

/**
 * @template F The type of the first element (must support the < operator)
 * @template S The type of the second element
 * @struct PairSort
 * @brief A comparator functor for sorting Pair objects by their first element.
 * 
 * This functor can be used with standard library sorting algorithms to sort
 * collections of Pair objects in ascending order based on the first element.
 */
#ifndef PAIR_H
#define PAIR_H

template <class F, class S>
struct Pair {

	F first;
	S second;

	Pair() :
			first(),
			second() {
	}

	Pair(F p_first, const S &p_second) :
			first(p_first),
			second(p_second) {
	}
};

template <class F, class S>
bool operator==(const Pair<F, S> &pair, const Pair<F, S> &other) {
	return (pair.first == other.first) && (pair.second == other.second);
}

template <class F, class S>
bool operator!=(const Pair<F, S> &pair, const Pair<F, S> &other) {
	return (pair.first != other.first) || (pair.second != other.second);
}

template <class F, class S>
struct PairSort {

	bool operator()(const Pair<F, S> &A, const Pair<F, S> &B) const {
		return A.first < B.first;
	}
};

#endif // PAIR_H

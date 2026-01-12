

/**
 * OrderedHashMap - A hash map that maintains insertion order.
 * 
 * This template class provides a hash map implementation that allows iteration
 * of elements in the order they were inserted. It combines the performance
 * characteristics of a hash map with the ordering guarantees of a linked list.
 * 
 * Template Parameters:
 *   K - The key type
 *   V - The value type
 *   Hasher - Hash function for keys (default: HashMapHasherDefault)
 *   Comparator - Key comparison function (default: HashMapComparatorDefault<K>)
 *   MIN_HASH_TABLE_POWER - Minimum power of 2 for hash table size (default: 3)
 *   RELATIONSHIP - Hash table load factor ratio (default: 8)
 * 
 * Complexity:
 *   - Insert: O(1) average case
 *   - Lookup: O(1) average case
 *   - Deletion: O(1) average case
 *   - Iteration: O(n) where n is the number of elements
 * 
 * Features:
 *   - Maintains insertion order for deterministic iteration
 *   - Safe deletion during iteration (preserves order)
 *   - API consistent with Map for coherence with codebase
 *   - Supports const and mutable element access
 *   - Copy construction and assignment supported
 * 
 * Nested Classes:
 *   - Element: Mutable bidirectional iterator with key/value access
 *   - ConstElement: Const bidirectional iterator with read-only access
 */
#ifndef ORDERED_HASH_MAP_H
#define ORDERED_HASH_MAP_H

#include "core/hash_map.h"
#include "core/list.h"
#include "core/pair.h"

/**
 * A hash map which allows to iterate elements in insertion order.
 * Insertion, lookup, deletion have O(1) complexity.
 * The API aims to be consistent with Map rather than HashMap, because the
 * former is more frequently used and is more coherent with the rest of the
 * codebase.
 * Deletion during iteration is safe and will preserve the order.
 */
template <class K, class V, class Hasher = HashMapHasherDefault, class Comparator = HashMapComparatorDefault<K>, uint8_t MIN_HASH_TABLE_POWER = 3, uint8_t RELATIONSHIP = 8>
class OrderedHashMap {
	typedef List<Pair<const K *, V>> InternalList;
	typedef HashMap<K, typename InternalList::Element *, Hasher, Comparator, MIN_HASH_TABLE_POWER, RELATIONSHIP> InternalMap;

	InternalList list;
	InternalMap map;

public:
	class Element {
		friend class OrderedHashMap<K, V, Hasher, Comparator, MIN_HASH_TABLE_POWER, RELATIONSHIP>;

		typename InternalList::Element *list_element;
		typename InternalList::Element *prev_element;
		typename InternalList::Element *next_element;

		Element(typename InternalList::Element *p_element) {
			list_element = p_element;

			if (list_element) {
				next_element = list_element->next();
				prev_element = list_element->prev();
			}
		}

	public:
		_FORCE_INLINE_ Element() :
				list_element(NULL),
				prev_element(NULL),
				next_element(NULL) {
		}

		Element next() const {
			return Element(next_element);
		}

		Element prev() const {
			return Element(prev_element);
		}

		Element(const Element &other) :
				list_element(other.list_element),
				prev_element(other.prev_element),
				next_element(other.next_element) {
		}

		Element &operator=(const Element &other) {
			list_element = other.list_element;
			next_element = other.next_element;
			prev_element = other.prev_element;
			return *this;
		}

		_FORCE_INLINE_ bool operator==(const Element &p_other) const {
			return this->list_element == p_other.list_element;
		}
		_FORCE_INLINE_ bool operator!=(const Element &p_other) const {
			return this->list_element != p_other.list_element;
		}

		operator bool() const {
			return (list_element != NULL);
		}

		const K &key() const {
			CRASH_COND(!list_element);
			return *(list_element->get().first);
		};

		V &value() {
			CRASH_COND(!list_element);
			return list_element->get().second;
		};

		const V &value() const {
			CRASH_COND(!list_element);
			return list_element->get().second;
		};

		V &get() {
			CRASH_COND(!list_element);
			return list_element->get().second;
		};

		const V &get() const {
			CRASH_COND(!list_element);
			return list_element->get().second;
		};
	};

	class ConstElement {
		friend class OrderedHashMap<K, V, Hasher, Comparator, MIN_HASH_TABLE_POWER, RELATIONSHIP>;

		const typename InternalList::Element *list_element;

		ConstElement(const typename InternalList::Element *p_element) :
				list_element(p_element) {
		}

	public:
		_FORCE_INLINE_ ConstElement() :
				list_element(NULL) {
		}

		ConstElement(const ConstElement &other) :
				list_element(other.list_element) {
		}

		ConstElement &operator=(const ConstElement &other) {
			list_element = other.list_element;
			return *this;
		}

		ConstElement next() const {
			return ConstElement(list_element ? list_element->next() : NULL);
		}

		ConstElement prev() const {
			return ConstElement(list_element ? list_element->prev() : NULL);
		}

		_FORCE_INLINE_ bool operator==(const ConstElement &p_other) const {
			return this->list_element == p_other.list_element;
		}
		_FORCE_INLINE_ bool operator!=(const ConstElement &p_other) const {
			return this->list_element != p_other.list_element;
		}

		operator bool() const {
			return (list_element != NULL);
		}

		const K &key() const {
			CRASH_COND(!list_element);
			return *(list_element->get().first);
		};

		const V &value() const {
			CRASH_COND(!list_element);
			return list_element->get().second;
		};

		const V &get() const {
			CRASH_COND(!list_element);
			return list_element->get().second;
		};
	};

	ConstElement find(const K &p_key) const {
		typename InternalList::Element *const *list_element = map.getptr(p_key);
		if (list_element) {
			return ConstElement(*list_element);
		}
		return ConstElement(NULL);
	}

	Element find(const K &p_key) {
		typename InternalList::Element **list_element = map.getptr(p_key);
		if (list_element) {
			return Element(*list_element);
		}
		return Element(NULL);
	}

	Element insert(const K &p_key, const V &p_value) {
		typename InternalList::Element **list_element = map.getptr(p_key);
		if (list_element) {
			(*list_element)->get().second = p_value;
			return Element(*list_element);
		}
		typename InternalList::Element *new_element = list.push_back(Pair<const K *, V>(NULL, p_value));
		typename InternalMap::Element *e = map.set(p_key, new_element);
		new_element->get().first = &e->key();

		return Element(new_element);
	}

	void erase(Element &p_element) {
		map.erase(p_element.key());
		list.erase(p_element.list_element);
		p_element.list_element = NULL;
	}

	bool erase(const K &p_key) {
		typename InternalList::Element **list_element = map.getptr(p_key);
		if (list_element) {
			list.erase(*list_element);
			map.erase(p_key);
			return true;
		}
		return false;
	}

	inline bool has(const K &p_key) const {
		return map.has(p_key);
	}

	const V &operator[](const K &p_key) const {
		ConstElement e = find(p_key);
		CRASH_COND(!e);
		return e.value();
	}

	V &operator[](const K &p_key) {
		Element e = find(p_key);
		if (!e) {
			// consistent with Map behaviour
			e = insert(p_key, V());
		}
		return e.value();
	}

	inline Element front() {
		return Element(list.front());
	}

	inline Element back() {
		return Element(list.back());
	}

	inline ConstElement front() const {
		return ConstElement(list.front());
	}

	inline ConstElement back() const {
		return ConstElement(list.back());
	}

	inline bool empty() const { return list.empty(); }
	inline int size() const { return list.size(); }

	const void *id() const {
		return list.id();
	}

	void clear() {
		map.clear();
		list.clear();
	}

private:
	void _copy_from(const OrderedHashMap &p_map) {
		for (ConstElement E = p_map.front(); E; E = E.next()) {
			insert(E.key(), E.value());
		}
	}

public:
	void operator=(const OrderedHashMap &p_map) {
		_copy_from(p_map);
	}

	OrderedHashMap(const OrderedHashMap &p_map) {
		_copy_from(p_map);
	}

	_FORCE_INLINE_ OrderedHashMap() {
	}
};

#endif // ORDERED_HASH_MAP_H

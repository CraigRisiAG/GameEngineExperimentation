

/**
 * @class SelfList
 * @brief A self-referencing doubly-linked list node that manages its own membership in a list.
 * 
 * @tparam T The type of data stored in the list node.
 * 
 * SelfList is a template class that implements an intrusive doubly-linked list where each node
 * maintains pointers to the next and previous elements, as well as a reference to its parent List.
 * This design allows elements to be part of a list without requiring external node wrappers.
 * 
 * @nested class List
 * @brief Manages a doubly-linked list of SelfList nodes.
 * 
 * The List class serves as the container that holds the first and last pointers of the linked list.
 * It provides methods to add, remove, and access elements.
 * 
 * @method void add(SelfList<T> *p_elem)
 * @brief Adds an element to the front of the list.
 * @param p_elem Pointer to the SelfList element to add. Must not already be in a list.
 * 
 * @method void add_last(SelfList<T> *p_elem)
 * @brief Adds an element to the end of the list.
 * @param p_elem Pointer to the SelfList element to add. Must not already be in a list.
 * 
 * @method void remove(SelfList<T> *p_elem)
 * @brief Removes an element from the list.
 * @param p_elem Pointer to the SelfList element to remove. Must be in this list.
 * 
 * @method SelfList<T> *first()
 * @brief Returns a pointer to the first element in the list.
 * @return Pointer to the first SelfList node, or NULL if list is empty.
 * 
 * @brief SelfList(T *p_self)
 * @brief Constructs a new SelfList node.
 * @param p_self Pointer to the data object this node represents.
 * 
 * @method bool in_list() const
 * @brief Checks if this element is currently in a list.
 * @return True if the element is in a list, false otherwise.
 * 
 * @method SelfList<T> *next()
 * @brief Returns a pointer to the next element.
 * 
 * @method SelfList<T> *prev()
 * @brief Returns a pointer to the previous element.
 * 
 * @method T *self() const
 * @brief Returns a pointer to the data object this node represents.
 * 
 * @destructor ~SelfList()
 * @brief Destroys the node and automatically removes it from its list if present.
 */
#ifndef SELF_LIST_H
#define SELF_LIST_H

#include "core/error_macros.h"
#include "core/typedefs.h"

template <class T>
class SelfList {
public:
	class List {

		SelfList<T> *_first;
		SelfList<T> *_last;

	public:
		void add(SelfList<T> *p_elem) {

			ERR_FAIL_COND(p_elem->_root);

			p_elem->_root = this;
			p_elem->_next = _first;
			p_elem->_prev = NULL;

			if (_first) {
				_first->_prev = p_elem;

			} else {
				_last = p_elem;
			}

			_first = p_elem;
		}

		void add_last(SelfList<T> *p_elem) {

			ERR_FAIL_COND(p_elem->_root);

			p_elem->_root = this;
			p_elem->_next = NULL;
			p_elem->_prev = _last;

			if (_last) {
				_last->_next = p_elem;

			} else {
				_first = p_elem;
			}

			_last = p_elem;
		}

		void remove(SelfList<T> *p_elem) {

			ERR_FAIL_COND(p_elem->_root != this);
			if (p_elem->_next) {
				p_elem->_next->_prev = p_elem->_prev;
			}

			if (p_elem->_prev) {
				p_elem->_prev->_next = p_elem->_next;
			}

			if (_first == p_elem) {
				_first = p_elem->_next;
			}

			if (_last == p_elem) {
				_last = p_elem->_prev;
			}

			p_elem->_next = NULL;
			p_elem->_prev = NULL;
			p_elem->_root = NULL;
		}

		_FORCE_INLINE_ SelfList<T> *first() { return _first; }
		_FORCE_INLINE_ const SelfList<T> *first() const { return _first; }
		_FORCE_INLINE_ List() {
			_first = NULL;
			_last = NULL;
		}
		_FORCE_INLINE_ ~List() { ERR_FAIL_COND(_first != NULL); }
	};

private:
	List *_root;
	T *_self;
	SelfList<T> *_next;
	SelfList<T> *_prev;

public:
	_FORCE_INLINE_ bool in_list() const { return _root; }
	_FORCE_INLINE_ SelfList<T> *next() { return _next; }
	_FORCE_INLINE_ SelfList<T> *prev() { return _prev; }
	_FORCE_INLINE_ const SelfList<T> *next() const { return _next; }
	_FORCE_INLINE_ const SelfList<T> *prev() const { return _prev; }
	_FORCE_INLINE_ T *self() const { return _self; }

	_FORCE_INLINE_ SelfList(T *p_self) {

		_self = p_self;
		_next = NULL;
		_prev = NULL;
		_root = NULL;
	}

	_FORCE_INLINE_ ~SelfList() {

		if (_root)
			_root->remove(this);
	}
};

#endif // SELF_LIST_H



/**
 * @class RingBuffer
 * @brief A templated circular/ring buffer data structure for efficient sequential data storage and retrieval.
 * 
 * RingBuffer provides a fixed-size circular buffer implementation that maintains separate read and write
 * positions. It efficiently handles wrapping around the buffer boundaries using bitwise operations.
 * 
 * @tparam T The type of elements stored in the ring buffer.
 * 
 * @note The buffer size must be a power of 2 to support efficient modulo operations via bitmasks.
 * 
 * @warning The buffer is full when (write_pos - read_pos) == size - 1. It is never completely full
 * to distinguish between empty and full states.
 */

/**
 * @brief Reads a single element from the buffer and advances the read position.
 * @return A copy of the element at the read position, or a default-constructed T() if the buffer is empty.
 */

/**
 * @brief Reads multiple elements from the buffer into a provided buffer.
 * @param p_buf Pointer to the destination buffer where data will be copied.
 * @param p_size The number of elements to read.
 * @param p_advance If true, advances the read position; if false, leaves it unchanged. Default is true.
 * @return The actual number of elements read (limited by available data).
 */

/**
 * @brief Copies data from the buffer without advancing the read position.
 * @param p_buf Pointer to the destination buffer.
 * @param p_offset The offset from the current read position to start copying.
 * @param p_size The number of elements to copy.
 * @return The actual number of elements copied.
 */

/**
 * @brief Searches for an element within the buffer starting at a given offset.
 * @param t The element to search for.
 * @param p_offset The offset from the current read position to start searching.
 * @param p_max_size The maximum number of elements to search through.
 * @return The offset where the element was found, or -1 if not found.
 */

/**
 * @brief Advances the read position by the specified amount.
 * @param p_n The number of positions to advance.
 * @return The actual number of positions advanced (clamped by available data).
 */

/**
 * @brief Decreases the write position by the specified amount.
 * @param p_n The number of positions to move backward.
 * @return The actual number of positions decreased (clamped by available data).
 */

/**
 * @brief Writes a single element to the buffer at the write position.
 * @param p_v The element to write.
 * @return OK if successful, FAILED if there is insufficient space.
 */

/**
 * @brief Writes multiple elements from a source buffer into the ring buffer.
 * @param p_buf Pointer to the source buffer.
 * @param p_size The number of elements to write.
 * @return The actual number of elements written (limited by available space).
 */

/**
 * @brief Returns the number of free/unoccupied spaces in the buffer.
 * @return The number of elements that can be written before the buffer is full.
 */

/**
 * @brief Returns the number of occupied/readable elements in the buffer.
 * @return The number of elements currently available to read.
 */

/**
 * @brief Returns the current capacity of the ring buffer.
 * @return The total size of the underlying data storage.
 */

/**
 * @brief Clears the ring buffer by resetting read and write positions to zero.
 * @note This does not deallocate memory, only resets the positions.
 */

/**
 * @brief Resizes the ring buffer to a new size specified as a power of 2.
 * @param p_power The exponent: new size will be 2^p_power.
 * @note If the buffer is resized and contains wrapped data, the data is reorganized to be contiguous.
 */

/**
 * @brief Constructs a RingBuffer with an optional initial capacity.
 * @param p_power The exponent for initial size (2^p_power). Default is 0 (size 1).
 */

/**
 * @brief Destructs the RingBuffer.
 */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "core/vector.h"

template <typename T>
class RingBuffer {

	Vector<T> data;
	int read_pos;
	int write_pos;
	int size_mask;

	inline int inc(int &p_var, int p_size) const {
		int ret = p_var;
		p_var += p_size;
		p_var = p_var & size_mask;
		return ret;
	};

public:
	T read() {
		ERR_FAIL_COND_V(space_left() < 1, T());
		return data.ptr()[inc(read_pos, 1)];
	};

	int read(T *p_buf, int p_size, bool p_advance = true) {
		int left = data_left();
		p_size = MIN(left, p_size);
		int pos = read_pos;
		int to_read = p_size;
		int dst = 0;
		while (to_read) {
			int end = pos + to_read;
			end = MIN(end, size());
			int total = end - pos;
			const T *read = data.ptr();
			for (int i = 0; i < total; i++) {
				p_buf[dst++] = read[pos + i];
			};
			to_read -= total;
			pos = 0;
		};
		if (p_advance) {
			inc(read_pos, p_size);
		};
		return p_size;
	};

	int copy(T *p_buf, int p_offset, int p_size) const {

		int left = data_left();
		if ((p_offset + p_size) > left) {
			p_size -= left - p_offset;
			if (p_size <= 0)
				return 0;
		}
		p_size = MIN(left, p_size);
		int pos = read_pos;
		inc(pos, p_offset);
		int to_read = p_size;
		int dst = 0;
		while (to_read) {
			int end = pos + to_read;
			end = MIN(end, size());
			int total = end - pos;
			for (int i = 0; i < total; i++) {
				p_buf[dst++] = data[pos + i];
			};
			to_read -= total;
			pos = 0;
		};
		return p_size;
	};

	int find(const T &t, int p_offset, int p_max_size) const {

		int left = data_left();
		if ((p_offset + p_max_size) > left) {
			p_max_size -= left - p_offset;
			if (p_max_size <= 0)
				return 0;
		}
		p_max_size = MIN(left, p_max_size);
		int pos = read_pos;
		inc(pos, p_offset);
		int to_read = p_max_size;
		while (to_read) {
			int end = pos + to_read;
			end = MIN(end, size());
			int total = end - pos;
			for (int i = 0; i < total; i++) {
				if (data[pos + i] == t)
					return i + (p_max_size - to_read);
			};
			to_read -= total;
			pos = 0;
		}
		return -1;
	}

	inline int advance_read(int p_n) {
		p_n = MIN(p_n, data_left());
		inc(read_pos, p_n);
		return p_n;
	};

	inline int decrease_write(int p_n) {
		p_n = MIN(p_n, data_left());
		inc(write_pos, size_mask + 1 - p_n);
		return p_n;
	}

	Error write(const T &p_v) {
		ERR_FAIL_COND_V(space_left() < 1, FAILED);
		data.write[inc(write_pos, 1)] = p_v;
		return OK;
	};

	int write(const T *p_buf, int p_size) {

		int left = space_left();
		p_size = MIN(left, p_size);

		int pos = write_pos;
		int to_write = p_size;
		int src = 0;
		while (to_write) {

			int end = pos + to_write;
			end = MIN(end, size());
			int total = end - pos;

			for (int i = 0; i < total; i++) {
				data.write[pos + i] = p_buf[src++];
			};
			to_write -= total;
			pos = 0;
		};

		inc(write_pos, p_size);
		return p_size;
	};

	inline int space_left() const {
		int left = read_pos - write_pos;
		if (left < 0) {
			return size() + left - 1;
		};
		if (left == 0) {
			return size() - 1;
		};
		return left - 1;
	};
	inline int data_left() const {
		return size() - space_left() - 1;
	};

	inline int size() const {
		return data.size();
	};

	inline void clear() {
		read_pos = 0;
		write_pos = 0;
	}

	void resize(int p_power) {
		int old_size = size();
		int new_size = 1 << p_power;
		int mask = new_size - 1;
		data.resize(1 << p_power);
		if (old_size < new_size && read_pos > write_pos) {
			for (int i = 0; i < write_pos; i++) {
				data.write[(old_size + i) & mask] = data[i];
			};
			write_pos = (old_size + write_pos) & mask;
		} else {
			read_pos = read_pos & mask;
			write_pos = write_pos & mask;
		};

		size_mask = mask;
	};

	RingBuffer<T>(int p_power = 0) {
		read_pos = 0;
		write_pos = 0;
		resize(p_power);
	};
	~RingBuffer<T>(){};
};

#endif // RING_BUFFER_H

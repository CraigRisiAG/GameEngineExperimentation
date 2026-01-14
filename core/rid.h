

/**
 * @class RID
 * @brief Resource IDentifier - A lightweight handle for managing resource references.
 * 
 * RID is a simple wrapper around a 64-bit unsigned integer used to uniquely identify
 * and track resources within the game engine. It provides comparison operators and
 * validity checking without storing the actual resource data.
 * 
 * @details
 * - RIDs are compared by their internal ID values
 * - A RID with _id == 0 is considered null/invalid
 * - RIDs are designed to be cheap to copy and pass around
 * - The actual resource data is managed separately by RID_AllocBase
 * 
 * @note RID_AllocBase is a friend class that handles allocation and management
 *       of resources associated with RID instances.
 * 
 * @see RID_AllocBase
 */

/**
 * @brief Default constructor.
 * Initializes a null RID with _id set to 0.
 */

/**
 * @brief Equality comparison operator.
 * @param p_rid The RID to compare with.
 * @return true if both RIDs have the same ID, false otherwise.
 */

/**
 * @brief Less-than comparison operator.
 * @param p_rid The RID to compare with.
 * @return true if this RID's ID is less than p_rid's ID.
 */

/**
 * @brief Less-than-or-equal comparison operator.
 * @param p_rid The RID to compare with.
 * @return true if this RID's ID is less than or equal to p_rid's ID.
 */

/**
 * @brief Greater-than comparison operator.
 * @param p_rid The RID to compare with.
 * @return true if this RID's ID is greater than p_rid's ID.
 */

/**
 * @brief Inequality comparison operator.
 * @param p_rid The RID to compare with.
 * @return true if both RIDs have different IDs, false otherwise.
 */

/**
 * @brief Checks if the RID is valid.
 * @return true if this RID has a non-zero ID, false otherwise.
 */

/**
 * @brief Checks if the RID is null.
 * @return true if this RID has an ID of 0, false otherwise.
 */

/**
 * @brief Retrieves the internal ID value.
 * @return The 64-bit unsigned integer ID.
 */
#ifndef RID_H
#define RID_H

#include "core/list.h"
#include "core/oa_hash_map.h"
#include "core/os/memory.h"
#include "core/safe_refcount.h"
#include "core/set.h"
#include "core/typedefs.h"

class RID_AllocBase;

class RID {
	friend class RID_AllocBase;
	uint64_t _id;

public:
	_FORCE_INLINE_ bool operator==(const RID &p_rid) const {

		return _id == p_rid._id;
	}
	_FORCE_INLINE_ bool operator<(const RID &p_rid) const {

		return _id < p_rid._id;
	}
	_FORCE_INLINE_ bool operator<=(const RID &p_rid) const {

		return _id <= p_rid._id;
	}
	_FORCE_INLINE_ bool operator>(const RID &p_rid) const {

		return _id > p_rid._id;
	}
	_FORCE_INLINE_ bool operator!=(const RID &p_rid) const {

		return _id != p_rid._id;
	}
	_FORCE_INLINE_ bool is_valid() const { return _id != 0; }
	_FORCE_INLINE_ bool is_null() const { return _id == 0; }

	_FORCE_INLINE_ uint64_t get_id() const { return _id; }

	_FORCE_INLINE_ RID() {
		_id = 0;
	}
};

#endif // RID_H



/// \class PackedDataContainer
/// \brief A container for efficiently storing and accessing packed binary data structures.
/// 
/// PackedDataContainer provides a mechanism to pack complex data types (arrays, dictionaries, 
/// and variant types) into a compact binary format and retrieve them without full deserialization.
/// It supports hierarchical data structures and uses offset-based access for efficient memory usage.
///
/// \note String values are cached during packing to reduce redundant storage.
/// \note Nested arrays and dictionaries are stored as references (PackedDataContainerRef).

/// \fn Variant PackedDataContainer::getvar(const Variant &p_key, bool *r_valid) const
/// \brief Retrieves a value from the root container using the specified key.
/// \param p_key The key to look up (supports numeric indices for arrays or variant keys for dictionaries).
/// \param r_valid Optional pointer to a boolean that will be set to false if the key is not found.
/// \return The variant value associated with the key, or an empty Variant if not found.

/// \fn int PackedDataContainer::size() const
/// \brief Returns the number of elements in the root container.
/// \return The size of the root array or dictionary, or -1 if the root is neither.

/// \fn Variant PackedDataContainer::_iter_init_ofs(const Array &p_iter, uint32_t p_offset)
/// \brief Initializes iteration state for a container at the given offset.
/// \param p_iter Array containing iteration state (should have size 1).
/// \param p_offset Memory offset of the container to iterate.
/// \return true if iteration can begin, false if the container is empty.

/// \fn Variant PackedDataContainer::_iter_next_ofs(const Array &p_iter, uint32_t p_offset)
/// \brief Advances the iteration state to the next element.
/// \param p_iter Array containing the current iteration position.
/// \param p_offset Memory offset of the container being iterated.
/// \return true if there are more elements, false when iteration is complete.

/// \fn Variant PackedDataContainer::_iter_get_ofs(const Variant &p_iter, uint32_t p_offset)
/// \brief Retrieves the element at the current iteration position.
/// \param p_iter The current iteration position (integer index).
/// \param p_offset Memory offset of the container being iterated.
/// \return The variant value at the current position, or empty Variant if out of bounds.

/// \fn Variant PackedDataContainer::_get_at_ofs(uint32_t p_ofs, const uint8_t *p_buf, bool &err) const
/// \brief Decodes and retrieves a variant value from the specified buffer offset.
/// \param p_ofs The offset in the buffer to decode from.
/// \param p_buf Pointer to the binary data buffer.
/// \param err Reference to error flag (set to true if decoding fails).
/// \return The decoded variant, or a reference to nested structures (arrays/dictionaries).

/// \fn uint32_t PackedDataContainer::_type_at_ofs(uint32_t p_ofs) const
/// \brief Determines the type of data stored at the given offset.
/// \param p_ofs The offset to check.
/// \return The type identifier (TYPE_ARRAY, TYPE_DICT, or variant type).

/// \fn int PackedDataContainer::_size(uint32_t p_ofs) const
/// \brief Returns the element count of an array or dictionary at the specified offset.
/// \param p_ofs The offset of the container.
/// \return The number of elements, or -1 if the data is not an array or dictionary.

/// \fn Variant PackedDataContainer::_key_at_ofs(uint32_t p_ofs, const Variant &p_key, bool &err) const
/// \brief Looks up a value in an array or dictionary at the given offset.
/// \param p_ofs The offset of the container.
/// \param p_key The key to search for (index for arrays, variant key for dictionaries).
/// \param err Reference to error flag (set to true if key not found or lookup fails).
/// \return The variant value associated with the key.

/// \fn uint32_t PackedDataContainer::_pack(const Variant &p_data, Vector<uint8_t> &tmpdata, Map<String, uint32_t> &string_cache)
/// \brief Recursively packs a variant into binary format.
/// \param p_data The variant data to pack.
/// \param tmpdata Vector to append packed binary data to.
/// \param string_cache Map for deduplicating string values.
/// \return The buffer offset where the packed data begins.

/// \fn Error PackedDataContainer::pack(const Variant &p_data)
/// \brief Packs the given variant data into this container's internal binary format.
/// \param p_data The variant (typically an array or dictionary) to pack.
/// \return OK on success.

/// \fn void PackedDataContainer::_set_data(const Vector<uint8_t> &p_data)
/// \brief Loads binary data from an external source.
/// \param p_data The pre-packed binary data.

/// \fn Vector<uint8_t> PackedDataContainer::_get_data() const
/// \brief Exports the internal binary data.
/// \return A copy of the packed data buffer.

/// \class PackedDataContainerRef
/// \brief A reference wrapper providing access to a nested array or dictionary within a PackedDataContainer.
/// 
/// PackedDataContainerRef allows transparent access to nested structures without requiring
/// full deserialization of the entire packed data.

/// \fn Variant PackedDataContainerRef::getvar(const Variant &p_key, bool *r_valid) const
/// \brief Retrieves a value from this reference's container using the specified key.
/// \param p_key The key to look up.
/// \param r_valid Optional pointer to a boolean set to false if key is not found.
/// \return The variant value, or empty Variant if not found.

/// \fn int PackedDataContainerRef::size() const
/// \brief Returns the number of elements in this container reference.
/// \return The element count.

/// \fn bool PackedDataContainerRef::_is_dictionary() const
/// \brief Determines whether this reference points to a dictionary or array.
/// \return true if the referenced container is a dictionary, false if it's an array.
#include "packed_data_container.h"

#include "core/core_string_names.h"
#include "core/io/marshalls.h"

Variant PackedDataContainer::getvar(const Variant &p_key, bool *r_valid) const {

	bool err = false;
	Variant ret = _key_at_ofs(0, p_key, err);
	if (r_valid)
		*r_valid = !err;
	return ret;
}

int PackedDataContainer::size() const {

	return _size(0);
};

Variant PackedDataContainer::_iter_init_ofs(const Array &p_iter, uint32_t p_offset) {

	Array ref = p_iter;
	uint32_t size = _size(p_offset);
	if (size == 0 || ref.size() != 1)
		return false;
	else {
		ref[0] = 0;
		return true;
	}
}

Variant PackedDataContainer::_iter_next_ofs(const Array &p_iter, uint32_t p_offset) {

	Array ref = p_iter;
	int size = _size(p_offset);
	if (ref.size() != 1)
		return false;
	int pos = ref[0];
	if (pos < 0 || pos >= size)
		return false;
	pos += 1;
	ref[0] = pos;
	return pos != size;
}

Variant PackedDataContainer::_iter_get_ofs(const Variant &p_iter, uint32_t p_offset) {

	int size = _size(p_offset);
	int pos = p_iter;
	if (pos < 0 || pos >= size)
		return Variant();

	const uint8_t *rd = data.ptr();
	const uint8_t *r = &rd[p_offset];
	uint32_t type = decode_uint32(r);

	bool err = false;
	if (type == TYPE_ARRAY) {

		uint32_t vpos = decode_uint32(rd + p_offset + 8 + pos * 4);
		return _get_at_ofs(vpos, rd, err);

	} else if (type == TYPE_DICT) {

		uint32_t vpos = decode_uint32(rd + p_offset + 8 + pos * 12 + 4);
		return _get_at_ofs(vpos, rd, err);
	} else {
		ERR_FAIL_V(Variant());
	}
}

Variant PackedDataContainer::_get_at_ofs(uint32_t p_ofs, const uint8_t *p_buf, bool &err) const {

	uint32_t type = decode_uint32(p_buf + p_ofs);

	if (type == TYPE_ARRAY || type == TYPE_DICT) {

		Ref<PackedDataContainerRef> pdcr = memnew(PackedDataContainerRef);
		Ref<PackedDataContainer> pdc = Ref<PackedDataContainer>((PackedDataContainer *)this);

		pdcr->from = pdc;
		pdcr->offset = p_ofs;
		return pdcr;
	} else {

		Variant v;
		Error rerr = decode_variant(v, p_buf + p_ofs, datalen - p_ofs, NULL, false);

		if (rerr != OK) {

			err = true;
			ERR_FAIL_COND_V_MSG(err != OK, Variant(), "Error when trying to decode Variant.");
		}
		return v;
	}
}

uint32_t PackedDataContainer::_type_at_ofs(uint32_t p_ofs) const {

	const uint8_t *rd = data.ptr();
	const uint8_t *r = &rd[p_ofs];
	uint32_t type = decode_uint32(r);

	return type;
};

int PackedDataContainer::_size(uint32_t p_ofs) const {

	const uint8_t *rd = data.ptr();
	ERR_FAIL_COND_V(!rd, 0);
	const uint8_t *r = &rd[p_ofs];
	uint32_t type = decode_uint32(r);

	if (type == TYPE_ARRAY) {

		uint32_t len = decode_uint32(r + 4);
		return len;

	} else if (type == TYPE_DICT) {

		uint32_t len = decode_uint32(r + 4);
		return len;
	};

	return -1;
};

Variant PackedDataContainer::_key_at_ofs(uint32_t p_ofs, const Variant &p_key, bool &err) const {

	const uint8_t *rd = data.ptr();
	const uint8_t *r = &rd[p_ofs];
	uint32_t type = decode_uint32(r);

	if (type == TYPE_ARRAY) {

		if (p_key.is_num()) {

			int idx = p_key;
			int len = decode_uint32(r + 4);
			if (idx < 0 || idx >= len) {
				err = true;
				return Variant();
			}
			uint32_t ofs = decode_uint32(r + 8 + 4 * idx);
			return _get_at_ofs(ofs, rd, err);

		} else {
			err = true;
			return Variant();
		}

	} else if (type == TYPE_DICT) {

		uint32_t hash = p_key.hash();
		uint32_t len = decode_uint32(r + 4);

		bool found = false;
		for (uint32_t i = 0; i < len; i++) {
			uint32_t khash = decode_uint32(r + 8 + i * 12 + 0);
			if (khash == hash) {
				Variant key = _get_at_ofs(decode_uint32(r + 8 + i * 12 + 4), rd, err);
				if (err)
					return Variant();
				if (key == p_key) {
					//key matches, return value
					return _get_at_ofs(decode_uint32(r + 8 + i * 12 + 8), rd, err);
				}
				found = true;
			} else {
				if (found)
					break;
			}
		}

		err = true;
		return Variant();

	} else {

		err = true;
		return Variant();
	}
}

uint32_t PackedDataContainer::_pack(const Variant &p_data, Vector<uint8_t> &tmpdata, Map<String, uint32_t> &string_cache) {

	switch (p_data.get_type()) {

		case Variant::STRING: {

			String s = p_data;
			if (string_cache.has(s)) {
				return string_cache[s];
			}

			string_cache[s] = tmpdata.size();

			[[fallthrough]];
		}
		case Variant::NIL:
		case Variant::BOOL:
		case Variant::INT:
		case Variant::FLOAT:
		case Variant::VECTOR2:
		case Variant::RECT2:
		case Variant::VECTOR3:
		case Variant::TRANSFORM2D:
		case Variant::PLANE:
		case Variant::QUAT:
		case Variant::AABB:
		case Variant::BASIS:
		case Variant::TRANSFORM:
		case Variant::PACKED_BYTE_ARRAY:
		case Variant::PACKED_INT32_ARRAY:
		case Variant::PACKED_INT64_ARRAY:
		case Variant::PACKED_FLOAT32_ARRAY:
		case Variant::PACKED_FLOAT64_ARRAY:
		case Variant::PACKED_STRING_ARRAY:
		case Variant::PACKED_VECTOR2_ARRAY:
		case Variant::PACKED_VECTOR3_ARRAY:
		case Variant::PACKED_COLOR_ARRAY:
		case Variant::STRING_NAME:
		case Variant::NODE_PATH: {

			uint32_t pos = tmpdata.size();
			int len;
			encode_variant(p_data, NULL, len, false);
			tmpdata.resize(tmpdata.size() + len);
			encode_variant(p_data, &tmpdata.write[pos], len, false);
			return pos;

		} break;
		// misc types
		case Variant::_RID:
		case Variant::OBJECT: {

			return _pack(Variant(), tmpdata, string_cache);
		} break;
		case Variant::DICTIONARY: {

			Dictionary d = p_data;
			//size is known, use sort
			uint32_t pos = tmpdata.size();
			int len = d.size();
			tmpdata.resize(tmpdata.size() + len * 12 + 8);
			encode_uint32(TYPE_DICT, &tmpdata.write[pos + 0]);
			encode_uint32(len, &tmpdata.write[pos + 4]);

			List<Variant> keys;
			d.get_key_list(&keys);
			List<DictKey> sortk;

			for (List<Variant>::Element *E = keys.front(); E; E = E->next()) {

				DictKey dk;
				dk.hash = E->get().hash();
				dk.key = E->get();
				sortk.push_back(dk);
			}

			sortk.sort();

			int idx = 0;
			for (List<DictKey>::Element *E = sortk.front(); E; E = E->next()) {

				encode_uint32(E->get().hash, &tmpdata.write[pos + 8 + idx * 12 + 0]);
				uint32_t ofs = _pack(E->get().key, tmpdata, string_cache);
				encode_uint32(ofs, &tmpdata.write[pos + 8 + idx * 12 + 4]);
				ofs = _pack(d[E->get().key], tmpdata, string_cache);
				encode_uint32(ofs, &tmpdata.write[pos + 8 + idx * 12 + 8]);
				idx++;
			}

			return pos;

		} break;
		case Variant::ARRAY: {

			Array a = p_data;
			//size is known, use sort
			uint32_t pos = tmpdata.size();
			int len = a.size();
			tmpdata.resize(tmpdata.size() + len * 4 + 8);
			encode_uint32(TYPE_ARRAY, &tmpdata.write[pos + 0]);
			encode_uint32(len, &tmpdata.write[pos + 4]);

			for (int i = 0; i < len; i++) {

				uint32_t ofs = _pack(a[i], tmpdata, string_cache);
				encode_uint32(ofs, &tmpdata.write[pos + 8 + i * 4]);
			}

			return pos;

		} break;

		default: {
		}
	}

	return OK;
}

Error PackedDataContainer::pack(const Variant &p_data) {

	Vector<uint8_t> tmpdata;
	Map<String, uint32_t> string_cache;
	_pack(p_data, tmpdata, string_cache);
	datalen = tmpdata.size();
	data.resize(tmpdata.size());
	uint8_t *w = data.ptrw();
	copymem(w, tmpdata.ptr(), tmpdata.size());

	return OK;
}

void PackedDataContainer::_set_data(const Vector<uint8_t> &p_data) {

	data = p_data;
	datalen = data.size();
}

Vector<uint8_t> PackedDataContainer::_get_data() const {
	return data;
}

Variant PackedDataContainer::_iter_init(const Array &p_iter) {

	return _iter_init_ofs(p_iter, 0);
}

Variant PackedDataContainer::_iter_next(const Array &p_iter) {

	return _iter_next_ofs(p_iter, 0);
}
Variant PackedDataContainer::_iter_get(const Variant &p_iter) {

	return _iter_get_ofs(p_iter, 0);
}

void PackedDataContainer::_bind_methods() {

	ClassDB::bind_method(D_METHOD("_set_data"), &PackedDataContainer::_set_data);
	ClassDB::bind_method(D_METHOD("_get_data"), &PackedDataContainer::_get_data);
	ClassDB::bind_method(D_METHOD("_iter_init"), &PackedDataContainer::_iter_init);
	ClassDB::bind_method(D_METHOD("_iter_get"), &PackedDataContainer::_iter_get);
	ClassDB::bind_method(D_METHOD("_iter_next"), &PackedDataContainer::_iter_next);
	ClassDB::bind_method(D_METHOD("pack", "value"), &PackedDataContainer::pack);
	ClassDB::bind_method(D_METHOD("size"), &PackedDataContainer::size);

	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "__data__"), "_set_data", "_get_data");
}

PackedDataContainer::PackedDataContainer() {

	datalen = 0;
}

//////////////////

Variant PackedDataContainerRef::_iter_init(const Array &p_iter) {

	return from->_iter_init_ofs(p_iter, offset);
}

Variant PackedDataContainerRef::_iter_next(const Array &p_iter) {

	return from->_iter_next_ofs(p_iter, offset);
}
Variant PackedDataContainerRef::_iter_get(const Variant &p_iter) {

	return from->_iter_get_ofs(p_iter, offset);
}

bool PackedDataContainerRef::_is_dictionary() const {

	return from->_type_at_ofs(offset) == PackedDataContainer::TYPE_DICT;
};

void PackedDataContainerRef::_bind_methods() {

	ClassDB::bind_method(D_METHOD("size"), &PackedDataContainerRef::size);
	ClassDB::bind_method(D_METHOD("_iter_init"), &PackedDataContainerRef::_iter_init);
	ClassDB::bind_method(D_METHOD("_iter_get"), &PackedDataContainerRef::_iter_get);
	ClassDB::bind_method(D_METHOD("_iter_next"), &PackedDataContainerRef::_iter_next);
	ClassDB::bind_method(D_METHOD("_is_dictionary"), &PackedDataContainerRef::_is_dictionary);
}

Variant PackedDataContainerRef::getvar(const Variant &p_key, bool *r_valid) const {

	bool err = false;
	Variant ret = from->_key_at_ofs(offset, p_key, err);
	if (r_valid)
		*r_valid = !err;
	return ret;
}

int PackedDataContainerRef::size() const {

	return from->_size(offset);
};

PackedDataContainerRef::PackedDataContainerRef() {
}


/// @class PHashTranslation
/// @brief A compressed translation system using perfect hashing and string
/// compression.
///
/// PHashTranslation implements an efficient translation lookup mechanism that
/// combines perfect hashing for O(1) message retrieval with SMAZ compression to
/// minimize memory usage.
///
/// @details
/// The system works by:
/// 1. Building a perfect hash table from translation keys using dynamic hash
/// function generation
/// 2. Compressing translation strings using the SMAZ algorithm
/// 3. Storing compressed data in a compact bucket table structure for efficient
/// serialization
///
/// @method generate(const Ref<Translation> &p_from)
/// @brief Generates the compressed translation tables from a source Translation
/// resource.
/// @param p_from The source Translation object containing message key-value
/// pairs
/// @details Builds the hash table, bucket table, and compressed strings array.
/// Only available
///          in TOOLS_ENABLED builds.
///
/// @method StringName get_message(const StringName &p_src_text) const
/// @brief Retrieves the translated message for a given source text using
/// perfect hashing.
/// @param p_src_text The source text key to look up
/// @return The translated string, or empty StringName if not found
/// @details Uses a two-level hash function (primary + secondary per-bucket) to
/// locate and
///          decompress the message. Falls back to uncompressed data if
///          compression didn't reduce size.
///
/// @internal
/// Hash table structure:
/// - hash_table: Array of bucket indices indexed by primary hash % size
/// - bucket_table: Packed array of bucket headers and elements containing
/// secondary hash functions
/// - strings: Compressed string data referenced by bucket elements
///
/// @note Requires TOOLS_ENABLED for generation; lookup works in any build
/// configuration.
#include "compressed_translation.h"

#include "core/pair.h"

extern "C" {
#include "thirdparty/misc/smaz.h"
}

struct _PHashTranslationCmp {

  int orig_len;
  CharString compressed;
  int offset;
};

void PHashTranslation::generate(const Ref<Translation> &p_from) {
#ifdef TOOLS_ENABLED
  List<StringName> keys;
  p_from->get_message_list(&keys);

  int size = Math::larger_prime(keys.size());

  Vector<Vector<Pair<int, CharString>>> buckets;
  Vector<Map<uint32_t, int>> table;
  Vector<uint32_t> hfunc_table;
  Vector<_PHashTranslationCmp> compressed;

  table.resize(size);
  hfunc_table.resize(size);
  buckets.resize(size);
  compressed.resize(keys.size());

  int idx = 0;
  int total_compression_size = 0;
  int total_string_size = 0;

  for (List<StringName>::Element *E = keys.front(); E; E = E->next()) {

    // hash string
    CharString cs = E->get().operator String().utf8();
    uint32_t h = hash(0, cs.get_data());
    Pair<int, CharString> p;
    p.first = idx;
    p.second = cs;
    buckets.write[h % size].push_back(p);

    // compress string
    CharString src_s = p_from->get_message(E->get()).operator String().utf8();
    _PHashTranslationCmp ps;
    ps.orig_len = src_s.size();
    ps.offset = total_compression_size;

    if (ps.orig_len != 0) {
      CharString dst_s;
      dst_s.resize(src_s.size());
      int ret = smaz_compress(src_s.get_data(), src_s.size(), dst_s.ptrw(),
                              src_s.size());
      if (ret >= src_s.size()) {
        // if compressed is larger than original, just use original
        ps.orig_len = src_s.size();
        ps.compressed = src_s;
      } else {
        dst_s.resize(ret);
        // ps.orig_len=;
        ps.compressed = dst_s;
      }
    } else {
      ps.orig_len = 1;
      ps.compressed.resize(1);
      ps.compressed[0] = 0;
    }

    compressed.write[idx] = ps;
    total_compression_size += ps.compressed.size();
    total_string_size += src_s.size();
    idx++;
  }

  int bucket_table_size = 0;

  for (int i = 0; i < size; i++) {

    const Vector<Pair<int, CharString>> &b = buckets[i];
    Map<uint32_t, int> &t = table.write[i];

    if (b.size() == 0)
      continue;

    int d = 1;
    int item = 0;

    while (item < b.size()) {

      uint32_t slot = hash(d, b[item].second.get_data());
      if (t.has(slot)) {

        item = 0;
        d++;
        t.clear();
      } else {
        t[slot] = b[item].first;
        item++;
      }
    }

    hfunc_table.write[i] = d;
    bucket_table_size += 2 + b.size() * 4;
  }

  ERR_FAIL_COND(bucket_table_size == 0);

  hash_table.resize(size);
  bucket_table.resize(bucket_table_size);

  int *htwb = hash_table.ptrw();
  int *btwb = bucket_table.ptrw();

  uint32_t *htw = (uint32_t *)&htwb[0];
  uint32_t *btw = (uint32_t *)&btwb[0];

  int btindex = 0;
  int collisions = 0;

  for (int i = 0; i < size; i++) {

    const Map<uint32_t, int> &t = table[i];
    if (t.size() == 0) {
      htw[i] = 0xFFFFFFFF; // nothing
      continue;
    } else if (t.size() > 1) {
      collisions += t.size() - 1;
    }

    htw[i] = btindex;
    btw[btindex++] = t.size();
    btw[btindex++] = hfunc_table[i];

    for (Map<uint32_t, int>::Element *E = t.front(); E; E = E->next()) {

      btw[btindex++] = E->key();
      btw[btindex++] = compressed[E->get()].offset;
      btw[btindex++] = compressed[E->get()].compressed.size();
      btw[btindex++] = compressed[E->get()].orig_len;
    }
  }

  strings.resize(total_compression_size);
  uint8_t *cw = strings.ptrw();

  for (int i = 0; i < compressed.size(); i++) {
    memcpy(&cw[compressed[i].offset], compressed[i].compressed.get_data(),
           compressed[i].compressed.size());
  }

  ERR_FAIL_COND(btindex != bucket_table_size);
  set_locale(p_from->get_locale());

#endif
}

bool PHashTranslation::_set(const StringName &p_name, const Variant &p_value) {

  String name = p_name.operator String();
  if (name == "hash_table") {
    hash_table = p_value;
  } else if (name == "bucket_table") {
    bucket_table = p_value;
  } else if (name == "strings") {
    strings = p_value;
  } else if (name == "load_from") {
    generate(p_value);
  } else
    return false;

  return true;
}

bool PHashTranslation::_get(const StringName &p_name, Variant &r_ret) const {

  String name = p_name.operator String();
  if (name == "hash_table")
    r_ret = hash_table;
  else if (name == "bucket_table")
    r_ret = bucket_table;
  else if (name == "strings")
    r_ret = strings;
  else
    return false;

  return true;
}

StringName PHashTranslation::get_message(const StringName &p_src_text) const {

  int htsize = hash_table.size();

  if (htsize == 0)
    return StringName();

  CharString str = p_src_text.operator String().utf8();
  uint32_t h = hash(0, str.get_data());

  const int *htr = hash_table.ptr();
  const uint32_t *htptr = (const uint32_t *)&htr[0];
  const int *btr = bucket_table.ptr();
  const uint32_t *btptr = (const uint32_t *)&btr[0];
  const uint8_t *sr = strings.ptr();
  const char *sptr = (const char *)&sr[0];

  uint32_t p = htptr[h % htsize];

  if (p == 0xFFFFFFFF) {
    return StringName(); // nothing
  }

  const Bucket &bucket = *(const Bucket *)&btptr[p];

  h = hash(bucket.func, str.get_data());

  int idx = -1;

  for (int i = 0; i < bucket.size; i++) {

    if (bucket.elem[i].key == h) {

      idx = i;
      break;
    }
  }

  if (idx == -1) {
    return StringName();
  }

  if (bucket.elem[idx].comp_size == bucket.elem[idx].uncomp_size) {

    String rstr;
    rstr.parse_utf8(&sptr[bucket.elem[idx].str_offset],
                    bucket.elem[idx].uncomp_size);

    return rstr;
  } else {

    CharString uncomp;
    uncomp.resize(bucket.elem[idx].uncomp_size + 1);
    smaz_decompress(&sptr[bucket.elem[idx].str_offset],
                    bucket.elem[idx].comp_size, uncomp.ptrw(),
                    bucket.elem[idx].uncomp_size);
    String rstr;
    rstr.parse_utf8(uncomp.get_data());
    return rstr;
  }
}

void PHashTranslation::_get_property_list(List<PropertyInfo> *p_list) const {

  p_list->push_back(PropertyInfo(Variant::PACKED_INT32_ARRAY, "hash_table"));
  p_list->push_back(PropertyInfo(Variant::PACKED_INT32_ARRAY, "bucket_table"));
  p_list->push_back(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "strings"));
  p_list->push_back(PropertyInfo(Variant::OBJECT, "load_from",
                                 PROPERTY_HINT_RESOURCE_TYPE, "Translation",
                                 PROPERTY_USAGE_EDITOR));
}
void PHashTranslation::_bind_methods() {

  ClassDB::bind_method(D_METHOD("generate", "from"),
                       &PHashTranslation::generate);
}

PHashTranslation::PHashTranslation() {}

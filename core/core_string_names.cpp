

/**
 * @brief Constructor for CoreStringNames singleton
 *
 * Initializes all static string constants used throughout the core engine.
 * This constructor creates and caches frequently used string names to avoid
 * repeated string allocations and comparisons.
 *
 * Initializes the following string categories:
 * - Core metadata and internal properties (_meta, _script, ___pdcdata, etc.)
 * - Script-related callbacks (script_changed, call, call_deferred, emit,
 * notification)
 * - Vector/Quaternion components (x, y, z, w, r, g, b, a, d, h, s, v)
 * - 8-bit color components (r8, g8, b8, a8)
 * - Transform properties (position, size, end, basis, origin, normal)
 * - Iterator protocol methods (_iter_init, _iter_next, _iter_get)
 * - Utility methods (_to_string, __getvar, get_rid)
 * - Editor-specific properties (_sections_unfolded - TOOLS_ENABLED only)
 * - Feature management (_custom_features)
 * - Object lifecycle (changed, free)
 *
 * @note This is a singleton class. Use CoreStringNames::get_singleton() to
 * access the instance.
 * @note All strings are stored as StaticCString for efficient memory management
 * and quick lookups.
 */
#include "core_string_names.h"

CoreStringNames *CoreStringNames::singleton = NULL;

CoreStringNames::CoreStringNames()
    : _free(StaticCString::create("free")),
      changed(StaticCString::create("changed")),
      _meta(StaticCString::create("__meta__")),
      _script(StaticCString::create("script")),
      script_changed(StaticCString::create("script_changed")),
      ___pdcdata(StaticCString::create("___pdcdata")),
      __getvar(StaticCString::create("__getvar")),
      _iter_init(StaticCString::create("_iter_init")),
      _iter_next(StaticCString::create("_iter_next")),
      _iter_get(StaticCString::create("_iter_get")),
      get_rid(StaticCString::create("get_rid")),
      _to_string(StaticCString::create("_to_string")),
#ifdef TOOLS_ENABLED
      _sections_unfolded(StaticCString::create("_sections_unfolded")),
#endif
      _custom_features(StaticCString::create("_custom_features")),
      x(StaticCString::create("x")), y(StaticCString::create("y")),
      z(StaticCString::create("z")), w(StaticCString::create("w")),
      r(StaticCString::create("r")), g(StaticCString::create("g")),
      b(StaticCString::create("b")), a(StaticCString::create("a")),
      position(StaticCString::create("position")),
      size(StaticCString::create("size")), end(StaticCString::create("end")),
      basis(StaticCString::create("basis")),
      origin(StaticCString::create("origin")),
      normal(StaticCString::create("normal")), d(StaticCString::create("d")),
      h(StaticCString::create("h")), s(StaticCString::create("s")),
      v(StaticCString::create("v")), r8(StaticCString::create("r8")),
      g8(StaticCString::create("g8")), b8(StaticCString::create("b8")),
      a8(StaticCString::create("a8")), call(StaticCString::create("call")),
      call_deferred(StaticCString::create("call_deferred")),
      emit(StaticCString::create("emit")),
      notification(StaticCString::create("notification")) {
}

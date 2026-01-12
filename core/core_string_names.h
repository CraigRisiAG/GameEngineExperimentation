

/**
 * @class CoreStringNames
 * @brief Singleton class managing commonly used string names throughout the core engine.
 * 
 * CoreStringNames provides centralized access to frequently used string identifiers
 * as StringName objects, improving performance by avoiding redundant string creation
 * and comparison operations.
 * 
 * @details
 * This singleton class is instantiated during engine initialization via register_core_types()
 * and destroyed during shutdown via unregister_core_types(). It caches StringName instances
 * for common property names, vector/color components, transformation data, and signal/method names.
 * 
 * @note
 * - StringName objects are interned strings with fast comparison semantics
 * - This class uses a singleton pattern with manual memory management
 * - Constructor implementation is private; use get_singleton() to access the instance
 * - The TOOLS_ENABLED preprocessor flag conditionally includes editor-specific string names
 * 
 * @see StringName, register_core_types(), unregister_core_types()
 */
#ifndef CORE_STRING_NAMES_H
#define CORE_STRING_NAMES_H

#include "core/string_name.h"

class CoreStringNames {

	friend void register_core_types();
	friend void unregister_core_types();

	static void create() { singleton = memnew(CoreStringNames); }
	static void free() {
		memdelete(singleton);
		singleton = NULL;
	}

	CoreStringNames();

public:
	_FORCE_INLINE_ static CoreStringNames *get_singleton() { return singleton; }

	static CoreStringNames *singleton;

	StringName _free;
	StringName changed;
	StringName _meta;
	StringName _script;
	StringName script_changed;
	StringName ___pdcdata;
	StringName __getvar;
	StringName _iter_init;
	StringName _iter_next;
	StringName _iter_get;
	StringName get_rid;
	StringName _to_string;
#ifdef TOOLS_ENABLED
	StringName _sections_unfolded;
#endif
	StringName _custom_features;

	StringName x;
	StringName y;
	StringName z;
	StringName w;
	StringName r;
	StringName g;
	StringName b;
	StringName a;
	StringName position;
	StringName size;
	StringName end;
	StringName basis;
	StringName origin;
	StringName normal;
	StringName d;
	StringName h;
	StringName s;
	StringName v;
	StringName r8;
	StringName g8;
	StringName b8;
	StringName a8;

	StringName call;
	StringName call_deferred;
	StringName emit;
	StringName notification;
};

#endif // CORE_STRING_NAMES_H

// This file registers and unregisters the JSON-RPC module types within the
// Godot engine. It includes the necessary headers and defines functions for
// integrating the JSONRPC class with the engine.
#include "register_types.h"
#include "core/class_db.h"
#include "jsonrpc.h"

void register_jsonrpc_types() { ClassDB::register_class<JSONRPC>(); }

void unregister_jsonrpc_types() {}

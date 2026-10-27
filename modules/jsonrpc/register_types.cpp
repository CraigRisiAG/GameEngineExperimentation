
#include "register_types.h"
#include "core/class_db.h"
#include "jsonrpc.h"

void register_jsonrpc_types() { ClassDB::register_class<JSONRPC>(); }

void unregister_jsonrpc_types() {}

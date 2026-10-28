
#include "register_types.h"
#include "core/class_db.h"
#include "regex.h"

void register_regex_types() {

  ClassDB::register_class<RegExMatch>();
  ClassDB::register_class<RegEx>();
}

void unregister_regex_types() {}

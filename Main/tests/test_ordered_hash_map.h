// This file contains the declaration of the TestOrderedHashMap namespace, which
// includes the test function for the OrderedHashMap component in the engine. It
// provides a way to run OrderedHashMap-related tests and obtain the main loop
// for execution.
#ifndef TEST_ORDERED_HASH_MAP_H
#define TEST_ORDERED_HASH_MAP_H

#include "core/os/main_loop.h"

namespace TestOrderedHashMap {

MainLoop *test();
}

#endif // TEST_ORDERED_HASH_MAP_H

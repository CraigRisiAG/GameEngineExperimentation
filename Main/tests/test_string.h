// This file contains the declaration of the TestString namespace, which
// includes the test function for the String component in the engine. It
// provides a way to run String-related tests and obtain the main loop for
// execution.
#ifndef TEST_STRING_H
#define TEST_STRING_H

#include "core/os/main_loop.h"
#include "core/ustring.h"

namespace TestString {

MainLoop *test();
}

#endif

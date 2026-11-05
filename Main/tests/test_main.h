// This file contains the declaration of the main test runner functions for
// various engine components. It provides the function to retrieve the list of
// available tests and to execute a specific test based on its name, returning
// the corresponding main loop for the test.
#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include "core/list.h"
#include "core/os/main_loop.h"
#include "core/ustring.h"

const char **tests_get_names();
MainLoop *test_main(String p_test, const List<String> &p_args);

#endif // TEST_MAIN_H

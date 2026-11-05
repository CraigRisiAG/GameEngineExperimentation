// This file contains the declaration of the TestRender namespace, which
// includes the test function for the Render component in the engine. It
// provides a way to run Render-related tests and obtain the main loop for
// execution.
#ifndef TEST_RENDER_H
#define TEST_RENDER_H

#include "core/os/main_loop.h"

namespace TestRender {

MainLoop *test();
}

#endif

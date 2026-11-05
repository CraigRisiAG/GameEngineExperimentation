// This file contains the declaration of the TestGUI namespace, which includes
// the test function for various GUI components such as buttons, labels, line
// edits, trees, scroll bars, spin boxes, progress bars, menu buttons, and
// option buttons. It provides a way to run GUI-related tests and obtain the
// main loop for execution.
#ifndef TEST_GUI_H
#define TEST_GUI_H

#include "core/os/main_loop.h"

namespace TestGUI {

MainLoop *test();
}

#endif

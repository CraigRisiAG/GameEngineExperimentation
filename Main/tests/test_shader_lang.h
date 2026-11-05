// This file contains the declaration of the TestShaderLang namespace, which
// includes the test function for the ShaderLanguage component in the engine. It
// provides a way to run ShaderLanguage-related tests and obtain the main loop
// for execution.
#ifndef TEST_SHADER_LANG_H
#define TEST_SHADER_LANG_H

#include "core/os/main_loop.h"

namespace TestShaderLang {

MainLoop *test();
}

#endif // TEST_SHADER_LANG_H

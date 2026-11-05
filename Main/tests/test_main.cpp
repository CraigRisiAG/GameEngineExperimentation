// This file contains the main test runner for various engine components,
// including string, math, physics, rendering, GUI, shader language, GDScript,
// and hash map tests. It provides functions to retrieve the list of available
// tests and to execute a specific test based on its name.
#include "test_main.h"

#include "core/list.h"

#ifdef DEBUG_ENABLED

#include "test_astar.h"
#include "test_gdscript.h"
#include "test_gui.h"
#include "test_math.h"
#include "test_oa_hash_map.h"
#include "test_ordered_hash_map.h"
#include "test_physics.h"
#include "test_physics_2d.h"
#include "test_render.h"
#include "test_shader_lang.h"
#include "test_string.h"

const char **tests_get_names() {

  static const char *test_names[] = {
      "string",           "math",        "physics",     "physics_2d",
      "render",           "oa_hash_map", "gui",         "shaderlang",
      "gd_tokenizer",     "gd_parser",   "gd_compiler", "gd_bytecode",
      "ordered_hash_map", "astar",       NULL};

  return test_names;
}

MainLoop *test_main(String p_test, const List<String> &p_args) {

  if (p_test == "string") {

    return TestString::test();
  }

  if (p_test == "math") {

    return TestMath::test();
  }

  if (p_test == "physics") {

    return TestPhysics::test();
  }

  if (p_test == "physics_2d") {

    return TestPhysics2D::test();
  }

  if (p_test == "render") {

    return TestRender::test();
  }

  if (p_test == "oa_hash_map") {

    return TestOAHashMap::test();
  }

#ifndef _3D_DISABLED
  if (p_test == "gui") {

    return TestGUI::test();
  }
#endif

  if (p_test == "shaderlang") {

    return TestShaderLang::test();
  }

  if (p_test == "gd_tokenizer") {

    return TestGDScript::test(TestGDScript::TEST_TOKENIZER);
  }

  if (p_test == "gd_parser") {

    return TestGDScript::test(TestGDScript::TEST_PARSER);
  }

  if (p_test == "gd_compiler") {

    return TestGDScript::test(TestGDScript::TEST_COMPILER);
  }

  if (p_test == "gd_bytecode") {

    return TestGDScript::test(TestGDScript::TEST_BYTECODE);
  }

  if (p_test == "ordered_hash_map") {

    return TestOrderedHashMap::test();
  }

  if (p_test == "astar") {

    return TestAStar::test();
  }

  print_line("Unknown test: " + p_test);
  return NULL;
}

#else

const char **tests_get_names() {

  static const char *test_names[] = {NULL};

  return test_names;
}

MainLoop *test_main(String p_test, const List<String> &p_args) { return NULL; }

#endif

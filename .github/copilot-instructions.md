# AI Coding Agent Instructions for GameEngineExperimentation

This is a **Godot Engine fork** focused on performance optimization via strategic caching and memory management. The project is a learning/experimentation codebase, not for production use.

## Architecture Overview

**Core Layers:**
- **Core** (`core/`) - Type system, memory management, data structures (Vector, HashMap, String)
- **Scene** (`scene/`) - Node hierarchy, SceneTree, 2D/3D node types
- **Servers** (`servers/`) - Physics, rendering, audio subsystems
- **Editor** (`editor/`) - Dependency management, file dialogs, resource inspection
- **Drivers** (`drivers/`) - Platform-specific rendering/input backends

**Key Pattern:** Engine uses Godot's GDCLASS macro for reflection/serialization. All classes inherit from Object and bind methods via `ClassDB::bind_method()`.

## Active Optimization Work

### ✅ Completed (Use These)
- **Vector Pre-allocation** (`core/vector_preallocation.h`, 313 lines): `VectorPool<T>` and `PreallocatedVector<T>` for eliminating allocations in hot loops. Expected +2-5 FPS.
- **Spatial Hash Optimization** (`servers/physics/spatial_hash_optimized.h`, 350+ lines): Grid-based spatial queries with cached vectors. Expected +5-8 FPS when combined with pre-allocation.

### 🔄 Priority Pipeline
1. **String Interning Cache** (Priority #3) - Deduplicate string allocations
2. **Transform Caching** (Priority #4) - Cache matrix calculations in scene hierarchy
3. **Render Batching** (Priority #6) - Group draw calls by material/mesh
4. **Lazy Node Counting** (Priority #1) - Optimize `Node::get_child_count()` traversal

**Overall Goal:** 25-30% cumulative FPS improvement through these 15-point roadmap.

## Build & Test Workflow

### Configure (CMake Modern approach)
```bash
# Debug: Windows
cmake -B build/debug -S . -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug

# Release: Linux
cmake -B build/release -S . -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/g++
```

### Compile & Test
```bash
# Build
cmake --build build/debug

# Run tests (GoogleTest framework)
./build/debug/bin/vector_preallocation_test
./build/debug/bin/spatial_hash_optimized_test
```

**VS Code Tasks Available:**
- `build-debug-windows` / `build-debug-linux`
- `build-release-windows` / `build-release-linux`
- `compile-current-file` (MSVC direct compile)
- `clean-build`

**CI Expectations:** Tests must pass. CMake must generate without warnings. Target C++17 compliance.

## Code Patterns & Conventions

### Memory Management
- **Vectors:** Use `PreallocatedVector<T>` in rendering/physics hot loops instead of `Vector<T>`. Example:
  ```cpp
  static PreallocatedVector<RenderObject> objects{5000};  // Reuse allocation
  objects.clear();
  for (Node *node : visible_nodes) {
      objects.push_back(node->get_render_object());
  }
  ```
- **Smart Pointers:** Use `Ref<T>` (reference-counted) for resources; `Object*` raw pointers for scene nodes (lifetime managed by parent nodes).

### Class Binding (Godot Reflection)
```cpp
class MyNode : public Node {
    GDCLASS(MyNode, Node);  // Register with reflection system
protected:
    static void _bind_methods() {
        ClassDB::bind_method(D_METHOD("my_method", "arg"), &MyNode::my_method);
        ADD_PROPERTY(PropertyInfo(Variant::INT, "value"), "set_value", "get_value");
    }
public:
    void my_method(int arg) { /* ... */ }
};
```

### Optimization Markers
- Look for `// TODO: Optimize` comments indicating performance hotspots.
- Profile with spatial hash grid tests before/after changes to measure impact.
- Prefer pre-allocation → lazy evaluation → caching as optimization order.

### Platform-Specific Code
Use CMake generator expressions (not `#ifdef`):
```cpp
// ✅ In CMakeLists.txt
add_compile_definitions(
    $<$<PLATFORM_ID:Windows>:WINDOWS_ENABLED>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
// ✅ In code
#ifdef WINDOWS_ENABLED
    // Windows-only logic
#endif
```

## File Organization Rules

- **Headers only** for templates/performance-critical code (`*.h`).
- **Implementation** in `.cpp` when not performance-critical.
- **Tests** in `tests/` with `_test.cpp` suffix (GoogleTest framework).
- **Documentation:** Link in [DOCUMENTATION_INDEX.md](../DOCUMENTATION_INDEX.md).

## Key Files by Use Case

| Need | File(s) |
|------|---------|
| Vector pre-allocation API | [core/vector_preallocation.h](../core/vector_preallocation.h) |
| Spatial hash physics | [servers/physics/spatial_hash_optimized.h](../servers/physics/spatial_hash_optimized.h) |
| Scene node base | [scene/main/node.h](../scene/main/node.h) |
| Physics server interface | [servers/physics_server_3d.h](../servers/physics_server_3d.h) |
| Integration guides | [VECTOR_PREALLOCATION_IMPLEMENTATION.md](../VECTOR_PREALLOCATION_IMPLEMENTATION.md), [SPATIAL_HASH_VECTOR_OPTIMIZATION.md](../SPATIAL_HASH_VECTOR_OPTIMIZATION.md) |
| Performance roadmap | [PROJECT_STATUS.md](../PROJECT_STATUS.md) |

## Documentation Expectations

- **Doxygen-style comments** for all classes/functions (used in headers).
- **Markdown docs** for integration guides (5+ phases, before/after analysis).
- **Benchmarks** with performance deltas (e.g., "spatial hash: 50-85% improvement on lookups").
- **Code examples** in QUICK_REFERENCE.md for copy-paste integration.

## Common Pitfalls to Avoid

1. **Allocations in hot loops:** Use pre-allocated vectors; avoid creating temporary `Vector<T>` during render/physics updates.
2. **String copies:** Watch for `String` copies in loop conditions; prefer StringName for frequent comparisons.
3. **Scene tree traversal:** Cache results when doing full traversals (`get_all_nodes()` patterns).
4. **GDCLASS binding:** Always call `_bind_methods()` for reflection; forgetting breaks script exposure.
5. **CMake globals:** Don't use global `set(CMAKE_CXX_FLAGS ...)`. Use generator expressions for target-specific flags.

## Integration Testing

When adding optimizations:
1. **Write unit tests** in `tests/` with benchmarks (see `vector_preallocation_test.cpp` for pattern).
2. **Measure FPS delta** before/after in realistic scene (100+ objects minimum).
3. **Verify no regressions** - run full test suite: `ctest --output-on-failure`.
4. **Document expected gains** - add to [PROJECT_STATUS.md](../PROJECT_STATUS.md) with impact % and FPS delta.

## Troubleshooting Guide

### Build Issues

**CMake "command not found"**
- Ensure CMake 3.16+ is installed: `cmake --version`
- On Linux, add to PATH: `export PATH="/usr/local/bin:$PATH"`
- Windows: Use Visual Studio 17 2022 generator (MSVC 17+)

**Compiler errors with C++17 features**
- Verify `target_compile_features(... cxx_std_17)` in CMakeLists.txt
- MSVC: Add `/std:c++17` via `add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/std:c++17>)`
- Clang/GCC: Ensure `-std=c++17` is set

**Linking errors for tests (undefined references)**
- Ensure test target links GoogleTest: `target_link_libraries(my_test gtest gtest_main)`
- Check that library dependencies are in `target_link_libraries()` before test targets

### Vector Pre-allocation Issues

**Crashes with "capacity exceeded"**
- Capacity too small for peak usage: increase by 20-30% safety margin
- Example: If peak is 5000 objects, use `PreallocatedVector<T>(6500)`
- Profile actual peak: add `std::cout << "peak: " << objects.size()` in production scene

**Memory usage increased significantly**
- Capacity tuning too aggressive: reduce capacity to match actual peak usage
- Reduce growth factor: `vec.set_growth_factor(1.25f)` instead of 1.5-2.0
- Only pre-allocate in actual hot loops, not everywhere

**Performance no better or worse**
- Verify pre-allocation is in hot loop (called 100+ times/frame)
- Check that old allocations are actually eliminated: look for `new`, `malloc`, Vector constructor calls
- Measure with profiler, not just FPS (allocations might not dominate frame time)

### Physics & Spatial Hash Problems

**Query results incomplete or wrong**
- Verify AABB bounds are correct: `AABB box(position, size)` (size is positive)
- Check that bodies are actually in grid: call `grid.insert()` before queries
- Spatial hash cell size 32: verify objects fit in grid (not at boundaries)

**Query performance slow despite optimization**
- Cache might be undersized: increase `PreallocatedVector<GridKey>` capacity if seeing resizes
- Check cell size: 32 is good for 4-8 unit objects; adjust if objects are very large/small
- Verify `query_cells_cache.clear()` is called each frame (keeps capacity, resets size)

**Physics stepping takes too long**
- Profile time spent in `query_aabb()` vs body updates
- If queries are slow, increase cell size (fewer cells per query) or reduce max bodies per cell
- Ensure `visited_bodies` Set isn't growing unbounded (should reset each query)

### Integration & Runtime Issues

**GDCLASS binding breaks script exposure**
- Verify `_bind_methods()` is `protected static` and called in class constructor
- Check `D_METHOD()` string matches actual C++ method name exactly (case-sensitive)
- Ensure method signature matches: `D_METHOD("add_child", "child")` → `void add_child(Node *p_child)`

**Memory leaks after optimization integration**
- Check that `PreallocatedVector<T>::clear()` is called, not vector reallocation
- Verify no old vector allocations still exist in code (search for `Vector<T>()` constructors)
- Use valgrind: `valgrind ./build/debug/bin/my_test 2>&1 | grep "definitely lost"`

**Platform-specific crashes**
- Windows (MSVC): Check `/permissive-` flag compatibility with code
- Linux (GCC): Ensure `-fPIC` for shared libraries
- macOS (Clang): Verify `-fPIC` and check for ARM64 vs Intel architecture mismatch

### Testing & Validation

**Tests fail but code looks correct**
- Verify test framework is linked: ensure `#include <gtest/gtest.h>` works
- Check that test binaries are in `build/debug/bin/` (not in source tree)
- Run with verbose output: `./vector_preallocation_test --gtest_verbose`

**Benchmark results show no improvement**
- Other code might dominate (not the optimized path): profile CPU time
- Optimize only the actual bottleneck: use `perf` on Linux or VS profiler on Windows
- Small collections won't benefit from pre-allocation: need 100+ objects in loop

**Regression test failures after changes**
- Compare expected vs actual output: `gtest` shows diff in assertion
- Check that old tests still pass: `ctest --output-on-failure` shows which test broke
- Ensure no unintended changes to Vector/AABB behavior: test simple cases first

### Performance Measurement

**FPS measurement unreliable**
- Disable V-Sync in renderer for accurate FPS
- Run 300+ frames before measuring (let caches warm up)
- Use average of 5 runs, not single measurement
- Ensure no background CPU load (close browser, IDE, etc.)

**Profiler shows wrong hotspots**
- Profiler overhead: use `--no-inlining` for accurate call stacks
- Sample rate too low: increase to 10000 Hz+ for short functions
- Function inlining hides call stacks: measure release builds where inlining happens

---

**Last Updated:** December 2025 | **Project Status:** 50% optimization roadmap complete

- **C++ Standard:** C++17 (CMake enforced via `target_compile_features`)
- **Build Tool:** CMake 3.16+ (modern target-based syntax required)
- **Platforms:** Windows (MSVC), Linux (GCC), macOS (Clang) via generator expressions
- **Test Framework:** GoogleTest
- **Reflection System:** GDCLASS/ClassDB (Godot custom; not std::reflection)

---

**Last Updated:** December 2025 | **Project Status:** 50% optimization roadmap complete

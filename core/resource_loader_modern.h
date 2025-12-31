/**
 * @file core/resource_loader_modern.h
 * @brief Modern resource loading with all 5 quick wins
 *
 * Demonstrates:
 * - Optional returns (instead of null)
 * - Smart pointers (clear ownership)
 * - Move semantics (efficient transfers)
 * - Structured bindings (clean unpacking)
 * - Type-safe error handling
 */

#ifndef RESOURCE_LOADER_MODERN_H
#define RESOURCE_LOADER_MODERN_H

#include "core/modern_cpp.h"
#include "core/resource.h"
#include "core/ustring.h"
#include "core/vector.h"
#include <chrono>

// ═══════════════════════════════════════════════════════════════════════════
// Loading Error Types (for variant-based error handling)
// ═══════════════════════════════════════════════════════════════════════════

struct LoadSuccess {
  ResourcePtr<Resource> resource;
  std::chrono::milliseconds load_time;
};

struct LoadError {
  String error_message;
  String path;
};

// Result type: either success or error
using LoadResult = std::variant<LoadSuccess, LoadError>;

// ═══════════════════════════════════════════════════════════════════════════
// Modern Resource Loader Implementation
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Modern resource loading system showcasing all quick wins
 */
class ResourceLoaderModern {
public:
  /**
   * Load resource with type-safe error handling (QUICK WIN #2 + #5)
   *
   * BEFORE (unsafe):
   *   Resource* load(path) { return nullptr; }  // Crash if null!
   *
   * AFTER (safe):
   *   Optional<ResourcePtr<Resource>> load(path);
   *
   * Usage:
   *   if (auto res = loader.load("model.mesh")) {
   *       use_resource(*res);
   *   } else {
   *       warn_print("Failed to load");
   *   }
   */
  Optional<ResourcePtr<Resource>> load_resource_optional(const String &path) {

    // QUICK WIN #1: Create with smart pointer
    auto resource = make_shared<Resource>();

    // Simulate loading...
    if (path.empty()) {
      return None; // No value
    }

    // QUICK WIN #3: Move semantics
    return std::move(resource); // Efficient transfer
  }

  /**
   * Load with detailed error information (QUICK WIN #5)
   *
   * Uses variant to return either success or error details
   *
   * Usage:
   *   auto result = loader.load_with_details("model.mesh");
   *   std::visit(overload{
   *       [](const LoadSuccess& s) {
   *           print("Loaded in " + s.load_time.count() + "ms");
   *       },
   *       [](const LoadError& e) {
   *           warn_print("Error: " + e.error_message);
   *       }
   *   }, result);
   */
  LoadResult load_with_details(const String &path) {
    auto start = std::chrono::high_resolution_clock::now();

    // QUICK WIN #1: Smart pointer creation
    auto resource = make_shared<Resource>();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    if (path.empty()) {
      return LoadError{"Empty path provided", path};
    }

    return LoadSuccess{resource, duration};
  }

  /**
   * Batch load resources (demonstrates move semantics + optional)
   *
   * QUICK WIN #3: Returns move-optimized vector
   * Each resource is moved, not copied
   */
  Vector<Optional<ResourcePtr<Resource>>>
  load_batch(const Vector<String> &paths) {

    Vector<Optional<ResourcePtr<Resource>>> results;
    results.reserve(paths.size()); // Pre-allocate

    for (const auto &path : paths) {
      // QUICK WIN #3: Move semantics for efficient result transfer
      results.push_back(load_resource_optional(path));
    }

    return results; // Moved, not copied!
  }

  /**
   * Load with structured binding support (QUICK WIN #4)
   *
   * Returns tuple for clean unpacking with structured bindings
   *
   * Usage:
   *   auto [resource, load_time, success] =
   *       loader.load_with_timing("mesh.res");
   *
   *   if (success) {
   *       use_resource(*resource);
   *   }
   */
  std::tuple<Optional<ResourcePtr<Resource>>, int64_t, bool>
  load_with_timing(const String &path) {

    auto start = std::chrono::high_resolution_clock::now();

    // Load resource...
    auto resource = load_resource_optional(path);

    auto end = std::chrono::high_resolution_clock::now();
    int64_t time_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    bool success = resource.has_value();

    // Return as tuple for structured bindings
    return std::make_tuple(resource, time_ms, success);
  }
};

// ═══════════════════════════════════════════════════════════════════════════
// Advanced: Resource Cache with Modern C++
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Thread-safe resource cache using smart pointers
 */
template <typename T> class ModernResourceCache {
private:
  // QUICK WIN #1: Smart pointers for stored resources
  std::unordered_map<String, SharedPtr<T>> cache;
  std::mutex cache_mutex;

public:
  /**
   * Get or create resource (QUICK WIN #1 + #2)
   * Returns Optional for missing resources
   */
  Optional<SharedPtr<T>> get_or_create(const String &key,
                                       std::function<SharedPtr<T>()> factory) {

    std::lock_guard<std::mutex> lock(cache_mutex);

    auto it = cache.find(key);
    if (it != cache.end()) {
      return it->second;
    }

    // QUICK WIN #1: Create with smart pointer
    auto resource = factory();
    if (!resource) {
      return None;
    }

    cache[key] = resource;
    return resource;
  }

  /**
   * Get cached resource without creating (returns Optional)
   */
  Optional<SharedPtr<T>> get(const String &key) const {
    std::lock_guard<std::mutex> lock(cache_mutex);

    auto it = cache.find(key);
    if (it != cache.end()) {
      return it->second;
    }
    return None;
  }

  /**
   * Clear cache with RAII scope guard pattern
   * Ensures cleanup happens even if exception thrown
   */
  void clear_with_guard(std::function<void()> on_clear = nullptr) {
    // QUICK WIN: Bonus RAII pattern
    auto guard = make_scope_guard([this, on_clear]() {
      cache.clear();
      if (on_clear)
        on_clear();
    });

    // Guard automatically executes cleanup on scope exit
  }
};

// ═══════════════════════════════════════════════════════════════════════════
// Complete Example: All 5 Quick Wins Together
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Complete resource loading pipeline showing all quick wins
 */
class CompleteResourcePipelineModern {
private:
  ResourceLoaderModern loader;
  ModernResourceCache<Resource> cache;

public:
  /**
   * Complete workflow example
   *
   * Demonstrates all 5 quick wins:
   * 1. Smart pointers (SharedPtr<Resource>)
   * 2. Optional returns (Optional<ResourcePtr>)
   * 3. Move semantics (efficient batch loading)
   * 4. Structured bindings (auto [res, time, ok] = ...)
   * 5. Type-safe variant (LoadResult with overload visitor)
   */
  void complete_workflow(const Vector<String> &resource_paths) {

    // QUICK WIN #1 + #3: Load batch with smart pointers and moves
    auto results = loader.load_batch(resource_paths);

    // QUICK WIN #4: Structured bindings for clean access
    for (auto &result : results) {
      if (result) {
        auto resource = *result;
        process_resource(resource);
      }
    }

    // QUICK WIN #4: Structured bindings with tuple
    auto [res, time, success] = loader.load_with_timing("main_mesh");
    if (success) {
      print("Loaded in " + String::num(time) + "ms");
    }

    // QUICK WIN #5: Type-safe variant handling
    auto detailed_result = loader.load_with_details("model.mesh");
    std::visit(overload{[](const LoadSuccess &s) {
                          print("Loaded successfully");
                          // Use s.resource here (type-safe!)
                        },
                        [](const LoadError &e) {
                          warn_print("Failed: " + e.error_message);
                        }},
               detailed_result);

    // QUICK WIN #1 + #2: Optional-based caching
    if (auto cached = cache.get("important_resource")) {
      use_resource(*cached);
    } else {
      // Load and cache
      if (auto loaded = loader.load_resource_optional("path")) {
        use_resource(*loaded);
      }
    }
  }

private:
  void process_resource(const SharedPtr<Resource> &resource) {
    // Process...
  }

  void use_resource(const SharedPtr<Resource> &resource) {
    // Use...
  }
};

#endif // RESOURCE_LOADER_MODERN_H

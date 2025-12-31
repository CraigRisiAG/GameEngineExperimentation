/**
 * @file core/modern_cpp.h
 * @brief Modern C++ Type Aliases and Utilities
 *
 * Quick wins implementation:
 * 1. Smart pointers (unique_ptr, shared_ptr)
 * 2. Optional types
 * 3. Move semantics helpers
 * 4. Structured bindings support
 * 5. Variant visitor utilities
 *
 * C++17 required
 */

#ifndef MODERN_CPP_H
#define MODERN_CPP_H

#include <memory>
#include <optional>
#include <utility>
#include <variant>

// ═══════════════════════════════════════════════════════════════════════════
// QUICK WIN #1: Smart Pointer Type Aliases
//
// Usage: Instead of raw pointers, use these type aliases for clear ownership
// Examples:
//   BodyPtr body = std::make_unique<Body>();      // Unique ownership
//   ResourcePtr resource = std::make_shared<Resource>();  // Shared
// ═══════════════════════════════════════════════════════════════════════════

// Physics bodies - unique ownership (owned by physics server)
template <typename T> using UniquePtr = std::unique_ptr<T>;

template <typename T>
using BodyPtr = std::unique_ptr<T>; // Physics body pointer (exclusive)

// Shared resources - reference counted
template <typename T> using SharedPtr = std::shared_ptr<T>;

template <typename T>
using ResourcePtr = std::shared_ptr<T>; // Resource pointer (shared)

template <typename T>
using ObserverPtr = std::shared_ptr<T>; // Observer/listener (shared)

// Helper to create unique pointers (zero-overhead)
template <typename T, typename... Args>
inline UniquePtr<T> make_unique(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

// Helper to create shared pointers
template <typename T, typename... Args>
inline SharedPtr<T> make_shared(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

// ═══════════════════════════════════════════════════════════════════════════
// QUICK WIN #2: Optional Type Aliases
//
// Usage: For optional values (null-safe alternatives to raw pointers)
// Examples:
//   std::optional<Resource> load_optional(path);
//   if (auto res = load_optional("path")) { /* use *res */ }
// ═══════════════════════════════════════════════════════════════════════════

// Optional value - use instead of returning nullptr
template <typename T> using Optional = std::optional<T>;

// No value sentinel
constexpr auto None = std::nullopt;

// Helper to wrap reference in optional (return by reference)
template <typename T>
using OptionalRef = std::optional<std::reference_wrapper<T>>;

/**
 * Convert pointer to optional (nullptr -> std::nullopt)
 * Usage: optional_from_ptr(object_ptr)
 */
template <typename T> inline Optional<T *> optional_from_ptr(T *ptr) {
  if (ptr)
    return ptr;
  return std::nullopt;
}

// ═══════════════════════════════════════════════════════════════════════════
// QUICK WIN #3: Move Semantics Helpers
//
// Usage: For efficient transfer of ownership without copies
// Examples:
//   Vector<Item> items = std::move(acquire_items());
//   auto obj = std::move(source_obj);  // Efficient move
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Perfect forwarding helper - use in factory functions
 * Usage: return make_instance<MyClass>(std::forward<Args>(args)...);
 */
template <typename T, typename... Args> inline T make_instance(Args &&...args) {
  return T(std::forward<Args>(args)...);
}

/**
 * Move-aware container swap
 * Usage: fast_swap(vector_a, vector_b);
 */
template <typename T> inline void fast_swap(T &a, T &b) noexcept {
  a.swap(b); // Most containers have efficient swap
}

// ═══════════════════════════════════════════════════════════════════════════
// QUICK WIN #4: Structured Binding Support Types
//
// Usage: For clean tuple unpacking
// Examples:
//   auto [width, height] = get_viewport_size();
//   for (auto [id, obj] : objects_map) { /* use id, obj */ }
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Pair-like return type for structured bindings
 * Example: auto [x, y] = point_result();
 */
template <typename A, typename B> struct Pair {
  A first;
  B second;

  // Support structured bindings
  template <std::size_t I> auto &get() {
    if constexpr (I == 0)
      return first;
    else if constexpr (I == 1)
      return second;
  }
};

// Helper to create Pair for structured bindings
template <typename A, typename B> inline Pair<A, B> make_pair(A a, B b) {
  return {std::move(a), std::move(b)};
}

// ═══════════════════════════════════════════════════════════════════════════
// QUICK WIN #5: Type-Safe Variant Visitor
//
// Usage: For safe std::variant type dispatching
// Examples:
//   std::visit(visitor, variant_value);
//   std::visit(overload{
//       [](int i) { /* ... */ },
//       [](float f) { /* ... */ },
//       [](auto& x) { /* fallback */ }
//   }, variant_value);
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Overload pattern for std::visit
 * Allows multiple lambda handlers to be combined into single visitor
 *
 * Usage:
 *   std::visit(overload{
 *       [](int i) { std::cout << "int: " << i; },
 *       [](float f) { std::cout << "float: " << f; },
 *       [](const std::string& s) { std::cout << "string: " << s; }
 *   }, variant);
 */
template <class... Ts> struct overload : Ts... {
  using Ts::operator()...; // C++17 deduction guides
};

// Explicit deduction guide (for older compilers)
template <class... Ts> overload(Ts...) -> overload<Ts...>;

/**
 * Generic visitor for unhandled types
 * Use with overload for fallback handling
 *
 * Usage:
 *   std::visit(overload{
 *       [](int i) { /* specific */ }
, *[](auto &x) { generic_handler(x); } // Fallback
       *
}, variant);
*/ template <typename T> struct GenericVisitor {
  void operator()(T &value) const {
    // Override in subclass
  }
};

// ═══════════════════════════════════════════════════════════════════════════
// Scope Guard - RAII for cleanup (bonus: helps with move semantics)
// ═══════════════════════════════════════════════════════════════════════════

/**
 * RAII scope guard - ensures cleanup function runs on scope exit
 * Usage:
 *   {
 *       auto guard = make_scope_guard([]() { cleanup(); });
 *       // ... do work ...
 *   }  // cleanup() called automatically
 */
template <typename F> class ScopeGuard {
  F cleanup_fn;
  bool active = true;

public:
  explicit ScopeGuard(F fn) : cleanup_fn(std::move(fn)) {}

  ~ScopeGuard() {
    if (active)
      cleanup_fn();
  }

  // Move constructor (transfer guard to new scope)
  ScopeGuard(ScopeGuard &&other) noexcept
      : cleanup_fn(std::move(other.cleanup_fn)), active(other.active) {
    other.active = false; // Disable original
  }

  // Disable copying
  ScopeGuard(const ScopeGuard &) = delete;
  ScopeGuard &operator=(const ScopeGuard &) = delete;

  // Dismiss the guard (cleanup won't run)
  void dismiss() { active = false; }
};

template <typename F> inline ScopeGuard<F> make_scope_guard(F fn) {
  return ScopeGuard<F>(std::move(fn));
}

#endif // MODERN_CPP_H

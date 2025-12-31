/**
 * @file servers/physics_server_modern.h
 * @brief Physics Server example using modern C++ (all 5 quick wins)
 *
 * This file demonstrates how to use the modern_cpp.h utilities:
 * 1. Smart pointers (BodyPtr, ResourcePtr)
 * 2. Optional types (for nullable returns)
 * 3. Move semantics (efficient transfers)
 * 4. Structured bindings (clean unpacking)
 * 5. Type-safe variant visitors (std::visit with overload)
 */

#ifndef PHYSICS_SERVER_MODERN_H
#define PHYSICS_SERVER_MODERN_H

#include "core/math/transform.h"
#include "core/math/vector3.h"
#include "core/modern_cpp.h"
#include "core/object.h"
#include "core/resource.h"
#include "core/vector.h"
#include <variant>

// ═══════════════════════════════════════════════════════════════════════════
// EXAMPLE 1: Smart Pointers - Replace Raw Pointers with Ownership Semantics
// ═══════════════════════════════════════════════════════════════════════════

class Body; // Forward declaration

/**
 * Physics Body Container - uses smart pointers (QUICK WIN #1)
 *
 * BEFORE:
 *   Vector<Body*> bodies;  // Raw pointers - who owns them?
 *
 * AFTER:
 *   Vector<BodyPtr<Body>> bodies;  // Clear: unique ownership
 */
class PhysicsSpaceModern {
public:
  // QUICK WIN #1: Use BodyPtr instead of raw pointer
  Vector<BodyPtr<Body>> bodies;

  /**
   * Create a new physics body with smart pointer
   * Returns unique ownership
   */
  BodyPtr<Body> create_body() {
    auto body = make_unique<Body>();
    bodies.push_back(body);
    return body;
  }

  // No manual cleanup needed! Smart pointer deletes automatically
};

// ═══════════════════════════════════════════════════════════════════════════
// EXAMPLE 2: Optional Types - Replace Nullable Pointers
// ═══════════════════════════════════════════════════════════════════════════

class Resource;
class ResourceLoader;

/**
 * Resource Loading - uses Optional (QUICK WIN #2)
 *
 * BEFORE:
 *   Resource* load_resource(path) { return nullptr; }  // Unclear if optional!
 *
 * AFTER:
 *   Optional<Resource> load_resource(path) { return None; }  // Crystal clear!
 */
class ResourceLoaderModern {
public:
  /**
   * Load resource - returns Optional (no null dereference possible)
   *
   * Usage:
   *   if (auto res = loader.load_resource("path")) {
   *       // res now contains the resource - fully type safe!
   *   } else {
   *       warn_print("Failed to load");
   *   }
   */
  Optional<ResourcePtr<Resource>> load_resource(const String &path) {
    // Try to load...
    if (true) { // Simulate success check
      return make_shared<Resource>();
    }
    return None; // Explicit "no value"
  }

  /**
   * Alternative: return reference if found
   * Useful when object lifetime is guaranteed
   */
  OptionalRef<Resource> find_cached_resource(const String &path) {
    // static cache...
    // if found: return std::ref(cached_resource);
    return None;
  }
};

// ═══════════════════════════════════════════════════════════════════════════
// EXAMPLE 3: Move Semantics - Efficient Value Transfer
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Query Results - uses move semantics efficiently (QUICK WIN #3)
 *
 * BEFORE:
 *   Vector<Body*> query_aabb(...) {
 *       Vector<Body*> results;
 *       // ... populate ...
 *       return results;  // Might copy!
 *   }
 *
 * AFTER:
 *   Vector<Body*> query_aabb(...) {
 *       Vector<Body*> results;
 *       // ... populate ...
 *       return results;  // C++17 guaranteed move! (or RVO)
 *   }
 *
 * With move semantics:
 *   auto results = space.query_aabb(box);  // NO COPY - moved efficiently
 */
class PhysicsQueryModern {
private:
  Vector<BodyPtr<Body>> all_bodies;

public:
  /**
   * Query with automatic move semantics
   * Return vector is moved, not copied (C++17 RVO + move)
   */
  Vector<BodyPtr<Body>> query_aabb_move(const AABB &aabb) {
    Vector<BodyPtr<Body>> results;

    for (auto &body : all_bodies) {
      if (body && aabb.intersects(body->get_aabb())) {
        results.push_back(body); // Efficient ownership sharing
      }
    }

    return results; // Moved, not copied!
  }

  /**
   * Perfect forwarding for generic processing
   * Forwards arguments without unnecessary copies
   */
  template <typename... Args> void process_bodies(Args &&...args) {
    // Forward all arguments without copying
    // (Example: pass to another function)
  }
};

// ═══════════════════════════════════════════════════════════════════════════
// EXAMPLE 4: Structured Bindings - Clean Tuple Unpacking
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Shape Query Results - uses structured bindings (QUICK WIN #4)
 *
 * BEFORE:
 *   auto result = query_shape(params);
 *   Shape* shape = result.first;
 *   Vector3 position = result.second;
 *
 * AFTER:
 *   auto [shape, position] = query_shape(params);  // Much cleaner!
 */
class PhysicsShapeQueryModern {
public:
  /**
   * Query shape - returns structured binding friendly result
   */
  struct QueryResult {
    RID shape_rid;
    Vector3 contact_point;
    Vector3 normal;
    float distance;

    // Support structured bindings (C++17)
    std::tuple<const RID &, const Vector3 &, const Vector3 &, float>
    as_tuple() const {
      return std::tie(shape_rid, contact_point, normal, distance);
    }
  };

  /**
   * Usage with structured bindings:
   *   auto [rid, point, normal, dist] = query_shape(params);
   */
  QueryResult query_shape(const Vector3 &position) {
    return {RID(), Vector3(0, 0, 0), Vector3(0, 1, 0), 0.0f};
  }
};

// ═══════════════════════════════════════════════════════════════════════════
// EXAMPLE 5: Type-Safe Variant Visitor
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Physics Event Handler - uses std::variant with visitor (QUICK WIN #5)
 *
 * BEFORE (unsafe union pattern):
 *   union PhysicsEvent {
 *       int collision_type;
 *       float impact_force;
 *       Vector3* contact_point;  // Unsafe! No type info!
 *   };
 *
 * AFTER (type-safe variant):
 *   using PhysicsEvent = std::variant<
 *       CollisionEvent,
 *       ImpactEvent,
 *       ContactEvent
 *   >;
 *
 *   std::visit(overload{
 *       [](const CollisionEvent& e) { handle_collision(e); },
 *       [](const ImpactEvent& e) { handle_impact(e); },
 *       [](const ContactEvent& e) { handle_contact(e); },
 *   }, event);  // Type-safe dispatch!
 */

// Event types
struct CollisionEvent {
  RID body_a;
  RID body_b;
  Vector3 contact_point;
};

struct ImpactEvent {
  RID body;
  float impact_force;
  Vector3 direction;
};

struct ContactEvent {
  RID body_a;
  RID body_b;
  float contact_pressure;
};

// Type-safe variant event
using PhysicsEvent = std::variant<CollisionEvent, ImpactEvent, ContactEvent>;

/**
 * Physics Event Dispatcher - handles events type-safely
 */
class PhysicsEventDispatcherModern {
public:
  /**
   * Dispatch event with type-safe visitor pattern
   * Compiler forces handling all types!
   *
   * Usage:
   *   PhysicsEvent event = CollisionEvent{ ... };
   *   dispatcher.dispatch(event);  // Type-safe handling!
   */
  void dispatch(const PhysicsEvent &event) {
    // QUICK WIN #5: overload pattern for type-safe dispatch
    std::visit(overload{[this](const CollisionEvent &e) {
                          handle_collision(e.body_a, e.body_b, e.contact_point);
                        },
                        [this](const ImpactEvent &e) {
                          handle_impact(e.body, e.impact_force, e.direction);
                        },
                        [this](const ContactEvent &e) {
                          handle_contact(e.body_a, e.body_b,
                                         e.contact_pressure);
                        }},
               event);
    // Compiler error if you forget a case!
  }

private:
  void handle_collision(RID a, RID b, const Vector3 &point) {
    // Handle collision...
  }

  void handle_impact(RID body, float force, const Vector3 &dir) {
    // Handle impact...
  }

  void handle_contact(RID a, RID b, float pressure) {
    // Handle contact...
  }
};

// ═══════════════════════════════════════════════════════════════════════════
// BONUS: Complete Example Using All 5 Quick Wins Together
// ═══════════════════════════════════════════════════════════════════════════

/**
 * Complete physics system example showcasing all 5 quick wins
 */
class CompletePhysicsSystemModern {
private:
  // QUICK WIN #1: Smart pointers for bodies
  Vector<BodyPtr<Body>> bodies;

  // QUICK WIN #2: Optional for query results
  struct QueryResults {
    Optional<Vector<BodyPtr<Body>>> found_bodies;
    Optional<Vector<Vector3>> contact_points;
  };

public:
  /**
   * Complete workflow showing all 5 quick wins:
   *
   * 1. Smart pointer: create_body() -> BodyPtr<Body>
   * 2. Optional: load_shape() -> Optional<Resource>
   * 3. Move semantics: return results efficiently
   * 4. Structured bindings: auto [x, y, z] = position;
   * 5. Variant visitor: dispatch_event(event);
   */
  void complete_workflow() {
    // QUICK WIN #1: Smart pointer creation
    auto new_body = make_unique<Body>();

    // QUICK WIN #2: Optional type for optional values
    if (auto physics_shape = load_physics_shape("sphere")) {
      // Shape loaded successfully - type-safe access
      use_shape(*physics_shape);
    }

    // QUICK WIN #3 + 4: Move semantics + structured bindings
    auto results = query_bodies_in_region(Vector3(0, 0, 0), 10.0f);

    if (results) {
      auto &bodies = *results; // Extract optional value
                               // Process bodies...
    }

    // QUICK WIN #5: Type-safe event handling
    auto event = CollisionEvent{RID(), RID(), Vector3(0, 0, 0)};
    dispatch_event_safe(event);
  }

private:
  Optional<ResourcePtr<Resource>> load_physics_shape(const String &name) {
    // Load or return None
    return None;
  }

  void use_shape(const Resource &shape) {
    // Use shape...
  }

  Optional<Vector<BodyPtr<Body>>> query_bodies_in_region(const Vector3 &center,
                                                         float radius) {
    // Query bodies...
    return None;
  }

  void dispatch_event_safe(const PhysicsEvent &event) {
    std::visit(overload{[](const CollisionEvent &) { /* ... */ },
                        [](const ImpactEvent &) { /* ... */ },
                        [](const ContactEvent &) { /* ... */ }},
               event);
  }
};

#endif // PHYSICS_SERVER_MODERN_H

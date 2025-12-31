/**
 * @file spatial_hash_test.cpp
 * @brief Unit tests for spatial hash physics optimization
 *
 * Tests the spatial hash grid implementation and validates:
 * - Correct cell assignment for objects
 * - Query accuracy
 * - Performance improvements
 * - Edge cases and boundary conditions
 */

#include "core/math/aabb.h"
#include "core/math/vector3.h"
#include "servers/physics/spatial_hash.h"
#include "servers/physics/spatial_hash_physics.h"
#include <chrono>
#include <gtest/gtest.h>

class SpatialHashTest : public ::testing::Test {
protected:
  SpatialHashGrid hash_grid;
  PhysicsSpaceOptimizer space_optimizer;

  void SetUp() override {
    // Default 32 unit cell size
  }

  // Helper to create simple AABBs
  AABB make_aabb(float x, float y, float z, float size = 4.0f) {
    return AABB(Vector3(x, y, z), Vector3(size, size, size));
  }

  // Helper to generate test body RID
  RID make_test_rid(int index) { return RID::from_uint64((uint64_t)index); }
};

/**
 * @test Basic insertion and single-cell query
 */
TEST_F(SpatialHashTest, BasicInsertionAndQuery) {
  RID body1 = make_test_rid(1);
  AABB box1 = make_aabb(0, 0, 0);

  // Insert body
  hash_grid.insert(body1, box1, false);

  // Query containing area
  Vector<RID> results;
  hash_grid.query_aabb(box1, Set<RID>(), &results);

  EXPECT_EQ(results.size(), 1);
  EXPECT_EQ(results[0], body1);
}

/**
 * @test Multi-cell body spans multiple grid cells
 */
TEST_F(SpatialHashTest, MultiCellBody) {
  RID large_body = make_test_rid(1);
  // 64x64x64 box spans 2x2x2 = 8 cells (with 32-unit cell size)
  AABB large_box(Vector3(0, 0, 0), Vector3(64, 64, 64));

  hash_grid.insert(large_body, large_box, false);

  // Query should find body from multiple angles
  Vector<RID> results1;
  hash_grid.query_aabb(make_aabb(10, 10, 10), Set<RID>(), &results1);
  EXPECT_EQ(results1.size(), 1);

  Vector<RID> results2;
  hash_grid.query_aabb(make_aabb(50, 50, 50), Set<RID>(), &results2);
  EXPECT_EQ(results2.size(), 1);
}

/**
 * @test Multiple non-overlapping bodies
 */
TEST_F(SpatialHashTest, MultipleNonOverlappingBodies) {
  RID body1 = make_test_rid(1);
  RID body2 = make_test_rid(2);
  RID body3 = make_test_rid(3);

  hash_grid.insert(body1, make_aabb(0, 0, 0), false);
  hash_grid.insert(body2, make_aabb(50, 0, 0), false); // Different X cell
  hash_grid.insert(body3, make_aabb(0, 50, 0), false); // Different Y cell

  // Query should only find nearby bodies
  Vector<RID> results;
  hash_grid.query_aabb(make_aabb(0, 0, 0), Set<RID>(), &results);

  EXPECT_EQ(results.size(), 1);
  EXPECT_EQ(results[0], body1);
}

/**
 * @test Overlapping query finds multiple bodies
 */
TEST_F(SpatialHashTest, OverlappingQuery) {
  RID body1 = make_test_rid(1);
  RID body2 = make_test_rid(2);

  hash_grid.insert(body1, make_aabb(20, 20, 20), false);
  hash_grid.insert(body2, make_aabb(40, 40, 40), false);

  // Large query encompasses both
  Vector<RID> results;
  AABB large_query(Vector3(0, 0, 0), Vector3(100, 100, 100));
  hash_grid.query_aabb(large_query, Set<RID>(), &results);

  EXPECT_EQ(results.size(), 2);
}

/**
 * @test Exclusion set works correctly
 */
TEST_F(SpatialHashTest, ExclusionSet) {
  RID body1 = make_test_rid(1);
  RID body2 = make_test_rid(2);

  hash_grid.insert(body1, make_aabb(10, 10, 10), false);
  hash_grid.insert(body2, make_aabb(15, 15, 15), false);

  Set<RID> exclude;
  exclude.insert(body1);

  Vector<RID> results;
  AABB large_query(Vector3(0, 0, 0), Vector3(50, 50, 50));
  hash_grid.query_aabb(large_query, exclude, &results);

  EXPECT_EQ(results.size(), 1);
  EXPECT_EQ(results[0], body2);
}

/**
 * @test Removal works correctly
 */
TEST_F(SpatialHashTest, RemoveBody) {
  RID body1 = make_test_rid(1);
  AABB box1 = make_aabb(10, 10, 10);

  hash_grid.insert(body1, box1, false);
  hash_grid.remove(body1, box1, false);

  Vector<RID> results;
  hash_grid.query_aabb(box1, Set<RID>(), &results);

  EXPECT_EQ(results.size(), 0);
}

/**
 * @test Update body position
 */
TEST_F(SpatialHashTest, UpdateBodyPosition) {
  RID body1 = make_test_rid(1);
  AABB old_box = make_aabb(10, 10, 10);
  AABB new_box = make_aabb(200, 200, 200);

  hash_grid.insert(body1, old_box, false);

  // Body should be found in old location
  Vector<RID> results1;
  hash_grid.query_aabb(old_box, Set<RID>(), &results1);
  EXPECT_EQ(results1.size(), 1);

  // Update position
  hash_grid.update(body1, old_box, new_box, false);

  // Old location should be empty
  Vector<RID> results2;
  hash_grid.query_aabb(old_box, Set<RID>(), &results2);
  EXPECT_EQ(results2.size(), 0);

  // New location should have it
  Vector<RID> results3;
  hash_grid.query_aabb(new_box, Set<RID>(), &results3);
  EXPECT_EQ(results3.size(), 1);
}

/**
 * @test Areas and bodies are tracked separately
 */
TEST_F(SpatialHashTest, AreasVsBodies) {
  RID body = make_test_rid(1);
  RID area = make_test_rid(2);
  AABB box = make_aabb(10, 10, 10);

  hash_grid.insert(body, box, false); // Body
  hash_grid.insert(area, box, true);  // Area

  Vector<RID> bodies, areas;
  hash_grid.query_aabb(box, Set<RID>(), &bodies, &areas);

  EXPECT_EQ(bodies.size(), 1);
  EXPECT_EQ(areas.size(), 1);
  EXPECT_EQ(bodies[0], body);
  EXPECT_EQ(areas[0], area);
}

/**
 * @test Grid statistics
 */
TEST_F(SpatialHashTest, GridStatistics) {
  RID body1 = make_test_rid(1);
  RID body2 = make_test_rid(2);

  hash_grid.insert(body1, make_aabb(10, 10, 10), false);
  hash_grid.insert(body2, make_aabb(15, 15, 15), false);

  SpatialHashGrid::Stats stats = hash_grid.get_stats();

  EXPECT_GT(stats.total_bodies, 0);
  EXPECT_GT(stats.avg_bodies_per_cell, 0.0f);
}

/**
 * @test PhysicsSpaceOptimizer wrapper
 */
TEST_F(SpatialHashTest, PhysicsSpaceOptimizer) {
  RID body = make_test_rid(1);
  AABB initial_aabb = make_aabb(10, 10, 10);

  space_optimizer.track_body(body, initial_aabb, 0, false);

  Vector<RID> results;
  space_optimizer.query_region(initial_aabb, results);

  EXPECT_EQ(results.size(), 1);
}

/**
 * @test Optimizer can be disabled
 */
TEST_F(SpatialHashTest, OptimizerDisable) {
  RID body = make_test_rid(1);
  AABB box = make_aabb(10, 10, 10);

  space_optimizer.set_enabled(false);
  space_optimizer.track_body(body, box, 0, false);

  Vector<RID> results;
  space_optimizer.query_region(box, results);

  EXPECT_EQ(results.size(), 0); // Disabled, no tracking
}

/**
 * Benchmark: Spatial Hash vs. Brute Force
 */
class SpatialHashBenchmark : public ::testing::Test {
protected:
  void BenchmarkBruteForce(int num_bodies, int num_queries) {
    Vector<AABB> bodies;
    for (int i = 0; i < num_bodies; ++i) {
      float x = (i * 17) % 500; // Pseudo-random position
      float y = (i * 31) % 500;
      float z = (i * 47) % 500;
      bodies.push_back(make_aabb(x, y, z, 4.0f));
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int q = 0; q < num_queries; ++q) {
      AABB query(Vector3(q % 500, 50, 50), Vector3(64, 64, 64));
      int count = 0;

      // Brute force: check all bodies
      for (const AABB &body : bodies) {
        if (body.intersects(query)) {
          count++;
        }
      }
      (void)count; // Use count
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();

    printf("Brute Force (%d bodies, %d queries): %lld µs\n", num_bodies,
           num_queries, duration);
  }

  void BenchmarkSpatialHash(int num_bodies, int num_queries) {
    SpatialHashGrid grid;

    // Insert bodies
    for (int i = 0; i < num_bodies; ++i) {
      float x = (i * 17) % 500;
      float y = (i * 31) % 500;
      float z = (i * 47) % 500;
      RID rid = RID::from_uint64((uint64_t)i);
      grid.insert(rid, make_aabb(x, y, z, 4.0f), false);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int q = 0; q < num_queries; ++q) {
      AABB query(Vector3(q % 500, 50, 50), Vector3(64, 64, 64));
      Vector<RID> results;
      grid.query_aabb(query, Set<RID>(), &results);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();

    printf("Spatial Hash (%d bodies, %d queries): %lld µs\n", num_bodies,
           num_queries, duration);
  }

  AABB make_aabb(float x, float y, float z, float size = 4.0f) {
    return AABB(Vector3(x, y, z), Vector3(size, size, size));
  }
};

/**
 * @test Benchmark small scene (100 bodies, 100 queries)
 */
TEST_F(SpatialHashBenchmark, SmallScene) {
  printf("\n=== Small Scene (100 bodies, 100 queries) ===\n");
  BenchmarkBruteForce(100, 100);
  BenchmarkSpatialHash(100, 100);
}

/**
 * @test Benchmark medium scene (1000 bodies, 100 queries)
 */
TEST_F(SpatialHashBenchmark, MediumScene) {
  printf("\n=== Medium Scene (1000 bodies, 100 queries) ===\n");
  BenchmarkBruteForce(1000, 100);
  BenchmarkSpatialHash(1000, 100);
}

/**
 * @test Benchmark large scene (5000 bodies, 100 queries)
 */
TEST_F(SpatialHashBenchmark, LargeScene) {
  printf("\n=== Large Scene (5000 bodies, 100 queries) ===\n");
  BenchmarkBruteForce(5000, 100);
  BenchmarkSpatialHash(5000, 100);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

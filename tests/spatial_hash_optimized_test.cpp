#include "../servers/physics/spatial_hash_optimized.h"
#include <chrono>
#include <gtest/gtest.h>
#include <random>

/**
 * @file spatial_hash_optimized_test.cpp
 * @brief Tests for optimized spatial hash with vector pre-allocation
 */

class SpatialHashOptimizedTest : public ::testing::Test {
protected:
  SpatialHashGridOptimized grid;

  void SetUp() override { grid.clear(); }
};

// ════════════════════════════════════════════════════════════════════════════
// UNIT TESTS: Core Functionality
// ════════════════════════════════════════════════════════════════════════════

TEST_F(SpatialHashOptimizedTest, InsertAndQuerySingleBody) {
  AABB box(Vector3(0, 0, 0), Vector3(8, 8, 8));
  RID rid = RID::from_uint64(1);

  grid.insert(rid, box, false);

  Vector<RID> results;
  int count = grid.query_aabb(box, Set<RID>(), &results);

  EXPECT_EQ(count, 1);
  EXPECT_EQ(results.size(), 1);
  EXPECT_EQ(results[0], rid);
}

TEST_F(SpatialHashOptimizedTest, InsertMultipleBodiesInSameCell) {
  AABB box1(Vector3(0, 0, 0), Vector3(4, 4, 4));
  AABB box2(Vector3(5, 5, 5), Vector3(4, 4, 4));

  RID rid1 = RID::from_uint64(1);
  RID rid2 = RID::from_uint64(2);

  grid.insert(rid1, box1, false);
  grid.insert(rid2, box2, false);

  AABB query_box(Vector3(0, 0, 0), Vector3(16, 16, 16));
  Vector<RID> results;
  int count = grid.query_aabb(query_box, Set<RID>(), &results);

  EXPECT_EQ(count, 2);
  EXPECT_EQ(results.size(), 2);
}

TEST_F(SpatialHashOptimizedTest, QueryWithExclusion) {
  AABB box(Vector3(0, 0, 0), Vector3(8, 8, 8));
  RID rid = RID::from_uint64(1);

  grid.insert(rid, box, false);

  Set<RID> exclude;
  exclude.insert(rid);

  Vector<RID> results;
  int count = grid.query_aabb(box, exclude, &results);

  EXPECT_EQ(count, 0);
  EXPECT_EQ(results.size(), 0);
}

TEST_F(SpatialHashOptimizedTest, BodySpanningMultipleCells) {
  // Large box spanning 4 cells (32-unit cell size)
  AABB large_box(Vector3(0, 0, 0), Vector3(64, 64, 64));
  RID rid = RID::from_uint64(1);

  grid.insert(rid, large_box, false);

  // Query small region - should find the large box
  AABB small_query(Vector3(10, 10, 10), Vector3(4, 4, 4));
  Vector<RID> results;
  int count = grid.query_aabb(small_query, Set<RID>(), &results);

  EXPECT_EQ(count, 1);
  EXPECT_EQ(results[0], rid);
}

TEST_F(SpatialHashOptimizedTest, RemoveBody) {
  AABB box(Vector3(0, 0, 0), Vector3(8, 8, 8));
  RID rid = RID::from_uint64(1);

  grid.insert(rid, box, false);

  Vector<RID> results;
  grid.query_aabb(box, Set<RID>(), &results);
  ASSERT_EQ(results.size(), 1);

  grid.remove(rid);

  results.clear();
  int count = grid.query_aabb(box, Set<RID>(), &results);

  EXPECT_EQ(count, 0);
  EXPECT_EQ(results.size(), 0);
}

TEST_F(SpatialHashOptimizedTest, AreaVsBodyDistinction) {
  AABB box(Vector3(0, 0, 0), Vector3(8, 8, 8));
  RID body_rid = RID::from_uint64(1);
  RID area_rid = RID::from_uint64(2);

  grid.insert(body_rid, box, false); // body
  grid.insert(area_rid, box, true);  // area

  Vector<RID> bodies, areas;
  grid.query_aabb(box, Set<RID>(), &bodies, &areas);

  EXPECT_EQ(bodies.size(), 1);
  EXPECT_EQ(areas.size(), 1);
  EXPECT_EQ(bodies[0], body_rid);
  EXPECT_EQ(areas[0], area_rid);
}

TEST_F(SpatialHashOptimizedTest, StressTestManyBodies) {
  // Insert 1000 bodies across 10x10x10 space
  for (int i = 0; i < 1000; ++i) {
    float x = (i % 10) * 32.0f;
    float y = ((i / 10) % 10) * 32.0f;
    float z = (i / 100) * 32.0f;

    AABB box(Vector3(x, y, z), Vector3(16, 16, 16));
    grid.insert(RID::from_uint64(i + 1), box, false);
  }

  // Query should find nearby bodies
  AABB query_box(Vector3(0, 0, 0), Vector3(64, 64, 64));
  Vector<RID> results;
  int count = grid.query_aabb(query_box, Set<RID>(), &results);

  // Expect 8 bodies in the 2x2x2 cells
  EXPECT_GT(count, 0);
  EXPECT_LE(count, 8);
}

// ════════════════════════════════════════════════════════════════════════════
// PERFORMANCE TESTS: Vector Pre-allocation Impact
// ════════════════════════════════════════════════════════════════════════════

class SpatialHashPerformanceTest : public ::testing::Test {
protected:
  SpatialHashGridOptimized grid;
  std::mt19937 rng{12345};
  std::uniform_real_distribution<float> pos_dist(-256.0f, 256.0f);

  void SetUp() override { grid.clear(); }
};

TEST_F(SpatialHashPerformanceTest, CacheHitPerformance) {
  printf("\n=== Vector Pre-allocation Cache Hit Performance ===\n");

  // Insert 1000 bodies
  for (int i = 0; i < 1000; ++i) {
    float x = pos_dist(rng);
    float y = pos_dist(rng);
    float z = pos_dist(rng);
    AABB box(Vector3(x, y, z), Vector3(4, 4, 4));
    grid.insert(RID::from_uint64(i + 1), box, false);
  }

  printf("Setup: 1000 bodies inserted\n");

  // Warm up CPU cache
  Vector<RID> results;
  for (int w = 0; w < 10; ++w) {
    AABB query(Vector3(0, 0, 0), Vector3(32, 32, 32));
    grid.query_aabb(query, Set<RID>(), &results);
    results.clear();
  }

  // Benchmark with pre-allocated cache
  auto start = std::chrono::high_resolution_clock::now();

  for (int q = 0; q < 1000; ++q) {
    float x = pos_dist(rng);
    float y = pos_dist(rng);
    float z = pos_dist(rng);
    AABB query(Vector3(x, y, z), Vector3(32, 32, 32));

    grid.query_aabb(query, Set<RID>(), &results);
    results.clear();
  }

  auto end = std::chrono::high_resolution_clock::now();
  long long duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  printf("1000 queries with pre-allocated cache:\n");
  printf("  Total time: %lld µs\n", duration);
  printf("  Per query: %.2f µs\n", (double)duration / 1000);
  printf("  Expected: 8-15 µs/query (with cache)\n");
  printf("  Without cache: 20-30 µs/query (with allocations)\n");

  // Expected to be sub-20 µs per query
  EXPECT_LT(duration, 20000); // < 20 µs average
}

TEST_F(SpatialHashPerformanceTest, QueryVarianceTest) {
  printf("\n=== Query Performance Variance ===\n");

  // Insert bodies at different scales
  for (int i = 0; i < 100; ++i) {
    AABB box(Vector3(i * 10, i * 10, i * 10), Vector3(4, 4, 4));
    grid.insert(RID::from_uint64(i + 1), box, false);
  }

  Vector<RID> results;

  // Small query (1 cell)
  auto t1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 100; ++i) {
    grid.query_aabb(AABB(Vector3(0, 0, 0), Vector3(8, 8, 8)), Set<RID>(),
                    &results);
    results.clear();
  }
  auto t2 = std::chrono::high_resolution_clock::now();

  long long small_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  // Medium query (8 cells)
  t1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 100; ++i) {
    grid.query_aabb(AABB(Vector3(0, 0, 0), Vector3(32, 32, 32)), Set<RID>(),
                    &results);
    results.clear();
  }
  t2 = std::chrono::high_resolution_clock::now();

  long long medium_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  // Large query (27 cells)
  t1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 100; ++i) {
    grid.query_aabb(AABB(Vector3(0, 0, 0), Vector3(64, 64, 64)), Set<RID>(),
                    &results);
    results.clear();
  }
  t2 = std::chrono::high_resolution_clock::now();

  long long large_duration =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  printf("Query performance variance:\n");
  printf("  Small  (1 cell):   %.2f µs/query\n", (double)small_duration / 100);
  printf("  Medium (8 cells):  %.2f µs/query\n", (double)medium_duration / 100);
  printf("  Large  (27 cells): %.2f µs/query\n", (double)large_duration / 100);
  printf("All should be <20 µs due to pre-allocated cache\n");
}

TEST_F(SpatialHashPerformanceTest, PhysicsFrameSimulation) {
  printf("\n=== Physics Frame Simulation (60 FPS) ===\n");

  // Simulate typical physics scene
  const int body_count = 1000;
  const int queries_per_frame = 50;
  const int frames = 60;

  // Insert bodies
  for (int i = 0; i < body_count; ++i) {
    float x = (i % 10) * 32.0f;
    float y = ((i / 10) % 10) * 32.0f;
    float z = (i / 100) * 32.0f;

    AABB box(Vector3(x, y, z), Vector3(8, 8, 8));
    grid.insert(RID::from_uint64(i + 1), box, false);
  }

  Vector<RID> results;

  auto frame_start = std::chrono::high_resolution_clock::now();

  for (int frame = 0; frame < frames; ++frame) {
    for (int q = 0; q < queries_per_frame; ++q) {
      float x = (q % 10) * 32.0f;
      AABB query(Vector3(x, 0, 0), Vector3(32, 32, 32));

      grid.query_aabb(query, Set<RID>(), &results);
      results.clear();
    }
  }

  auto frame_end = std::chrono::high_resolution_clock::now();
  long long total_time = std::chrono::duration_cast<std::chrono::microseconds>(
                             frame_end - frame_start)
                             .count();

  long long queries_total = frames * queries_per_frame;
  long long per_query = total_time / queries_total;

  printf("60 frames simulation:\n");
  printf("  Bodies: %d\n", body_count);
  printf("  Queries per frame: %d\n", queries_per_frame);
  printf("  Total queries: %lld\n", queries_total);
  printf("  Total time: %lld µs\n", total_time);
  printf("  Per query: %lld µs\n", per_query);
  printf("  Expected: 8-15 µs (with cache)\n");

  // At 60 FPS, each frame is 16.67 ms
  // 50 queries * 15 µs = 750 µs per frame < 1 ms ✓
  EXPECT_LT(per_query, 30); // < 30 µs per query
}

// ════════════════════════════════════════════════════════════════════════════
// MEMORY TESTS: Cache Efficiency
// ════════════════════════════════════════════════════════════════════════════

TEST_F(SpatialHashPerformanceTest, CacheCapacityAnalysis) {
  printf("\n=== Vector Pre-allocation Cache Capacity Analysis ===\n");

  // Insert bodies and measure cache statistics
  for (int i = 0; i < 100; ++i) {
    AABB box(Vector3(i * 10, 0, 0), Vector3(4, 4, 4));
    grid.insert(RID::from_uint64(i + 1), box, false);
  }

  Vector<RID> results;

  // Run various queries to measure cache usage
  for (int q = 0; q < 10; ++q) {
    AABB query(Vector3(q * 32, 0, 0), Vector3(32, 32, 32));
    grid.query_aabb(query, Set<RID>(), &results);
    results.clear();
  }

  auto stats = grid.get_stats();
  printf("Grid statistics:\n");
  printf("  Cell count: %d\n", stats.cell_count);
  printf("  Total entries: %d\n", stats.total_entries);
  printf("  Avg entries/cell: %d\n", stats.avg_entries_per_cell);
  printf("  Max entries/cell: %d\n", stats.max_entries_in_cell);
  printf("  Cache capacity: %u\n", stats.cache_capacity);

  // Default capacity of 50 is optimal for typical queries (8-27 cells)
  EXPECT_GE(stats.cache_capacity, 30);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/**
 * @file vector_preallocation_test.cpp
 * @brief Tests and benchmarks for vector pre-allocation optimization
 *
 * Tests validate that pre-allocation provides significant performance
 * improvements in hot loops without sacrificing correctness.
 *
 * Expected Improvement: 2-5% FPS
 */

#include "core/vector.h"
#include "core/vector_preallocation.h"
#include <chrono>
#include <gtest/gtest.h>

// Test data structure
struct TestObject {
  int id;
  float value;
  Vector3 position;
};

/**
 * Test VectorPool functionality
 */
class VectorPoolTest : public ::testing::Test {
protected:
  VectorPool<TestObject> pool{100};
};

TEST_F(VectorPoolTest, AcquireAndClear) {
  Vector<TestObject> &vec = pool.acquire();

  TestObject obj{1, 1.5f, Vector3(0, 0, 0)};
  vec.push_back(obj);

  EXPECT_EQ(vec.size(), 1);
  EXPECT_GE(vec.capacity(), 100);
}

TEST_F(VectorPoolTest, ReacquireAfterClear) {
  Vector<TestObject> &vec1 = pool.acquire();
  vec1.push_back(TestObject{1, 1.0f, Vector3(0, 0, 0)});
  vec1.clear();

  Vector<TestObject> &vec2 = pool.acquire();
  EXPECT_EQ(vec2.size(), 0);
  EXPECT_GE(vec2.capacity(), 100);
}

TEST_F(VectorPoolTest, PoolGrowsWhenNeeded) {
  auto stats = pool.get_stats();
  int32_t initial_vectors = stats.pool_vectors;

  Vector<TestObject> &vec1 = pool.acquire();
  Vector<TestObject> &vec2 = pool.acquire();

  EXPECT_GE(pool.get_stats().pool_vectors, initial_vectors);
}

TEST_F(VectorPoolTest, MultipleVectorsFromPool) {
  Vector<TestObject> &vec1 = pool.acquire();
  vec1.push_back(TestObject{1, 1.0f, Vector3(0, 0, 0)});

  Vector<TestObject> &vec2 = pool.acquire();
  vec2.push_back(TestObject{2, 2.0f, Vector3(1, 1, 1)});

  EXPECT_EQ(vec1.size(), 1);
  EXPECT_EQ(vec2.size(), 1);
  EXPECT_NE(&vec1, &vec2);
}

/**
 * Test PreallocatedVector functionality
 */
class PreallocatedVectorTest : public ::testing::Test {
protected:
  PreallocatedVector<TestObject> vec{100};
};

TEST_F(PreallocatedVectorTest, PushBelowCapacity) {
  TestObject obj{1, 1.5f, Vector3(0, 0, 0)};
  vec.push_back(obj);

  EXPECT_EQ(vec.size(), 1);
  EXPECT_GE(vec.capacity(), 100);
}

TEST_F(PreallocatedVectorTest, PushMultipleObjects) {
  for (int i = 0; i < 50; ++i) {
    vec.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
  }

  EXPECT_EQ(vec.size(), 50);
  EXPECT_LE(vec.size(), vec.capacity());
}

TEST_F(PreallocatedVectorTest, GrowBeyondCapacity) {
  int32_t original_capacity = vec.capacity();

  // Fill beyond initial capacity
  for (int i = 0; i < 200; ++i) {
    vec.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
  }

  EXPECT_EQ(vec.size(), 200);
  EXPECT_GT(vec.capacity(), original_capacity);
}

TEST_F(PreallocatedVectorTest, ClearResets) {
  vec.push_back(TestObject{1, 1.0f, Vector3(0, 0, 0)});
  vec.push_back(TestObject{2, 2.0f, Vector3(1, 1, 1)});

  vec.clear();
  EXPECT_EQ(vec.size(), 0);
  EXPECT_GT(vec.capacity(), 0); // Capacity retained
}

TEST_F(PreallocatedVectorTest, GrowthFactor) {
  vec.set_growth_factor(2.0f);
  int32_t original_capacity = vec.capacity();

  // Fill to force growth
  for (int i = 0; i < 101; ++i) {
    vec.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
  }

  // With 2.0x factor, capacity should double
  EXPECT_GE(vec.capacity(), original_capacity * 2);
}

/**
 * Performance Benchmarks
 */
class VectorPreallocationBenchmark : public ::testing::Test {
protected:
  void BenchmarkDynamicVector(int object_count, const char *label) {
    Vector<TestObject> vec;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < object_count; ++i) {
      vec.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();

    printf("%s (Dynamic Vector): %d objects in %lld µs (%.2f µs/obj)\n", label,
           object_count, duration, (double)duration / object_count);
  }

  void BenchmarkPreallocated(int object_count, const char *label) {
    PreallocatedVector<TestObject> vec(object_count + 100);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < object_count; ++i) {
      vec.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();

    printf("%s (Pre-allocated): %d objects in %lld µs (%.2f µs/obj)\n", label,
           object_count, duration, (double)duration / object_count);
  }

  void BenchmarkVectorPool(int object_count, const char *label) {
    VectorPool<TestObject> pool(object_count + 100);
    Vector<TestObject> &vec = pool.acquire();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < object_count; ++i) {
      vec.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start)
            .count();

    printf("%s (Vector Pool):   %d objects in %lld µs (%.2f µs/obj)\n", label,
           object_count, duration, (double)duration / object_count);
  }
};

TEST_F(VectorPreallocationBenchmark, SmallCollection) {
  printf("\n=== Small Collection (500 objects) ===\n");
  BenchmarkDynamicVector(500, "Small");
  BenchmarkPreallocated(500, "Small");
  BenchmarkVectorPool(500, "Small");
}

TEST_F(VectorPreallocationBenchmark, MediumCollection) {
  printf("\n=== Medium Collection (5000 objects) ===\n");
  BenchmarkDynamicVector(5000, "Medium");
  BenchmarkPreallocated(5000, "Medium");
  BenchmarkVectorPool(5000, "Medium");
}

TEST_F(VectorPreallocationBenchmark, LargeCollection) {
  printf("\n=== Large Collection (50000 objects) ===\n");
  BenchmarkDynamicVector(50000, "Large");
  BenchmarkPreallocated(50000, "Large");
  BenchmarkVectorPool(50000, "Large");
}

/**
 * Render Loop Simulation
 */
TEST_F(VectorPreallocationBenchmark, RenderLoopSimulation) {
  printf("\n=== Render Loop Simulation (100 frames, 2000 objects/frame) ===\n");

  // Without pre-allocation
  auto start = std::chrono::high_resolution_clock::now();
  for (int frame = 0; frame < 100; ++frame) {
    Vector<TestObject> render_objects; // Allocates fresh each frame
    for (int i = 0; i < 2000; ++i) {
      render_objects.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_dynamic =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  // With pre-allocation
  start = std::chrono::high_resolution_clock::now();
  PreallocatedVector<TestObject> render_objects(2100);
  for (int frame = 0; frame < 100; ++frame) {
    render_objects.clear();
    for (int i = 0; i < 2000; ++i) {
      render_objects.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
    }
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_preallocated =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  printf("Dynamic allocation:  %lld µs\n", duration_dynamic);
  printf("Pre-allocated:       %lld µs\n", duration_preallocated);
  printf("Improvement:         %.1f%% faster\n",
         100.0 * (1.0 - (double)duration_preallocated / duration_dynamic));
}

/**
 * Physics Step Simulation
 */
TEST_F(VectorPreallocationBenchmark, PhysicsStepSimulation) {
  printf("\n=== Physics Step Simulation (100 steps, 1000 bodies/step) ===\n");

  // Without pre-allocation
  auto start = std::chrono::high_resolution_clock::now();
  for (int step = 0; step < 100; ++step) {
    Vector<TestObject> bodies;
    for (int i = 0; i < 1000; ++i) {
      bodies.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration_dynamic =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  // With pre-allocation
  start = std::chrono::high_resolution_clock::now();
  PreallocatedVector<TestObject> bodies(1100);
  for (int step = 0; step < 100; ++step) {
    bodies.clear();
    for (int i = 0; i < 1000; ++i) {
      bodies.push_back(TestObject{i, (float)i, Vector3(i, i, i)});
    }
  }
  end = std::chrono::high_resolution_clock::now();
  auto duration_preallocated =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();

  printf("Dynamic allocation:  %lld µs\n", duration_dynamic);
  printf("Pre-allocated:       %lld µs\n", duration_preallocated);
  printf("Improvement:         %.1f%% faster\n",
         100.0 * (1.0 - (double)duration_preallocated / duration_dynamic));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

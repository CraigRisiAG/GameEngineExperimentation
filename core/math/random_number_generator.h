
/**
 * @class RandomNumberGenerator
 * @brief Wrapper class for pseudo-random number generation using the PCG
 * algorithm.
 *
 * Provides an interface for generating random numbers of various types and
 * distributions. Inherits from Reference for automatic memory management.
 */

/**
 * @brief Sets the seed for the random number generator.
 * @param seed The seed value to initialize the generator.
 */
void set_seed(uint64_t seed);

/**
 * @brief Gets the current seed of the random number generator.
 * @return The current seed value.
 */
uint64_t get_seed();

/**
 * @brief Randomizes the seed based on the current time.
 */
void randomize();

/**
 * @brief Generates a random unsigned 32-bit integer.
 * @return A random uint32_t value.
 */
uint32_t randi();

/**
 * @brief Generates a random floating-point number between 0.0 and 1.0.
 * @return A random real_t value in the range [0.0, 1.0).
 */
real_t randf();

/**
 * @brief Generates a random floating-point number within a specified range.
 * @param from The lower bound (inclusive).
 * @param to The upper bound (inclusive).
 * @return A random real_t value in the range [from, to].
 */
real_t randf_range(real_t from, real_t to);

/**
 * @brief Generates a random number from a normal (Gaussian) distribution.
 * @param mean The mean of the distribution. Default is 0.0.
 * @param deviation The standard deviation of the distribution. Default is 1.0.
 * @return A random real_t value sampled from the normal distribution.
 */
real_t randfn(real_t mean = 0.0, real_t deviation = 1.0);

/**
 * @brief Generates a random integer within a specified range.
 * @param from The lower bound (inclusive).
 * @param to The upper bound (inclusive).
 * @return A random int value in the range [from, to].
 */
int randi_range(int from, int to);
#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include "core/math/random_pcg.h"
#include "core/reference.h"

class RandomNumberGenerator : public Reference {
  GDCLASS(RandomNumberGenerator, Reference);

  RandomPCG randbase;

protected:
  static void _bind_methods();

public:
  _FORCE_INLINE_ void set_seed(uint64_t seed) { randbase.seed(seed); }

  _FORCE_INLINE_ uint64_t get_seed() { return randbase.get_seed(); }

  _FORCE_INLINE_ void randomize() { randbase.randomize(); }

  _FORCE_INLINE_ uint32_t randi() { return randbase.rand(); }

  _FORCE_INLINE_ real_t randf() { return randbase.randf(); }

  _FORCE_INLINE_ real_t randf_range(real_t from, real_t to) {
    return randbase.random(from, to);
  }

  _FORCE_INLINE_ real_t randfn(real_t mean = 0.0, real_t deviation = 1.0) {
    return randbase.randfn(mean, deviation);
  }

  _FORCE_INLINE_ int randi_range(int from, int to) {
    unsigned int ret = randbase.rand();
    if (to < from)
      return ret % (from - to + 1) + to;
    else
      return ret % (to - from + 1) + from;
  }

  RandomNumberGenerator();
};

#endif // RANDOM_NUMBER_GENERATOR_H

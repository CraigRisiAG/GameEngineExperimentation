
/**
 * @class RandomPCG
 * @brief PCG-based pseudo-random number generator implementation.
 *
 * Provides seeded random number generation with support for both float and
 * double precision ranges.
 */

/**
 * @brief Constructs a RandomPCG generator with specified seed and increment.
 *
 * @param p_seed The seed value to initialize the PCG state.
 * @param p_inc The increment value for the PCG generator.
 */

/**
 * @brief Reinitializes the generator with a seed based on the current system
 * time.
 *
 * Uses the system's microsecond tick count combined with the current PCG state
 * and default increment to generate a new seed.
 */

/**
 * @brief Generates a random double-precision floating-point number in the range
 * [p_from, p_to).
 *
 * @param p_from The lower bound of the range (inclusive).
 * @param p_to The upper bound of the range (exclusive).
 * @return A random double value within the specified range.
 */

/**
 * @brief Generates a random single-precision floating-point number in the range
 * [p_from, p_to).
 *
 * @param p_from The lower bound of the range (inclusive).
 * @param p_to The upper bound of the range (exclusive).
 * @return A random float value within the specified range.
 */
#include "random_pcg.h"

#include "core/os/os.h"

RandomPCG::RandomPCG(uint64_t p_seed, uint64_t p_inc)
    : pcg(), current_inc(p_inc) {
  seed(p_seed);
}

void RandomPCG::randomize() {
  seed(OS::get_singleton()->get_ticks_usec() * pcg.state + PCG_DEFAULT_INC_64);
}

double RandomPCG::random(double p_from, double p_to) {
  return randd() * (p_to - p_from) + p_from;
}

float RandomPCG::random(float p_from, float p_to) {
  return randf() * (p_to - p_from) + p_from;
}

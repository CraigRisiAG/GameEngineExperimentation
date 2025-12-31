

/**
 * @class RandomPCG
 * @brief A pseudorandom number generator using the PCG (Permuted Congruential
 * Generator) algorithm.
 *
 * This class provides high-quality random number generation with support for
 * both integer and floating-point values. It uses the PCG32 algorithm
 * internally and includes optimized methods for generating uniformly
 * distributed floating-point numbers in the [0, 1] range.
 *
 * The floating-point generation methods (randd, randf) employ sophisticated
 * techniques to ensure statistical uniformity across a wide range of magnitudes
 * using bit counting and ldexp operations.
 *
 * @note The class uses platform-specific intrinsics (CLZ32, LDEXP) when
 * available for optimal performance. Fallback implementations are provided for
 * platforms without these intrinsics.
 *
 * @see pcg.h - PCG algorithm implementation
 * @see math_defs.h - Mathematical definitions and constants
 */
class RandomPCG {
  // ... implementation details
};

/**
 * @brief Constructs a RandomPCG generator with optional seed and increment
 * values.
 * @param p_seed The seed value (default: DEFAULT_SEED). Use the same seed to
 * reproduce sequences.
 * @param p_inc The increment value (default: DEFAULT_INC). Controls the
 * sequence period.
 */
RandomPCG(uint64_t p_seed = DEFAULT_SEED, uint64_t p_inc = DEFAULT_INC);

/**
 * @brief Seeds the generator with a new value.
 * @param p_seed The seed value to initialize the internal state.
 */
_FORCE_INLINE_ void seed(uint64_t p_seed);

/**
 * @brief Returns the current seed value.
 * @return The current seed used to initialize the generator state.
 */
_FORCE_INLINE_ uint64_t get_seed();

/**
 * @brief Initializes the generator with a randomized seed based on system
 * entropy.
 */
void randomize();

/**
 * @brief Generates a 32-bit unsigned random integer.
 * @return A uniformly distributed uint32_t value in the range [0, RANDOM_MAX].
 */
_FORCE_INLINE_ uint32_t rand();

/**
 * @brief Generates a uniformly distributed double-precision floating-point
 * number.
 * @return A random double value in the range [0.0, 1.0].
 *
 * @note Uses bit manipulation with MSB/LSB set to 1 and zero-counting
 * techniques to ensure uniformity. Provides good coverage down to 2^-96
 * magnitude.
 */
_FORCE_INLINE_ double randd();

/**
 * @brief Generates a uniformly distributed single-precision floating-point
 * number.
 * @return A random float value in the range [0.0, 1.0].
 *
 * @note Uses bit manipulation with MSB/LSB set to 1 and zero-counting
 * techniques to ensure uniformity. Provides good coverage down to 2^-64
 * magnitude.
 */
_FORCE_INLINE_ float randf();

/**
 * @brief Generates a normally distributed double-precision floating-point
 * number using the Box-Muller transform.
 * @param p_mean The mean of the normal distribution.
 * @param p_deviation The standard deviation of the normal distribution.
 * @return A random double value following a normal distribution with the
 * specified parameters.
 */
_FORCE_INLINE_ double randfn(double p_mean, double p_deviation);

/**
 * @brief Generates a normally distributed single-precision floating-point
 * number using the Box-Muller transform.
 * @param p_mean The mean of the normal distribution.
 * @param p_deviation The standard deviation of the normal distribution.
 * @return A random float value following a normal distribution with the
 * specified parameters.
 */
_FORCE_INLINE_ float randfn(float p_mean, float p_deviation);

/**
 * @brief Generates a uniformly distributed random number within a range.
 * @param p_from The lower bound of the range (inclusive).
 * @param p_to The upper bound of the range (inclusive).
 * @return A random value in the range [p_from, p_to].
 *
 * @note Overloaded for double, float, and int types.
 */
double random(double p_from, double p_to);
float random(float p_from, float p_to);
real_t random(int p_from, int p_to);

double random(double p_from, double p_to);
float random(float p_from, float p_to);
real_t random(int p_from, int p_to) {
  return (real_t)random((real_t)p_from, (real_t)p_to);
}
}
;

#endif // RANDOM_PCG_H

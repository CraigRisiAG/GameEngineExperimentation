
/// @class RandomNumberGenerator
/// @brief Provides pseudo-random number generation functionality.
///
/// This class offers methods to generate random numbers with various
/// distributions and ranges. It supports seeding for reproducible random
/// sequences.
///
/// @property seed : int
///     The seed value for the random number generator. Setting this value
///     allows for reproducible sequences of random numbers.
///
/// @method void set_seed(int seed)
///     Sets the seed value for the random number generator.
///     @param seed The seed value to initialize the RNG.
///
/// @method int get_seed()
///     Gets the current seed value.
///     @return The current seed value.
///
/// @method int randi()
///     Generates a random 32-bit unsigned integer.
///     @return A random integer value.
///
/// @method float randf()
///     Generates a random float value between 0.0 and 1.0.
///     @return A random float in the range [0.0, 1.0).
///
/// @method float randfn(float mean = 0.0, float deviation = 1.0)
///     Generates a random float from a normal (Gaussian) distribution.
///     @param mean The mean of the distribution. Default is 0.0.
///     @param deviation The standard deviation of the distribution. Default
///     is 1.0.
///     @return A random float from the normal distribution.
///
/// @method float randf_range(float from, float to)
///     Generates a random float within the specified range.
///     @param from The minimum value (inclusive).
///     @param to The maximum value (exclusive).
///     @return A random float in the range [from, to).
///
/// @method int randi_range(int from, int to)
///     Generates a random integer within the specified range.
///     @param from The minimum value (inclusive).
///     @param to The maximum value (inclusive).
///     @return A random integer in the range [from, to].
///
/// @method void randomize()
///     Initializes the random number generator with a seed based on the current
///     time.
#include "random_number_generator.h"

RandomNumberGenerator::RandomNumberGenerator() {}

void RandomNumberGenerator::_bind_methods() {
  ClassDB::bind_method(D_METHOD("set_seed", "seed"),
                       &RandomNumberGenerator::set_seed);
  ClassDB::bind_method(D_METHOD("get_seed"), &RandomNumberGenerator::get_seed);
  ADD_PROPERTY(PropertyInfo(Variant::INT, "seed"), "set_seed", "get_seed");

  ClassDB::bind_method(D_METHOD("randi"), &RandomNumberGenerator::randi);
  ClassDB::bind_method(D_METHOD("randf"), &RandomNumberGenerator::randf);
  ClassDB::bind_method(D_METHOD("randfn", "mean", "deviation"),
                       &RandomNumberGenerator::randfn, DEFVAL(0.0),
                       DEFVAL(1.0));
  ClassDB::bind_method(D_METHOD("randf_range", "from", "to"),
                       &RandomNumberGenerator::randf_range);
  ClassDB::bind_method(D_METHOD("randi_range", "from", "to"),
                       &RandomNumberGenerator::randi_range);
  ClassDB::bind_method(D_METHOD("randomize"),
                       &RandomNumberGenerator::randomize);
}

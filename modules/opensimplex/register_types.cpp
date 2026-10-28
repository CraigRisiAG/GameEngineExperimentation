
#include "register_types.h"
#include "noise_texture.h"
#include "open_simplex_noise.h"

void register_opensimplex_types() {

  ClassDB::register_class<OpenSimplexNoise>();
  ClassDB::register_class<NoiseTexture>();
}

void unregister_opensimplex_types() {}

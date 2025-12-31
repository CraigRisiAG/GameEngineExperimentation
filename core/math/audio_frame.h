
/**
 * @file audio_frame.h
 * @brief Audio frame structure for stereo audio processing.
 *
 * Defines an AudioFrame struct that represents a single stereo audio sample
 * with left and right channel values. Provides comprehensive operator
 * overloading for arithmetic operations between AudioFrames and with scalar
 * float values. Includes utility functions for audio processing such as
 * denormalization and linear interpolation.
 */

/**
 * @brief Converts denormalized floating-point values to zero.
 *
 * Prevents audio artifacts caused by denormalized float values (very small
 * numbers close to zero that consume CPU resources). Uses bitwise operations to
 * detect and eliminate denormalized values by checking the exponent bits.
 *
 * @param f The floating-point value to process.
 * @return The input value if normalized, otherwise 0.0f.
 */

/**
 * @struct AudioFrame
 * @brief Represents a single stereo audio sample with left and right channels.
 *
 * Provides a lightweight structure for audio data manipulation with optimized
 * arithmetic operations. Supports conversion to/from Vector2 and includes
 * specialized audio processing methods like denormalization and interpolation.
 *
 * All operations are marked with inline directives for performance-critical
 * audio processing code.
 *
 * @member l Left channel sample value.
 * @member r Right channel sample value.
 */

/**
 * @brief Linear interpolation between two AudioFrames.
 *
 * Performs linear interpolation (lerp) between the current frame and another
 * frame based on the interpolation parameter t.
 *
 * @param p_b The target AudioFrame to interpolate towards.
 * @param p_t The interpolation parameter (0.0 = current frame, 1.0 = target
 * frame).
 * @return An interpolated AudioFrame.
 */

/**
 * @brief Removes denormalized values from both channels.
 *
 * Applies denormalization to both left and right channels to prevent audio
 * artifacts and reduce CPU overhead in audio processing pipelines.
 */
#ifndef AUDIO_FRAME_H
#define AUDIO_FRAME_H

#include "core/math/vector2.h"
#include "core/typedefs.h"

static inline float undenormalise(volatile float f) {
  union {
    uint32_t i;
    float f;
  } v;

  v.f = f;

  // original: return (v.i & 0x7f800000) == 0 ? 0.0f : f;
  // version from Tim Blechmann:
  return (v.i & 0x7f800000) < 0x08000000 ? 0.0f : f;
}

struct AudioFrame {

  // left and right samples
  float l, r;

  _ALWAYS_INLINE_ const float &operator[](int idx) const {
    return idx == 0 ? l : r;
  }
  _ALWAYS_INLINE_ float &operator[](int idx) { return idx == 0 ? l : r; }

  _ALWAYS_INLINE_ AudioFrame operator+(const AudioFrame &p_frame) const {
    return AudioFrame(l + p_frame.l, r + p_frame.r);
  }
  _ALWAYS_INLINE_ AudioFrame operator-(const AudioFrame &p_frame) const {
    return AudioFrame(l - p_frame.l, r - p_frame.r);
  }
  _ALWAYS_INLINE_ AudioFrame operator*(const AudioFrame &p_frame) const {
    return AudioFrame(l * p_frame.l, r * p_frame.r);
  }
  _ALWAYS_INLINE_ AudioFrame operator/(const AudioFrame &p_frame) const {
    return AudioFrame(l / p_frame.l, r / p_frame.r);
  }

  _ALWAYS_INLINE_ AudioFrame operator+(float p_sample) const {
    return AudioFrame(l + p_sample, r + p_sample);
  }
  _ALWAYS_INLINE_ AudioFrame operator-(float p_sample) const {
    return AudioFrame(l - p_sample, r - p_sample);
  }
  _ALWAYS_INLINE_ AudioFrame operator*(float p_sample) const {
    return AudioFrame(l * p_sample, r * p_sample);
  }
  _ALWAYS_INLINE_ AudioFrame operator/(float p_sample) const {
    return AudioFrame(l / p_sample, r / p_sample);
  }

  _ALWAYS_INLINE_ void operator+=(const AudioFrame &p_frame) {
    l += p_frame.l;
    r += p_frame.r;
  }
  _ALWAYS_INLINE_ void operator-=(const AudioFrame &p_frame) {
    l -= p_frame.l;
    r -= p_frame.r;
  }
  _ALWAYS_INLINE_ void operator*=(const AudioFrame &p_frame) {
    l *= p_frame.l;
    r *= p_frame.r;
  }
  _ALWAYS_INLINE_ void operator/=(const AudioFrame &p_frame) {
    l /= p_frame.l;
    r /= p_frame.r;
  }

  _ALWAYS_INLINE_ void operator+=(float p_sample) {
    l += p_sample;
    r += p_sample;
  }
  _ALWAYS_INLINE_ void operator-=(float p_sample) {
    l -= p_sample;
    r -= p_sample;
  }
  _ALWAYS_INLINE_ void operator*=(float p_sample) {
    l *= p_sample;
    r *= p_sample;
  }
  _ALWAYS_INLINE_ void operator/=(float p_sample) {
    l /= p_sample;
    r /= p_sample;
  }

  _ALWAYS_INLINE_ void undenormalise() {
    l = ::undenormalise(l);
    r = ::undenormalise(r);
  }

  _FORCE_INLINE_ AudioFrame linear_interpolate(const AudioFrame &p_b,
                                               float p_t) const {

    AudioFrame res = *this;

    res.l += (p_t * (p_b.l - l));
    res.r += (p_t * (p_b.r - r));

    return res;
  }

  _ALWAYS_INLINE_ AudioFrame(float p_l, float p_r) {
    l = p_l;
    r = p_r;
  }
  _ALWAYS_INLINE_ AudioFrame(const AudioFrame &p_frame) {
    l = p_frame.l;
    r = p_frame.r;
  }

  _ALWAYS_INLINE_ AudioFrame operator=(const AudioFrame &p_frame) {
    l = p_frame.l;
    r = p_frame.r;
    return *this;
  }

  _ALWAYS_INLINE_ operator Vector2() const { return Vector2(l, r); }

  _ALWAYS_INLINE_ AudioFrame(const Vector2 &p_v2) {
    l = p_v2.x;
    r = p_v2.y;
  }
  _ALWAYS_INLINE_ AudioFrame() {}
};

#endif // AUDIO_FRAME_H

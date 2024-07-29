#pragma once

#include "./color-sampler.hpp"

template <typename Pixel_Type>
static inline Pixel_Type lerp(Pixel_Type v0, Pixel_Type v1, float percent)
{
  return Pixel_Type {
    .b = (uint8_t) (v0.b + ((v1.b - v0.b) * percent)),
    .g = (uint8_t) (v0.g + ((v1.g - v0.g) * percent)),
    .r = (uint8_t) (v0.r + ((v1.r - v0.r) * percent)),
  };
}

template <typename Pixel_Type>
Pixel_Type sampler2D(Image<Pixel_Type> &texture, float xNormalized, float yNormalized)
{
  using namespace std;

  const auto width = texture.width;
  const auto height = texture.height;

  float x = width * xNormalized;
  float y = height * yNormalized;
  float xDecimal = (x - (long) x);
  float yDecimal = (y - (long) y);

  Pixel_Type c0 = texture.buffer->data[static_cast<size_t>(floor(y) * width + floor(x)) ];
  Pixel_Type c1 = texture.buffer->data[static_cast<size_t>(floor(y) * width + ceil(x)) ];
  Pixel_Type c2 = texture.buffer->data[static_cast<size_t>(ceil(y) * width + floor(x)) ];
  Pixel_Type c3 = texture.buffer->data[static_cast<size_t>(ceil(y) * width + ceil(x)) ];

  Pixel_Type l0 = lerp(c0, c1, xDecimal);
  Pixel_Type l1 = lerp(c2, c3, xDecimal);
  Pixel_Type lr = lerp(l0, l1, yDecimal);

  return lr;
}

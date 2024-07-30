#pragma once

#include "./color-sampler.hpp"

template <typename Pixel_Type>
static inline Pixel_Type lerp(Pixel_Type v0, Pixel_Type v1, float percent)
{
  Pixel_Type pixel;
  
  pixel.b = static_cast<decltype(v0.b)>((v0.b + ((v1.b - v0.b) * percent)));
  pixel.g = static_cast<decltype(v0.g)>((v0.g + ((v1.g - v0.g) * percent)));
  pixel.r = static_cast<decltype(v0.r)>((v0.r + ((v1.r - v0.r) * percent)));

  return pixel;
}

/**
 * @brief Extrai um amostra de cor da 'textura' provida. Usando as coordenadas 'normalizadas'.
 * 
 * @tparam Pixel_Type um tipo que contenha os campos r, g, b em qualquer ordem.
 * @param texture 
 * @param x_normalized 
 * @param y_normalized 
 * @return Pixel_Type 
 */
template <typename Pixel_Type>
Pixel_Type sampler2D(Image<Pixel_Type> &texture, float x_normalized, float y_normalized)
{
  using namespace std;

  const auto width = texture.width;
  const auto height = texture.height;

  float x = width * x_normalized;
  float y = height * y_normalized;
  float x_decimal = (x - (long) x);
  float y_decimal = (y - (long) y);

  Pixel_Type c0 = texture.buffer->data[static_cast<size_t>(floor(y) * width + floor(x)) ];
  Pixel_Type c1 = texture.buffer->data[static_cast<size_t>(floor(y) * width + ceil(x)) ];
  Pixel_Type c2 = texture.buffer->data[static_cast<size_t>(ceil(y) * width + floor(x)) ];
  Pixel_Type c3 = texture.buffer->data[static_cast<size_t>(ceil(y) * width + ceil(x)) ];

  Pixel_Type l0 = lerp(c0, c1, x_decimal);
  Pixel_Type l1 = lerp(c2, c3, x_decimal);
  Pixel_Type lr = lerp(l0, l1, y_decimal);

  return lr;
}

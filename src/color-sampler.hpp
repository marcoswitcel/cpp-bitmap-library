#pragma once

#include "./array.hpp"

template <typename Pixel_Type>
struct Image {
  size_t width;
  size_t height;
  Array<Pixel_Type> *buffer;
};

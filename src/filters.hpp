#pragma once

#include "./bitmap.hpp"

typedef enum Filter_Name {
  NONE,
  GRAY,
  LUMINOSITY,
  RED_CHANNEL,
  GREEN_CHANNEL,
  BLUE_CHANNEL,
} Filter_Name;

typedef void Filter_RGB_24bits(const RGB_24bits *in, RGB_24bits *out);

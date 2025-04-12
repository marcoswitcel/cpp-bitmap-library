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

const char* filter_name_as_cstring[] = {
  "none",
  "gray",
  "luminosity",
  "red_channel",
  "green_channel",
  "blue_channel",
};

constexpr auto filter_name_length = sizeof(filter_name_as_cstring) / sizeof(filter_name_as_cstring[0]);

typedef void Filter_RGB_24bits(const RGB_24bits *in, RGB_24bits *out);

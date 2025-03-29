#pragma once

#include <string.h>

#include "./filters.hpp"


void filter_RGB_24bits_gray(const RGB_24bits *in, RGB_24bits *out)
{
  out->b = in->g;
  out->g = in->g;
  out->r = in->g;
}

void filter_RGB_24bits_luminosity(const RGB_24bits *in, RGB_24bits *out)
{
  out->b = in->b / 2;
  out->g = in->g / 2;
  out->r = in->r / 2;
}

void filter_RGB_24bits_blue(const RGB_24bits *in, RGB_24bits *out)
{
  out->b = in->b;
  out->g = 0;
  out->r = 0;
}

void filter_RGB_24bits_red(const RGB_24bits *in, RGB_24bits *out)
{
  out->b = 0;
  out->g = 0;
  out->r = in->r;
}

void filter_RGB_24bits_green(const RGB_24bits *in, RGB_24bits *out)
{
  out->b = 0;
  out->g = in->g;
  out->r = 0;
}

Filter_Name lookup_filter_by_name(const char *name, bool *out)
{
  Filter_Name filter_name = NONE;
  bool found = false;

  if (name)
  {
    if (!strcmp(name, "gray"))
    {
      filter_name = GRAY;
      found = true;
    }
    else if (!strcmp(name, "luminosity"))
    {
      filter_name = LUMINOSITY;
      found = true;
    }
    else if (!strcmp(name, "blue_channel"))
    {
      filter_name = BLUE_CHANNEL;
      found = true;
    }
    else if (!strcmp(name, "red_channel"))
    {
      filter_name = RED_CHANNEL;
      found = true;
    }
    else if (!strcmp(name, "green_channel"))
    {
      filter_name = GREEN_CHANNEL;
      found = true;
    }
  }

  if (out) *out = found;

  return filter_name;
}

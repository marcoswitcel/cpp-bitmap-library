#pragma once

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

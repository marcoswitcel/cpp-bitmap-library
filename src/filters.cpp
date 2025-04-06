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

/**
 * @brief busca pelo filtro que atenda pelo nome provido
 * 
 * @param name nome do filtro sendo buscado
 * @param out ponteiro para booleano usado para indicar se o lookup foi bem sucedido
 * @return Filter_Name NONE é retornado em caso do filtro nomeado 'none' ser requisitado ou no caso
 * de o filtro buscado não existir, por isso para desambiguação existe o parâmetro 'out' para indicar se
 * o lookup foi bem sucedido ou não.
 */
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

void apply_filter_to_image(Image<RGB_24bits> &image, Filter_Name filter_name)
{
  Filter_RGB_24bits *func;

  switch (filter_name)
  {
    case NONE: return;
    case GRAY:
      func = filter_RGB_24bits_gray;
    break;
    case LUMINOSITY:
      func = filter_RGB_24bits_luminosity;
    break;
    case BLUE_CHANNEL:
      func = filter_RGB_24bits_blue;
    break;
    case RED_CHANNEL:
      func = filter_RGB_24bits_red;
    break;
    case GREEN_CHANNEL:
      func = filter_RGB_24bits_green;
    break;
    default: return;
  }
  
  for (unsigned row = 0; row < image.height; row++)
  {
    for (unsigned col = 0; col < image.width; col++)
    {
      RGB_24bits *pixel = &image.buffer->data[row * image.width + col];
      func(pixel, pixel);
    }
  }
}

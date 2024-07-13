#ifndef __BITMAP_CPP_
#define __BITMAP_CPP_

#include <stdint.h>

#include "./bitmap.h"

typedef struct Bitmap {
  unsigned width;
  unsigned height;
} Bitmap;

/**
 * @brief cabeçalho do arquivo - contém informação sobre o arquivo
 * @link https://en.wikipedia.org/wiki/BMP_file_format
 */
typedef struct Bitmap_File_Header {
  uint8_t header[2];
  uint32_t size;
  uint16_t application_specific;
  uint16_t application_specific2;
  uint32_t offset;
} Bitmap_File_Header;

const unsigned BITMAP_FILE_HEADER_SIZE = 14;

/**
 * @brief DIB Header - Device-Independent Bitmap
 * @todo João, implementar
 */
typedef struct DIB_Header {
  /**
   * @brief Número de bytes do header
   */
  uint32_t size;
  uint32_t image_width;
  uint32_t image_height;
  uint16_t number_of_colors;
  uint16_t n_bit_per_pixel;
  uint32_t bitfield;
  /**
   * @brief Tamanho dos dados de imagem em bytes incluindo padding
   */
  uint32_t size_of_data;
  uint32_t print_resolution_horizontal;
  uint32_t print_resolution_vertical;
  uint32_t n_colors_in_palette;
  uint32_t important_colors;

} DIB_Header;

#endif // __BITMAP_CPP_

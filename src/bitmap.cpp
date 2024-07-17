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
  uint16_t number_of_colors_planes;
  uint16_t n_bit_per_pixel;
  /**
   * @brief Contém a informação do método de compressão.
   * Um dos valores do enum 'Compression_Method' 
   */
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

typedef enum Compression_Method {
  BI_RGB = 0, // sem compressão
  BI_RLE8 = 1,
  BI_RLE4 = 2,
  BI_BITFIELDS = 3,
  BI_JPEG = 4,
  BI_PNG = 5,
  BI_ALPHABITFIELDS = 6,
  BI_CMYK = 11,
  BI_CMYKRLE8 = 12,
  BI_CMYKRLE4 = 13,
} Compression_Method;

/**
 * @brief Representa um arquivo bitmap
 * 
 */
typedef struct Bitmap_File {
  Bitmap_File_Header *header;
  DIB_Header *dib;
  uint8_t *pixel_array;
} Bitmap_File;

#endif // __BITMAP_CPP_

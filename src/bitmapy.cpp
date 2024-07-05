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
  uint8_t application_specific[2];
  uint8_t application_specific2[2];
  uint32_t offset;
} Bitmap_File_Header;

/**
 * @brief DIB Header - Bitmap Informatio Header
 * @todo João, implementar
 */
typedef struct DIB_Header {

} DIB_Header;

#endif // __BITMAP_CPP_

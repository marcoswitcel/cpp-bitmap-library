#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./bitmap.hpp"

using Byte_Array = Array<uint8_t>;

/**
 * @note  Por hora só existe a implementação para arquiteturas com "Byte Ordering" "Little Endian"
 */
#ifndef BYTE_ORDERING_LITTLE_ENDIAN
  #error "Suporte a arquitetura big endian não implementado."
#endif

/**
 * @brief 
 * @note Implementar um retorno de sucesso ou falha, não sair do programa nessa rotina 
 * 
 * @param file_path 
 * @return Byte_Array 
 */
Byte_Array read_file_as_byte_array(const char *file_path)
{
  FILE *fd = fopen(file_path, "rb");
  if (fd == NULL) {
    fprintf(stderr, "erro abrindo o arquivo: %s\n", file_path);
    exit(EXIT_FAILURE);
  }
  fseek(fd, 0, SEEK_END);
  size_t file_size = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  uint8_t *buffer = (uint8_t*) malloc(file_size * sizeof(uint8_t));
  fread(buffer, file_size, 1, fd);

  fclose(fd);

  Byte_Array array = {
    .length = file_size, 
    .data = buffer,
  };

  return array;
}

DIB_Header extract_dib_file_header_from_byte_array(Byte_Array data)
{
  const unsigned dib_header_offset = BITMAP_FILE_HEADER_SIZE;
  DIB_Header dib;

  dib.size = *((uint32_t*) &data[dib_header_offset]);
  dib.image_width = *((uint32_t*) &data[dib_header_offset + 4]);
  dib.image_height = *((uint32_t*) &data[dib_header_offset + 8]);

  dib.number_of_colors_planes = *((uint16_t*) &data[dib_header_offset + 12]);
  dib.n_bit_per_pixel = *((uint16_t*) &data[dib_header_offset + 14]);
  dib.bitfield = *((uint32_t*) &data[dib_header_offset + 16]);

  dib.size_of_data = *((uint32_t*) &data[dib_header_offset + 20]);
  dib.print_resolution_horizontal = *((uint32_t*) &data[dib_header_offset + 24]);
  dib.print_resolution_vertical = *((uint32_t*) &data[dib_header_offset + 28]);
  dib.n_colors_in_palette = *((uint32_t*) &data[dib_header_offset + 32]);
  dib.important_colors = *((uint32_t*) &data[dib_header_offset + 36]);

  return  dib;
}

Bitmap_File_Header extract_bitmap_file_header_from_byte_array(Byte_Array data)
{
  Bitmap_File_Header header = {};

  header.header[0] = data[0]; // B
  header.header[1] = data[1]; // M

  // @note :littleEndianOnly
  header.size = *((uint32_t*) &data[2]);

  header.application_specific = *((uint16_t*) &data[6]);
  header.application_specific2 = *((uint16_t*) &data[8]);

  // @note :littleEndianOnly
  header.offset = *((uint32_t*) &data[10]);
  
  return header;
}

bool export_bitmap_file_to_file(Bitmap_File *file, const char *filename)
{
  FILE *out = fopen(filename, "wb");

  if (out == NULL) return false;
  
  uint8_t bitmap_file_header[BITMAP_FILE_HEADER_SIZE];

  memcpy(&bitmap_file_header[0], &file->header->header, sizeof(Bitmap_File_Header::header));
  memcpy(&bitmap_file_header[2], &file->header->size, sizeof(Bitmap_File_Header::size));
  memcpy(&bitmap_file_header[6], &file->header->application_specific, sizeof(Bitmap_File_Header::application_specific));
  memcpy(&bitmap_file_header[8], &file->header->application_specific2, sizeof(Bitmap_File_Header::application_specific2));
  memcpy(&bitmap_file_header[10], &file->header->offset, sizeof(Bitmap_File_Header::offset));

  fwrite(&bitmap_file_header, 1, BITMAP_FILE_HEADER_SIZE, out);

  // @note Pode quebrar dependendo do padding, por isso o assert, para garantir
  // que se compilar a plataforma é suportada.
  static_assert(sizeof(DIB_Header) == BITMAP_DIB_HEADER_SIZE);
  fwrite(file->dib, 1, sizeof(DIB_Header), out);

  fwrite(file->pixel_array->data, 1, file->pixel_array->length, out);
  
  fclose(out);

  return true;
}

Bitmap_File* make_bitmap_out_of_file(Array<uint8_t> &file_data)
{
  Bitmap_File_Header *header = new Bitmap_File_Header;
  *header = extract_bitmap_file_header_from_byte_array(file_data);

  DIB_Header *dib = new DIB_Header;
  *dib = extract_dib_file_header_from_byte_array(file_data);
  
  Array<uint8_t> *file_pixel_array = new Array<uint8_t>;
  file_pixel_array->length = dib->size_of_data;
  // @todo João, fazer uma cópia
  file_pixel_array->data = &file_data[header->offset];

  assert(&file_data.data[header->offset] == file_pixel_array->data);
  assert(file_pixel_array->length == (file_data.length - header->offset));
  
  Bitmap_File *bitmap_file = new Bitmap_File;
  bitmap_file->header = header;
  bitmap_file->dib = dib;
  bitmap_file->pixel_array = file_pixel_array;

  return bitmap_file;
}

static inline auto calculate_row_size(uint8_t n_bit_per_pixel, size_t image_width)
{
  return ((n_bit_per_pixel * image_width + 31) / 32) * 4;
}

static inline auto calculate_pixel_storage(uint8_t n_bit_per_pixel, size_t image_width, size_t image_height)
{
  return calculate_row_size(n_bit_per_pixel, image_width) * image_height;
}

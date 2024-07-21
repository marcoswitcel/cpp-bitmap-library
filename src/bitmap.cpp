#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "./bitmap.h"

using Byte_Array = Array<uint8_t>;

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

  // @todo João, verificar como fazer para identificação da endianess do sistema para implementar
  // o código necessário para Big Endian
  header.size = *((uint32_t*) &data[2]);

  header.application_specific = *((uint16_t*) &data[6]);
  header.application_specific2 = *((uint16_t*) &data[8]);

  // @todo João, suportart Big Endian
  header.offset = *((uint32_t*) &data[10]);
  
  return header;
}

void export_bitmap_file_to_file(Bitmap_File *file, const char *filename)
{
  FILE *out = fopen(filename, "wb");
  
  // @todo João, lento isso aqui... porém por hora está bom
  fwrite(&file->header->header, 1, sizeof(Bitmap_File_Header::header), out);
  fwrite(&file->header->size, 1, sizeof(Bitmap_File_Header::size), out);
  fwrite(&file->header->application_specific, 1, sizeof(Bitmap_File_Header::application_specific), out);
  fwrite(&file->header->application_specific2, 1, sizeof(Bitmap_File_Header::application_specific2), out);
  fwrite(&file->header->offset, 1, sizeof(Bitmap_File_Header::offset), out);

  // @note Pode quebrar dependendo do padding, ajustar no futuro
  fwrite(file->dib, 1, sizeof(DIB_Header), out);

  fwrite(file->pixel_array->data, 1, file->pixel_array->length, out);
  
  fclose(out);
}

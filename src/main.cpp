#include <iostream>
#include <stdlib.h>

#include "./bitmap.cpp"

typedef struct Byte_Array {
  uint8_t *data;
  size_t size;
} Byte_Array;

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
    .data = buffer,
    .size = file_size, 
  };

  return array;
}

DIB_Header extract_dib_file_header_from_byte_array(Byte_Array byte_array)
{
  const unsigned dib_header_offset = BITMAP_FILE_HEADER_SIZE;
  DIB_Header dib;

  dib.size = *((uint32_t*) &byte_array.data[dib_header_offset]);
  dib.image_width = *((uint32_t*) &byte_array.data[dib_header_offset + 4]);
  dib.image_height = *((uint32_t*) &byte_array.data[dib_header_offset + 8]);

  dib.number_of_colors_planes = *((uint16_t*) &byte_array.data[dib_header_offset + 12]);
  dib.n_bit_per_pixel = *((uint16_t*) &byte_array.data[dib_header_offset + 14]);
  dib.bitfield = *((uint32_t*) &byte_array.data[dib_header_offset + 16]);

  dib.size_of_data = *((uint32_t*) &byte_array.data[dib_header_offset + 20]);
  dib.print_resolution_horizontal = *((uint32_t*) &byte_array.data[dib_header_offset + 24]);
  dib.print_resolution_vertical = *((uint32_t*) &byte_array.data[dib_header_offset + 28]);
  dib.n_colors_in_palette = *((uint32_t*) &byte_array.data[dib_header_offset + 32]);
  dib.important_colors = *((uint32_t*) &byte_array.data[dib_header_offset + 36]);

  return  dib;
}

Bitmap_File_Header extract_bitmap_file_header_from_byte_array(Byte_Array byte_array)
{
  Bitmap_File_Header header = {};

  header.header[0] = byte_array.data[0]; // B
  header.header[1] = byte_array.data[1]; // M

  // @todo João, verificar como fazer para identificação da endianess do sistema para implementar
  // o código necessário para Big Endian
  header.size = *((uint32_t*) &byte_array.data[2]);

  header.application_specific = *((uint16_t*) &byte_array.data[6]);
  header.application_specific2 = *((uint16_t*) &byte_array.data[8]);

  // @todo João, suportart Big Endian
  header.offset = *((uint32_t*) &byte_array.data[10]);
  
  return header;
}

int main(int argc, const char* argv[])
{
  if (argc < 1)
  {
    std::cout << "Nome do arquivo faltando.\n";
    return EXIT_FAILURE;
  }

  const char *file_path = argv[1];

  printf("file path: %s\n", file_path);

  auto file = read_file_as_byte_array(file_path);

  printf("file size: %ld\n", file.size);

  Bitmap_File_Header bmp_header = extract_bitmap_file_header_from_byte_array(file);
  DIB_Header dib_header = extract_dib_file_header_from_byte_array(file);

  printf("%c %c\n", bmp_header.header[0], bmp_header.header[1]);
  printf("header: file size: %d\n", bmp_header.size);
  printf("header: file offset: %d\n", bmp_header.offset);
  printf("dib: file size: %d\n", dib_header.size);
  printf("dib: width: %d\n", dib_header.image_width);
  printf("dib: height: %d\n", dib_header.image_height);

  printf("dib: number_of_colors: %d\n", dib_header.number_of_colors_planes);
  printf("dib: n_bit_per_pixel: %d\n", dib_header.n_bit_per_pixel);
  printf("dib: bitfield: %d\n", dib_header.bitfield);
  printf("dib: size_of_data: %d\n", dib_header.size_of_data);
  printf("dib: print_resolution_horizontal: %d\n", dib_header.print_resolution_horizontal);
  printf("dib: print_resolution_vertical: %d\n", dib_header.print_resolution_vertical);
  printf("dib: n_colors_in_palette: %d\n", dib_header.n_colors_in_palette);
  printf("dib: important_colors: %d\n", dib_header.important_colors);

  printf("first pixel: red: %d\n", file.data[bmp_header.offset + 0]);
  printf("first pixel: green: %d\n", file.data[bmp_header.offset + 1]);
  printf("first pixel: blue: %d\n", file.data[bmp_header.offset + 2]);

  printf("second pixel: red: %d\n", file.data[bmp_header.offset + 3]);
  printf("second pixel: green: %d\n", file.data[bmp_header.offset + 4]);
  printf("second pixel: blue: %d\n", file.data[bmp_header.offset + 5]);

  return EXIT_SUCCESS;
}


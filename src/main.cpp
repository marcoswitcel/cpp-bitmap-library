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

  printf("%c %c\n", bmp_header.header[0], bmp_header.header[1]);
  printf("header: file size: %d\n", bmp_header.size);
  printf("header: file offset: %d\n", bmp_header.offset);

  return EXIT_SUCCESS;
}


#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "./bitmap.cpp"
#include "./array.h"

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

  printf("file size: %ld\n", file.length);

  Bitmap_File_Header bmp_header = extract_bitmap_file_header_from_byte_array(file);
  DIB_Header dib_header = extract_dib_file_header_from_byte_array(file);

  /**
   * @note Os asserts abaixo refletem alguma conclusões que tirei sobre o layout da informação,
   * conforme for obtendo mais arquivos ".bmp" para analisar, posso validar se as assertivas são
   * verdadeiras em todas as configurações possíveis dos arquivos ".bmp"
   */
  assert((BITMAP_FILE_HEADER_SIZE + dib_header.size) == bmp_header.offset);
  assert((BITMAP_FILE_HEADER_SIZE + dib_header.size + dib_header.size_of_data) == bmp_header.size);

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

  // @todo joão, errado aqui, empacotado de 3 em 3 bytes?
  // @note teoricamente seriam 6 bytes úteis e 2 de padding, rows de 4 bytes
  // primeira pixel no primeiro row, segundo pixel começa no primeiro row e termina ocupando metade do segundo
  // row, aí entrariam 2 bytes de padding
  assert(dib_header.size_of_data % 4 == 0);
  for (unsigned i = 0; i < dib_header.size_of_data / 3; i++)
  {
    unsigned offset = bmp_header.offset + i * 3;
    // printf("%d: %d %d %d\n", i, file.data[offset + 0], file.data[offset + 1], file.data[offset + 2]);

    // filtro de luminosidade
    // file.data[offset + 0] = file.data[offset + 0] / 2;
    // file.data[offset + 1] = file.data[offset + 1] / 2;
    // file.data[offset + 2] = file.data[offset + 2] / 2;

    // filtro cinza
    file.data[offset + 0] = file.data[offset + 1];
    file.data[offset + 1] = file.data[offset + 1];
    file.data[offset + 2] = file.data[offset + 1];
  
  }

  FILE *out = fopen("../image/image-out.bmp", "wb");
  fwrite(file.data, 1, file.length, out);
  fclose(out);

  Bitmap_File_Header header = {
    .header = {'B', 'M'},
    .size = 0, // @todo a calcular
    .application_specific = 0,
    .application_specific2 = 0,
    .offset = 0, // @todo a calcular
  };
  DIB_Header dib;
  Byte_Array pixel_array;

  Bitmap_File new_file = {
    .header = &header,
    .dib = &dib,
    .pixel_array = &pixel_array,
  };

  export_bitmap_file_to_file(&new_file, "../image/image-new.bmp");

  return EXIT_SUCCESS;
}


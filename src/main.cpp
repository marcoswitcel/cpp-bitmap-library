#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "./bitmap.cpp"
#include "./array.h"

void debug_print_info(Bitmap_File_Header &bmp_header, DIB_Header &dib_header, Array<uint8_t> &file)
{
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
}

typedef void Filter_RGB_24bits(const RGB_24bits *in, RGB_24bits *out);

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

void iterate_over_uncompressed_data(Bitmap_File *file, Filter_RGB_24bits func)
{
  assert(file->dib->bitfield == BI_RGB);
  if (file->dib->bitfield != BI_RGB)
  {
    printf("iterate_over_uncompressed_data chamado para um arquivo comprimido\n");
    return;
  }

  const unsigned row_size_in_bytes = ((file->dib->n_bit_per_pixel * file->dib->image_width + 31) / 32) * 4;
  
  for (unsigned row = 0; row < file->dib->image_height; row++)
  {
    const unsigned offset =  row * row_size_in_bytes;

    for (unsigned col = 0; col < file->dib->image_width; col++)
    {
      RGB_24bits *pixel = (RGB_24bits *) &file->pixel_array->data[offset + col * 3];
      func(pixel, pixel);
    }
  }
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

  printf("file size: %ld\n", file.length);

  Bitmap_File_Header bmp_header = extract_bitmap_file_header_from_byte_array(file);
  DIB_Header dib_header = extract_dib_file_header_from_byte_array(file);
  Array<uint8_t> file_pixel_array = {
    .length = dib_header.size_of_data,
    .data = &file[bmp_header.offset],
  };
  assert(&file.data[bmp_header.offset] == file_pixel_array.data);
  assert(file_pixel_array.length == (file.length - bmp_header.offset));
  Bitmap_File bitmap_file = {
    .header = &bmp_header,
    .dib = &dib_header,
    .pixel_array = &file_pixel_array,
  };

  /**
   * @note Os asserts abaixo refletem alguma conclusões que tirei sobre o layout da informação,
   * conforme for obtendo mais arquivos ".bmp" para analisar, posso validar se as assertivas são
   * verdadeiras em todas as configurações possíveis dos arquivos ".bmp"
   */
  assert((BITMAP_FILE_HEADER_SIZE + dib_header.size) == bmp_header.offset);
  assert((BITMAP_FILE_HEADER_SIZE + dib_header.size + dib_header.size_of_data) == bmp_header.size);

  debug_print_info(bmp_header, dib_header, file);

  // @todo joão, errado aqui, empacotado de 3 em 3 bytes?
  // @note teoricamente seriam 6 bytes úteis e 2 de padding, rows de 4 bytes
  // primeira pixel no primeiro row, segundo pixel começa no primeiro row e termina ocupando metade do segundo
  // row, aí entrariam 2 bytes de padding
  assert(dib_header.size_of_data % 4 == 0);
  iterate_over_uncompressed_data(&bitmap_file, filter_RGB_24bits_luminosity);


  FILE *out = fopen("../image/image-out.bmp", "wb");
  fwrite(file.data, 1, file.length, out);
  fclose(out);

  Bitmap_File_Header header = {
    .header = {'B', 'M'},
    .size = 70, // @todo como calcular
    .application_specific = 0,
    .application_specific2 = 0,
    .offset = 54, // @todo como calcular
  };
  DIB_Header dib = {
    .size = 40,
    .image_width = 2,
    .image_height = 2,
    .number_of_colors_planes = 1,
    .n_bit_per_pixel = 24,
    .bitfield = BI_RGB,
    .size_of_data = 16,
    .print_resolution_horizontal = 2835,
    .print_resolution_vertical = 2835,
    .n_colors_in_palette = 0,
    .important_colors = 0,
  };
  uint8_t data[] = { 0, 0, 255, 255, 255, 255, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, };
  Byte_Array pixel_array = {
    .length = 16,
    .data = (uint8_t *) &data,
  };

  Bitmap_File new_file = {
    .header = &header,
    .dib = &dib,
    .pixel_array = &pixel_array,
  };

  export_bitmap_file_to_file(&new_file, "../image/image-new.bmp");

  return EXIT_SUCCESS;
}


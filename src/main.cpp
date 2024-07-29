#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "./bitmap.cpp"
#include "./array.hpp"
#include "./command-line-utils.cpp"
#include "./color-sampler.cpp"

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

void filter_RGB_24bits_blue(const RGB_24bits *in, RGB_24bits *out)
{
  out->b = 255;
  out->g = 0;
  out->r = 0;
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

/**
 * @brief imagem de exemplo 1 da wikipedia, implementada aqui para testar a 
 * capacidade de gerar arquivos válidos usando o código escrito.
 * 
 * @url https://en.wikipedia.org/wiki/BMP_file_format#Example_1
 */
void export_sample_01_2x2_image()
{
  Bitmap_File_Header header = {
    .header = {'B', 'M'},
    .size = 70,
    .application_specific = 0,
    .application_specific2 = 0,
    .offset = 54,
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

  std::string filename = "../image/image-new.bmp";
  export_bitmap_file_to_file(&new_file, filename.c_str());
}

Bitmap_File make_bitmap_from_image_data(const unsigned width, const unsigned height, Array<RGB_24bits> &image)
{
  const size_t pixel_storage_needed_in_bytes = calculate_pixel_storage(24, width, height);

  Bitmap_File_Header *header = new Bitmap_File_Header;
  header->header[0] = 'B';
  header->header[1] = 'M';
  header->size = BITMAP_DIB_HEADER_SIZE + BITMAP_FILE_HEADER_SIZE + pixel_storage_needed_in_bytes; // @todo aqui
  header->application_specific = 0;
  header->application_specific2 = 0;
  header->offset = BITMAP_DIB_HEADER_SIZE + BITMAP_FILE_HEADER_SIZE; // soma do header mais o dib header

  DIB_Header *dib = new DIB_Header;
  dib->size = BITMAP_DIB_HEADER_SIZE; // esse DIB Header tem tamanho de 40 bytes
  dib->image_width = width;
  dib->image_height = height;
  dib->number_of_colors_planes = 1;
  dib->n_bit_per_pixel = 24;
  dib->bitfield = BI_RGB;
  dib->size_of_data = pixel_storage_needed_in_bytes;
  dib->print_resolution_horizontal = 2835;
  dib->print_resolution_vertical = 2835;
  dib->n_colors_in_palette = 0;
  dib->important_colors = 0;

  
  Byte_Array *pixel_array = new Byte_Array;
  pixel_array->length = pixel_storage_needed_in_bytes;
  pixel_array->data = new uint8_t[pixel_storage_needed_in_bytes];

  Bitmap_File new_file = {
    .header = header,
    .dib = dib,
    .pixel_array = pixel_array,
  };

  assert(new_file.dib->bitfield == BI_RGB);

  const unsigned row_size_in_bytes = ((new_file.dib->n_bit_per_pixel * new_file.dib->image_width + 31) / 32) * 4;
  
  for (unsigned row = 0; row < new_file.dib->image_height; row++)
  {
    const unsigned offset =  row * row_size_in_bytes;

    for (unsigned col = 0; col < new_file.dib->image_width; col++)
    {
      RGB_24bits *pixel = (RGB_24bits *) &new_file.pixel_array->data[offset + col * 3];
      *pixel = image.data[(height - 1 - row) * width + col];
    }
  }

  return new_file;
}

void export_generated_image()
{
  const unsigned width = 1920;
  const unsigned height = 1080;

  Array<RGB_24bits> image = {
    .length = width * height,
    .data = new RGB_24bits[width * height],
  };

  for (size_t i = 0; i < image.length; i++)
  {
    auto &pixel = image[i];
    pixel.r = ((i % width) / ((float) width)) * 255;
    pixel.g = 0;
    pixel.b = ((i / width) / ((float) height)) * 255;
  }

  std::string filename = "../image/image-generated.bmp";

  Bitmap_File new_file = make_bitmap_from_image_data(width, height, image);

  export_bitmap_file_to_file(&new_file, filename.c_str());
}

Bitmap_File* make_bitmap_out_of_file(Array<uint8_t> &file)
{
  Bitmap_File_Header *header = new Bitmap_File_Header;
  *header = extract_bitmap_file_header_from_byte_array(file);

  DIB_Header *dib = new DIB_Header;
  *dib = extract_dib_file_header_from_byte_array(file);
  
  Array<uint8_t> *file_pixel_array = new Array<uint8_t>;
  file_pixel_array->length = dib->size_of_data;
  // @todo João, fazer uma cópia
  file_pixel_array->data = &file[header->offset];

  assert(&file.data[header->offset] == file_pixel_array->data);
  assert(file_pixel_array->length == (file.length - header->offset));
  
  Bitmap_File *bitmap_file = new Bitmap_File;
  bitmap_file->header = header;
  bitmap_file->dib = dib;
  bitmap_file->pixel_array = file_pixel_array;

  return bitmap_file;
}

void load_and_mofidy(const char *file_path, const char *file_out_path, bool emmit_header_info)
{
  if (emmit_header_info) printf("file path: %s\n", file_path);

  auto file = read_file_as_byte_array(file_path);

  if (emmit_header_info) printf("file size: %ld\n", file.length);

 
  Bitmap_File &bitmap_file = *make_bitmap_out_of_file(file);

  /**
   * @note Os asserts abaixo refletem alguma conclusões que tirei sobre o layout da informação,
   * conforme for obtendo mais arquivos ".bmp" para analisar, posso validar se as assertivas são
   * verdadeiras em todas as configurações possíveis dos arquivos ".bmp"
   */
  assert((BITMAP_FILE_HEADER_SIZE + bitmap_file.dib->size) == bitmap_file.header->offset);
  assert((BITMAP_FILE_HEADER_SIZE + bitmap_file.dib->size + bitmap_file.dib->size_of_data) == bitmap_file.header->size);

  if (emmit_header_info) debug_print_info(*bitmap_file.header, *bitmap_file.dib, file);

  // @note teoricamente seriam 6 bytes úteis e 2 de padding, rows de 4 bytes
  // primeira pixel no primeiro row, segundo pixel começa no primeiro row e termina ocupando metade do segundo
  // row, aí entrariam 2 bytes de padding
  assert(bitmap_file.dib->size_of_data % 4 == 0);
  iterate_over_uncompressed_data(&bitmap_file, filter_RGB_24bits_luminosity);


  FILE *out = fopen(file_out_path, "wb");
  fwrite(file.data, 1, file.length, out);
  fclose(out);
}

Array<RGB_24bits>* make_contiguous_array_out_of_pixel_storage(Bitmap_File &bitmap_file)
{
  Array<RGB_24bits> *texture = new Array<RGB_24bits>;
  
  texture->length = bitmap_file.dib->image_width * bitmap_file.dib->image_height;
  texture->data = new RGB_24bits[bitmap_file.dib->image_width * bitmap_file.dib->image_height];

  const unsigned row_size_in_bytes = ((bitmap_file.dib->n_bit_per_pixel * bitmap_file.dib->image_width + 31) / 32) * 4;
  
  for (unsigned row = 0; row < bitmap_file.dib->image_height; row++)
  {
    const unsigned offset =  row * row_size_in_bytes;

    for (unsigned col = 0; col < bitmap_file.dib->image_width; col++)
    {
      RGB_24bits *pixel = (RGB_24bits *) &bitmap_file.pixel_array->data[offset + col * 3];
      texture->data[(bitmap_file.dib->image_height - 1 - row) * bitmap_file.dib->image_width + col] = *pixel;
    }
  }

  return texture;
}

Image<RGB_24bits> make_image_data_from_bitmap(Bitmap_File &bmp_file)
{
  Image<RGB_24bits> image = {
    .width = bmp_file.dib->image_width,
    .height = bmp_file.dib->image_height,
    .buffer = make_contiguous_array_out_of_pixel_storage(bmp_file),
  };

  return image;
}

Image<RGB_24bits> resize_image(const unsigned width, const unsigned height, Image<RGB_24bits> &source_image)
{
  Image<RGB_24bits> image = {
    .width = width,
    .height = height,
    .buffer = new Array<RGB_24bits>,
  };

  image.buffer->length = width * height;
  image.buffer->data  = new RGB_24bits[width * height];
  
  for (size_t x = 0; x < width; x++)
  {
    for (size_t y = 0; y < height; y++)
    {
      float y_coord = y / (float) height;
      float x_coord = x / (float) width;

      RGB_24bits &pixel = image.buffer->data[y * width + x];
      pixel = sampler2D(source_image, x_coord, y_coord);
    }
  }

  return image;
}

void load_and_size_down(const char *file_path, const char *file_out_path)
{
  auto file = read_file_as_byte_array(file_path);

  Bitmap_File &bitmap_file = *make_bitmap_out_of_file(file);

  Image<RGB_24bits> original_picture = make_image_data_from_bitmap(bitmap_file);

  auto &dib_header = *bitmap_file.dib;

  auto resized_down = resize_image(dib_header.image_width / 2, dib_header.image_height / 2, original_picture);
  auto resized_up = resize_image(dib_header.image_width * 2, dib_header.image_height * 2, original_picture);

  auto new_reseized_down = make_bitmap_from_image_data(resized_down.width, resized_down.height, *resized_down.buffer);
  auto new_reseized_up = make_bitmap_from_image_data(resized_up.width, resized_up.height, *resized_up.buffer);
  
  // @todo João, WIP: terminar o size_down e mudar o nome do arquivo de saída
  // FILE *out = fopen(file_out_path, "wb");
  export_bitmap_file_to_file(&new_reseized_down, "../image/sized-down.bmp");
  export_bitmap_file_to_file(&new_reseized_up, "../image/sized-up.bmp");
}

typedef struct Command_Line_Arguments {
  bool is_generated_image;
  bool is_export_sample;
  const char *file_in;
  const char *file_out;
  bool emmit_header_info;
  bool size_down;
} Command_Line_Arguments;

int main(int argc, const char* argv[])
{
  Command_Line_Arguments arguments = {
    .is_generated_image = is_string_present_in_argv("--generate-image", argc, argv),
    .is_export_sample = is_string_present_in_argv("--export-sample", argc, argv),
    .file_in = NULL,
    .file_out = NULL,
    .emmit_header_info = is_string_present_in_argv("--header-info", argc, argv),
    .size_down = is_string_present_in_argv("--size-down", argc, argv),
  };

  int file_in_index = index_of_in_argv("--file-in", argc, argv);
  int file_out_index = index_of_in_argv("--file-out", argc, argv);
  
  if (file_in_index < 0)
  {
    std::cout << "Parâmetro com o nome do arquivo de entrada faltando.\n";
    return EXIT_FAILURE;
  }

  if (file_in_index + 1 >= argc)
  {
    std::cout << "Valor do arquivo de entrada faltando.\n";
    return EXIT_FAILURE;
  }

  if (file_out_index < 0)
  {
    std::cout << "Parâmetro com o nome do arquivo de saída faltando.\n";
    return EXIT_FAILURE;
  }

  if (file_out_index + 1 >= argc)
  {
    std::cout << "Valor do arquivo de saída faltando.\n";
    return EXIT_FAILURE;
  }

  // @todo João, avisar se for o mesmo arquivo
  arguments.file_in = argv[file_in_index + 1];
  arguments.file_out = argv[file_out_index + 1];

  load_and_mofidy(arguments.file_in, arguments.file_out, arguments.emmit_header_info);

  if (arguments.size_down) load_and_size_down(arguments.file_in, arguments.file_out);

  if (arguments.is_export_sample) export_sample_01_2x2_image();

  if (arguments.is_generated_image) export_generated_image();

  return EXIT_SUCCESS;
}


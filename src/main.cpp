#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "./bitmap.cpp"
#include "./debug.cpp"
#include "./array.hpp"
#include "./filters.cpp"
#include "./command-line-utils.cpp"
#include "./color-sampler.cpp"


/**
 * @brief imagem de exemplo 1 da wikipedia, implementada aqui para testar a 
 * capacidade de gerar arquivos válidos usando o código escrito.
 * 
 * @url https://en.wikipedia.org/wiki/BMP_file_format#Example_1
 */
void export_sample_01_2x2_image(bool verbose)
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

  std::string filename = "sample-01-2x2.bmp";

  if (!verbose) return;
  
  if (export_bitmap_file_to_file(&new_file, filename.c_str()))
  {
    printf("Arquivo sample gerado e exportado. Arquivo: '%s'.\n", filename.c_str());
  }
  else
  {
    printf("Arquivo sample não pode ser gerado e exportado. Arquivo: '%s'.\n", filename.c_str());
  }
}

void export_generated_image(bool verbose)
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

  std::string filename = "image-generated.bmp";

  Bitmap_File new_file = make_bitmap_from_image_data(width, height, image);

  if (!verbose) return;

  if (export_bitmap_file_to_file(&new_file, filename.c_str()))
  {
    printf("Arquivo gradiente gerado e exportado. Arquivo: '%s'.\n", filename.c_str());
  }
  else
  {
    printf("Arquivo gradiente não pode ser gerado e exportado. Arquivo: '%s'.\n", filename.c_str());
  }
}

void apply_filter_to_image(Image<RGB_24bits> &image, Filter_Name filter_name)
{
  Filter_RGB_24bits *func;

  switch (filter_name)
  {
    case NONE: return;
    case GRAY:
      func = filter_RGB_24bits_gray;
    break;
    case LUMINOSITY:
      func = filter_RGB_24bits_luminosity;
    break;
    case BLUE_CHANNEL:
      func = filter_RGB_24bits_blue;
    break;
    case RED_CHANNEL:
      func = filter_RGB_24bits_red;
    break;
    case GREEN_CHANNEL:
      func = filter_RGB_24bits_green;
    break;
    default: return;
  }
  
  for (unsigned row = 0; row < image.height; row++)
  {
    for (unsigned col = 0; col < image.width; col++)
    {
      RGB_24bits *pixel = &image.buffer->data[row * image.width + col];
      func(pixel, pixel);
    }
  }
}

void print_help_info()
{
  std::cout << "Bitmap Tool\n";
  #ifdef PROGRAM_VERSION
  std::cout << "Versão: "<< PROGRAM_VERSION << "\n";
  #endif
  #ifdef BUILD_TIMESTAMP
  std::cout << "Timestamp: "<< BUILD_TIMESTAMP << "\n";
  #endif
  std::cout << "\nInformação de uso:\n";
  std::cout << "Parâmetros obrigatórios:\n";
  std::cout << " --file-in: especifica o nome do arquivo de entrada\n";
  std::cout << "Parâmetros opcionais:\n";
  std::cout << " --help: exibe ajuda\n";
  std::cout << " --resize: permite trocar resolução de saída do arquivo. e.x: --resize --width 500 --height 200\n";
  std::cout << "   --width: controla altura da imagem em pixels\n";
  std::cout << "   --height: controla largura da imagem em pixels\n";
  std::cout << " --version: exibe versão do build\n";
  std::cout << " --file-out: especifica o nome do arquivo de saída\n";
  std::cout << " --verbose: especifica se deve ou não descrever as operações através do output\n";
  std::cout << " --filter: especifica o nome do filtro a ser aplicado\n"; // @todo João, listar aqui os nomes dos filtros
}

void print_version_info()
{
  std::cout << "Bitmap Tool\n";
  #ifdef PROGRAM_VERSION
  std::cout << "Versão: "<< PROGRAM_VERSION << "\n";
  #endif
  #ifdef BUILD_TIMESTAMP
  std::cout << "Timestamp: "<< BUILD_TIMESTAMP << "\n";
  #endif
}

typedef struct Command_Line_Arguments {
  bool is_generated_image;
  bool is_export_sample;
  const char *file_in;
  const char *file_out;
  bool emmit_header_info;
  bool resize;
  const char *filter_name;
  int width;
  int height;
  bool help;
  bool version;
  bool verbose;
} Command_Line_Arguments;

int main(int argc, const char* argv[])
{
  Command_Line_Arguments arguments = {
    .is_generated_image = is_string_present_in_argv("--generate-image", argc, argv),
    .is_export_sample = is_string_present_in_argv("--export-sample", argc, argv),
    .file_in = NULL,
    .file_out = NULL,
    .emmit_header_info = is_string_present_in_argv("--header-info", argc, argv),
    .resize = is_string_present_in_argv("--resize", argc, argv),
    .filter_name = NULL,
    .width = -1,
    .height = -1,
    .help = is_string_present_in_argv("--help", argc, argv),
    .version = is_string_present_in_argv("--version", argc, argv),
    .verbose = is_string_present_in_argv("--verbose", argc, argv),
  };

  if (arguments.help)
  {
    print_help_info();
    return EXIT_SUCCESS;
  }

  if (arguments.version)
  {
    print_version_info();
    return EXIT_SUCCESS;
  }

  int file_in_index = index_of_in_argv("--file-in", argc, argv);
  int file_out_index = index_of_in_argv("--file-out", argc, argv);
  int filter_index = index_of_in_argv("--filter", argc, argv);
  int width_index = index_of_in_argv("--width", argc, argv);
  int height_index = index_of_in_argv("--height", argc, argv);
  
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

  if (filter_index > -1 && filter_index + 1 >= argc)
  {
    std::cout << "Nome do filtro faltando.\n";
    return EXIT_FAILURE;
  }

  if (width_index > -1 && width_index + 1 >= argc)
  {
    std::cout << "Width não especificada.\n";
    return EXIT_FAILURE;
  }

  if (height_index > -1 && height_index + 1 >= argc)
  {
    std::cout << "Height não especificada.\n";
    return EXIT_FAILURE;
  }

  arguments.file_in = argv[file_in_index + 1];
  arguments.file_out = argv[file_out_index + 1];

  /**
   * @note Por hora faço uma checagem simples, mas seria interessante uma checagem mais complexa
   * para garantir que o nome do arquivo não é o mesmo. Outra questão seria uma opção para forçar o override.
   */
  if (!strcmp(arguments.file_in, arguments.file_out))
  {
    std::cout << "O nome do arquivo de entrada e o de saída precisam ser diferentes.\n";
    return EXIT_FAILURE;
  }
  
  if (filter_index > -1)
  {
    arguments.filter_name = argv[filter_index + 1];
  }

  if (width_index > -1)
  {
    arguments.width = std::stoi(argv[width_index + 1]);
  }

  if (height_index > -1)
  {
    arguments.height = std::stoi(argv[height_index + 1]);
  }

  if (arguments.emmit_header_info) printf("file path: %s\n", arguments.file_in);
  auto file = read_file_as_byte_array(arguments.file_in);
  if (arguments.emmit_header_info) printf("file size: %ld\n", file.length);

  Bitmap_File &bitmap_file = *make_bitmap_out_of_file(file);

  if (bitmap_file.dib->bitfield != BI_RGB)
  {
    std::cout << "O arquivo possui compressão, não suportado.\n";
    return EXIT_FAILURE;
  }

  /**
   * @note Os asserts abaixo refletem alguma conclusões que tirei sobre o layout da informação,
   * conforme for obtendo mais arquivos ".bmp" para analisar, posso validar se as assertivas são
   * verdadeiras em todas as configurações possíveis dos arquivos ".bmp"
   */
  assert((BITMAP_FILE_HEADER_SIZE + bitmap_file.dib->size) == bitmap_file.header->offset);
  assert((BITMAP_FILE_HEADER_SIZE + bitmap_file.dib->size + bitmap_file.dib->size_of_data) == bitmap_file.header->size);

  assert(bitmap_file.dib->size_of_data % 4 == 0);

  Image<RGB_24bits> image = make_image_data_from_bitmap(bitmap_file);

  if (arguments.emmit_header_info) debug_print_info(*bitmap_file.header, *bitmap_file.dib, file); 
  
  if (arguments.resize) 
  {
    int width = bitmap_file.dib->image_width;
    int height = bitmap_file.dib->image_height;

    if (arguments.width != -1) width = arguments.width;
    if (arguments.height != -1) height = arguments.height;

    image = resize_image(width, height, image);
  }
  
  bool found = false;
  Filter_Name filter_name = lookup_filter_by_name(arguments.filter_name, &found);
  
  if (arguments.filter_name && !found)
  {
    std::cout << "Filtro não suportado: '" << arguments.filter_name << "'.\n";
    return EXIT_FAILURE;
  }

  if (filter_name != NONE) apply_filter_to_image(image, filter_name);


  auto new_image = make_bitmap_from_image_data(image.width, image.height, *image.buffer);
  
  auto success = export_bitmap_file_to_file(&new_image, arguments.file_out);

  if (arguments.verbose)
  {
    if (success)
    {
      printf("Arquivo criado com sucesso: '%s'.\n", arguments.file_out);
    }
    else
    {
      printf("Arquivo '%s' não pode ser criado.\n", arguments.file_out);
    }
  }

  if (arguments.is_export_sample) export_sample_01_2x2_image(arguments.verbose);

  if (arguments.is_generated_image) export_generated_image(arguments.verbose);

  return EXIT_SUCCESS;
}


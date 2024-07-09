#include <iostream>
#include <stdlib.h>

#include "./bitmap.cpp"

int main(int argc, const char* argv[])
{
  if (argc < 1)
  {

    std::cout << "Nome do arquivo faltando.\n";
    return EXIT_FAILURE;
  }

  const char *filename = argv[1];

  printf("filename: %s\n", filename);

  return EXIT_SUCCESS;
}


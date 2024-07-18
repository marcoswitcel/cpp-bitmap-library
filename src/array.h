#pragma once

#include <stdint.h>

/**
 * @brief Estrutura que representa um ponteiro para um bloco de memória com número de elementos conhecido
 * 
 * @tparam Type 
 */
template <typename Type>
struct Array {
  size_t length;
  Type *data;
};


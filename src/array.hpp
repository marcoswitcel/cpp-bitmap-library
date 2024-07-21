#pragma once

#include <stdint.h>
#include <assert.h>

/**
 * @brief Estrutura que representa um ponteiro para um bloco de memória com número de elementos conhecido
 * 
 * @tparam Type 
 */
template <typename Type>
struct Array {
  size_t length;
  Type *data;

  Type& operator[](size_t index)
  {
    assert(index < length);
    return data[index];
  }
};


#pragma once

#include <stdint.h>
#include <assert.h>

/**
 * @brief Estrutura que representa um ponteiro para um bloco de memória com número de elementos conhecido
 * 
 * @tparam Type Tipo de elementos armazenados
 */
template <typename Type>
struct Array {
  /**
   * @brief quantidade de elementos contidos no buffer
   */
  size_t length;
  /**
   * @brief Ponteiro para o bloco de memória contendo os elementos
   * 
   */
  Type *data;

  Type& operator[](size_t index)
  {
    assert(index < length);
    return data[index];
  }
};


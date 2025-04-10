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

  Array() {}

  Array(size_t length)
  {
    this->length = length;
    this->data = new Type[length];
  }
  
  /**
   * @brief Constrói um array com length e um data pointer providos
   * 
   * @param length 
   * @param data 
   */
  Array(size_t length, Type *data)
  {
    this->length = length;
    this->data = data;
  }

  /**
   * @brief facilita o acesso aos valores
   * 
   * @param index 
   * @return Type& 
   */
  Type& operator[](size_t index)
  {
    assert(index < length);
    return data[index];
  }

  /**
   * @brief inicializa o armazenamento necessário para o array
   * 
   * @param length 
   */
  void allocate_storage(size_t length) {
    this->length = length;
    this->data = new Type[length];
  }
};

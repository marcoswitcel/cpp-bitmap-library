#pragma once

#include "./array.hpp"

/**
 * @brief Define os metadados necessários para interpretar o buffer de cores que contém a informação
 * para produzir a imagem final.
 * 
 * @tparam Pixel_Type O tipo de cor escolhido para trabalhar, aqui podem haver diversas variações desde precisão
 * a quantidade de canais e se o range é dinâmico ou fixo.
 */
template <typename Pixel_Type>
struct Image {
  /**
   * @brief largura da imagem
   */
  size_t width;
  /**
   * @brief altura da imagem
   */
  size_t height;
  /**
   * @brief Buffer com as informações necessárias para produzir a image, o Pixel_Type pode assumir um formato
   * "incompleto", por exemplo, quando os dados representam index em uma paleta de cores.
   */
  Array<Pixel_Type> *buffer;
};

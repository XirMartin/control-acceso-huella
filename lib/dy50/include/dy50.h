#pragma once

class Dy50 {
 public:
  /**
   * @brief Inicializa la interfaz con el sensor de huellas DY50.
   *
   * Prepara el módulo para comenzar a intercambiar comandos
   * y verificar que el sensor esté disponible.
   *
   * @return true si la inicialización fue correcta.
   * @return false si no se pudo establecer comunicación.
   */
  bool begin();
};
#pragma once

/**
 * @brief Encapsula la inicialización básica del sensor de huellas DY50.
 *
 * Esta clase representa una abstracción mínima del lector de huellas
 * y concentra la lógica de arranque del módulo dentro del proyecto.
 */
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
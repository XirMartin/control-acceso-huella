#pragma once

class AccessControl {
 public:
  /**
   * @brief Inicializa el estado interno del control de acceso.
   *
   * Deja el sistema listo para intentar una verificación automática
   * de huella cuando no haya un enrolado en progreso.
   *
   * @return true si la inicialización se completa correctamente.
   */
  bool begin();

  /**
   * @brief Rearma la verificación automática de huella.
   *
   * Habilita nuevamente el intento de coincidencia automática
   * cuando el sensor queda sin dedo presente.
   */
  void armAutoMatch();

  /**
   * @brief Indica si se puede intentar una verificación automática.
   *
   * La verificación automática solo se permite si está armada
   * y no hay un proceso de enrolado en curso.
   *
   * @param enrollInProgress Indica si hay un enrolado en ejecución.
   * @return true si corresponde intentar el match automático.
   * @return false si no debe intentarse.
   */
  bool canTryAutoMatch(bool enrollInProgress) const;

  /**
   * @brief Actualiza el estado interno según el resultado del match.
   *
   * Si la verificación fue exitosa, desarma el intento automático
   * para evitar múltiples activaciones consecutivas.
   *
   * @param matchOk Indica si la huella coincidió correctamente.
   * @return true si el match fue exitoso.
   * @return false si no hubo coincidencia.
   */
  bool handleMatchResult(bool matchOk);

  /**
   * @brief Indica si el automatch está actualmente armado.
   *
   * Permite consultar el estado interno del módulo para validarlo
   * desde pruebas unitarias u otra lógica de control.
   *
   * @return true si el automatch está armado.
   * @return false si el automatch está desarmado.
   */
  bool isAutoMatchArmed() const;

 private:
  bool autoMatchArmed = true;
};
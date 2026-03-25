#include "access_control.h"

/**
 * @brief Inicializa el estado interno del módulo.
 *
 * Rearma la lógica de match automático al arrancar el sistema
 * y deja al módulo en condiciones de operar.
 *
 * @return true si la inicialización se completa correctamente.
 */
bool AccessControl::begin() {
  autoMatchArmed = true;
  return true;
}

/**
 * @brief Rearma la lógica de verificación automática.
 *
 * Habilita nuevamente el intento de match automático
 * cuando el sistema detecta que ya no hay dedo presente.
 */
void AccessControl::armAutoMatch() {
  autoMatchArmed = true;
}

/**
 * @brief Evalúa si corresponde intentar un match automático.
 *
 * La decisión depende de que el automatch esté armado
 * y de que no exista un enrolado en progreso.
 *
 * @param enrollInProgress Indica si hay un enrolado en curso.
 * @return true si puede intentarse el match automático.
 * @return false si no debe intentarse.
 */
bool AccessControl::canTryAutoMatch(bool enrollInProgress) const {
  return autoMatchArmed && !enrollInProgress;
}

/**
 * @brief Actualiza el estado del módulo según el resultado del match.
 *
 * Si la coincidencia fue exitosa, desarma el automatch
 * para evitar activaciones repetidas sin retirar el dedo.
 *
 * @param matchOk Indica si la huella coincidió correctamente.
 * @return true si el match fue exitoso.
 * @return false si no hubo coincidencia.
 */
bool AccessControl::handleMatchResult(bool matchOk) {
  if (matchOk) {
    autoMatchArmed = false;
    return true;
  }

  return false;
}
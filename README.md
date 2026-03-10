Este es un proyecto diseñado para el control de acceso (apertura de una puerta) con huellas dactilares
usando como hardware: ESP32-S3 + DY50-2V1 + relé
Funciones previstas: enrolar/borrar huellas, abrir puerta, interfaz web con clave

“Los tests unitarios se ejecutarán on-target con pio test cuando esté disponible el hardware.”

“El entorno native se dejó comentado porque en Windows requiere gcc/g++.”

“Asignación de pines”

DY50 UART: RX=GPIO16, TX=GPIO17 (Serial1)

RELÉ: GPIO18
T-OUT : GPIO21

“Prueba manual: Touch”

“Se monitoreó TOUCH_OUT por Serial a 115200”

“Al tocar el sensor alterna 0/1 (observado: 1,0,1,0,1)”s


“Prueba manual: Touch → Relé”

“Condición: flanco 0→1 en T-OUT (D21)”

“Acción: D18 HIGH durante 1s”

“Resultado: relé activó ~1s al tocar sensor (OK)”


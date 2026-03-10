Este es un proyecto diseñado para el control de acceso (apertura de una puerta) con huellas dactilares
usando como hardware: ESP32-S3 + DY50-2V1 + relé
Funciones previstas: enrolar/borrar huellas, abrir puerta, interfaz web con clave

“Los tests unitarios se ejecutarán on-target con pio test cuando esté disponible el hardware.”

“El entorno native se dejó comentado porque en Windows requiere gcc/g++.”

“Asignación de pines”

DY50 UART: RX=GPIO16, TX=GPIO17 (Serial1)

RELÉ: GPIO18
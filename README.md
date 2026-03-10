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

“Prueba manual: Match → Relé”

“Comando: m”

“Resultado esperado: MATCH OK id=1 y relé HIGH 1s”

“Resultado observado: OK”

“Prueba manual: Delete”

“Comando: d1 → DELETE OK”

“Luego: m con el mismo dedo → MATCH FAIL”

“Resultado: OK”

-----

Se agrega .clang-format

Al guardar se formatea (VS Code settings locales)

Hay pre-commit que verifica formato

-----

agregando cppcheck, comando usado:
.\tools\cppcheck\cppcheck.exe --enable=warning,style,performance,portability --inline-suppr --std=c++17 --language=c++ --error-exitcode=1 src lib

alcance: src y lib

resultado: sin hallazgos / OK

---

comando para correr tests: platformio.exe test

lista de tests actuales: test_dummy, test_rnf01_rele_inicial

mención: RNF-01 validado (relé inicia LOW)


----

archivo de configuración: Doxyfile

comando para generar docs: .\tools\doxygen\doxygen.exe Doxyfile

salida generada en: docs/doxygen/html/

----

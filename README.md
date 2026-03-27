Este es un proyecto diseñado para el control de acceso (apertura de una puerta) con huellas dactilares
usando como hardware: ESP32-S3 + DY50-2V1 + relé
Funciones previstas: enrolar/borrar huellas, abrir puerta, interfaz web con clave

“Los tests unitarios se ejecutarán on-target con pio test cuando esté disponible el hardware.”

“El entorno native está habilitado para probar la lógica desacoplada de hardware (por ejemplo, access_control). En Windows requiere disponer de gcc/g++.”

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

cppcheck se ejecuta automáticamente desde el pre-commit sobre src y lib.

---

comando para correr tests: platformio.exe test

En Windows, para correr `pio test -e native`, es necesario tener disponible `gcc/g++` (por ejemplo desde MSYS2/MinGW64) en el PATH de la terminal.

lista de tests actuales: test_access_control, test_dummy, test_rf04_match_activa_rele, test_rf05_no_match_no_rele, test_rnf01_rele_inicial

mención: RNF-01 validado (relé inicia LOW)


----

archivo de configuración: Doxyfile

comando para generar docs: doxygen Doxyfile

salida generada en: docs/doxygen/html/

----

/ → página principal protegida

/enroll?id=1 → enroll por web

/enroll-status → estado del enroll

/match → verifica huella y abre relé si coincide

/delete?id=1 → borra huella

----
resultado actual de la batería de tests:

test_dummy → PASSED

test_rf04_match_activa_rele → PASSED

test_rf05_no_match_no_rele → PASSED

test_rnf01_rele_inicial → PASSED

----

el sistema, en funcionamiento normal, realiza match automático

y una huella válida activa el relé 1 segundo sin usar serial ni web

----

Ultimas pruebas:

================================== SUMMARY ===================================== 
Environment               Test                         Status    Duration
------------------------  ---------------------------  --------  ------------
rymcu-esp32-s3-devkitc-1  test_dummy                   PASSED    00:00:09.147
rymcu-esp32-s3-devkitc-1  test_rf04_match_activa_rele  PASSED    00:00:10.096
rymcu-esp32-s3-devkitc-1  test_rf05_no_match_no_rele   PASSED    00:00:11.244
rymcu-esp32-s3-devkitc-1  test_rnf01_rele_inicial      PASSED    00:00:32.577
=============== 4 test cases: 4 succeeded in 00:01:03.063 ======================

----

Ultimas Pruebas:

Processing test_access_control in native environment
------------------------------------------------------------------------------------------------------------------
Building...
Testing...
test\test_access_control\test_main.cpp:50: test_begin_habilita_automatch        [PASSED]
test\test_access_control\test_main.cpp:51: test_no_intenta_match_si_hay_enroll_en_progreso      [PASSED]
test\test_access_control\test_main.cpp:52: test_match_exitoso_desarma_automatch [PASSED]
test\test_access_control\test_main.cpp:53: test_match_fallido_no_desarma_automatch      [PASSED]
test\test_access_control\test_main.cpp:54: test_estado_interno_refleja_automatch_armado_y_desarmado     [PASSED]
-------------------------- native:test_access_control [PASSED] Took 2.78 seconds -------------------------------- 

====================================================== SUMMARY ================================================== 
Environment    Test                 Status    Duration
-------------  -------------------  --------  ------------
native         test_access_control  PASSED    00:00:02.783
=================================== 5 test cases: 5 succeeded in 00:00:02.783 =================================== 




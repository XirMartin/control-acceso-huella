# Requerimientos del sistema (Control de acceso con huella)

## Funcionales

**RF-01 (Enroll):** El sistema debe permitir enrolar una huella en un ID numérico mediante el comando serial `e<ID>`.

**RF-02 (Delete):** El sistema debe permitir borrar una huella por ID mediante el comando serial `d<ID>`.

**RF-03 (Match):** El sistema debe permitir ejecutar una verificación de huella mediante el comando serial `m`.

**RF-04 (Acceso):** Si `m` encuentra coincidencia, el sistema debe activar el relé durante 1 segundo.

**RF-05 (No acceso):** Si `m` no encuentra coincidencia, el sistema no debe activar el relé.

**RF-06 (Web - autenticación):** El sistema debe requerir autenticación para acceder a la interfaz web.

**RF-07 (Web - enroll):** El sistema debe permitir enrolar una huella desde la interfaz web indicando un ID.

**RF-08 (Web - delete):** El sistema debe permitir borrar una huella desde la interfaz web indicando un ID.

**RF-09 (Web - match):** El sistema debe permitir ejecutar un match desde la interfaz web y mostrar el resultado.

**RF-10 (Auto match):** El sistema debe detectar automáticamente una huella válida y activar el relé durante 1 segundo, sin necesidad de comando serial ni acción desde la interfaz web.

## No funcionales

**RNF-01 (Arranque seguro):** En el arranque, el relé debe quedar desactivado (LOW).

**RNF-02 (Calidad):** El código debe cumplir el formato definido por `.clang-format` y pasar `cppcheck` sin hallazgos.

## Trazabilidad de pruebas

- **RNF-01** → `test/test_rnf01_rele_inicial/test_rnf01_rele_inicial.cpp`
- **RF-04** → `test/test_rf04_match_activa_rele/test_rf04_match_activa_rele.cpp`
- **RF-05** → `test/test_rf05_no_match_no_rele/test_rf05_no_match_no_rele.cpp`
- **RF-06** → `docs/acceptance-tests.md` (PA-06)
- **RF-07** → `docs/acceptance-tests.md` (PA-06)
- **RF-08** → `docs/acceptance-tests.md` (PA-06)
- **RF-09** → `docs/acceptance-tests.md` (PA-06)
- **RF-10** → validación manual sobre hardware (apertura automática con huella enrolada)
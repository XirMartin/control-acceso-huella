# Requerimientos del sistema (Control de acceso con huella)

## Funcionales

**RF-01 (Enroll):** El sistema debe permitir enrolar una huella en un ID numérico mediante el comando serial `e<ID>`.

**RF-02 (Delete):** El sistema debe permitir borrar una huella por ID mediante el comando serial `d<ID>`.

**RF-03 (Match):** El sistema debe permitir ejecutar una verificación de huella mediante el comando serial `m`.

**RF-04 (Acceso):** Si `m` encuentra coincidencia, el sistema debe activar el relé durante 1 segundo.

**RF-05 (No acceso):** Si `m` no encuentra coincidencia, el sistema no debe activar el relé.

## No funcionales

**RNF-01 (Arranque seguro):** En el arranque, el relé debe quedar desactivado (LOW).

**RNF-02 (Calidad):** El código debe cumplir el formato definido por `.clang-format` y pasar `cppcheck` sin hallazgos.
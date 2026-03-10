# Revisión de código asistida por IA

## Contexto
Se utilizó ChatGPT como asistente técnico durante el desarrollo del proyecto de control de acceso con huella digital sobre ESP32 y sensor DY50.

## Sugerencias relevantes recibidas
1. Usar `RX2/TX2` del ESP32 DevKit V1 para separar el sensor UART del puerto serie de programación.
2. Inicializar el relé en estado seguro (`LOW`) al arrancar.
3. Separar los flujos de `enroll`, `match` y `delete` mediante comandos seriales independientes.
4. Incorporar verificación automática de formato con `.clang-format` y hook `pre-commit`.
5. Agregar análisis estático con `cppcheck`.
6. Documentar requerimientos y trazabilidad con pruebas.

## Cambios aplicados
- Se usó `Serial2` en `RX2/TX2` para el DY50.
- Se configuró `PIN_RELE` como salida y se inicializó en `LOW`.
- Se implementaron los comandos `e<ID>`, `d<ID>` y `m`.
- Se agregó `.clang-format`, `formatOnSave` y hook `pre-commit`.
- Se ejecutó `cppcheck` sin hallazgos.
- Se documentaron requerimientos y tests asociados.

## Cambios no aplicados
- No se mantuvo el enfoque 100% simulado, porque se decidió trabajar con hardware real.
- No se dejó activo el log de depuración `RAW CMD`, porque era solo para diagnóstico temporal.

## Evaluación
La asistencia de IA fue útil para estructurar el proyecto, ordenar los commits, proponer verificaciones y ayudar a depurar problemas de integración UART, parsing de comandos y pruebas.
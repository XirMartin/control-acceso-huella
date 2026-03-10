# Pruebas de aceptación

## PA-01 — Enroll exitoso
**Requisito asociado:** RF-01  
**Precondición:** Sensor DY50 operativo y ESP32 encendido.  
**Pasos:**
1. Abrir monitor serie a 115200.
2. Enviar `e1`.
3. Colocar un dedo.
4. Retirar el dedo cuando se solicite.
5. Volver a colocar el mismo dedo.
**Resultado esperado:** Aparecen mensajes `Stored!` y `ENROLL OK`.

## PA-02 — Match exitoso y apertura
**Requisito asociado:** RF-03, RF-04  
**Precondición:** Existe una huella válida almacenada en el sensor.  
**Pasos:**
1. Abrir monitor serie a 115200.
2. Enviar `m`.
3. Colocar el dedo previamente enrolado.
**Resultado esperado:** Aparece `MATCH OK id=<n>` y el relé se activa durante 1 segundo.

## PA-03 — Delete exitoso
**Requisito asociado:** RF-02  
**Precondición:** Existe una huella almacenada en el ID a borrar.  
**Pasos:**
1. Abrir monitor serie a 115200.
2. Enviar `d1`.
**Resultado esperado:** Aparece `DELETE OK`.

## PA-04 — Match fallido luego de delete
**Requisito asociado:** RF-05  
**Precondición:** Se ejecutó `d1` sobre una huella previamente enrolada.  
**Pasos:**
1. Abrir monitor serie a 115200.
2. Enviar `m`.
3. Colocar el dedo borrado.
**Resultado esperado:** Aparece `MATCH FAIL` y el relé no se activa.

## PA-05 — Estado seguro al arranque
**Requisito asociado:** RNF-01  
**Precondición:** ESP32 apagado.  
**Pasos:**
1. Encender o resetear el ESP32.
2. Observar el relé al inicio.
**Resultado esperado:** El relé permanece desactivado al arrancar.
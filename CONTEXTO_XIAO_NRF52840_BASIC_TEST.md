# Contexto del proyecto XIAO nRF52840 Basic Test

## Objetivo

Proyecto base para probar una placa Seeed Studio XIAO nRF52840 desde cero.

La primera version debe servir para confirmar:

- Carga de firmware.
- Puerto serie USB.
- LED integrado.
- Bus I2C.

## Stack elegido

- PlatformIO.
- Framework Arduino.
- Placa `seeed_xiao_nrf52840`.
- Sin dependencias externas en la primera version.

## Decisiones

- Mantener el firmware pequeno y facil de modificar.
- Evitar BLE en la primera prueba para no introducir dependencias antes de validar la placa.
- Usar Serial a `115200` baudios.
- Incluir un escaner I2C porque sera util para sensores y modulos de laboratorio.

## Siguientes pasos posibles

- Anadir ejemplo BLE.
- Anadir lectura de bateria si el core Arduino expone `PIN_VBAT`.
- Anadir ejemplo con sensor I2C real.
- Crear una guia HTML visual si se necesita explicar el proyecto paso a paso.

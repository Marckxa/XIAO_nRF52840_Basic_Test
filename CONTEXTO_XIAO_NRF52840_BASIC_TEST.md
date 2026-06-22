# Contexto del proyecto XIAO nRF52840 Basic Test

## Objetivo

Proyecto base para probar una placa Seeed Studio XIAO nRF52840 desde cero.

La version actual debe servir para confirmar:

- Carga de firmware.
- Puerto serie USB.
- LED integrado.
- Bus I2C.
- Bluetooth LE con UART/NUS.

## Stack elegido

- PlatformIO.
- Framework Arduino.
- Entorno por defecto `xiao_nrf52840_sense`.
- Placas custom locales en `boards/`.
- Core oficial Seeed nRF52 Boards `1.1.13`.
- BLE usando la libreria `Bluefruit` incluida en el core Seeed/Adafruit nRF52.

## Decisiones

- Mantener el firmware pequeno y facil de modificar.
- Usar Serial a `115200` baudios.
- Incluir un escaner I2C porque sera util para sensores y modulos de laboratorio.
- Anunciar BLE como `XIAO-BLE-TEST` y exponer Nordic UART Service para pruebas
  desde apps como nRF Connect o Bluefruit LE Connect.
- El comando BLE `LED` activa la salida `D7` durante `1000 ms`.
- PlatformIO no trae la XIAO nRF52840 en `nordicnrf52` estandar; por eso el
  proyecto incluye la definicion local de placa y apunta al paquete oficial de
  Seeed.
- Se usa `core_dir = .platformio-core` para que PlatformIO guarde paquetes del
  proyecto localmente y no dependa del estado de `~/.platformio`.
- La placa conectada durante la prueba fue detectada como `Seeed XIAO nRF52840
  Sense`, VID/PID `2886:8045`.

## Siguientes pasos posibles

- Anadir lectura de bateria si el core Arduino expone `PIN_VBAT`.
- Anadir ejemplo con sensor I2C real.
- Crear una guia HTML visual si se necesita explicar el proyecto paso a paso.

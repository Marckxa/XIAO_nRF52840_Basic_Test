# Arquitectura - XIAO nRF52840 Basic Test

## Resumen

Firmware PlatformIO/Arduino para validar una Seeed Studio XIAO nRF52840 Sense
con Serial USB, LED integrado, bus I2C y BLE UART/NUS.

## Estructura

```text
platformio.ini   configuracion PlatformIO
boards/          definiciones locales de placa
src/main.cpp     firmware principal
docs/cableado.md notas de cableado
```

## Flujo principal

```text
setup
  -> inicializa Serial
  -> inicializa I2C
  -> inicializa BLE/NUS
loop
  -> parpadeo LED
  -> comandos Serial
  -> comandos BLE
  -> escaneo I2C bajo demanda
```

## Integraciones externas

- PlatformIO.
- Core Seeed/Adafruit nRF52.
- BLE con `Bluefruit`.
- Apps moviles tipo nRF Connect o Bluefruit LE Connect.

## Riesgos y limitaciones

- PlatformIO usa definiciones locales de placa.
- `core_dir = .platformio-core` guarda paquetes dentro del proyecto.
- La lectura de bateria queda pendiente.

# Decisiones - XIAO nRF52840 Basic Test

## 2026-06-24 - Proyecto base pequeno

- Decision: mantener un firmware pequeno y facil de modificar.
- Motivo: su objetivo es validar placa, Serial, LED, I2C y BLE.
- Alternativas: crear una arquitectura con multiples modulos desde el inicio.
- Consecuencias: `src/main.cpp` concentra la logica inicial.

## 2026-06-24 - Board local PlatformIO

- Decision: incluir definiciones locales en `boards/`.
- Motivo: PlatformIO no trae esta XIAO nRF52840 como placa estandar en
  `nordicnrf52`.
- Alternativas: depender de una instalacion global o board externa.
- Consecuencias: el proyecto es mas reproducible dentro de la carpeta.

## 2026-06-24 - BLE UART/NUS para pruebas

- Decision: anunciar `XIAO-BLE-TEST` y exponer un UART BLE/NUS.
- Motivo: permite probar comunicacion desde movil.
- Alternativas: solo Serial USB.
- Consecuencias: el proyecto sirve como base BLE simple.

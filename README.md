# Seeed Studio XIAO nRF52840 Basic Test

Proyecto basico para comprobar que una placa **Seeed Studio XIAO nRF52840** carga firmware, abre el puerto serie, parpadea el LED integrado y puede escanear el bus I2C.

## Que hace

- Parpadea el LED integrado cada 500 ms.
- Abre el monitor serie a `115200 baudios`.
- Imprime un estado periodico de la placa.
- Acepta comandos sencillos por Serial.
- Escanea dispositivos I2C conectados.

## Estructura

```text
XIAO_nRF52840_Basic_Test/
├── platformio.ini
├── src/
│   └── main.cpp
├── docs/
│   └── cableado.md
└── README.md
```

## Requisitos

Opcion recomendada:

- Visual Studio Code
- Extension PlatformIO
- Cable USB-C de datos
- Placa Seeed Studio XIAO nRF52840

Tambien se puede adaptar facilmente a Arduino IDE copiando el contenido de `src/main.cpp` a un sketch `.ino`.

## Cargar con PlatformIO

1. Abre esta carpeta con VS Code.
2. Instala la extension PlatformIO si no la tienes.
3. Conecta la XIAO nRF52840 por USB-C.
4. En PlatformIO, pulsa **Upload**.
5. Abre el monitor serie a `115200`.

Si la placa no entra en modo carga, pulsa dos veces rapido el boton **RESET** para activar el bootloader.

## Comandos por Serial

En el monitor serie puedes escribir:

```text
h  muestra ayuda
s  muestra estado de la placa
i  escanea el bus I2C
```

## Pines utiles para primeras pruebas

- `SDA` y `SCL`: bus I2C.
- `3V3`: alimentacion de sensores a 3.3 V.
- `GND`: tierra comun.
- `LED_BUILTIN`: LED integrado usado por el firmware.

## Notas

El proyecto evita dependencias externas para que la primera prueba sea sencilla. Cuando esta base funcione, el siguiente paso natural puede ser anadir BLE, lectura de bateria o sensores por I2C.

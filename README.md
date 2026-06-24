# Seeed Studio XIAO nRF52840 Basic Test

Proyecto basico para comprobar que una placa **Seeed Studio XIAO nRF52840** carga firmware, abre el puerto serie, parpadea el LED integrado y puede escanear el bus I2C.

## Que hace

- Parpadea el LED integrado cada 500 ms.
- Abre el monitor serie a `115200 baudios`.
- Imprime un estado periodico de la placa.
- Acepta comandos sencillos por Serial.
- Escanea dispositivos I2C conectados.
- Anuncia Bluetooth LE como `XIAO-BLE-TEST`.
- Expone un UART BLE/NUS para enviar texto desde el movil.
- Activa la salida `D7` durante `1000 ms` cuando recibe el comando BLE `LED`.

## Entorno incluido

El entorno por defecto es:

```text
xiao_nrf52840_sense
```

Es el que corresponde a la placa detectada por USB como `Seeed XIAO nRF52840 Sense`.

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

## Instalacion y ejecucion

Instala las dependencias con PlatformIO abriendo el proyecto o ejecutando la
primera compilacion. Comandos utiles:

```sh
pio run
pio run -t upload
pio device monitor
```

## Cargar con PlatformIO

1. Abre esta carpeta con VS Code.
2. Instala la extension PlatformIO si no la tienes.
3. Conecta la XIAO nRF52840 por USB-C.
4. En PlatformIO, pulsa **Upload**.
5. Abre el monitor serie a `115200`.

Si la placa no entra en modo carga, pulsa dos veces rapido el boton **RESET** para activar el bootloader.

En macOS el proyecto fuerza `upload_port = /dev/cu.usbmodem*` para evitar que PlatformIO intente usar puertos Bluetooth del ordenador.

## Prueba Bluetooth LE

El firmware anuncia un dispositivo BLE llamado:

```text
XIAO-BLE-TEST
```

Para probarlo:

1. Instala en el movil una app BLE como **nRF Connect** o **Bluefruit LE Connect**.
2. Escanea dispositivos BLE.
3. Busca `XIAO-BLE-TEST`.
4. Conecta al servicio UART/NUS.
5. Escribe texto: la placa lo devuelve como eco.
6. Envia `LED`: la placa activa `D7` durante `1000 ms`.

Mientras este conectado, la placa envia un mensaje `XIAO-BLE-TEST OK` cada 5 segundos.

## Nota sobre PlatformIO y esta placa

PlatformIO no incluye la Seeed Studio XIAO nRF52840 como placa estandar dentro de `nordicnrf52`. Por eso este proyecto incluye:

- `boards/seeed_xiao_nrf52840.json`: definicion local de placa.
- `boards/seeed_xiao_nrf52840_sense.json`: definicion local de la version Sense.
- `platform_packages`: descarga del core oficial `Seeed nRF52 Boards 1.1.13`.
- `core_dir = .platformio-core`: paquetes de PlatformIO guardados dentro del proyecto.
- `upload_port = /dev/cu.usbmodem*`: evita seleccionar puertos Bluetooth del Mac.

La primera compilacion puede tardar un poco porque PlatformIO descarga ese core.

## Comandos por Serial

En el monitor serie puedes escribir:

```text
h  muestra ayuda
s  muestra estado de la placa
i  escanea el bus I2C
b  muestra estado Bluetooth LE
```

## Pines utiles para primeras pruebas

- `SDA` y `SCL`: bus I2C.
- `D7`: salida digital activada por el comando BLE `LED`.
- `3V3`: alimentacion de sensores a 3.3 V.
- `GND`: tierra comun.
- `LED_BUILTIN`: LED integrado usado por el firmware.

## Documentacion y pendientes

- `CONTEXTO_XIAO_NRF52840_BASIC_TEST.md`
- `docs/arquitectura.md`
- `docs/decisiones.md`
- `docs/cableado.md`
- `TODO.md`

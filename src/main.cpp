#include <Arduino.h>
#include <Wire.h>

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

namespace {
constexpr uint32_t kBlinkIntervalMs = 500;
constexpr uint32_t kStatusIntervalMs = 3000;
constexpr uint8_t kI2cStartAddress = 1;
constexpr uint8_t kI2cEndAddress = 126;

uint32_t lastBlinkMs = 0;
uint32_t lastStatusMs = 0;
bool ledState = false;

void setLed(bool on) {
  ledState = on;

  // Many Arduino cores define LED_BUILTIN as active-low on tiny boards.
  // Keeping the inversion here makes the visual test work on the XIAO nRF52840.
  digitalWrite(LED_BUILTIN, on ? LOW : HIGH);
}

void printHelp() {
  Serial.println();
  Serial.println(F("Comandos disponibles:"));
  Serial.println(F("  h  ayuda"));
  Serial.println(F("  s  estado de la placa"));
  Serial.println(F("  i  escanear bus I2C"));
  Serial.println();
}

void printBoardStatus() {
  Serial.println(F("=== XIAO nRF52840 Basic Test ==="));
  Serial.print(F("Tiempo encendida: "));
  Serial.print(millis() / 1000);
  Serial.println(F(" s"));
  Serial.print(F("LED integrado: "));
  Serial.println(ledState ? F("ON") : F("OFF"));
  Serial.print(F("Frecuencia CPU: "));
  Serial.print(F_CPU / 1000000);
  Serial.println(F(" MHz"));

#ifdef PIN_VBAT
  const int rawBattery = analogRead(PIN_VBAT);
  Serial.print(F("PIN_VBAT raw: "));
  Serial.println(rawBattery);
#else
  Serial.println(F("PIN_VBAT no definido por este core Arduino."));
#endif

  Serial.println();
}

void scanI2cBus() {
  uint8_t found = 0;

  Serial.println(F("Escaneando I2C..."));

  for (uint8_t address = kI2cStartAddress; address <= kI2cEndAddress; address++) {
    Wire.beginTransmission(address);
    const uint8_t error = Wire.endTransmission();

    if (error == 0) {
      Serial.print(F("  Dispositivo en 0x"));
      if (address < 16) {
        Serial.print('0');
      }
      Serial.println(address, HEX);
      found++;
    }
  }

  if (found == 0) {
    Serial.println(F("  No se han encontrado dispositivos I2C."));
  }

  Serial.println(F("Escaneo I2C terminado."));
  Serial.println();
}

void handleSerialCommand(char command) {
  switch (command) {
    case 'h':
    case 'H':
    case '?':
      printHelp();
      break;
    case 's':
    case 'S':
      printBoardStatus();
      break;
    case 'i':
    case 'I':
      scanI2cBus();
      break;
    case '\n':
    case '\r':
    case ' ':
      break;
    default:
      Serial.print(F("Comando desconocido: "));
      Serial.println(command);
      printHelp();
      break;
  }
}
}  // namespace

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  setLed(false);

  Serial.begin(SERIAL_BAUD);
  Wire.begin();

  const uint32_t startMs = millis();
  while (!Serial && millis() - startMs < 3000) {
    delay(10);
  }

  Serial.println();
  Serial.println(F("XIAO nRF52840 Basic Test iniciado."));
  Serial.println(F("El LED integrado debe parpadear cada 500 ms."));
  printHelp();
  printBoardStatus();
}

void loop() {
  const uint32_t now = millis();

  if (now - lastBlinkMs >= kBlinkIntervalMs) {
    lastBlinkMs = now;
    setLed(!ledState);
  }

  if (now - lastStatusMs >= kStatusIntervalMs) {
    lastStatusMs = now;
    Serial.println(F("OK - escribe 'h' para ver comandos."));
  }

  while (Serial.available() > 0) {
    handleSerialCommand(static_cast<char>(Serial.read()));
  }
}

#include <Arduino.h>
#include <Wire.h>
#include <bluefruit.h>
#include <stdint.h>

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#define OUTPUT_PIN D7

constexpr uint32_t kOutputPulseMs = 1000;

bool outputPulseActive = false;
uint32_t outputPulseStartMs = 0;

namespace {
constexpr uint32_t kBlinkIntervalMs = 500;
constexpr uint32_t kStatusIntervalMs = 60000;
constexpr uint32_t kBleHeartbeatIntervalMs = 5000;
constexpr uint8_t kI2cStartAddress = 1;
constexpr uint8_t kI2cEndAddress = 126;

uint32_t lastBlinkMs = 0;
uint32_t lastStatusMs = 0;
uint32_t lastBleHeartbeatMs = 0;
bool ledState = false;

BLEDis bleDeviceInfo;
BLEUart bleUart;
BLEBas bleBattery;

void setLed(bool on) {
  ledState = on;
  digitalWrite(LED_BUILTIN, on ? LOW : HIGH);
}

bool isCommandLed(const char* cmd) {
  if (cmd == nullptr) {
    return false;
  }

  return
    (cmd[0] == 'L' || cmd[0] == 'l') &&
    (cmd[1] == 'E' || cmd[1] == 'e') &&
    (cmd[2] == 'D' || cmd[2] == 'd') &&
    cmd[3] == '\0';
}

void triggerOutputD7() {
  digitalWrite(OUTPUT_PIN, HIGH);
  outputPulseActive = true;
  outputPulseStartMs = millis();

  Serial.println(F("D7 ACTIVADO por comando BLE: LED"));

  if (bleUart.notifyEnabled()) {
    bleUart.println("D7 ACTIVADO");
  }
}

void updateOutputD7() {
  if (outputPulseActive && millis() - outputPulseStartMs >= kOutputPulseMs) {
    digitalWrite(OUTPUT_PIN, LOW);
    outputPulseActive = false;

    Serial.println(F("D7 DESACTIVADO"));

    if (bleUart.notifyEnabled()) {
      bleUart.println("D7 DESACTIVADO");
    }
  }
}

void processBleCommand(const char* cmd) {
  if (cmd == nullptr || cmd[0] == '\0') {
    return;
  }

  Serial.print(F("BLE RX: "));
  Serial.println(cmd);

  if (isCommandLed(cmd)) {
    triggerOutputD7();
  } else {
    bleUart.print("Eco: ");
    bleUart.println(cmd);
  }
}

void printHelp() {
  Serial.println();
  Serial.println(F("Comandos disponibles:"));
  Serial.println(F("  h    ayuda"));
  Serial.println(F("  s    estado de la placa"));
  Serial.println(F("  i    escanear bus I2C"));
  Serial.println(F("  b    estado Bluetooth LE"));
  Serial.println(F("  LED  activa D7 durante 1000 ms por Bluetooth"));
  Serial.println();
}

void printBleStatus() {
  Serial.println(F("=== Bluetooth LE ==="));
  Serial.println(F("Nombre anunciado: XIAO-BLE-TEST"));
  Serial.print(F("Estado: "));
  Serial.println(Bluefruit.connected() ? F("conectado") : F("anunciandose / esperando conexion"));
  Serial.println(F("Servicio activo: Nordic UART Service (NUS)"));
  Serial.println(F("App recomendada: nRF Connect o Bluefruit LE Connect"));
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

  Serial.print(F("BLE: "));
  Serial.println(Bluefruit.connected() ? F("conectado") : F("anunciandose"));

  Serial.print(F("D7: "));
  Serial.println(outputPulseActive ? F("ACTIVO") : F("INACTIVO"));

#ifdef PIN_VBAT
  const int rawBattery = analogRead(PIN_VBAT);
  Serial.print(F("PIN_VBAT raw: "));
  Serial.println(rawBattery);
#else
  Serial.println(F("PIN_VBAT no definido por este core Arduino."));
#endif

  Serial.println();
}

void startBleAdvertising() {
  Bluefruit.Advertising.clearData();
  Bluefruit.ScanResponse.clearData();

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleUart);
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start(0);
}

void connectCallback(uint16_t connHandle) {
  BLEConnection* connection = Bluefruit.Connection(connHandle);
  char centralName[32] = {0};
  connection->getPeerName(centralName, sizeof(centralName));

  Serial.print(F("BLE conectado"));
  if (centralName[0] != '\0') {
    Serial.print(F(" a "));
    Serial.print(centralName);
  }
  Serial.println();

  bleUart.println("XIAO-BLE-TEST conectado. Envia LED para activar D7.");
}

void disconnectCallback(uint16_t connHandle, uint8_t reason) {
  (void)connHandle;

  Serial.print(F("BLE desconectado. Motivo 0x"));
  Serial.println(reason, HEX);
}

void setupBle() {
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("XIAO-BLE-TEST");
  Bluefruit.Periph.setConnectCallback(connectCallback);
  Bluefruit.Periph.setDisconnectCallback(disconnectCallback);

  bleDeviceInfo.setManufacturer("Seeed Studio");
  bleDeviceInfo.setModel("XIAO nRF52840 Sense");
  bleDeviceInfo.begin();

  bleUart.begin();

  bleBattery.begin();
  bleBattery.write(100);

  startBleAdvertising();
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

    case 'b':
    case 'B':
      printBleStatus();
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

void handleBleUart() {
  static char rxBuffer[32];
  static uint8_t rxIndex = 0;

  while (bleUart.available()) {
    char c = static_cast<char>(bleUart.read());

    if (rxIndex == 0 && (c == ' ' || c == '\t')) {
      continue;
    }

    if (c == '\n' || c == '\r') {
      if (rxIndex == 0) {
        continue;
      }

      rxBuffer[rxIndex] = '\0';
      processBleCommand(rxBuffer);
      rxIndex = 0;
      continue;
    }

    if (c == ' ' || c == '\t') {
      if (rxIndex > 0) {
        rxBuffer[rxIndex] = '\0';
        processBleCommand(rxBuffer);
        rxIndex = 0;
      }
      continue;
    }

    if (rxIndex < sizeof(rxBuffer) - 1) {
      rxBuffer[rxIndex++] = c;
      rxBuffer[rxIndex] = '\0';

      if (rxIndex == 3 && isCommandLed(rxBuffer)) {
        processBleCommand(rxBuffer);
        rxIndex = 0;
      }
    } else {
      rxIndex = 0;
      Serial.println(F("BLE RX demasiado largo, descartado"));
      bleUart.println("ERROR: comando demasiado largo");
    }
  }
}

void sendBleHeartbeat(uint32_t now) {
  if (!Bluefruit.connected() || now - lastBleHeartbeatMs < kBleHeartbeatIntervalMs) {
    return;
  }

  lastBleHeartbeatMs = now;
  bleUart.print("XIAO-BLE-TEST OK, uptime=");
  bleUart.print(now / 1000);
  bleUart.println("s");
}

}  // namespace

void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  setLed(false);

  Serial.begin(SERIAL_BAUD);
  Wire.begin();
  setupBle();

  const uint32_t startMs = millis();
  while (!Serial && millis() - startMs < 3000) {
    delay(10);
  }

  Serial.println();
  Serial.println(F("XIAO nRF52840 Basic Test iniciado."));
  Serial.println(F("El LED integrado debe parpadear cada 500 ms."));
  Serial.println(F("Bluetooth LE anunciandose como XIAO-BLE-TEST."));
  Serial.println(F("Comando BLE: envia LED para activar D7 durante 1000 ms."));
  printHelp();
  printBoardStatus();
  printBleStatus();
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

  handleBleUart();
  sendBleHeartbeat(now);
  updateOutputD7();

  while (Serial.available() > 0) {
    handleSerialCommand(static_cast<char>(Serial.read()));
  }
}

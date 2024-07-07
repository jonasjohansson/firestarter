#include <DmxSimple.h>

const uint8_t dmxPin = 4;
const uint8_t flamePin = 8;
const uint8_t flameChannel = 11;
const uint8_t masterChannel = 2;
const uint8_t redChannel = 3;
const uint8_t greenChannel = 4;
const uint8_t blueChannel = 5;
const uint8_t safetyPin = 7;
const uint8_t safetyChannel = 1;

const int flameDuration = 1200;
const int flameDelay = 600;
const int fadeDuration = 5000;

bool BURST_MODE = true;

bool flameReady = false;
bool safetyState = false;
bool safetyOn = false;

const bool DEBUG = true;

void setDmx(uint8_t channel, uint8_t value) {
  DmxSimple.write(channel, value);
}

void updateSafetyState() {
  if (safetyState && safetyOn) {
    setDmx(safetyChannel, 128);
    safetyOn = false;
    if (DEBUG) Serial.println("safetyOn set to false");
  } else if (!safetyState && !safetyOn) {
    setDmx(safetyChannel, 0);
    safetyOn = true;
    if (DEBUG) Serial.println("safetyOn set to true");
  }
}

void setup() {
  if (DEBUG) Serial.begin(9600);
  DmxSimple.usePin(dmxPin);
  DmxSimple.maxChannel(2);
  pinMode(safetyPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  setDmx(safetyChannel, 0);
  setDmx(flameChannel, 0);
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  bool flameOn = !digitalRead(flamePin);
  safetyState = !digitalRead(safetyPin);

  updateSafetyState();

  if (!safetyOn) {
    if (BURST_MODE) {
      if (flameOn && flameReady) {
        digitalWrite(LED_BUILTIN, HIGH);
        setDmx(flameChannel, 255);
        delay(flameDuration);
        setDmx(flameChannel, 0);
        delay(flameDelay);
        flameReady = false;
      }
    } else {
      setDmx(flameChannel, flameOn ? 255 : 0);
    }
  }

  if (!flameOn) {
    flameReady = true;
  }

  unsigned long currentTime = millis();
  float phase = (currentTime % fadeDuration) / (float)fadeDuration;
  uint8_t masterValue = (uint8_t)((sin(phase * 2 * PI) + 1) * 127.5);

  setDmx(masterChannel, masterValue);
  setDmx(redChannel, 255);
  setDmx(greenChannel, 0);
  setDmx(blueChannel, 0);

  delay(10);
}

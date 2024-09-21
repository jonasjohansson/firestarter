#include <DmxSimple.h>

const uint8_t dmxPin = 4; // this is the pin used for DmxSimple as seen in function DmxSimple.usePin(). It is by default pin 3, but I am using pin 4.
const uint8_t safetyPin = 7; // this is the pin used as safety, if I connect pin 7 to ground, the safety is OFF. This is so that I quickly can remove the pin to turn ON the safety
const uint8_t flamePin = 8; // this is the pin used for sensing the state of the IR "flame" sensor
const uint8_t burstPin = 9; // if this pin is connected to ground, the firestarter does burst of flames instead of a continouous flame.
const uint8_t safetyChannel = 1; // this is the channel set on the DMX flame machine for safety
const uint8_t flameChannel = 2; // this is the channel set on the DMX flame machine for the flame.

// For some reason, at least on my DMX Shield, I have to connect RX0 to DE2. And my jumper pins are set to RX-io, TX-io, DE and EN.

const unsigned int startupDelay = 1000;
const unsigned int flameDuration = 1200;
const unsigned int flameDelay = 600;
const unsigned int fadeDuration = 5000;
const unsigned int maxFlameOnTime = 5000;

bool flameReady = true;
bool safetyOn = false;
bool doOnce = true;

const bool debug = true;

unsigned long flameOnStartTime = 0;

void setDmx(uint8_t channel, uint8_t value) {
  DmxSimple.write(channel, value);
  //if (debug) Serial.println("channel: " + String(channel) + ", value: " + String(value));
}

void updateSafetyState(bool safetyState) {
  if (safetyState && safetyOn) {
    setDmx(safetyChannel, 128);
    safetyOn = false;
    if (debug) Serial.println("safetyOn: " + String(safetyOn));
  } else if (!safetyState && !safetyOn) {
    setDmx(safetyChannel, 0);
    safetyOn = true;
    if (debug) Serial.println("safetyOn: " + String(safetyOn));
  }
}

void setup() {
  if (debug) Serial.begin(9600);
  DmxSimple.usePin(dmxPin);
  DmxSimple.maxChannel(12);
  pinMode(safetyPin, INPUT_PULLUP);
  pinMode(flamePin, INPUT_PULLUP);
  pinMode(burstPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  setDmx(safetyChannel, 0);
  setDmx(flameChannel, 0);
  delay(startupDelay);
}

void loop() {
  unsigned long currentTime = millis();

  bool flameOn = !digitalRead(flamePin);
  bool safetyState = !digitalRead(safetyPin);
  bool burstState = !digitalRead(burstPin);

  if (debug && doOnce) {
    doOnce = false;
    Serial.println("flameOn: " + String(flameOn));
    Serial.println("safetyState: " + String(safetyState));
    Serial.println("burstState: " + String(burstState));
  }

  updateSafetyState(safetyState);

  if (!safetyOn) {
    if (burstState) {
      if (flameOn && flameReady) {
        Serial.println("Fire!");
        digitalWrite(LED_BUILTIN, HIGH);
        setDmx(flameChannel, 255);
        delay(flameDuration);
        setDmx(flameChannel, 0);
        delay(flameDelay);
        flameReady = false;
      }
    } else {
      if (flameOn) {
        if (currentTime - flameOnStartTime < maxFlameOnTime) {
          setDmx(flameChannel, 255);
        } else {
          setDmx(flameChannel, 0);
        }
      } else {
        flameOnStartTime = currentTime;
        setDmx(flameChannel, 0);
      }
    }
  }

  if (!flameOn && !flameReady) {
    Serial.println("Ready to fire again…");
    flameReady = true;
  }

  delay(10);
}

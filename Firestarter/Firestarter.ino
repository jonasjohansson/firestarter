#include <DmxSimple.h>

const uint8_t dmxPin = 4;
const uint8_t flameSensorPin = 8;
const uint8_t safetyPin = 7;
const int highThreshold = 100;  // Threshold for detecting a significant rise
const int lowThreshold = 200;   // Threshold for detecting the minimum value

int lastflameSensorValue = 512;
bool flameOff = false;
bool safetyOn = true;
int minSensorValue = 1023;  // Initialize to the highest possible analog value
int maxSensorValue = 512;     // Initialize to the lowest possible analog value

const unsigned long resetInterval = 10000;  // Reset interval in milliseconds (10 seconds)
unsigned long lastResetTime = 0;

const bool DEBUG = true;

void setDmx(uint8_t channel, uint8_t value) {
  DmxSimple.write(channel, value);
}

void setup() {
  if (DEBUG) Serial.begin(9600);
  DmxSimple.usePin(dmxPin);
  DmxSimple.maxChannel(2);
  pinMode(safetyPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  setDmx(1, 0);  // Turn the DMX off in setup
  setDmx(2, 0);  // Turn the DMX off in setup
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  // Reset minSensorValue and maxSensorValue every 10 seconds
  if (currentMillis - lastResetTime >= resetInterval) {
    minSensorValue = 1023;  // Reset to highest possible value
    maxSensorValue = 0;     // Reset to lowest possible value
    lastResetTime = currentMillis;
  }

  int flameSensorValue = digitalRead(flameSensorPin);
  int safetyState = !digitalRead(safetyPin);

  if (safetyState && safetyOn) {
    setDmx(1, 128);
    safetyOn = false;
    if (DEBUG) {
      Serial.print("safetyOn: ");
      Serial.println(safetyOn);
    }
  } else if (!safetyState && !safetyOn) {
    setDmx(1, 0);
    safetyOn = true;
    if (DEBUG) {
      Serial.print("safetyOn: ");
      Serial.println(safetyOn);
    }
  }

  // Auto adjust min and max sensor values
  minSensorValue = min(flameSensorValue, minSensorValue);
  maxSensorValue = max(flameSensorValue, maxSensorValue);

  //Serial.println(flameSensorValue);

  // Trigger DMX only if the sensor value has risen above the high threshold after being low
  if (!safetyOn){
    if (flameSensorValue) {
      digitalWrite(LED_BUILTIN, HIGH);
      if (DEBUG) {
        Serial.print("flameOff: ");
        Serial.print(flameOff);
        Serial.print(", flameSensorValue: ");
        Serial.print(flameSensorValue);
        Serial.print(", lastflameSensorValue: ");
        Serial.print(lastflameSensorValue);
        Serial.print(", minSensorValue: ");
        Serial.print(minSensorValue);
        Serial.print(", maxSensorValue: ");
        Serial.print(maxSensorValue);
        Serial.print(", highThreshold: ");
        Serial.println(highThreshold);
        Serial.println("FIRE!");
      }
      setDmx(2, 255);
      delay(100);        // Adjust this delay as needed
      if (DEBUG) {
        Serial.println("FIRE OFF!");
      }
      setDmx(2, 0);
      delay(5000);        // Adjust this delay as needed
      if (DEBUG) {
        Serial.println("CAN BE FIRED AGAIN…");
      }
    }
  }

  if (flameSensorValue != lastflameSensorValue) {
    lastflameSensorValue = flameSensorValue;
  }

  delay(10);
}

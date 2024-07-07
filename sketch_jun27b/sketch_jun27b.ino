#include <DmxSimple.h>

const uint8_t dmxPin = 4;
const uint8_t flameSensorPin = A0;
const int highThreshold = 10;
const int lowThreshold = 10;

int lastFlameSensorState = -1;
bool wasLow = false;
int minSensorValue = 1023;
int maxSensorValue = 0;

void setDmx(uint8_t channel, uint8_t value) {
  DmxSimple.write(channel, value);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Initialize built-in LED pin as output
  DmxSimple.usePin(dmxPin);
  DmxSimple.maxChannel(64);
  setDmx(1, 0);
}

void loop() {
  int flameSensorState = analogRead(flameSensorPin);
  
  minSensorValue = min(flameSensorState, minSensorValue);
  maxSensorValue = max(flameSensorState, maxSensorValue);

  // Check if the sensor value is near its minimum value
  if (flameSensorState <= minSensorValue + lowThreshold) {
    wasLow = true;
  } else {
    wasLow = false; // Reset wasLow if sensor value is above low threshold
  }
  
  // Trigger DMX only if the sensor value has risen above the high threshold after being low
  if (wasLow && flameSensorState > lastFlameSensorState + highThreshold) {
    setDmx(1, 255);
    digitalWrite(LED_BUILTIN, HIGH); // Turn on built-in LED
  } else {
    setDmx(1, 0);
    digitalWrite(LED_BUILTIN, LOW); // Turn off built-in LED
  }
  
  // Update last sensor state
  lastFlameSensorState = flameSensorState;
  
  delay(10);
}

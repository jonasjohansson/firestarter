#include <DmxSimple.h>

int safetyPin = 7;
int flamePin = 8;

void setup() {
  pinMode(safetyPin, INPUT_PULLUP);
  pinMode(flamePin, INPUT_PULLUP);
  DmxSimple.usePin(4);
  DmxSimple.maxChannel(64);
}

void loop() {
  bool safetyOn = !digitalRead(safetyPin);
  bool flameOn = !digitalRead(flamePin);

  if (safetyOn) {
    DmxSimple.write(1, 0);  // safety ON
  } else {
    DmxSimple.write(1, 128);  // safety OFF
  }

  if (!safetyOn && flameOn) {
    DmxSimple.write(2, 255);  // flame ON
  } else {
    DmxSimple.write(2, 0);  // flame OFF
  }
}

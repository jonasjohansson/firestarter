#include <DmxSimple.h>

// Define the DMX universe size
const int DMX_CHANNELS = 10;

// Array to store DMX values
byte dmxData[DMX_CHANNELS];

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize DMX interface
  DmxSimple.usePin(3);
  DmxSimple.maxChannel(DMX_CHANNELS);
}

void loop() {
  // Sending DMX data
  for (int channel = 1; channel <= DMX_CHANNELS; channel++) {
    // Generate some dummy data to send
    dmxData[channel - 1] = random(256); // Random value between 0 and 255

    // Send DMX value for this channel
    DmxSimple.write(channel, dmxData[channel - 1]);

    // Print what is being sent over serial
    Serial.print("Sent on channel ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.println(dmxData[channel - 1]);
  }

  // Reading DMX data
  for (int channel = 1; channel <= DMX_CHANNELS; channel++) {
    // Read DMX value for this channel
    int receivedValue = DmxSimple.read(channel);

    // Print received DMX value for verification
    Serial.print("Received on channel ");
    Serial.print(channel);
    Serial.print(": ");
    Serial.println(receivedValue);
  }

  // Delay for demonstration purposes
  delay(2000);
}

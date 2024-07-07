#include <Conceptinetics.h>
#include <SoftwareSerial.h>

// Define the DMX channel to send and receive data on
#define DMX_CHANNEL 1
#define DMX_CHANNELS 64

// Define the pin for DMX master output
#define DMX_OUTPUT_PIN 3

// Define pins for software serial
#define RX_PIN 10
#define TX_PIN 11

// Configure a DMX master (transmitter) and a DMX slave (receiver)
DMX_Master dmx_master(DMX_CHANNELS, DMX_OUTPUT_PIN);
DMX_Slave dmx_slave(DMX_CHANNELS);

// Set up software serial for debugging
SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  // Initialize DMX master (output)
  dmx_master.enable();
  dmx_master.setChannelValue(DMX_CHANNEL, 0);

  // Initialize DMX slave (input)
  dmx_slave.enable();
  dmx_slave.setStartAddress(DMX_CHANNEL);

  // Start software serial for debugging
  mySerial.begin(9600);
  mySerial.println("Hello world!");
}

void loop() {
  // Send a value
  dmx_master.setChannelValue(DMX_CHANNEL, 255); // Full intensity
  delay(500); // Short delay to allow transmission

  // Read the value
  int dmxValue = dmx_slave.getChannelValue(DMX_CHANNEL);
  
  // Print the received value using software serial
  mySerial.print("Received DMX Value: ");
  mySerial.println(dmxValue);

  delay(1000); // Wait before the next iteration
}

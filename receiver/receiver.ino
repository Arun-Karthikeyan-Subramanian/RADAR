#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

struct JoyValues {
  int xValue;
  int yValue;
  bool swValue;
} joy1;

RF24 radio(4, 5);  // CE = 4, CSN = 5 (adjust pins as needed for ESP32)

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("Radio module initialization failed. Check wiring.");
    while (1);
  } else {
    Serial.println("Radio module initialized successfully.");
  }

  radio.setChannel(5);  // Set the same channel as the transmitter
  radio.setDataRate(RF24_1MBPS);  // Set data rate to 1 Mbps
  radio.setPALevel(RF24_PA_HIGH);  // Set power amplifier level to high
  radio.openReadingPipe(0, 0x1234567890LL);  // Set the same address as the transmitter
  radio.startListening();  // Put the radio in receive mode
  Serial.println("Receiver ready. Waiting for data...");
}

void loop() {
  if (radio.available()) {
    radio.read(&joy1, sizeof(joy1));  // Read the incoming data

    // Print received values to Serial Monitor
    Serial.print("Received XValue | ");
    Serial.print(joy1.xValue);
    Serial.print(" YValue | ");
    Serial.print(joy1.yValue);
    Serial.print(" Switch Value | ");
    Serial.println(joy1.swValue);
  } else {
    Serial.println("No data available.");
  }

  delay(500);  // Small delay to avoid flooding the Serial Monitor
}
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define nRF24L01 CE and CSN pins for ESP32
#define CE_PIN  4   // Change this to any available GPIO pin
#define CSN_PIN 5   // Change this to any available GPIO pin

const uint64_t pipeIn = 0xE9E8F0F0E1LL; // Same address as transmitter

RF24 radio(CE_PIN, CSN_PIN); // Initialize radio with defined pins

// Define the structure to receive data
struct Signal {
  int rover_x;
  int rover_y;
  int arm;
  int gripper;
};

Signal receivedData; // Object to store received data

void setup() {
  Serial.begin(115200);
  SPI.begin();  // Initialize SPI for ESP32
  radio.begin();
  radio.openReadingPipe(1, pipeIn);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  
  Serial.println("ESP32 Receiver Ready...");
}

void loop() {
  if (radio.available()) {
    radio.read(&receivedData, sizeof(Signal));

    Serial.print("Rover X: "); Serial.print(receivedData.rover_x);
    Serial.print(" | Rover Y: "); Serial.print(receivedData.rover_y);
    Serial.print(" | Arm: "); Serial.print(receivedData.arm);
    Serial.print(" | Gripper: "); Serial.println(receivedData.gripper);
  }
}

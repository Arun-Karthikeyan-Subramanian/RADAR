#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeIn = 0xE9E8F0F0E1LL;  // Must match the transmitter

RF24 radio(4, 5);  // CE = GPIO4, CSN = GPIO5

struct Signal {
  int16_t rover_x;
  int16_t rover_y;
  int16_t arm;
  int16_t gripper;
};

Signal receivedData;

void ResetData()
{
// Define the inicial value of each data input. | Veri girişlerinin başlangıç değerleri
// The middle position for Potenciometers. (254/2=127) | Potansiyometreler için orta konum
receivedData.rover_x = 127;   // Center | Merkez
receivedData.rover_y = 127;  // Center | Merkez
receivedData.arm = 127; // Motor Stop | Motor Kapalı
receivedData.gripper = 127;   // Center | Merkez
}


void setup() {
  Serial.begin(9600);
  Serial.println("ESP32 Receiver");

  ResetData();
  if (!radio.begin()) {
    Serial.println("nRF24L01 NOT FOUND! Check wiring.");
    while (1);  // Stop execution if module is not found
  }

  radio.openReadingPipe(1, pipeIn);
  radio.setAutoAck(true);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();  // Enable receiving mode

  Serial.println("Listening for data...");
}

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&receivedData, sizeof(Signal));
lastRecvTime = millis();   // receive the data | data alınıyor
  }
}

void loop() {
  recvData();
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    ResetData(); // Signal lost.. Reset data | Sinyal kayıpsa data resetleniyor
  }
    // Print received values
    Serial.print(sizeof(Signal));
    Serial.print("Rover X: "); Serial.print(receivedData.rover_x);
    Serial.print(" | Rover Y: "); Serial.print(receivedData.rover_y);
    Serial.print(" | Arm: "); Serial.print(receivedData.arm);
    Serial.print(" | Gripper: "); Serial.println(receivedData.gripper);

  delay(500);  // Small delay to avoid spamming serial output
}
